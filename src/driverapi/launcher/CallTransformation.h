#pragma once
#include "DyninstProcess.h"
#include "Constants.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <iostream>
#include <memory>
#include <algorithm>
#include <sys/types.h>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include "APICaptureInstrimentation.h"

#include "DynOpsClass.h"
#include "Common.h"

// Dyninst includes
#include "CodeObject.h"
#include "CFG.h"
#include "PatchObject.h"
#include "PatchMgr.h"
#include "Point.h"
#include "BPatch_object.h"
#include "BPatch_snippet.h"
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_image.h"
#include "BPatch_function.h"
#include "BPatch_Vector.h"
#include "BPatch_point.h"
#include "BPatch_addressSpace.h"
#include "BPatch_statement.h"
#include "BPatch_basicBlock.h"
#include "InstructionCategories.h"
#include "dynC.h"
#include "set"
#include "LogInfo.h"
#include "Constants.h"
#include "StackPoint.h"
#include "StackStorage.h"
#include "ReadStackKeys.h"
#include "TFReaderWriter.h"
#include "InstrimentationTracker.h"
#include "BinaryLocationIDMap.h"

#include "DyninstFunctionWraps.h"
#include "DyninstFunction.h"
#include "DyninstCallsite.h"
#include "MemRecDataFile.h"

/*typedef std::vector<CUMallocTracker *> GPUMallocVec;
typedef std::vector<GLIBMallocTracker *> CPUMallocVec;
typedef std::vector<CUMemTransferTracker *> MemTransVec;
*/

typedef	std::vector<int64_t> Int64Vec;

struct MallocSite;
struct FreeSite;
struct TransferPoint;

typedef std::shared_ptr<MallocSite> MallocPtr;
typedef std::shared_ptr<FreeSite> FreeSitePtr;
typedef std::shared_ptr<TransferPoint> TransferPointPtr;

typedef std::map<int64_t, TransferPointPtr> TransferPointMap;
typedef std::set<MallocPtr> MallocSiteSet;
typedef std::set<FreeSitePtr> FreeSiteSet;



struct FreeSite{	
	int64_t id;
	StackPoint p;
	uint64_t count;
	int required;
	FreeSite(int64_t _id, StackPoint _p) : id(_id), p(_p), count(0), required(-1) {};

	void AddFree(int64_t inCount) {
		count += inCount;
	};

	MallocSiteSet::iterator GetStart() {
		return parents.begin();
	};

	MallocSiteSet::iterator GetEnd() {
		return parents.end();
	}


	std::string Print() {
		std::stringstream ss;
		ss << "[Free ID=" << std::dec << id << "] " << p.libname << "@" << std::hex << p.libOffset << std::dec << " Count=" << count << " ";
		return ss.str();
	};
	MallocSiteSet parents;
};

struct MallocSite {
	int64_t	id;
	StackPoint p;
	uint64_t count;
	FreeSiteSet children;
	int unknownChild;
	MallocSite(int64_t _id, StackPoint _p) : id(_id), p(_p), count(0), unknownChild(-1) {};

	void AddAlloc(FreeSitePtr destroyer, MallocPtr myself, int64_t inCount) {
		count += inCount;
		children.insert(destroyer);
		destroyer->AddFree(inCount);
		destroyer->parents.insert(myself);

	};

	bool IsEmpty() {
		if (p.libOffset == 0 || id == -1)
			return true;
		return false;
	};
	FreeSiteSet::iterator GetStart() {
		return children.begin();
	};

	FreeSiteSet::iterator GetEnd() {
		return children.end();
	}

	int HasUnknownChild() {
		if (unknownChild != -1)
			return unknownChild;
		unknownChild = 0;
		for (auto i : children)
			if (i->id < 0) {
				unknownChild = 1;
				break;
			}
		return unknownChild;
	};

	std::string Print() {
		std::stringstream ss;
		ss << "[Malloc ID=" << std::dec << id << "] " << p.libname << "@" << std::hex << p.libOffset << std::dec << " Count=" << count << " ";
		return ss.str();
	};
	void Destroy() {
		children.clear();
	};
};

