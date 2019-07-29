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
#include "SymbolLookup.h"
#include "StackPoint.h"
#include "StackStorage.h"
#include "TFReaderWriter.h"

typedef std::map<uint64_t, StackRecord> StackRecMap;

class ReadStackKeys {
public:
	ReadStackKeys(std::string key, std::string bin); 	
	void GetStackRecords(StackRecMap & ret, std::function<void(StackRecMap &)> parsingFunc);
	void ExtractLineInfo(StackRecMap & rec);
	void ProcessTFTimingData(StackRecMap & ret);
	void ProcessStacktraceSynch(StackRecMap & ret);
private:
	std::string _key;
	std::string _bin;
};
