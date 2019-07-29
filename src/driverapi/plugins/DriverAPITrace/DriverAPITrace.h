#pragma once
#include "Parameters.h"
#include "MemoryTransfer.h"
#include "PluginCommon.h"
#include "LogInfo.h"
#include <iostream>
#include <fstream>    
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <boost/thread/recursive_mutex.hpp>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include <sstream>
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
#include "StackPoint.h"
#include "StackwalkingCommon.h"

struct OutputFile {
	FILE * outFile;
	OutputFile(std::string filename) {
		outFile = fopen(filename.c_str(),"w");
	}

	~OutputFile() {
		fflush(outFile);
		fclose(outFile);
	}
};

class CallInstance{
public:
	CallInstance();
	CallInstance(uint64_t callId, char * name);
	~CallInstance();
	void InsertInstance(uint64_t stackID, double time);
	std::string Print();
private:
	uint64_t _callId;
	uint64_t _count;
	std::string _name;
	std::set<uint64_t> _stackIDs;
	double maxTime, minTime, totalTime;
};

class APITracker {
public:
	APITracker();
	~APITracker();
	void InsertNewEntry(uint64_t callid, uint64_t stackID, char * name, double time);
	std::string Print();
private:
	std::map<uint64_t, CallInstance> _calls;
};

class DriverAPITracer {
public:
	DriverAPITracer();
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
	~DriverAPITracer();
private:
	std::map<std::shared_ptr<Parameters>, std::chrono::high_resolution_clock::time_point> _points;
	std::shared_ptr<APITracker> _tracer;
	std::shared_ptr<OutputFile> _outFile;
	std::shared_ptr<StackKeyWriter> _keyFile;
};

#define PLUG_BUILD_FACTORY \
	if (__DriverTracer.get() == NULL) { \
		__DriverTracer.reset(new DriverAPITracer()); \
	} 

#define PLUG_FACTORY_PTR __DriverTracer.get()
