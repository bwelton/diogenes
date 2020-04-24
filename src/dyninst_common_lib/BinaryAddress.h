#ifndef DIOGENES_BINARY_ADDRESS
#define DIOGENES_BINARY_ADDRESS 1
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <memory>
#include <thread>

#include "SymbolInfo.h"
namespace DiogenesCommon {

struct BinaryAddress {
    uint64_t libraryOffset;
    uint64_t processAddress;
    std::shared_ptr<char> binaryName;
    DiogenesCommon::SymbolInfo symbolInfo;
};

};


#endif
