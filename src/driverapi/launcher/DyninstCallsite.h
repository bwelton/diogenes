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
// #include "DyninstFunction.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

class DyninstCallsite {
public: 
	DyninstCallsite(std::shared_ptr<DyninstProcess> proc, BPatch_function * func, BPatch_point & point);
	void ReplaceFunctionCall(BPatch_function * _newCall);
	std::string * GetCalledFunction();
	uint64_t GetPointAddress();
	uint64_t GetPointFileAddress();
	uint64_t ReplaceFunctionCallWithID(BPatch_function * _newCall,int64_t id);
	StackPoint GetStackPoint();
private:
	std::shared_ptr<DyninstProcess> _proc;
	BPatch_function * _func;
	BPatch_point _point;
	std::string _calledFunc;
	bool _replaced;
	uint64_t _libOffsetAddr;
	std::string _libName;
	std::string _funcName;
};