struct TransferPoint {
	int64_t id;
	StackPoint p;
	uint64_t count;

	TransferPoint(int64_t _id, StackPoint _p) : id(_id), p(_p), count(0) {};

	void AddTransfer(MallocPtr m, int64_t inCount) {
		count += inCount;
		memAllocLocations.insert(m);
	}

	// Returns malloc sites, removes any malloc site that takes place 
	// Within libstdc++.so
	MallocSiteSet GetMallocSites() {
		MallocSiteSet ret;
		for (auto i : memAllocLocations)
			//if (i->p.libname.find("libstdc++") == std::string::npos) {
				ret.insert(i);
			//}
		return ret;
	}

	MallocSiteSet memAllocLocations;
};

template<typename T> 
void PrintSet(T & a, std::stringstream & out) {
	for (auto i : a)
		out << i->id << ",";
};

struct MemGraph {
	std::map<int64_t, MallocPtr> mallocPoints;
	std::map<int64_t, FreeSitePtr> freePoints;
	uint64_t totalFrees;
	~MemGraph() {
		for (auto i : mallocPoints)
			i.second->Destroy();
	};
	MemGraph() : totalFrees(0) {};

	template <typename T, typename D>
	void TraverseFromPoint(std::vector<T> & tlist, std::vector<D> & dlist) {
		if (tlist.size() == 0)
			return;
		std::deque<T> cn;
		std::set<T> tAlreadySeen;
		std::set<D> dAlreadySeen;
		cn.insert(cn.begin(), tlist.begin(), tlist.end());
		tlist.clear();
		while(cn.size() > 0) {
			auto i = cn.front();
			cn.pop_front();
			if (tAlreadySeen.find(i) != tAlreadySeen.end()) {
				continue;
			}
			tAlreadySeen.insert(i);
			tlist.push_back(i);
			std::deque<D> nn;
			nn.insert(nn.begin(), i->GetStart(), i->GetEnd());
			while (nn.size() > 0) {
				auto n = nn.front();
				nn.pop_front();
				if (dAlreadySeen.find(n) != dAlreadySeen.end())
					continue;
				dAlreadySeen.insert(n);
				//if (n->HasUnknownChild() != 0)
				//	continue;
				dlist.push_back(n);
				cn.insert(cn.end(), n->GetStart(), n->GetEnd());
			}
		}
	};

	void CheckForExitFrees() {
		// Checks for the presence of exit frees
		// If the number of frees at exit == unknown frees, remove free[-1].
		/*FILE * fp = fopen("DIOGENES_UnknownWriteCount.bin", "rb");
		if (fp == NULL)
			return;
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (size != sizeof(uint64_t) * 2) {
			std::cerr << "[MemGraph::CheckForExitFrees] File size of UnknownWriteCount is not what it should be (reported = " << size <<", expected 16)" << std::endl;
			fclose(fp);
			return;
		}
		uint64_t mallocCount = 0;
		uint64_t freeCount = 0;
		fread(&mallocCount, 1, sizeof(uint64_t), fp);
		fread(&freeCount, 1, sizeof(uint64_t), fp);
		fclose(fp);

		FreeSitePtr fsite = GetFreeSite(-1);
		if (fsite.get() == NULL)
			return;
		assert(fsite.get() != NULL);
		if (fsite->count == abs(totalFrees - freeCount)) {
			for (auto i : fsite->parents)
				i->children.erase(fsite);
			fsite->parents.clear();

			//fsite->Remove(fsite);
		}*/
	};

