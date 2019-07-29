#pragma once
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
#include <queue>
#include <boost/program_options.hpp>

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
#include "LogInfo.h"
#include "Constants.h"
#include "StackPoint.h"
#include "Symbol.h"
#include "Module.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

#define INTERNAL_SYNC_ST 0x2B7E28
typedef std::pair<uint64_t, BPatch_function *> FuncIDPair;
class StacktraceInst {
public:
	StacktraceInst(BPatch_addressSpace * addrSpace, BPatch_image * img);
	void Setup();
	void InsertStackInst();
private:
	BPatch_function * _wrapperFunc;
	BPatch_function * _cudaSync;
	Dyninst::SymtabAPI::Symbol * _wrapSym;

	BPatch_addressSpace * _addrSpace;
	BPatch_image * _img;
	// Stores the current id
	uint64_t _curID;
	// Offset to <bpatch_function, id> map
	std::map<uint64_t, FuncIDPair > _idMap; 
	std::set<uint64_t> _alreadyInstrimented; 
};