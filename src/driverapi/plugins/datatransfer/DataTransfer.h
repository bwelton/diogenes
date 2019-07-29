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
struct CollisionOut {
	FILE * outFile;
	CollisionOut(std::string filename) {
		outFile = fopen(filename.c_str(),"w");
	}
	void Write(uint64_t stackId, uint64_t overwrite, uint64_t previousTransfer, uint64_t previousTransferID) {
		fwrite(&stackId, 1, sizeof(uint64_t), outFile);
		fwrite(&overwrite, 1, sizeof(uint64_t), outFile);
		fwrite(&previousTransfer, 1, sizeof(uint64_t), outFile);
		fwrite(&previousTransferID, 1, sizeof(uint64_t), outFile);
	}
	~CollisionOut() {
		fflush(outFile);
		fclose(outFile);
	}
};

class DataTransfer {
public:
	DataTransfer(std::vector<std::string> & cmd_list);
	~DataTransfer();
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
private:
	//std::set<CallID> _callsToMonitor;
	//std::vector<std::tuple<const char *,size_t, double> > _timers;
	std::shared_ptr<StackKeyWriter> _keyFile;
	std::map<uint64_t, uint64_t> _prevTransfers;
	//std::map<CallID, std::chrono::high_resolution_clock::time_point> _activeTimers;
	std::shared_ptr<CollisionOut> _out;
};

#define PLUG_BUILD_FACTORY(param) \
	if (Worker.get() == NULL) { \
		Worker.reset(new DataTransfer(param)); \
	} 

#define PLUG_FACTORY_PTR Worker.get()