	FreeSitePtr GetFreeSite(int64_t id) {
		auto it = freePoints.find(id);
		if (it == freePoints.end())
			return NULL;
		return it->second;
	};
	MallocPtr GetMallocSite(int64_t id) {
		if (mallocPoints.find(id) == mallocPoints.end())
			return NULL;
		return mallocPoints[id];
	};
	std::string PrintRequiredSetFree() {
		std::stringstream ss;
		for(auto i : freePoints) {
			ss << "[Free ID=" << i.first << "]" << " Call Count = " << i.second->count << " required bits = " << i.second->required << std::endl;
		}
		return ss.str();

	}

	std::string PrintMemoryGraph() {
		std::stringstream ss;
		totalFrees = 0;
		uint64_t mallocCount = 0;
		for (auto i : mallocPoints) {
			ss << "[Malloc ID=" << i.first << "]" << " Call Count = " << i.second->count << " Associated FreeSites = ";
			mallocCount+= i.second->count;
			PrintSet<FreeSiteSet>(i.second->children, ss);
			ss << std::endl;
		}

		for (auto i : freePoints) {
			ss << std::dec << "[Free ID=" << i.first << "]" << " Call Count = " << i.second->count << " Associated MallocSites = ";
			PrintSet<MallocSiteSet>(i.second->parents, ss);
			totalFrees += i.second->count;
			ss << std::endl;
			ss << "[Free ID=" << i.first << "] " << i.second->p.libname << "@" << std::hex << i.second->p.libOffset << std::dec << std::endl;
		}
		ss << "[PrintMemoryGraph] Malloc Count = " << std::dec << mallocCount << " Free Count = " << totalFrees << std::endl;
		return ss.str();
	};
};

struct TransferGraph {
	TransferPointMap transfers;
	MallocPtr emptyMalloc;

	TransferGraph() {
		StackPoint p;
		emptyMalloc.reset(new MallocSite(-1,p));
	}


	TransferPointPtr ReturnTransfer(int64_t id) {
		if (transfers.find(id) != transfers.end())
			return transfers[id];
		return NULL;
	};
	
	void AddTransfer(int64_t id, int64_t mallocID, int64_t count, MemGraph & cpuGraph, std::map<int64_t, StackPoint> & idPoints) {
		MallocPtr tmp = cpuGraph.GetMallocSite(mallocID);
		TransferPointPtr tpoint;
		if (tmp == NULL) {
			tmp = emptyMalloc;
		}
		if (transfers.find(id) == transfers.end()) {
			tpoint.reset(new TransferPoint(id, idPoints[id]));
			transfers[id] = tpoint;
		} else {
			tpoint = transfers[id];
		}
		tpoint->AddTransfer(tmp, count);
	};

	std::string PrintTransferGraph() {
		std::stringstream ss;
		ss << "[TransferPoint::PrintTransferGraph] Transfer Graph" << std::endl;

		for (auto i : transfers) {
			ss << "[TransferGraph::PrintTransferGraph] Transfer ID " << i.first << " call count = " << i.second->count << " Associated MallocSites=";
			PrintSet<MallocSiteSet>(i.second->memAllocLocations, ss);
			ss << std::endl;
		}
		return ss.str();
	};
};


struct LSStackGraph {
	std::vector<StackPoint> _points;
	StackPoint _beforeLibcuda;
	uint64_t _oID;
	int64_t  _idPointID;
	bool _found;
	bool _required;
	LSStackGraph(std::vector<StackPoint> points, uint64_t oID) : _points(points), _oID(oID), _idPointID(-1),_found(false), _required(false) {
		bool tmpFound = false;

		for (int i = 0; i < _points.size(); i++) {
			if (_points[i].IsDriverAPI() || _points[i].IsRuntimeAPI()) {
				if (i - 1 >= 0) {
					_beforeLibcuda = _points[i-1];
					_found = true;					
				}
				return;
			}
		}

/*		for(int i = _points.size() - 1; i >= 0; i--) {
			if (_points[i].libname.find("libcudart") != std::string::npos) {
				tmpFound = true;
			} else if (tmpFound == true) {
				_beforeLibcuda = _points[i];
				_found = true;
				break;
			}
		}
*/
	};
};

typedef std::vector<LSStackGraph> LSStackGraphVec;

