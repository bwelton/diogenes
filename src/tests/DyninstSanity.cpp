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
#include <mutex>

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
#include "dynC.h"
#include "set"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;
BPatch bpatch;

BPatch_addressSpace * LaunchProcess(const char * name, const char * argv[]) {
	BPatch_addressSpace * handle = NULL;
	handle = bpatch.processCreate(name, argv);
	assert(handle != NULL);
	return handle;
}

void does_process_create_work(const char * name, const char * argv[]) {
	BPatch_addressSpace * addrs = LaunchProcess(name, argv);
	BPatch_process* appProc = dynamic_cast<BPatch_process*>(addrs);
	assert(appProc != NULL);
	appProc->continueExecution();
	while (!appProc->isTerminated()) {
		bpatch.waitForStatusChange();
		appProc->continueExecution();
	}	
	std::cout << "Process Attach appears to work...." << std::endl;
}


int main(int argc, const char * argv[]) {
	does_process_create_work(argv[1], &(argv[1]));
}