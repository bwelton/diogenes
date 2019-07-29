#pragma once
#include "InstrumentBase.h"
#include "TransferBase.h"
#include "LogInfo.h"
#include <boost/thread/recursive_mutex.hpp>
#include <string>
#include <sstream> 
#include <map>

class DuplicateTracker : public InstrumentBase {
private: 
	boost::recursive_mutex _mtx; 
	bool _enabled;
	size_t _collisionCount;
	size_t _collisionSize;
    size_t _totalSize;
	size_t _totalCount;
	std::shared_ptr<LogInfo> _log;
	std::map<uint32_t, size_t> _prevTransfers; 
public:
	DuplicateTracker(bool enabled = false, FILE * file = stderr);
	bool DetectDuplicate(TransferPtr t);
	int PerformAction(TransferPtr t);
	int PostTransfer(TransferPtr t);
	~DuplicateTracker();
};