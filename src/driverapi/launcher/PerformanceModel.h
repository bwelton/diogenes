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
#include <dirent.h>
#include <unistd.h>
#include <mutex>
#include <fstream>
#include <boost/program_options.hpp>
//#include "ProcessControl.h"
#include "SymbolLookup.h"
#include "StackPoint.h"
#include "StackStorage.h"
#include "TFReaderWriter.h"
struct CallPoint {
	std::string libCudaCallname;
	std::vector<uint64_t> callStacks;
	double time;
	// Number of times this specific call synchronizes
	uint64_t syncCount;
	uint64_t startPos;
	CallPoint(std::string name, uint64_t count, double t) {
		time = t;
		syncCount = count;
		libCudaCallname = name;
	}
};

class PerformanceModel {
public:
	PerformanceModel();
	void AddExecutionTime(double secs);
	void AddStack(std::vector<StackPoint> stack);
	void ExtractLineInfo(std::map<uint64_t, StackRecord> & rec);
	void ProcessStacks();
	void CaptureSyncTime();
	std::map<uint64_t, StackRecord> GetTimingStackRecords();
	void GetTimingList(std::vector<StackPoint> & timingList);
	void AddFirstUses(std::map<uint64_t, StackPoint> uses);
	void FinalProcessing();
	void ReadStackFile(std::string key, std::string timeline);
	void ReadStackFiles();
	bool FindElement(uint64_t genId, uint64_t & startPos, std::vector<TimingData> & array);
	void ReadTimingStacks(std::string keyFile, std::string timelineFile);
	std::map<uint64_t,uint64_t> MatchStackTraces(std::map<uint64_t, StackRecord> & a, std::map<uint64_t, StackRecord> & b);
	void WriteStackFile(std::string stackFile, std::string fileName);
	void TranslateStackfile(std::string fileName, std::string outFilename);
	void TranslateStackRecords(std::vector<StackPoint> & points);
private:
	double _fastestExecTime;
	uint64_t _capturedSyncs;
	uint64_t _totalSyncs;
	std::vector<uint64_t> _stackOrder;
	std::vector<CallPoint> _callPoints;
	std::map<uint64_t, uint64_t> _stackCount;
	std::map<uint64_t, std::vector<StackPoint> > _stackPoints;
	std::map<uint64_t, StackPoint> _firstUses;
	std::map<uint64_t, std::vector<std::pair<std::string, LineInfo> > > _lineInfo;
	std::map<uint64_t, std::tuple<std::string, std::string, std::string> > _callPair;

	//std::vector<SyncRangeRecord> _orderingInfo;

	CudaCallMap _callMapper;
	std::vector<TF_SyncRecord> _orderingInfo;

	std::map<std::string, uint64_t> _nameMap;
	std::vector<TF_SyncRecord> _timingData;
	std::map<uint64_t, StackRecord> _stackRecords; 
	std::map<uint64_t, StackRecord> _timingStackRecords; 
	std::map<uint64_t, uint64_t> _stackToDynId;
};
