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
#include <memory>
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
#include "LoadStoreInst.h"
#include "StacktraceInst.h"
#include "StackStorage.h"
#include "TimeFirstUse.h"
#define INTERNAL_SYNC_C 0x2B7E28
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;


class ProcessController {
public:
	ProcessController(boost::program_options::variables_map vm, LogInfo * log );
	BPatch_addressSpace * LaunchProcess();
	BPatch_addressSpace * LaunchProcessInstrimenter(std::string WrapperDef);
	BPatch * GetBPatch();
	void InsertFirstUse(std::vector<uint64_t> & skips, uint64_t & instUntil, std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks);
	void Run();
	void RunWithTimeout(int timeout);
	void InsertTimers(std::vector<StackPoint> points);
	void InsertLoadStores(std::vector<uint64_t> & skips, uint64_t & instUntil, std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks);
	void InsertLoadStoreSingle(std::string funcName);
	bool IsObjectInList(std::vector<std::string> li, BPatch_object * obj);
	bool LoadWrapperLibrary(std::string libname);
	bool IsFunctionInList(std::vector<std::string> li, std::string functionName);
	bool InsertInstrimentation(std::string WrapperDef);
	void ReadDefinition(std::string WrapperDef);
	void InstrimentApplication();
	std::set<std::string> WrapperLibraries();
	void InsertWrapperDef(std::string type, std::string origName, std::string wrapperFunc, std::string lib, std::string origSymbol);
	bool IsTerminated();
	bool ContinueExecution();
	bool IsStopped();
	void InsertBreakpoints(std::vector<std::string> functionNames);
	std::map<uint64_t, std::vector<StackPoint> > GetThreadStacks();
	BPatch_addressSpace * GenerateDebugBinary(std::string bin);
	void WriteOutput(std::string outputName);
	void GetModules(std::map<std::string, BPatch_object *> & objs);
	void DontFinalize();
	void InsertStacktracing();
	std::map<uint64_t, StackPoint> GetFirstUse();
	void InsertAPICapture();
	BPatch_addressSpace * LaunchMPIProcess();
	//void LibraryLoadCallback(BPatch_thread * thread, BPatch_object * obj, bool l);
private:
	std::shared_ptr<LoadStoreInst> _loadStore;
	std::shared_ptr<TimeFirstUse> _timeFirstUse;
	LogInfo * _log;
	double _instrimentationOverhead;
	boost::program_options::variables_map _vm;
	BPatch_addressSpace * _addrSpace;
	bool _launched;
	bool _insertedInstrimentation;
	bool _terminated;
	std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > _wrapFunctions;
	BPatch_process * _appProc;
	std::map<std::string, BPatch_object *> _loadedLibraries;
	std::map<uint64_t, std::string> _idToFunction;
	bool _binaryEdit;
	BPatch_binaryEdit * _appBE;
	bool _dontFin;

	bool _WithLoadStore;
	StacktraceInst * _stackTracer;

	bool _ProcessAttached;
};