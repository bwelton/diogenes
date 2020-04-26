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

void MemGraphBuild::AllocationTracker::DeSerializeData(char * data, uint64_t size) {
    std::map<uint64_t, std::vector<uint64_t>> idPointers;
    if (data == NULL)
        return;
    _stackToAlloc.clear();
    _stackToFree.clear();
    _stackToTrans.clear();
    uint64_t * binData = (uint64_t *)data;
    uint64_t pos = 0;
    uint64_t count = binData[pos];
    pos++;
    for (int i = 0; i < count; i++) {
        AllocSitePtr p = std::shared_ptr<MemGraphBuild::AllocationSite>(new MemGraphBuild::AllocationSite(binData[pos]));
        pos++;
        p->callCount = binData[pos];
        pos++;
        p->freed = binData[pos];
        pos++;
        uint64_t freeSites = binData[pos];
        pos++;
        idPointers[p->stackID] = std::vector<uint64_t>();
        for(int j = 0; j < freeSites; j++) {
            idPointers[p->stackID].push_back(binData[pos]);
            pos++;
        }
        _stackToAlloc[p->stackID] = p;
    }
    count = binData[pos];
    pos++;
    for (int i = 0; i < count; i++) {
        FreeSitePtr p = std::shared_ptr<MemGraphBuild::FreeSite>(new MemGraphBuild::FreeSite(binData[pos]));
        pos++;
        p->callCount = binData[pos];
        pos++;
        p->unknownAlloc = binData[pos];
        pos++;
        uint64_t freeSites = binData[pos];
        pos++;
        idPointers[p->stackID] = std::vector<uint64_t>();
        for(int j = 0; j < freeSites; j++) {
            idPointers[p->stackID].push_back(binData[pos]);
            pos++;
        }
        _stackToFree[p->stackID] = p;
    }
    count = binData[pos];
    pos++;
    for (int i = 0; i < count; i++) {
        TransSitePtr p = std::shared_ptr<MemGraphBuild::TransferSite>(new MemGraphBuild::TransferSite(binData[pos]));
        pos++;
        p->callCount = binData[pos];
        pos++;
        p->unknownAlloc = binData[pos];
        pos++;
        uint64_t freeSites = binData[pos];
        pos++;
        idPointers[p->stackID] = std::vector<uint64_t>();
        for(int j = 0; j < freeSites; j++) {
            idPointers[p->stackID].push_back(binData[pos]);
            pos++;
        }
        _stackToTrans[p->stackID] = p;
    }

    for(auto & i : _stackToAlloc) {
        for (auto & x : idPointers[i.first])
            i.second->freeSites.insert(_stackToFree[x]);
    }
    for(auto & i : _stackToFree) {
        for (auto & x : idPointers[i.first])
            i.second->allocSites.insert(_stackToAlloc[x]);
    }
    for(auto & i : _stackToTrans) {
        for (auto & x : idPointers[i.first])
            i.second->allocSites.insert(_stackToAlloc[x]);
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
MemGraphBuild::FreeMap MemGraphBuild::AllocationTracker::GetFreeMapForID(uint64_t id){
    MemGraphBuild::FreeMap ret(id);
    if (_stackToFree.find(id) == _stackToFree.end())
        return ret;
    FreeSitePtr p = _stackToFree[id];
    ret.unknownAlloc = p->unknownAlloc;
    for (auto i : p->allocSites) 
        ret.allocSites.insert(i->stackID);
    return ret;
}

MemGraphBuild::TransferMap MemGraphBuild::AllocationTracker::GetTransferMapForID(uint64_t id) {
    MemGraphBuild::TransferMap ret(id);
    if (_stackToTrans.find(id) == _stackToTrans.end())
        return ret;
    TransSitePtr p = _stackToTrans[id];
    std::vector<uint64_t> stack;
    for (auto i : p->allocSites)
        stack.push_back(i->stackID);
    while (stack.size() > 0) {
        uint64_t cur = stack.back();
        stack.pop_back();
        if (_stackToAlloc.find(cur) != _stackToAlloc.end()){
            ret.allocSites.insert(cur);
            for(auto i : _stackToAlloc[cur]->freeSites) 
                ret.freeSites.insert(i->stackID);
        }
    }
    ret.unknownAllocations = p->unknownAlloc;
    return ret;
}

MemGraphBuild::AllocTrackPtr MemGraphBuild_globalAllocPointer;
MemGraphBuild::AllocTrackPtr MemGraphBuild::GetGlobalTracker() {
    if (MemGraphBuild_globalAllocPointer == NULL){
        MemGraphBuild_globalAllocPointer = new AllocationTracker();
    }
    return MemGraphBuild_globalAllocPointer;
}