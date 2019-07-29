#pragma once
#include "Parameters.h"
#include "PluginCommon.h"
#include "LogInfo.h"
#include <iostream>
#include <fstream>    
#include <string>
#include <map>
#include <sstream>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/timer/timer.hpp>
#include <unistd.h>
#include <sys/syscall.h>
#include <cuda.h>
#include <cupti.h>

class CUPTIEventHandler {
private: 
	boost::recursive_mutex _mtx;
	bool _enabled;
	boost::timer::cpu_timer  _runtime;
public: 
	PluginReturn PerformAction(std::shared_ptr<Parameters> params);
	PluginReturn PostTransfer(std::shared_ptr<Parameters> params);
	~CUPTIEventHandler();
	CUPTIEventHandler();
};


#define PLUG_BUILD_FACTORY \
	if (Worker.get() == NULL) { \
		Worker.reset(new CUPTIEventHandler()); \
	} 

#define PLUG_FACTORY_PTR Worker.get()
