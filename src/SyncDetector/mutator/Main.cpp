#include "DynHelper.h"
#include <stdint.h>

int main(int argc, char * argv[]) {
    uint64_t cudaOffset = DynHelper_GetSynchronizationOffset();
    std::cout << "We found cuda offset at location: " << std::hex << cudaOffset << std::endl;
}