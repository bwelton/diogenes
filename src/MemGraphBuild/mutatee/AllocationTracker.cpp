#include "AllocationTracker.h"
void MemGraphBuild::AllocationTracker::HandleAllocation(uint64_t stackAddr, uint64_t memAddr, uint64_t size) {
    if (stackAddr == 0)
        return;
    auto it = _stackToAlloc.find(stackAddr);
    if (it == _stackToAlloc.end()) {
        _stackToAlloc[stackAddr] = std::shared_ptr<MemGraphBuild::AllocationSite>(new MemGraphBuild::AllocationSite(stackAddr));
        it = _stackToAlloc.find(stackAddr);
    }
    _addrToAlloc[memAddr] = std::make_pair(size,it->second);
    it->second->callCount++;
}

void MemGraphBuild::AllocationTracker::HandleFree(uint64_t stackAddr, uint64_t memAddr) {
    if (stackAddr == 0)
        return;
    auto it = _stackToFree.find(stackAddr);
    if (it == _stackToFree.end()) {
        _stackToFree[stackAddr] = std::shared_ptr<MemGraphBuild::FreeSite>(new MemGraphBuild::FreeSite(stackAddr));
        it = _stackToFree.find(stackAddr);
    }
    it->second->callCount++;
    auto it2 = _addrToAlloc.find(memAddr);
    if (it2 == _addrToAlloc.end()){
        it->second->unknownAlloc++;
    } else {
        it->second->allocSites.insert(it2->second.second);
        it2->second.second->freeSites.insert(it->second);
        it2->second.second->freed++;
        _addrToAlloc.erase(it2);
    }
}

void MemGraphBuild::AllocationTracker::HandleTransfer(uint64_t stackAddr, uint64_t cpuAddress, uint64_t size) {
    if (stackAddr == 0)
        return;
    auto it = _stackToTrans.find(stackAddr);    
    if (it == _stackToTrans.end()) {    
        _stackToTrans[stackAddr] = std::shared_ptr<MemGraphBuild::TransferSite>(new MemGraphBuild::TransferSite(stackAddr));
        it = _stackToTrans.find(stackAddr);
    }
    it->second->callCount++;
    auto it2 = _addrToAlloc.lower_bound(cpuAddress);
    if (it2 == _addrToAlloc.end()){
        it->second->unknownAlloc++;
    } else {
        if (it2 != _addrToAlloc.begin() && it2->first != cpuAddress) {
            std::advance(it2,-1);
        }
        if (it2->first <= cpuAddress && it2->second.first + it2->first >= cpuAddress) {
            it->second->allocSites.insert(it2->second.second);
        } else {
            it->second->unknownAlloc++;
        }
    }
}
            // std::unordered_map<uint64_t, AllocSitePtr> _stackToAlloc;
            // std::unordered_map<uint64_t, FreeSitePtr> _stackToFree;
            // std::unordered_map<uint64_t, TransSitePtr> _stackToTrans;
char * MemGraphBuild::AllocationTracker::SerializeData(uint64_t & size) {
    size = 0;
    for (auto i : _stackToAlloc) {
        size += sizeof(uint64_t) * 4 + (i.second->freeSites.size() * sizeof(uint64_t));
    }
    for (auto i : _stackToFree) {
        size += sizeof(uint64_t) * 4 + (i.second->allocSites.size()* sizeof(uint64_t));
    }
    for (auto i : _stackToTrans) {
        size += sizeof(uint64_t) * 4 + (i.second->allocSites.size()* sizeof(uint64_t));
    }
    size += sizeof(uint64_t) * 3;

    uint64_t curPos = 0;
    uint64_t * binData = (uint64_t *) malloc(size);
    binData[curPos] = _stackToAlloc.size();
    curPos++;
    for (auto i : _stackToAlloc) {
        AllocSitePtr p = i.second;
        binData[curPos] = p->stackID;
        curPos++;
        binData[curPos] = p->callCount;
        curPos++;
        binData[curPos] = p->freed;
        curPos++;        
        binData[curPos] = p->freeSites.size();
        curPos++;    
        for (auto n : p->freeSites){
            binData[curPos] = n->stackID;
            curPos++;                
        }  
    }
    binData[curPos] = _stackToFree.size();
    curPos++;
    for (auto i : _stackToFree) {
        FreeSitePtr p = i.second;
        binData[curPos] = p->stackID;
        curPos++;
        binData[curPos] = p->callCount;
        curPos++;
        binData[curPos] = p->unknownAlloc;
        curPos++;        
        binData[curPos] = p->allocSites.size();
        curPos++;    
        for (auto n : p->allocSites){
            binData[curPos] = n->stackID;
            curPos++;                
        }
    }
    binData[curPos] = _stackToTrans.size();
    curPos++;
    for (auto i : _stackToTrans) {
        TransSitePtr p = i.second;
        binData[curPos] = p->stackID;
        curPos++;
        binData[curPos] = p->callCount;
        curPos++;
        binData[curPos] = p->unknownAlloc;
        curPos++;        
        binData[curPos] = p->allocSites.size();
        curPos++;    
        for (auto n : p->allocSites){
            binData[curPos] = n->stackID;
            curPos++;                
        }
    }
    return (char*)binData;
}
MemGraphBuild::AllocTrackPtr MemGraphBuild_globalAllocPointer;
MemGraphBuild::AllocTrackPtr MemGraphBuild::GetGlobalTracker() {
    if (MemGraphBuild_globalAllocPointer == NULL){
        MemGraphBuild_globalAllocPointer = std::shared_ptr<AllocationTracker>(new AllocationTracker());
    }
    return MemGraphBuild_globalAllocPointer;
}