#pragma once
#include <iostream>
#include <fstream>    
#include <string>
#include <map>
#include <boost/algorithm/string.hpp>
#include "InstrumentBase.h"
#include "DuplicateTracker.h"
#include "TransferTimeline.h"
#include "DestinationHash.h"
#include "HashData.h"
#include "CUPTIEventHandler.h"
#include "StacktraceGen.h"
class InstrumentFactory : public InstrumentBase{
private:
	std::vector<std::shared_ptr<InstrumentBase> > _workers;
	boost::recursive_mutex _mtx;
	uint64_t _globalID;
public:
	InstrumentFactory();
	int PerformAction(TransferPtr t);
	int PostTransfer(TransferPtr t);
	~InstrumentFactory();
};

//extern std::shared_ptr<InstrumentFactory> InstFactory;
#define BUILD_STORAGE_CLASS \
	if (InstFactory.get() == NULL) { \
		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
		InstFactory.reset(new InstrumentFactory()); \
	} 

#define STORAGE_PTR InstFactory.get()