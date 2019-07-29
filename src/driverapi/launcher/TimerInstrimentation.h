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
#include "dynC.h"
#include "set"
#include "LogInfo.h"
#include "Constants.h"
#include "StackPoint.h"
#include "StackStorage.h"
#include "ReadStackKeys.h"
#include "TFReaderWriter.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;


class TimerInstrimentation {
public:
	TimerInstrimentation(std::shared_ptr<DyninstProcess> proc);
	void InsertTimers(StackRecMap & recs);
	void PostProcessing(StackRecMap & recs);
private:
	void FindUniqueCudaFuncs(StackRecMap & recs, std::vector<BPatch_function *> & ret);
	void InsertTimer(BPatch_function * func, uint64_t ident);

	StackPoint GetPointFromBpatchFunc(BPatch_function * func);
	std::map<uint64_t, StackPoint> _idToFunc;

	std::set<BPatch_function*> _processed;
	std::shared_ptr<DyninstProcess> _proc;
	std::map<uint64_t, std::string> _idToCallname;
	BPatch_object * _libcuda;
	BPatch_object * _libtime; 


};
