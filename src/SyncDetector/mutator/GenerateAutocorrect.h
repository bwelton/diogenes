#ifndef SYNCDETECT_GENAUTOCORRECT
#define SYNCDETECT_GENAUTOCORRECT 1
#include "SyncdetectFilenames.h"
#include "MemGraphFilenames.h"
#include "AutocorrectFilenames.h"
#include <set>
#include <map>
#include <string> 
#include <inttypes.h>
#include "BinaryAddress.h"
#include "SymbolInfo.h"

namespace DiogenesCommon {
    bool CompareStacks(std::vector<DiogenesCommon::BinaryAddress> & s1, std::vector<DiogenesCommon::BinaryAddress> & s2);
    void NormalizeStack(std::vector<DiogenesCommon::BinaryAddress> & s1);
    bool IsTransferCall(std::vector<std::string> & fnames);
    bool IsCudaAllocation(std::vector<std::string> & fnames);
    bool IsCudaFree(std::vector<std::string> & fnames);
    bool IsCudaHostAlloc(std::vector<std::string> & fnames);
    bool IsFree(std::vector<std::string> & fnames);
    bool IsMalloc(std::vector<std::string> & fnames);
    bool IsSynchronization(std::vector<std::string> & fnames);
    std::set<uint64_t> FindTransfers(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks);
    std::set<uint64_t> FindCudaAllocators(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks);
    std::set<uint64_t> FindCudaFree(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks);
    std::set<uint64_t> FindSynchronizations(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks);
    std::set<uint64_t> FindFree(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks);
    std::set<uint64_t> FindMalloc(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks);
};
#endif