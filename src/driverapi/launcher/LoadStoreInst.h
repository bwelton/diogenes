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
#include "BPatch_basicBlock.h"
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
#include "BinaryLocationIDMap.h"
#include "Constants.h"
#include "StackPoint.h"
#include "StackStorage.h"
#include "DynOpsClass.h"
#include "InstrimentationTracker.h"
#include "ReadLoadStoreFiles.h"
#include <limits.h>

using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

#define INTERNAL_SYNC_LS 0x2B7E28


namespace std
{
    template<> struct less<BPatch_object::Region>
    {
       bool operator() (const BPatch_object::Region& lhs, const BPatch_object::Region& rhs) const
       {
           return (uint64_t)lhs.base < (uint64_t)rhs.base;
       }
    };
}


//class DynOpsClass;

class LoadStoreInst {
public: 
	~LoadStoreInst();
	void FixLSStackFiles();
	LoadStoreInst(BPatch_addressSpace * addrSpace, BPatch_image * img);
	bool InstrimentAllModules(bool finalize, std::vector<uint64_t> & skips, uint64_t & instUntil, std::vector<std::string> & syncFunctions, std::vector<StackPoint> & points,std::map<uint64_t, StackRecord> & syncStacks);
	bool InstrimentNewModule(BPatch_object * obj, bool finalize);
	void Finalize();
	void BeginInsertionSet();
	void SetWrappedFunctions(std::vector<std::string> & wrappedFunctions);
	std::map<uint64_t, StackPoint> _firstUses;
	void WrapEntryAndExit(std::map<uint64_t, StackRecord> & syncStacks);
	void InsertSyncNotifierSnippet(BPatch_function * func, uint64_t offset);
	void InsertSyncCallNotifier();
	void InsertLoadStoreSnippets(BPatch_function * func, std::vector<BPatch_point*> * points);
	void InsertLoadStoreInstrimentation();
	void InsertGotchaEntries();
	void AddSpecialCase(std::string functionName, std::string libName, uint64_t id);
private:
	void Setup();
	void InsertEntryExitSnippets(BPatch_function * func, std::vector<BPatch_point*> * points);
	BPatch_image * _img;
	BPatch_addressSpace * _addrSpace;
	bool _started;
	bool _runOneTime;
	std::vector<std::string> _wrappedFunctions;
	uint64_t _funcId;
	std::map<uint64_t, std::string> _idToFunction;

	BPatch_function * _enterSync;
	BPatch_function * _recordMemAccess;
	BPatch_function * _entryFunction;
	BPatch_function * _exitingFunction;
	BPatch_function * _libcudaSync;
	int _debugPrinting;
	std::ofstream _logFile;
	DynOpsClass _dynOps;
	BinaryLocationIDMap _binLoc;
	InstrimentationTracker _instTracker;
	std::map<BPatch_object::Region, std::pair<std::string,std::string> > _regionToLibname;
};