// 
#pragma once
#include <map>
#include <tuple>
#include <vector>
#include <iostream>
#include <sstream> 
#include <utility> 
#include <memory>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <sys/wait.h>
#include <stdio.h>
#include <memory>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <streambuf>
#include <utility>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <functional>

//               Timline id,   size, duplicate id
typedef std::tuple<uint64_t, size_t, uint64_t> TimelineRec;

//                Timeline id, stream, procid, threadid
typedef std::tuple<uint64_t, uint64_t, uint64_t, uint64_t> CorrelationRec;

//               Timline id, size, duplicate id, stream, procid, threadid
typedef std::tuple<uint64_t, size_t, uint64_t, uint64_t, uint64_t, uint64_t> CombinedRecord;

//                 corr id, type_key, cname_key, start_time, end_time,   procid, threadid,     size, runcorr, ctx, dev, stream,   
typedef std::tuple<uint64_t, uint32_t, uint32_t,   uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,     int, int, int, uint64_t > TimingRec;

//                 corr id, cname_key, transferCname,  GPU Time,  CPU time,   procid, threadid,     size,  runcorr, ctx, dev, stream,   
typedef std::tuple<uint64_t, uint32_t,  uint64_t,  uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,     int, int, int, uint64_t > CUPTIRecord;

struct CPUProcess {
	uint64_t procid;
	uint64_t threadid;
	std::vector<CombinedRecord> transferRecords;
	bool operator==(const CombinedRecord & a) {
		if (std::get<4>(a) == procid && std::get<5>(a) == threadid)
			return true;
		return false;
	}
};

struct CUDAProcess {
	uint64_t procid;
	uint64_t threadid;
	bool matched;
	uint64_t pos;
	std::vector<CUPTIRecord> records;
	std::vector<CombinedRecord> transferRecords;
	std::string ASYNC;
	CUDAProcess() : matched(false){
		pos = 0;
		ASYNC = std::string("Async");
	}

	bool IsSynchronization(CUPTIRecord & a, std::map<uint32_t, std::string> & typeKeys) {
		uint32_t key = std::get<1>(a);
		if (typeKeys[key].find(ASYNC) != std::string::npos) {
			return false;
		} 
		return true;
	}
	bool IsTransfer(CUPTIRecord & a) {
		if (std::get<2>(a) != 0) {
			return true;
		}
		return false;
	}

	bool GetNextCUPTITransferOrSynchronization(CUPTIRecord & rec, int & type, std::map<uint32_t, std::string> & typeKeys) {
		for (int i = pos; i < records.size(); i++) {
			if (IsTransfer(records[i]) && IsSynchronization(records[i], typeKeys)){
				rec = records[i];
				pos = i + 1;
				type = 3;
				return true;
			}
			else if (IsTransfer(records[i])) {
				rec = records[i];
				pos = i + 1;
				type = 1;
				return true;
			}
			else if (IsSynchronization(records[i], typeKeys)) {
				rec = records[i];
				pos = i + 1;
				type = 2;
				return true;				
			}
		}
		pos = records.size();
		return false;
	}

	bool operator==(const TimingRec & a) {
		if (std::get<5>(a) == procid && std::get<6>(a) == threadid)
			return true;
		return false;		
	}

	bool operator==(const CombinedRecord & a) {
		if (std::get<4>(a) == procid && std::get<5>(a) == threadid)
			return true;
		return false;
	}
	bool operator==(const CPUProcess & a) {
		if (a.procid == procid && a.threadid == threadid)
			return true;
		return false;
	}
	int MatchEntries(CPUProcess & proc) {
		int pos = 0;
		for (auto i : records) {
			if (std::get<2>(i) == 0) 
				continue;
			if (std::get<7>(i) == std::get<1>(proc.transferRecords[0]))
				return pos;
			else
				pos++;
		}
		return -1;
	}
};

class CalculateDedupSavings {
private:
	char * _timeline_file;
	char * _correlation_file;
	char * _timing_file;
	std::map<uint32_t, std::string> _typeKeys;
public:
	CalculateDedupSavings(char * timeline_file, char * correlation_file, char * timing_file);
	void ReadTimeline(std::vector<TimelineRec> & records);
	void ReadCorrelation(std::vector<CorrelationRec> & records);
	void ReadTiming(std::vector<TimingRec> & records, double & finalTime);
	void CombineTimelineCorrelation(std::vector<TimelineRec> & timeline, 
									std::vector<CorrelationRec> & correlation,
									std::vector<CombinedRecord> & output);
	void NormalizeProcessIDs(std::vector<CombinedRecord> & correlation,
							std::vector<CUDAProcess> & procs);
	bool IsTransfer(CUPTIRecord & a);
	std::pair<uint64_t, uint64_t> GenerateEstimate(std::vector<TimingRec> & timing, 
								                   std::vector<CombinedRecord> & correlation);
	std::pair<uint64_t, uint64_t> CalculateProcessSavings(CUDAProcess & proc);
	// For debug purposes
	int NameToId(std::string name);
	void GenerateCUDAProcesses(std::vector<TimingRec> & timing,
		  					   std::vector<CUDAProcess> & procs, std::map<uint64_t, CUPTIRecord> & c_records);

};
