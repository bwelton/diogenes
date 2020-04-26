#ifndef DYNINST_SYMBOLINFO
#define DYNINST_SYMBOLINFO 1
#include <string>
#include <vector>
#include <inttypes.h>
#include <iostream>
#include <sstream>
// Dyninst includes

#include "DynHelper.h"
//#include "Constants.h"

namespace DiogenesCommon {

struct LineMap{
    std::string filename;
    uint64_t line;
    uint64_t column;
    std::string Print(uint64_t spaceLength);
};

struct SymbolInfo {
    std::vector<std::string> funcName;
    std::vector<LineMap> lineinfo;
    SymbolInfo() {};
    SymbolInfo(const SymbolInfo &p2) : funcName(p2.funcName), lineinfo(p2.lineinfo) {};
    std::string Print(uint64_t spaceLength);
};

};
#endif
