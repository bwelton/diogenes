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
#include "DyninstCallsite.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

struct InstStats { 
	uint64_t callTracedInsts;
	uint64_t lsInsts;
	std::set<std::string> ct_instNames;
	std::set<std::string> ls_instNames;
};


class DyninstFunction {
public:
	DyninstFunction(std::shared_ptr<DyninstProcess> proc, BPatch_function * func, std::shared_ptr<InstrimentationTracker> tracker, std::shared_ptr<BinaryLocationIDMap> bmap);
	void EntryExitWrapping();
	std::string PrintInst(InstStats & stats);
	void InsertLoadStoreAnalysis();
	void GetFuncInfo(std::string & libName, std::string & funcName);
	uint64_t HandleEmulated(BPatch_basicBlock * block);
	bool GenExclusionSet(std::set<uint64_t> & excludedAddress);
	void InsertTimingAtPoint(StackPoint p);
	void GetCallsites(std::vector<DyninstCallsite> & ret);
	bool IsExcludedFunction(InstType T);
private: 
	uint64_t GetSmallestEntryBlockSize();
	DyninstFunctionWraps _wrapper;
	std::shared_ptr<DynOpsClass> _ops;
	std::shared_ptr<InstrimentationTracker> _track;
	std::shared_ptr<DyninstProcess> _proc;
	std::set<BPatch_basicBlock *> _bblocks;
	std::shared_ptr<BinaryLocationIDMap> _bmap;
	std::map<uint64_t, std::pair<Dyninst::InstructionAPI::Instruction, BPatch_basicBlock *>  > _instmap;
	std::map<uint64_t, std::tuple<int,int,uint64_t>> _insertedInst;
	BPatch_function * _func;
	BPatch_object * _obj;
	uint64_t _entrySize;
	bool _exitEntryDone;
	bool _lsDone;
	bool _addedLS;
	bool _addedTRAC;
};