struct MatchLoadStoreStacksRecursive {
	std::unordered_map<std::string, MatchLoadStoreStacksRecursive *> _libmap; 

	uint64_t myId;
	MatchLoadStoreStacksRecursive() : myId(0) {};

	void InsertEntry(std::vector<StackPoint> & points, int pos, int stackID) {
		if(points.size() == 0)
			return;

		if (points.size() <= pos){
			myId = stackID;
			return;
		}

		std::stringstream ss;
		boost::filesystem::path p(points[pos].libname);
		std::string tmpfilename = p.stem().string();
		if (tmpfilename.find('.') != std::string::npos) {
			tmpfilename = tmpfilename.substr(0, tmpfilename.find('.'));
		}
		ss << tmpfilename << "@" << std::hex << points[pos].libOffset;
		std::string tmp = ss.str();
		std::cerr << "Inserting entry: " << tmp << std::endl;
		if (_libmap.find(tmp) == _libmap.end()) {
			
			_libmap[tmp] = new MatchLoadStoreStacksRecursive();
		}
		_libmap[tmp]->InsertEntry(points, pos+1, stackID);

/*		uint64_t libOffset = points[pos].libOffset;
		std::string libname = points[pos].libname;
		if (_libmap.find(libname) == _libmap.end()) 
			_libmap[libname] = new MatchLoadStoreStacksRecursive();

		if (pos + 1 >= points.size()) {
			_libmap[libname]->AddOffset(libOffset, stackID);
		} else
			_libmap[libname]->AddOffset(libOffset, 0);
		_libmap[libname]->InsertEntry(points, pos+1, stackID);

/*		
		if (points.size() == 0)
			return;
		if (points.size() <= pos){
			if (myId != 0) {
				std::cerr << "Stacks " << myId << " and " << stackID << " are identical!" << std::endl;
				assert(myId == 0);
			}
			//assert(myId == 0);
			myId = stackID;
		} else {
			uint64_t libOffset = points[pos].libOffset;
			std::string libname = points[pos].libname;
			if (_libmap.find(libname) == _libmap.end()) 
				_libmap[libname] = new MatchLoadStoreStacksRecursive();
			_libmap[libname]->AddOffset(libOffset);
			_libmap[libname]->InsertEntry(points, pos+1, stackID);
		}*/
	};

	uint64_t RecurseSinglePath() {
		if (_libmap.size() == 0)
			return myId;
		if (_libmap.size() != 1)
			return 0;
		return _libmap.begin()->second->RecurseSinglePath();
	};

	uint64_t FindEntry(std::vector<StackPoint> & points, int pos) {
		if(points.size() == 0)
			return 0;

		if (points.size() <= pos){
			std::cerr << "In exit at pos " << pos << std::endl;
			uint64_t ret = myId;
			if (myId == 0){
				std::cerr << "In recurse single path " << pos << std::endl;
				//if there is one path from here to end, assume that this is a match.
				// This is to deal with ambiguity between LS and MR stacks that may exists 
				ret = RecurseSinglePath();
			}
			return ret;
		}
		for (int i = pos; i < points.size(); i++){
			std::stringstream ss;
			boost::filesystem::path p(points[i].libname);
			std::string tmpfilename = p.stem().string();
			if (tmpfilename.find('.') != std::string::npos) {
				tmpfilename = tmpfilename.substr(0, tmpfilename.find('.'));
			}
			ss << tmpfilename << "@" << std::hex << points[i].libOffset;
			std::string libname = ss.str();
			std::cerr << "Looking up entry: " << libname << std::endl;
			if (_libmap.find(libname) == _libmap.end())
				continue;
			std::cerr << "ENTRY FOUND: " << libname << std::endl;
			uint64_t ret = _libmap[libname]->FindEntry(points, i+1);
			if (ret != 0)
				return ret;
		}
		return myId;
		// if (points.size() == 0)
		// 	return 0;
		// uint64_t ret = 0;
		// uint64_t startPos = 20000;
		// for (int i = pos; i < points.size(); i++){
		// 	uint64_t libOffset = points[i].libOffset;
		// 	std::string libname = points[i].libname;	
		// 	if (_libmap.find(libname) == _libmap.end())
		// 		continue;
		// 	if (_libmap[libname]->HasOffset(libOffset)){
		// 		uint64_t nextOffset = _libmap[libname]->_offsetMap[libOffset];
		// 		uint64_t tmp = _libmap[libname]->FindEntry(points, i + 1);
		// 		tmp  = std::max(tmp,nextOffset);
		// 		if ((ret  == 0 || i < startPos) && tmp != 0) {
		// 			ret = tmp;
		// 			startPos = i;
		// 		}
		// 	}
		// }
		// return ret;
	};

};


