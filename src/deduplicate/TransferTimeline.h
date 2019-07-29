#pragma once
#include <memory>
#include <iostream>
#include <set>
#include <sstream> 
#include <boost/thread/recursive_mutex.hpp>
#include "LogInfo.h"
#include "TransferBase.h"
#include "InstrumentBase.h"

class TransferTimeline : public InstrumentBase {
private:
	boost::recursive_mutex _timeline_mtx;
	std::shared_ptr<LogInfo> _timeline_log;
	std::map<uint32_t, uint64_t> _pastTransfers;
	uint64_t _count; 
	bool _enabled;
	void Write(std::string a);
	void AddTransfer(std::string & type, uint32_t id, size_t size, uint64_t globalId);
	void WriteLogEntry(std::string & type, uint32_t id, size_t size, uint64_t count, uint64_t origTransfer, uint64_t globalId);
public:
	TransferTimeline(bool enabled = false, FILE * file = stderr);
	int PerformAction(TransferPtr t);
	int PostTransfer(TransferPtr t);
	~TransferTimeline();
};





// class TransferTimeline{
// public:
// 	TransferTimeline();
// 	~TransferTimeline();
// 	void Write(std::string a);
// 	void AddTransfer(std::string type, uint32_t id, size_t size);
// 	void WriteLogEntry(std::string & type, uint32_t id, size_t size, uint64_t count, uint64_t origTransfer);
// private:
// 	std::shared_ptr<LogInfo> _timeline_log;
// 	std::map<uint32_t, uint64_t> _pastTransfers;
// 	uint64_t _count; 
// };
