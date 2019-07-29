#pragma once
#include "Parameters.h"
#include "MemoryTransfer.h"
#include "PluginCommon.h"
#include "LogInfo.h"
#include <iostream>
#include <fstream>    
#include <string>
#include <map>
#include <sstream>
#include <boost/thread/recursive_mutex.hpp>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include <sstream>
#include "MemoryRange.h"
#include "HelperFunctions.h"
#include <stdlib.h>
#include "framestepper.h"
#include "walker.h"
#include "procstate.h"
#include "swk_errors.h"
#include "steppergroup.h"
#include "frame.h"
#include "sw_c.h"
#include "Symtab.h"
#include "BPatch.h"
#include "BPatch_process.h"
#include <chrono>
#include "MemoryRange.h"
#include "CheckAccesses.h"
#include "LoadStoreDriver.h"
using namespace Dyninst;
using namespace Dyninst::Stackwalker;
using namespace SymtabAPI;
#define SYNCH_DEBUG 1

extern "C" {
	void init(std::vector<std::string> & cmd_list);
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
}

class SynchTool {
public:
	SynchTool(std::vector<std::string> & cmd_list);
	~SynchTool();
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
	void UnifiedAllocation(std::shared_ptr<Parameters> params);
	void GetLiveTransfer(std::shared_ptr<Parameters> params);
	void FreeMemoryAllocation(std::shared_ptr<Parameters> params);
private:
	std::shared_ptr<LogInfo> _sync_log;
	std::map<uint64_t, MemoryRange> _ranges;
	std::map<uint64_t, size_t> _protectedMem;
	// std::map<int,std::string> _cmdToName;
	// size_t callcount;
	// std::map<CallID, bool> _transferCalls;
	boost::recursive_mutex _sync_mtx;
	// std::shared_ptr<LogInfo> _timeline_log;
	std::map<uint32_t, uint64_t> _pastTransfers;
	std::set<CallID> _callsContainingSynch;
	std::vector<std::string> _cmd_list;
	// uint64_t _count; 
	// bool _enabled;
};

#define PLUG_BUILD_FACTORY(param) \
	if (Worker.get() == NULL) { \
		Worker.reset(new SynchTool(param)); \
	} 

#define PLUG_FACTORY_PTR Worker.get()