struct StackPointTree {
	std::map<std::string, std::map<uint64_t, int64_t>> _idMapper;
	StackPointTree(std::map<int64_t, StackPoint> & mmap) {
		for (auto i : mmap) {
			auto it = _idMapper.find(i.second.libname);
			if (it == _idMapper.end()){
				_idMapper[i.second.libname] = std::map<uint64_t,int64_t>();
				it = _idMapper.find(i.second.libname);
			}
			if (it->second.find(i.second.libOffset) != it->second.end())
				std::cerr << "[StackPointTree::StackPointTree] ERROR! Multiple id's at same offset! - " << i.second.libname << "@" << std::hex << i.second.libOffset << std::dec << std::endl;
			it->second[i.second.libOffset] = i.first;
		}
	};

	void AddEntry(StackPoint p, int64_t id) {
		auto it = _idMapper.find(p.libname);
		if (it == _idMapper.end()){
			_idMapper[p.libname] = std::map<uint64_t,int64_t>();
			it = _idMapper.find(p.libname);
		}
			
		auto it2 = it->second.find(p.libOffset);
		if (it2 == it->second.end()) {
			it->second[p.libOffset] = id;
		}
	};
	int64_t FindID(StackPoint & p) {
		auto it = _idMapper.find(p.libname);
		if (it == _idMapper.end())
			return -1;
		auto it2 = it->second.find(p.libOffset);
		if (it2 == it->second.end())
			return -1;
		return it2->second;
	};


};

enum RemovePointsVecTypes {
    CUMALLOC_REP,
    CUFREE_REP,
    CUFREE_REQUIRED,
    CUMEMCPY_REP,
    MALLOC_REP,
    FREE_REP
};

enum RemovePointsReturns{
	ENTRY_EXISTS,
	NEW_ENTRY
};
struct RemovePoints {
	StackPointVec cudaMallocReplacements;
	StackPointVec cudaFreeReplacements;
	StackPointVec cudaFreeReqSync;
	
	StackPointVec cudaMemcpyAsyncRepl;

	StackPointVec mallocReplacements;
	StackPointVec freeReplacements;





