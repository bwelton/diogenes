#include <stdio.h>
#include "BPatch.h"
#include "BPatch_addressSpace.h"
#include "BPatch_process.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_point.h"
#include "BPatch_function.h"
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
#include "BPatch_object.h"
#include "BPatch_statement.h"
#include "dynC.h"
#include "set"

//using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

// Pulls all function symbols from binary, places them into textfile. 
#define INTERNAL_SYNC_ST 0x2864C0

int FindFuncByName(BPatch_binaryEdit * aspace, BPatch_function * & ret, std::string name) {
	if (aspace == NULL) 
		return -1;
	BPatch_image * img = aspace->getImage();
	std::vector<BPatch_function *> tmp;
	img->findFunction(name.c_str(), tmp);
	ret = tmp[0];
	return tmp.size();
}

int FindFuncByOffset(BPatch_binaryEdit * app, BPatch_function * & ret, uint64_t offset) {
	BPatch_image * img = app->getImage();
	BPatch_function * func = img->findFunction(offset);
	if (func == NULL)
		return 0;
	ret = func;
	return 1;	
}

void InsertLoadStore(BPatch_binaryEdit * app) {
	std::string functionName = std::string("targ105576c8");
	app->loadLibrary("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/lib/libDriverAPIWrapper.so");
	app->loadLibrary("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/lib/plugins/libSynchTool.so");
	BPatch_function * _recordMemAccess;
	BPatch_function * _editingFunction;
	assert(FindFuncByName(app, _recordMemAccess, std::string("SYNC_RECORD_MEM_ACCESS")) == 1);	
	assert(FindFuncByOffset(app, _editingFunction, 0x105576c8) == 1);
	std::set<BPatch_opCode> axs;
	axs.insert(BPatch_opLoad);
	axs.insert(BPatch_opStore);
	std::vector<BPatch_point*> * loadsAndStores = _editingFunction->findPoint(axs);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_snippet * loadAddr = new BPatch_effectiveAddressExpr();
	recordArgs.push_back(loadAddr);
	recordArgs.push_back(new BPatch_constExpr(0));
	BPatch_funcCallExpr recordAddrCall(*_recordMemAccess, recordArgs);
	assert(app->insertSnippet(recordAddrCall,*loadsAndStores) != NULL);
}


int main(const int argc, const char * argv[]){
	BPatch patch;
	patch.setInstrStackFrames(true);
	BPatch_binaryEdit * app = patch.openBinary(argv[1], false);
	patch.setInstrStackFrames(true);
	InsertLoadStore(app);
	// //patch.setSaveFPR(false);
	// //patch.setTrampRecursive(true);
	// BPatch_image * img = app->getImage();
	// app->loadLibrary("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/lib/plugins/libStacktrace.so");
	// //BPatch_object * obj = app->loadLibrary("libcuda.so.1");
	// std::vector<BPatch_function *> tracerCall;
	// std::vector<BPatch_function *> wrappingCall;
	// img->findFunction("RecordStack", tracerCall);
	// img->findFunction("funcWrapping", wrappingCall);
	// std::vector<BPatch_point*> * entryLocations = wrappingCall[0]->findPoint(BPatch_locEntry);//cuCtxSync[0]->findPoint(BPatch_locExit) //_cudaSync->findPoint(BPatch_locExit);
	// std::vector<BPatch_snippet*> testArgs;
	// BPatch_funcCallExpr recordFuncEntry(*(tracerCall[0]), testArgs);
	// assert(img->getAddressSpace()->insertSnippet(recordFuncEntry,*entryLocations) != false);

	if(!app->writeFile(argv[2])) {
		fprintf(stderr, "Could not write output file %s\n", argv[2]);
		return -1;
	}
}