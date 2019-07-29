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
#include <unistd.h>
#include <mutex>
#include <boost/program_options.hpp>
#include <numeric>
#include <chrono>
#include <tuple>
#include "ProcessControl.h"
#include "LogInfo.h"
#include <libgen.h>
#include <unistd.h>

#include "StackPoint.h"
#include "StackStorage.h"
class TimeApplications {
public:
	TimeApplications(boost::program_options::variables_map vm);
	~TimeApplications();
	double RunWithLSInstrimentation(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras,
				std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks);
	double Run();
	double InternalRun();
	double InternalRunWithTiming(std::vector<StackPoint> points);
	double RunWithInstrimentation(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras);
	double RunWithBreakpoints(std::string wrapperDef, 
											std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras,
											std::vector<std::string> breakFunctions,
											std::vector<std::string> libLoads,
											std::function<void(ProcessController *)> cb);
	double RunWithLoadStore(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras, std::vector<std::string> libLoads);
	void IdentifyDyninstBugs(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras, std::vector<std::string> libLoads);
	void RedirectOutToFile(std::string filename);
	void ReturnToTerminal();
	double RunWithStackTracing();
	double RunWithDriverAPITrace();
	double GetFirstUses(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras,
				std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks);
	std::map<uint64_t, StackPoint> _firstUses;
private:
	int terminal_stdout;
	int terminal_stderr;
	boost::program_options::variables_map _vm;

};