	void Serialize() {
		StackKeyWriter out(fopen("AC_RemovePoints.txt","w"));
		if(cudaMallocReplacements.size() > 0)
			out.InsertStack(10, cudaMallocReplacements);
		if (cudaFreeReplacements.size() > 0)
			out.InsertStack(20, cudaFreeReplacements);
		if (cudaFreeReqSync.size() > 0)
			out.InsertStack(30, cudaFreeReqSync);
		if (cudaMemcpyAsyncRepl.size() > 0)
			out.InsertStack(40, cudaMemcpyAsyncRepl);
		if (mallocReplacements.size() > 0)
			out.InsertStack(50, mallocReplacements);
		if (freeReplacements.size() > 0)
			out.InsertStack(60, freeReplacements);
	};
	void Deserialze() {
		StackKeyReader out(fopen("AC_RemovePoints.txt","rb"));
		std::map<uint64_t, std::vector<StackPoint> > points = out.ReadStacks();
		if (points.find(10) != points.end())
			cudaMallocReplacements = points[10];
		if (points.find(20) != points.end()) 
			cudaFreeReplacements = points[20];
		if (points.find(30) != points.end())
			cudaFreeReqSync = points[30];
		if (points.find(40) != points.end())
			cudaMemcpyAsyncRepl = points[40];
		if (points.find(50) != points.end()) 
			mallocReplacements = points[50];
		if (points.find(60) != points.end())
			freeReplacements = points[60];

	};

//    std::shared_ptr<StackPointTree> _TreeMapper;
    std::map<RemovePointsVecTypes, std::shared_ptr<StackPointTree>> _TreeMapper;
	std::map<RemovePointsVecTypes, uint64_t> _curCounts;
	void BuildTreeMap() {
	   std::map<RemovePointsVecTypes, StackPointVec *> vectorMapper = {{CUMALLOC_REP ,&cudaMallocReplacements}, 
	                                                                   {CUFREE_REP , &cudaFreeReplacements},
	                                                                   {CUFREE_REQUIRED , &cudaFreeReqSync},
	                                                                   {CUMEMCPY_REP , &cudaMemcpyAsyncRepl},
	                                                                   {MALLOC_REP , &mallocReplacements},
	                                                                   {FREE_REP , &freeReplacements}};
	    _TreeMapper.clear();                                                            
	   for (auto i : vectorMapper) {
	       int64_t count = 1;
    	   std::map<int64_t, StackPoint> tmpMap; 
	       for (auto n : *(i.second)) {
	            tmpMap[count] = n;
	            count++;
	       }
	       std::shared_ptr<StackPointTree> newTree(new StackPointTree(tmpMap));
	       _TreeMapper[i.first] = newTree;
       }
	};
	
	void PrintVector(StackPointVec * vec, std::string prePend, std::stringstream & ss) {
		for (auto i : *vec) {
			ss << "[" << prePend << "] "  << i.libname << "@" << i.libOffset << std::endl;
		}
	}

	std::string Print() {
	   std::map<std::string, StackPointVec *> vectorMapper = {{"CUDA_MAllOC_VEC" ,&cudaMallocReplacements}, 
	                                                                   {"CUDA_FREE_UNNECESSARY" , &cudaFreeReplacements},
	                                                                   {"CUDA_FREE_SYNC" , &cudaFreeReqSync},
	                                                                   {"CUDAMEMCPY_REP" , &cudaMemcpyAsyncRepl},
	                                                                   {"MALLOC_REP" , &mallocReplacements},
	                                                                   {"FREE_REP(IGNORED)" , &freeReplacements}};
        std::stringstream ss;
        for (auto i : vectorMapper) {
        	PrintVector(i.second, i.first, ss);
        }
        return ss.str();
	};


    StackPointVec GetAllStackPoints() {
        StackPointVec ret;
        ret.insert(ret.end(), cudaMallocReplacements.begin(), cudaMallocReplacements.end());
        ret.insert(ret.end(), cudaFreeReplacements.begin(), cudaFreeReplacements.end());
        ret.insert(ret.end(), cudaFreeReqSync.begin(), cudaFreeReqSync.end());
        ret.insert(ret.end(), cudaMemcpyAsyncRepl.begin(), cudaMemcpyAsyncRepl.end());
        ret.insert(ret.end(), mallocReplacements.begin(), mallocReplacements.end());
        ret.insert(ret.end(), freeReplacements.begin(), freeReplacements.end());
        return ret;
    };
	
	void GetLibsInt(std::set<std::string> & ret, StackPointVec & points) {
	    for (auto i : points)
	        ret.insert(i.libname);
	};
	
	std::set<std::string> GetLibs() {
	    std::set<std::string> ret;
	    GetLibsInt(ret, cudaMallocReplacements);
	    GetLibsInt(ret, cudaFreeReplacements);
	    GetLibsInt(ret, cudaMemcpyAsyncRepl);
	    GetLibsInt(ret, mallocReplacements);
	    GetLibsInt(ret, freeReplacements);
	    return ret;
	};
	
