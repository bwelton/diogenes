#pragma once
#include <map>
#include <string>
#include <sstream> 
#include <map>
#include <boost/thread/recursive_mutex.hpp>
#include "TransferBase.h"
#include "InstrumentBase.h"
#include "LogInfo.h"

class DestinationHash : public InstrumentBase {
private:
	boost::recursive_mutex _mtx;
	std::shared_ptr<LogInfo> _log;
	bool _enabled;
	uint64_t _identCount;
public:
	DestinationHash(bool enabled = false, FILE * file = stderr);
	int PerformAction(TransferPtr t);
	int PostTransfer(TransferPtr t);
	~DestinationHash();
};