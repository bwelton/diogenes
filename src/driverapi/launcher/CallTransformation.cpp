#include "CallTransformation.h"
#include "PerformanceModel.h"
#include <cxxabi.h>

CallTransformation::CallTransformation(GPUMallocVec & gpuVec,CPUMallocVec & cpuVec, 
	MemTransVec & memVec, std::map<int64_t, StackPoint> & idPoints, std::unordered_map<std::string, StackPoint> & wrapperReplacements) : _gpuVec(gpuVec), _cpuVec(cpuVec), _memVec(memVec), _idPoints(idPoints), _wrapperReplacements(wrapperReplacements) {

	BuildGraph();
	BuildRequiredSet();
}

std::map<uint64_t, std::vector<StackPoint> > CallTransformation::ReadMemRecorderKeys(std::map<uint64_t, std::string> & typeMap) {
	StackKeyReader r(fopen("DIOENES_MemRecUnknowns.bin","rb"));
	std::map<uint64_t, std::vector<StackPoint> > m = r.ReadStacks();
	std::set<uint64_t> removeEmpty;
	for (auto & i : m) {
		StackPoint tmp = i.second.back();
		i.second.pop_back();
		std::reverse(i.second.begin(), i.second.end());
		for(int x = i.second.size() - 1; x >= 0; x--) {
			if(i.second[x].libname.find("lib/plugins/") != std::string::npos || i.second[x].libname.find("libdyninstAPI") != std::string::npos)
				i.second.pop_back();
			else {
				typeMap[i.first] = tmp.libname;
				std::cerr << "Adding entry " << i.first << "," << tmp.libname << " to type map "<< std::endl;
				i.second.push_back(tmp);
				break;
			}
		}
		if (i.second.size() == 0)
			removeEmpty.insert(i.first);
		std::cerr << "Remade MemRecStack at index " << i.first << std::endl;
		for (auto x : i.second)
			std::cerr << x.libname << "@" << std::hex << x.libOffset << std::endl;
	}	
	for (auto i : removeEmpty)
		m.erase(i);
	return m;
}


std::map<uint64_t, uint64_t> CallTransformation::MatchLStoMR(std::map<uint64_t, std::vector<StackPoint> > & LS, std::map<uint64_t, std::vector<StackPoint> > & MR) {
	std::map<uint64_t, uint64_t> ret;
	MatchLoadStoreStacksRecursive LSRecursive;
	for (auto i : LS) {
		std::cerr << "LS Stack ID = " << std::dec << i.first << "(HEX=" << std::hex << i.first << ")" << std::dec << std::endl;
		for (auto x : i.second) {
			std::cerr << x.libname << "@" << x.libOffset << std::endl;
		}
		LSRecursive.InsertEntry(i.second, 0, i.first);
	}

	for (auto i : MR) {
		std::cerr << "MR Stack ID = " << std::dec << i.first << "(HEX=" << std::hex << i.first << ")" << std::dec << std::endl;
		for (auto x : i.second) {
			std::cerr << x.libname << "@" << x.libOffset << std::endl;
		}	
	}
	
	for (auto i : MR) {
		auto tmpstore = i.second.back();
		if (tmpstore.libname == "__libc_free" || tmpstore.libname == "__GI___libc_malloc")
			continue;
		i.second.pop_back();
		uint64_t tmp = LSRecursive.FindEntry(i.second, 0);
		i.second.push_back(tmpstore);
		std::cout << "[CallTransformation::MatchLStoMR] MR " << std::dec << i.first << " has matched with LR " << std::dec << tmp << std::endl;
		if (tmp == 0){
			std::cout << "[CallTransformation::MatchLStoMR] WARNING COULD NOT MATCH MR " << std::dec << i.first << std::endl;
			continue;
		}
		if (ret.find(tmp) != ret.end()){
			std::cout << "[CallTransformation::MatchLStoMR] WARNING DUPLICATE EXISTS AT " << std::dec << i.first << " matched with LR " << std::dec << tmp << std::endl;
			std::cout << "[CallTransformation::MatchLStoMR] ORIGINAL VALUES! " << std::dec << ret[tmp] << " matched with LR " << std::dec << tmp << std::endl;
		}
		ret[tmp] = i.first;
	}	
	return ret;

}

