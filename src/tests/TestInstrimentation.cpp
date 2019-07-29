#include <stdio.h>
#include "BPatch.h"
#include "BPatch_addressSpace.h"
#include "BPatch_process.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_point.h"
#include "BPatch_function.h"
#include "BPatch_object.h"
#include "BPatch_snippet.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <cassert>
#include <deque>
#include <sys/time.h>
#include <cstdlib>
#include <sstream>
#include <tuple>
#include <utility> 
#include <stdarg.h>
#include <map>
#include <set> 
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <unistd.h>

// Dyninst includes
#include "CodeObject.h"
#include "CFG.h"
#include "PatchObject.h"
#include "PatchMgr.h"
#include "Point.h"
#include "BPatch_snippet.h"
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_image.h"
#include "BPatch_function.h"
#include "BPatch_Vector.h"
#include "BPatch_point.h"
#include "BPatch_addressSpace.h"
#include "BPatch_statement.h"
#include "dynC.h"
#include "set"

//using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;
#include <map>
#include <vector>
#include <boost/filesystem.hpp>

std::map<std::string, int> _libnameToID;
std::map<int, std::string> _idToLibname;
std::map<std::string, BPatch_object *> _objCache;
BPatch_function * _recordMemAccess;
BPatch_function * _entryFunction;
BPatch_function * _exitingFunction;
struct InstPoints {
	int type;
	uint64_t pos;
	int libid;
};

std::vector<InstPoints> ToCheck;

void ReadInputFile(char * filename) {
	int curId = 0;
	std::ifstream t;
	std::string line;
	t.open(filename,std::ifstream::in);
	while (std::getline(t, line)) {
		std::string item;
		std::vector<std::string> token;
		std::stringstream ss(line);
	    while (std::getline(ss, item, '$')) {
	        token.push_back(item);
	    }
	    int id = curId;
	    assert(token.size() == 3);
	    if (_libnameToID.find(token[1]) != _libnameToID.end()){
	    	id = _libnameToID[token[1]];
	    } else {
	    	_libnameToID[token[1]] = curId;
	    	_idToLibname[curId] = token[1];
	    	curId++;
	    }
	    InstPoints m;
	    m.libid =  id;
	    m.pos = std::stoull(token[2]);
	    m.type = std::stoi(token[0]);
	    ToCheck.push_back(m);
	}
}
BPatch_object * GetObject(BPatch_addressSpace * addr, std::string libname) {
	boost::filesystem::path outName(libname);
	std::string filename = outName.filename().string();
	if (filename.find("libcuda") != std::string::npos)
		filename = std::string("libcuda.so.1");
	if (_objCache.find(filename) != _objCache.end())
		return _objCache[filename];
	// regenerate object cache...
	std::cerr << "Looking for " << filename << std::endl;
	BPatch_image * img = addr->getImage();
	std::vector<BPatch_object *> objects;
	img->getObjects(objects);
	for (auto i : objects){
		boost::filesystem::path lnme(i->pathName());
		_objCache[lnme.filename().string()] =  i;
	}
	assert(_objCache.find(filename) == _objCache.end());
	return _objCache[filename];

}

int FindFuncByName(BPatch_addressSpace * aspace, BPatch_function * & ret, std::string name) {
	if (aspace == NULL) 
		return -1;
	BPatch_image * img = aspace->getImage();
	std::vector<BPatch_function *> tmp;
	img->findFunction(name.c_str(), tmp);
	if (tmp.size() > 0) 
		ret = tmp[0];
	return tmp.size();
}


void InitDyninst(BPatch_addressSpace * addr) {
	addr->loadLibrary("libcuda.so.1");
	addr->loadLibrary("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/lib/libStubLib.so");
	addr->loadLibrary("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/lib/plugins/libSynchTool.so");
	addr->loadLibrary("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/lib/libDriverAPIWrapper.so");
	assert(FindFuncByName(addr, _entryFunction, std::string("RECORD_FUNCTION_ENTRY")) == 1);
	assert(FindFuncByName(addr, _exitingFunction, std::string("RECORD_FUNCTION_EXIT")) == 1);
	assert(FindFuncByName(addr, _recordMemAccess, std::string("SYNC_RECORD_MEM_ACCESS")) == 1);
}

std::vector<BPatch_point *> GetPoints(BPatch_addressSpace * addr, InstPoints point) {
	BPatch_object * obj =  GetObject(addr, _idToLibname[point.libid]);
	uint64_t realAddress = (uint64_t) obj->fileOffsetToAddr(point.pos);
	std::vector<BPatch_point *> points;
	assert(obj->findPoints(realAddress, points));
	if (points.size() > 2) 
		std::cerr << "Multiple functions returned for the same point at id " << std::hex << point.pos << std::dec << " within library - " << _idToLibname[point.libid] << std::endl;
	return points;
}

void InsertInstrimentationAtPoint(BPatch_addressSpace * addr, InstPoints point) {
	std::vector<BPatch_point *> lspoints = GetPoints(addr, point);	
	if (point.type == 0) {
		std::vector<BPatch_snippet*> recordArgs;		// Load Store inst test
		BPatch_snippet * loadAddr = new BPatch_effectiveAddressExpr();
		recordArgs.push_back(loadAddr);
		recordArgs.push_back(new BPatch_constExpr(0));
		BPatch_funcCallExpr recordAddrCall(*_recordMemAccess, recordArgs);		
		addr->insertSnippet(recordAddrCall,lspoints);
	} else if (point.type == 1) {
		//Call Tracing
		std::vector<BPatch_snippet*> recordArgs;
		recordArgs.push_back(new BPatch_constExpr(0));
		BPatch_funcCallExpr entryExpr(*_entryFunction, recordArgs);
		BPatch_funcCallExpr exitExpr(*_exitingFunction, recordArgs);		
		addr->insertSnippet(entryExpr,lspoints, BPatch_callBefore);
		addr->insertSnippet(exitExpr,lspoints, BPatch_callAfter);
	}
}

int main(int argc, const char * argv[]) {
	BPatch bpatch;
	bpatch.setInstrStackFrames(true);
	bpatch.setTrampRecursive(false);
	bpatch.setLivenessAnalysis(false);	
	BPatch_addressSpace * handle  = bpatch.processCreate(argv[2],(const char **)(&(argv[2])));
	assert(handle != NULL);
	bpatch.setLivenessAnalysis(false);
	bpatch.setInstrStackFrames(true);
	bpatch.setTrampRecursive(false);
	ReadInputFile((char *)argv[1]);
	InitDyninst(handle);
	handle->beginInsertionSet();
	for (auto i : ToCheck)
		InsertInstrimentationAtPoint(handle, i);
	handle->finalizeInsertionSet(false);
	return 0;
}
