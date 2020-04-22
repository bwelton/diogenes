#ifndef DIOGENES_BINARY_ADDRESS
#define DIOGENES_BINARY_ADDRESS 1
#include <unistd.h>
#include <inttypes.h>
#include <memory>
#include <thread>
namespace DiogenesCommon{
struct BinaryAddress {
    uint64_t libraryOffset;
    uint64_t processAddress;
    std::shared_ptr<char> binaryName;
};
};
#endif
