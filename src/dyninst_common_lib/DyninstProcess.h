#pragma once
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
#include <unistd.h>

// #include "Common.h"

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
#include "dynC.h"
#include "set"

#include "DynHelper.h"
//#include "Constants.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

namespace DiogenesCommon {
class DyninstProcess {
public:
	DyninstProcess(int argc, char ** argv);
	BPatch_addressSpace * LaunchProcess();
	BPatch_addressSpace * GetAddressSpace();
	bool RunUntilCompleation(std::string filename = std::string(""));
	BPatch_object * LoadLibrary(std::string library);
	void BeginInsertionSet();
	void DetachForDebug();
	void BreakAtMain();
private:
	bool IsMPIProgram();
	BPatch_addressSpace * LaunchMPIProcess();
	char ** _argv;
	int _argc;
	bool _debug;
	bool _MPIProc;
	bool _insertedInit;
	BPatch_addressSpace * _aspace;
	bool _openInsertions;
	
};
}