	RemovePointsReturns CheckArrayAndAddToIndex(RemovePointsVecTypes type, StackPoint p) {
		auto it = _TreeMapper.find(type);
		if (it == _TreeMapper.end())  {
			std::map<int64_t, StackPoint> tmpMap; 
			tmpMap[1] = p;
			_curCounts[type] = 2;
			std::shared_ptr<StackPointTree> newTree(new StackPointTree(tmpMap));
			_TreeMapper[type]  = newTree;
		} else {
			auto posId = it->second->FindID(p);
			if (posId == -1) {
				if (it->second->FindID(p) > -1)
					return ENTRY_EXISTS;
				it->second->AddEntry(p, _curCounts[type]);
				_curCounts[type]++;
			}
		}
		return NEW_ENTRY;
	};

	bool CheckArray(RemovePointsVecTypes type, StackPoint p) {
	    auto it = _TreeMapper.find(type);
	    if (it == _TreeMapper.end())
	        return false;
		if (it->second->FindID(p) >= 0)
			return true;	    
		return false;
	};
};

typedef std::shared_ptr<RemovePoints> RemovePointsPtr;


template<typename T> 
void BuildMemoryGraph(std::vector<T> & memSites, std::map<int64_t, StackPoint> & _idPoints, MemGraph & ret) {
	MallocPtr tmpMalloc;
	FreeSitePtr tmpFreePtr;
	for (auto i : memSites) {
		if (ret.mallocPoints.find(i->allocSite) == ret.mallocPoints.end()) {
			tmpMalloc.reset(new MallocSite(i->allocSite, _idPoints[i->allocSite]));
			ret.mallocPoints[i->allocSite] = tmpMalloc;
		} else {
			tmpMalloc = ret.mallocPoints[i->allocSite];
		}
		if (ret.freePoints.find(i->freeSite) == ret.freePoints.end()) {
			tmpFreePtr.reset(new FreeSite(i->freeSite, _idPoints[i->freeSite]));
			ret.freePoints[i->freeSite] = tmpFreePtr;
		} else {
			tmpFreePtr = ret.freePoints[i->freeSite];
		}
		tmpMalloc->AddAlloc(tmpFreePtr, tmpMalloc, i->count);
	}
}

class CallTransformation {
public:
	CallTransformation(GPUMallocVec & gpuVec,CPUMallocVec & cpuVec, MemTransVec & memVec, std::map<int64_t, StackPoint> & idPoints, std::unordered_map<std::string, StackPoint> & wrapperReplacements);
	void BuildRequiredSet();
	RemovePointsPtr GetRemoveCalls();
	std::map<uint64_t, std::vector<StackPoint> > ReadMemRecorderKeys(std::map<uint64_t, std::string> & typeMap);
	std::map<uint64_t, uint64_t> MatchLStoMR(std::map<uint64_t, std::vector<StackPoint> > & LS, std::map<uint64_t, std::vector<StackPoint> > & MR);
	void PrintStackSet(std::stringstream & outdata, std::string header, std::vector<StackPoint> & sp, int count);
	//void GetCudaFreeMallocPairs(std::map<uint64_t, std::shared_ptr<DyninstFunction> > & funcMap, CudaFreeCallsites & callsites);
	//void GetMemTransReplacement(std::map<uint64_t, std::shared_ptr<DyninstFunction> > & funcMap, MemTransCallsites & callsites);
private:
	void BuildGraph();
	MemGraph _cpuGraph;
	MemGraph _gpuGraph;
	TransferGraph _transGraph;
	GPUMallocVec _gpuVec;
	CPUMallocVec _cpuVec;
	MemTransVec _memVec;
	std::unordered_map<std::string, StackPoint> _wrapperReplacements;
	std::map<int64_t, StackPoint> _idPoints;
	RemovePointsPtr _removeCalls;
};

typedef std::shared_ptr<CallTransformation> CallTransPtr;


// TransMap
	// 
