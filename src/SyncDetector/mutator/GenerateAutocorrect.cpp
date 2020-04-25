#include "GenerateAutocorrect.h"

bool operator== (const DiogenesCommon::BinaryAddress &c1, const DiogenesCommon::BinaryAddress &c2)
{
    if (c1.binaryName == NULL || c2.binaryName == NULL) {
        if(c1.binaryName != c2.binaryName)
            return false;
    } else {
        if (strcmp(c1.binaryName.get(),c2.binaryName.get()) != 0)
            return false;
    }
    return (c1.libraryOffset== c2.libraryOffset);
}
bool operator!= (const DiogenesCommon::BinaryAddress &c1, const DiogenesCommon::BinaryAddress &c2)
{
    return !(c1== c2);
}

DiogenesCommon::BinaryAddressTree::BinaryAddressTree(DiogenesCommon::BinaryAddress addr, bool isMemGraph) : _binAddr(addr), _isMemGraph(isMemGraph), _stackID(0) {}

uint64_t DiogenesCommon::BinaryAddressTree::InsertElement(std::vector<DiogenesCommon::BinaryAddress> & stacks, uint64_t stackID, uint64_t pos) {
    if (pos == stacks.size()) {
        if (_stackID != 0)
            return _stackID;
        _stackID = stackID;
        return stackID;
    }
    for(auto y : _children) {
        if ( y->_binAddr == stacks[pos]) {
            return y->InsertElement(stacks, stackID, pos+1);
        }
    }

    DiogenesCommon::BinaryAddressTree * nAddr = new DiogenesCommon::BinaryAddressTree(stacks[pos],_isMemGraph);
    _children.push_back(nAddr);
    return nAddr->InsertElement(stacks, stackID, pos+1);
}

uint64_t DiogenesCommon::BinaryAddressTree::FindElement(std::vector<DiogenesCommon::BinaryAddress> & stacks, uint64_t pos){
    if (pos == stacks.size())
        return _stackID;
    for(auto y : _children) {
        if ( y->_binAddr == stacks[pos]) {
            return y->FindElement(stacks, pos+1);
        }
    }    
    return 0;
}



bool DiogenesCommon::CompareStacks(std::vector<DiogenesCommon::BinaryAddress> & s1, std::vector<DiogenesCommon::BinaryAddress> & s2) {
    if (s1.size() != s2.size())
        return false;
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i])
            return false;
    }
    return true;
}


void DiogenesCommon::NormalizeStack(std::vector<DiogenesCommon::BinaryAddress> & s1) {
    std::vector<DiogenesCommon::BinaryAddress> ret;
    std::string installDirectory = DynHelper_GetInstallDirectory();
    for (auto i : s1) {
        if (i.binaryName != NULL) {
            std::string tmp = std::string(i.binaryName.get());
            if (tmp.find(installDirectory) == std::string::npos && tmp.find("libcuda.so") == std::string::npos) {
                bool breakOut = false;
                for (auto z : i.symbolInfo.funcName) {
                    if (z == std::string("_start") || z == std::string("__libc_start_main"))
                        breakOut = true;
                }
                if (!breakOut)
                    ret.push_back(i);
            }
        }
    }
    s1 = ret;
}

std::set<std::string> cudaTransferCalls = {"cuMemcpyHtoD_v2","cuMemcpyDtoH_v2","cuMemcpyDtoHAsync_v2", "cuMemcpyHtoDAsync_v2","cuMemcpyAsync"};
std::map<std::string, std::string> diogTransConversion = {{"memgraph_cuMemcpyHtoD_v2","cuMemcpyHtoD_v2"},
                                                          {"memgraph_cuMemcpyDtoH_v2","cuMemcpyDtoH_v2"},
                                                          {"memgraph_cuMemcpyDtoHAsync_v2","cuMemcpyDtoHAsync_v2"},
                                                          {"memgraph_cuMemcpyHtoDAsync_v2", "cuMemcpyHtoDAsync_v2"},
                                                          {"memgraph_cuMemcpyAsync","cuMemcpyAsync"}};
bool DiogenesCommon::IsTransferCall(std::vector<std::string> & fnames) {
    for(auto funcName : fnames)
        if (cudaTransferCalls.find(funcName) != cudaTransferCalls.end() || diogTransConversion.find(funcName) != diogTransConversion.end()) {
            return true;
        }
    return false;
}

std::set<std::string> cudaAllocationCalls = {"cuMemAlloc_v2","cuMemAllocManaged"};
std::map<std::string, std::string> diogAllocators = {{"memgraph_cuMemAllocManaged", "cuMemAllocManaged"},
                                                     {"memgraph_cuMemAlloc","cuMemAlloc_v2"}};