std::string GetFileLineString(StackPoint & p) {
	std::stringstream ret;
	int status;
	if(p.lineNum > 0 && p.fileName != std::string("")) {
		char * demang = abi::__cxa_demangle(p.funcName.c_str(), 0,0, &status);
		if (status == 0){
			ret << demang <<  " at line " << std::dec << p.lineNum << " in " << p.fileName << " [DEBUG OFFSET = " << p.libOffset << "]";
			free(demang);
		} else {
			ret << p.funcName <<  " at line " << std::dec << p.lineNum << " in " << p.fileName << " [DEBUG OFFSET = " << p.libOffset << "]";
		}
	} else {
		ret << "Offset " << p.libOffset << " in " << p.libname;
	}
	return ret.str();
}
void CallTransformation::PrintStackSet(std::stringstream & outdata, std::string header, std::vector<StackPoint> & sp, int count) {
	int printCount = 0;
	for (int i  = sp.size() - 1; printCount < count && i >= 0; i--, printCount++){
		outdata << header << GetFileLineString(sp[i]) << std::endl;
	}

}
void CallTransformation::BuildRequiredSet() {
	int printStackSize = 20;
	StackKeyReader r(fopen("LS_stackkey.txt","rb"));
	std::map<uint64_t, std::string> typeMap;
	std::map<uint64_t, std::vector<StackPoint> > LS = r.ReadStacks();
	std::map<uint64_t, std::vector<StackPoint> > MR = ReadMemRecorderKeys(typeMap);
	std::map<uint64_t, uint64_t> matchSet = MatchLStoMR(LS,MR);

	LSDependencyVec lvec;
	ReadDependencyFile dep(fopen("LS_syncaccess.bin", "rb"));
	dep.Read(lvec);


	ReadLSTraceDepFile traceDep(fopen("LS_trace.bin", "rb"));
	traceDep.Read();

	std::set<uint64_t> required; 
	std::set<uint64_t> notRequired; 
	std::set<uint64_t> couldNotFindStack;
	// First pass: cudaFreeChecks
	for (size_t i = 0; i < lvec.size(); i++) {
		auto tmp = lvec[i];
		bool syncNotNeeded = false;

		if (!tmp->newDependents) {
			syncNotNeeded = true;
		} else {
			if (i < lvec.size() - 1) {
				if (!(traceDep.IsInSet(tmp->id))){
					syncNotNeeded = true;
				}
			} 			
		}
		if (syncNotNeeded) {
			if (required.find(tmp->id) != required.end())
				continue;
			auto it =  matchSet.find(tmp->id);
			if (it != matchSet.end()) {
				//std::cerr << "Checking matchset" << std::endl;
				if (typeMap[it->second].find("cuMemFree") != std::string::npos || 
					typeMap[it->second].find("cudaFree") != std::string::npos) {
					notRequired.insert(tmp->id);
				}
			} else {
				couldNotFindStack.insert(tmp->id);
			}
		} else {
			if(notRequired.find(tmp->id) != notRequired.end())
				notRequired.erase(tmp->id);
			required.insert(tmp->id);
		}
	}

	// Outpur list of remedies
	PerformanceModel mn;

	// Second pass: cudaMemcpy/cudaMemcpyAsync
	std::stringstream reqSet;
	std::set<int64_t> alreadyTranslated;
	for (size_t i = 0; i < lvec.size(); i++) {
		auto tmp = lvec[i];
		bool syncNotNeeded = false;
		if (!tmp->newDependents) {
			syncNotNeeded = true;
		} else {
			if (i < lvec.size() - 1) {
				if (!(traceDep.IsInSet(tmp->id))){ //&& notRequired.find(lvec[i+1]->id) == notRequired.end()){
					//required.insert(lvec[i+1]->id);
					syncNotNeeded = true;
				}
			} 			
		}
		if (syncNotNeeded) {
			if (required.find(tmp->id) != required.end())
				continue;
			auto it =  matchSet.find(tmp->id);
			if (it != matchSet.end()) {
				//std::cerr << "Checking matchset" << std::endl;
				if (typeMap[it->second].find("cuMemcpy") != std::string::npos || 
					typeMap[it->second].find("cudaMemcpy") != std::string::npos) {
					notRequired.insert(tmp->id);
				}
			} else {
				couldNotFindStack.insert(tmp->id);
			}
		}else {
			if(notRequired.find(tmp->id) != notRequired.end())
				notRequired.erase(tmp->id);
			required.insert(tmp->id);
			auto it =  matchSet.find(tmp->id);
			if (it == matchSet.end())
				continue;
			if (alreadyTranslated.find(matchSet[tmp->id]) == alreadyTranslated.end())
				mn.TranslateStackRecords(MR[matchSet[tmp->id]]);
			else 
				continue;
			alreadyTranslated.insert(matchSet[tmp->id]);

			reqSet << "Required Synchronization at " << std::endl;
			//utRemedies << type << " called at... " << std::endl;
			PrintStackSet(reqSet, std::string("\t\t"),MR[matchSet[tmp->id]], printStackSize);	
		}
	}	

	for(auto i : couldNotFindStack) {
		std::cerr << "[CallTransformation::BuildRequiredSet] Could not find a stack that matched with LS stack id " << i << std::endl;
	}

	for (auto i : notRequired) {
		std::cerr << "[CallTransformation::BuildRequiredSet] Following synchronization is NOT required: " << typeMap[matchSet[i]] <<  " at LS stack = " << matchSet[i] << std::endl;
	}

	StackKeyWriter outputKeys(fopen("AC_AutoCorrectStacks.txt", "wb"));


	for (auto i : required) {
		if (notRequired.find(i) != notRequired.end())
			notRequired.erase(i);
	}




	std::stringstream outRemedies;
	//std::set<int64_t> alreadyTranslated;

	for (auto i : notRequired) {	
		std::string type = typeMap[matchSet[i]];
		if (type.find("cuMemcpy") != std::string::npos || type.find("cudaMemcpy") != std::string::npos) {
			TransferPointPtr trans = _transGraph.ReturnTransfer(matchSet[i]);
			bool MarkSkipped = false;
			if (trans == NULL)
				continue;
			if (trans->GetMallocSites().size() == 0 && type.find("cuMemcpyHtoD") == std::string::npos){
				required.insert(i);
				MarkSkipped = true;
				//continue;
			}
			for (auto m : trans->GetMallocSites()) {
				if (m->id == -1 && type.find("cuMemcpyHtoD") == std::string::npos){
					required.insert(i);
					MarkSkipped = true;
					//continue;
				}
			}
			if (MarkSkipped)
				continue;
			MallocSiteSet memSiteSet = trans->GetMallocSites();
			if (alreadyTranslated.find(matchSet[i]) == alreadyTranslated.end())
				mn.TranslateStackRecords(MR[matchSet[i]]);
			alreadyTranslated.insert(matchSet[i]);
			//int printCount = 0;
			outRemedies << "(LS ID = " << i << " MR ID = " << matchSet[i] << " ) " << type << " called at... " << std::endl;
			PrintStackSet(outRemedies, std::string("\t\t\t"),MR[matchSet[i]], printStackSize);

			//for (int j = MR[matchSet[i]].size() - 1; printCount < printStackSize && j >= 0; j--, printCount++)
			//	outRemedies << type << " called at " << GetFileLineString(MR[matchSet[i]][j]) << std::endl;
			if (type.find("Async") != std::string::npos)
				outRemedies << "\tProblem: Unnecessary synchronization caused by non-pinned CPU memory in transfer (use cudaMallocHost)\n";
			else 
				outRemedies << "\tProblem: Unnecessary synchronous transfer, replace with cudaMemcpyAsync and pin CPU memory address\n";
			for (auto n : memSiteSet) {
				if (n->id == -1)
					continue;
				if (alreadyTranslated.find(n->id) == alreadyTranslated.end())
					mn.TranslateStackRecords(MR[n->id]);
				alreadyTranslated.insert(n->id);

				outRemedies << "\t\tPin non-pinned CPU Memory Allocated At " << std::endl;
				PrintStackSet(outRemedies, std::string("\t\t\t"),MR[n->id], printStackSize);

				// for (int index = MR[n->id].size() - 2; index >= 0; index--) {
				// 	if (MR[n->id][index].libname.find("libstdc++") == std::string::npos){
				// 		outRemedies << "\t\tPin non-pinned CPU Memory Allocated At " <<  GetFileLineString(MR[n->id][index])  << std::endl;
				// 		break;
				// 	}
				// }

				//outRemedies << "\t\tPin non-pinned CPU Memory Allocated At " <<  GetFileLineString(MR[n->id][MR[n->id].size()-2])  << std::endl;
				for (auto k : n->children) {
					if (k->id == -1)
						continue;
					if (alreadyTranslated.find(k->id) == alreadyTranslated.end())
						mn.TranslateStackRecords(MR[k->id]);
					alreadyTranslated.insert(k->id);


					outRemedies << "\t\t\tCPU memory freed at " << std::endl;
					PrintStackSet(outRemedies, std::string("\t\t\t\t"),MR[k->id], printStackSize);

					// for (int index = MR[k->id].size() - 2; index >= 0; index--) {
					// 	if (MR[k->id][index].libname.find("libstdc++") == std::string::npos){
					// 		//outRemedies << "\t\tPin non-pinned CPU Memory Allocated At " <<  GetFileLineString(MR[l->id][index])  << std::endl;
					// 		outRemedies << "\t\t\tCPU memory freed at " <<  GetFileLineString(MR[k->id][index])  << std::endl;
					// 		break;
					// 	}
					// }



					//outRemedies << "\t\t\tCPU memory freed at " <<  GetFileLineString(MR[k->id].back())  << std::endl;
				}
			}
		} else {
			FreeSitePtr msite = _gpuGraph.GetFreeSite(matchSet[i]);
			if (msite == NULL)
				continue;
			if (alreadyTranslated.find(matchSet[i]) == alreadyTranslated.end())
				mn.TranslateStackRecords(MR[matchSet[i]]);
			//outRemedies << type << " called at " << GetFileLineString(MR[matchSet[i]].back()) << std::endl;

			outRemedies << type << " called at... " << std::endl;
			PrintStackSet(outRemedies, std::string("\t\t\t"),MR[matchSet[i]], printStackSize);

			//int printCount = 0;
			//for (int j = MR[matchSet[i]].size() - 1; printCount < printStackSize && j >= 0; j--, printCount++)
			//	outRemedies << type << " called at " << GetFileLineString(MR[matchSet[i]][j]) << std::endl;
			outRemedies << "\tProblem: Unnecessary Synchronization at CudaFree. Consider using a custom memory allocator" << std::endl;
			for(auto n : msite->parents) {
				if (n->id == -1)
					continue;
				if (alreadyTranslated.find(n->id) == alreadyTranslated.end())
					mn.TranslateStackRecords(MR[n->id]);
				outRemedies << "\t\tGPU Malloc Site called at" << std::endl;
				PrintStackSet(outRemedies, std::string("\t\t\t"),MR[n->id], printStackSize);
				/*for (int index = MR[n->id].size() -2; index >= 0; index--){
					if (MR[n->id][index].libname.find("cudart") == std::string::npos){
						outRemedies << "\t\tGPU Malloc Site called at " <<  GetFileLineString(MR[n->id][index])  << std::endl;
						break;
					}
				}*/
			}
		}
	}
	// for (auto i : required) {
	// 	if (notRequired.find(i) != notRequired.end())
	// 		notRequired.erase(i);
	// }
	HostToDeviceLimiter transLimiter(fopen("AC_translimit.bin","wb"));

	for (auto i : notRequired) {
		MR[matchSet[i]].pop_back();
		outputKeys.InsertStack(matchSet[i], MR[matchSet[i]]);
		if (required.find(i) != required.end()) {
			transLimiter.WriteLimiter(true);
		} else {
			transLimiter.WriteLimiter(false);
		}
	}

	// Everything else
	for (auto & i : LS) {
		if (matchSet.find(i.first) != matchSet.end()) 
			continue;
		if (alreadyTranslated.find(i.first) == alreadyTranslated.end())
			mn.TranslateStackRecords(i.second);		
		if (!(traceDep.IsInSet(i.first))) {
			outRemedies << "Remove Unnecessary Synchronization at " << std::endl;
			//utRemedies << type << " called at... " << std::endl;
			PrintStackSet(outRemedies, std::string("\t\t"),i.second, printStackSize);
			// int printCount = 0;
			// for (int j = i.second.size() - 1; printCount < printStackSize && j >= 0; j--, printCount++)
			// 	outRemedies << " called at " << GetFileLineString(i.second[j]) << std::endl;
		} else {
			reqSet << "Required Synchronization at " << std::endl;
			//utRemedies << type << " called at... " << std::endl;
			PrintStackSet(reqSet, std::string("\t\t"),i.second, printStackSize);		

		}
	}


	// Print the required set for debugging
	// outRemedies << "REQUIRED SET BELOW [DEBUG]" << std::endl;
	// for (auto i : required) {


	// }

	std::cout << outRemedies.str() << std::endl;
	std::cout << " REQUIRED SYNCS [DEBUG]" << std::endl;
	std::cout << reqSet.str() << std::endl;

	// 	// LSStackGraphVec sgraph;
// 	// std::map<uint64_t, int> _mapToSgraph;
// 	// int pos = 0;
// 	// for (auto i : LS) {
// 	// 	sgraph.push_back(LSStackGraph(i.second, i.first));
// 	// 	_mapToSgraph[i.first] = pos;
// 	// 	if (matchSet.find(i.first) != matchSet.end())
// 	// 		sgraph.back()._idPointID = matchSet[i.first];
// 	// 	pos++;
// 	// }

// 	/*
// 	StackPointTree tree(_idPoints);
// 	for (auto & i : sgraph) {
// 		if(i._found == true){
// 			int64_t id = tree.FindID( i._beforeLibcuda);
// 			i._idPointID = id;
// 			if (id >= 0) {
// 				std::cerr << "[CallTransformation::BuildRequiredSet] Found ID Match for Call - " << i._beforeLibcuda.libname << "@" << i._beforeLibcuda.libOffset << " [ID = " << id << "]" << std::endl;
// 			} else {
// 				std::cerr << "[CallTransformation::BuildRequiredSet] NO MATCH Match for Call - " << i._beforeLibcuda.libname << "@" << i._beforeLibcuda.libOffset << " [ID = " << id << "]" << std::endl;			
// 			}
// 		}
// 	}*/

// 	for (size_t i = 0; i < lvec.size(); i++) {
// 		auto tmp = lvec[i];
// 		if (tmp->newDependents) {
// 			if (i < lvec.size() - 1) {
// 				if (!traceDep.IsInSet(lvec[i+1]->id))
// 					continue;
// 			} 
// 			auto it = _mapToSgraph.find(tmp->id);
// 			if (it != _mapToSgraph.end()) {
// 				sgraph[it->second]._required = true;
// 			}
// 		}
// 	}
// /*
// 	for (auto i : lvec) {
// 		if(i->newDependents) {
// 			auto it = _mapToSgraph.find(i->id);
// 			if (it != _mapToSgraph.end()) {
// 				sgraph[it->second]._required = true;
// 				//std::cerr << "[CallTransformation::BuildRequiredSet] " 
// 			}
// 		}
// 	}
// */
// 	std::vector<FreeSitePtr> freeSites;

// 	for (auto i : sgraph) {
// 		if (i._idPointID > 0) {
// 			FreeSitePtr tmp = _gpuGraph.GetFreeSite(i._idPointID);
// 			if (tmp != NULL) {
// 				if (i._required == true) {
// 					tmp->required = 1;
// 					freeSites.push_back(tmp);
// 				} else {
// 					tmp->required = 0;
// 					freeSites.push_back(tmp);
// 				}
// 			}
// 		}
// 	}
// 	std::cerr << _gpuGraph.PrintRequiredSetFree() << std::endl;
// 	std::vector<MallocPtr> mallocSites;	
// 	_gpuGraph.TraverseFromPoint<FreeSitePtr,MallocPtr>(freeSites, mallocSites);

// 	RemovePointsPtr ret(new RemovePoints());
// 	for (auto i : freeSites) {
// 		if (i->required != 0) {
// 			ret->cudaFreeReqSync.push_back(i->p);
// 			std::cout << "[REQ]" << i->Print() << std::endl;
// 		} else {
// 			ret->cudaFreeReplacements.push_back(i->p);
// 			std::cout << "[REMOVE]" << i->Print() << std::endl;
// 		}
// 	}
	
// 	for (auto i : mallocSites) {
// 		std::cout << "[WRAP]" << i->Print() << std::endl;
// 		ret->cudaMallocReplacements.push_back(i->p);
// 	}


// 	// cudaMemTransfer replacement
// 	for (auto i : _transGraph.transfers) {
// 		if (i.first == -1)
// 			continue;
// 		if(ret->CheckArrayAndAddToIndex(CUMEMCPY_REP, i.second->p) == NEW_ENTRY) {
// 			ret->cudaMemcpyAsyncRepl.push_back(i.second->p);
// 			MallocSiteSet tmp = i.second->GetMallocSites();
// 			for (auto n : tmp) {
// 				if(ret->CheckArrayAndAddToIndex(MALLOC_REP, n->p) == NEW_ENTRY && n->IsEmpty() == false) 
// 					ret->mallocReplacements.push_back(n->p);
// 			}
// 		}
// 	}

// 	std::cout << ret->Print() << std::endl;
// 	_removeCalls = ret;	
}


RemovePointsPtr CallTransformation::GetRemoveCalls() {
	return _removeCalls;
}

void CallTransformation::BuildGraph() {
	std::map<uint64_t, std::string> typeMap;
	auto MR = ReadMemRecorderKeys(typeMap);
	std::map<int64_t, StackPoint> idPointKeys;
	for (auto i : MR) {
		idPointKeys[int64_t(i.first)] = i.second.back();
	}
	BuildMemoryGraph(_cpuVec,idPointKeys, _cpuGraph);
	BuildMemoryGraph(_gpuVec,idPointKeys, _gpuGraph);
	for (auto i : _memVec) {
		_transGraph.AddTransfer(i->copyID, i->allocSite, i->count, _cpuGraph, idPointKeys);
	}
	std::cerr << _cpuGraph.PrintMemoryGraph() << std::endl;
	std::cerr << _gpuGraph.PrintMemoryGraph() << std::endl;
	_gpuGraph.CheckForExitFrees();
	std::cerr << _transGraph.PrintTransferGraph() << std::endl;
}
