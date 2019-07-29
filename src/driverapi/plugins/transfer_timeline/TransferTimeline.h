#pragma once
#include "Parameters.h"
#include "MemoryTransfer.h"
#include "PluginCommon.h"
#include "xxhash32.h"
#include "LogInfo.h"
#include <iostream>
#include <fstream>    
#include <string>
#include <map>
#include <sstream>
#include <boost/thread/recursive_mutex.hpp>
#include <unistd.h>
#include <sys/syscall.h>

extern "C" {
	void init(std::vector<std::string> & cmd_list);
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
}

class TransferTimeline {
public:
	TransferTimeline(std::vector<std::string> & cmd_list);
	~TransferTimeline();
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
	void WriteLog(std::shared_ptr<Parameters> params);
	void SetThreadLocals();
private:
	std::shared_ptr<LogInfo> _tt_log;
	// std::map<int,std::string> _cmdToName;
	// size_t callcount;
	// std::map<CallID, bool> _transferCalls;
	boost::recursive_mutex _timeline_mtx;
	// std::shared_ptr<LogInfo> _timeline_log;
	std::map<uint32_t, uint64_t> _pastTransfers;
	// uint64_t _count; 
	// bool _enabled;
};

#define PLUG_BUILD_FACTORY(param) \
	if (Worker.get() == NULL) { \
		Worker.reset(new TransferTimeline(param)); \
	} 

#define PLUG_FACTORY_PTR Worker.get()