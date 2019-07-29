#pragma once
#include <set>
#include <stdio.h>
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
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "InstrumentBase.h"
#include "TransferBase.h"
#include "LogInfo.h"
#include <boost/thread/recursive_mutex.hpp>

// Dyninst includes
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
using namespace Dyninst;
using namespace Dyninst::Stackwalker;
using namespace SymtabAPI;

class StackTraceGen : public InstrumentBase {
private:
	boost::recursive_mutex _st_mtx; 
	std::shared_ptr<LogInfo> _st_log;
	bool _enabled;
	std::string GenStackTrace();
	std::map<uint32_t, std::pair<std::string, int> > _stackMap;
	std::set<uint32_t> _transferHash;
public:
	StackTraceGen(bool enabled = false, FILE * output = NULL);
	int PerformAction(TransferPtr t);
	int PostTransfer(TransferPtr t);
	void WriteStackMap();
	~StackTraceGen();
};