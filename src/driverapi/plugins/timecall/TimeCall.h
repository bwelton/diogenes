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
#include <fstream>
#include <numeric>
#include <chrono>
#include "StackwalkingCommon.h"
#include "StackPoint.h"
#include "TFReaderWriter.h"
extern "C" {
	void init(std::vector<std::string> & cmd_list);
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
}

class TimeCall {
public:
	TimeCall(std::vector<std::string> & cmd_list);
	~TimeCall();
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
private:
	std::set<CallID> _callsToMonitor;
	std::vector<std::tuple<const char *,size_t, double> > _timers;
};

#define PLUG_BUILD_FACTORY(param) \
	if (Worker.get() == NULL) { \
		Worker.reset(new TimeCall(param)); \
	} 

#define PLUG_FACTORY_PTR Worker.get()