bool DiogenesCommon::IsCudaAllocation(std::vector<std::string> & fnames) {
    for(auto funcName : fnames)
        if (cudaAllocationCalls.find(funcName) != cudaAllocationCalls.end() || diogAllocators.find(funcName) != diogAllocators.end()) {
            return true;
        }
    return false;
}

std::set<std::string> cudaHostAllocators = {"cuMemHostAlloc"};
std::map<std::string, std::string> cudaHostDiog = {{"memgraph_cuMemAllocHost_v2", "cuMemHostAlloc"}};
bool DiogenesCommon::IsCudaHostAlloc(std::vector<std::string> & fnames) {
    for(auto funcName : fnames)
        if (cudaHostAllocators.find(funcName) != cudaHostAllocators.end() || cudaHostDiog.find(funcName) != cudaHostDiog.end()) {
            return true;
        }
    return false;
}

std::set<std::string> libcMallocSet = {"malloc"};
std::map<std::string, std::string> libcMallocDiog = {{"memgraph_malloc", "malloc"}};
bool DiogenesCommon::IsMalloc(std::vector<std::string> & fnames) {
    for(auto funcName : fnames)
        if (libcMallocSet.find(funcName) != libcMallocSet.end() || libcMallocDiog.find(funcName) != libcMallocDiog.end()) {
            return true;
        }
    return false;
}
std::set<std::string> freeCallset = {"free"};
std::map<std::string, std::string> freeGraphDiog = {{"memgraph_free", "free"}};
bool DiogenesCommon::IsFree(std::vector<std::string> & fnames) {
    for(auto funcName : fnames)
        if (freeCallset.find(funcName) != freeCallset.end() || freeGraphDiog.find(funcName) != freeGraphDiog.end()) {
            return true;
        }
    return false;
}

std::set<std::string> cudaFreeCalls = {"cuMemFree_v2"};
std::map<std::string, std::string> cudaFreeDiog = {{"memgraph_cuMemFree", "cuMemFree_v2"}};
bool DiogenesCommon::IsCudaFree(std::vector<std::string> & fnames) {
    for(auto funcName : fnames)
        if (cudaFreeCalls.find(funcName) != cudaFreeCalls.end() || cudaFreeDiog.find(funcName) != cudaFreeDiog.end()) {
            return true;
        }
    return false;
}

std::set<std::string> diogSynchronizationCall = {"DIOG_Synchronization_Post"};
bool DiogenesCommon::IsSynchronization(std::vector<std::string> & fnames) {
    for(auto funcName : fnames)
        if (diogSynchronizationCall.find(funcName) != diogSynchronizationCall.end()) {
            return true;
        }
    return false;
}


std::set<uint64_t> DiogenesCommon::FindTransfers(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks) {
    std::set<uint64_t> ret;
    for (auto i : stacks) {
        for (auto x : i.second) {
            if (DiogenesCommon::IsTransferCall(x.symbolInfo.funcName)) {
                ret.insert(i.first);
                break;
            }
        }
    }
    return ret;
}
std::set<uint64_t> DiogenesCommon::FindCudaAllocators(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks) {
    std::set<uint64_t> ret;
    for (auto i : stacks) {
        for (auto x : i.second) {
            if (DiogenesCommon::IsCudaAllocation(x.symbolInfo.funcName)) {
                ret.insert(i.first);
                break;
            }
        }
    }
    return ret;

}
std::set<uint64_t> DiogenesCommon::FindCudaFree(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks){
    std::set<uint64_t> ret;
    for (auto i : stacks) {
        for (auto x : i.second) {
            if (DiogenesCommon::IsCudaFree(x.symbolInfo.funcName)) {
                ret.insert(i.first);
                break;
            }
        }
    }
    return ret;
}
std::set<uint64_t> DiogenesCommon::FindMalloc(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks){
    std::set<uint64_t> ret;
    for (auto i : stacks) {
        for (auto x : i.second) {
            if (DiogenesCommon::IsMalloc(x.symbolInfo.funcName)) {
                ret.insert(i.first);
                break;
            }
        }
    }
    return ret;
}
std::set<uint64_t> DiogenesCommon::FindFree(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks){
    std::set<uint64_t> ret;
    for (auto i : stacks) {
        for (auto x : i.second) {
            if (DiogenesCommon::IsFree(x.symbolInfo.funcName)) {
                ret.insert(i.first);
                break;
            }
        }
    }
    return ret;
}
std::set<uint64_t> DiogenesCommon::FindSynchronizations(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & stacks){
    std::set<uint64_t> ret;
    for (auto i : stacks) {
        for (auto x : i.second) {
            if (DiogenesCommon::IsSynchronization(x.symbolInfo.funcName)) {
                ret.insert(i.first);
                break;
            }
        }
    }
    return ret;
}