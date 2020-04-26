#include "Autocorrect.h"

CPUMemoryAllocation::CPUMemoryAllocation(void * p, uint64_t s, bool appAlloc) : ptr(p), size(s), appallocated(appAlloc) {}

CPUAddressPair::CPUAddressPair(void * c, void * p, uint64_t s, bool dev) : origCPUAddr(c), pinnedCPUAddr(p), size(s), toDevice(dev) {};
inline void * CPUAddressPair::GetOverlap(void * c, uint64_t s) {
	if(c >= origCPUAddr && ((uint64_t)origCPUAddr) + size <= (uint64_t)c)
		return origCPUAddr;
	return NULL;
}
void CPUMemoryLocation::CopyToDestination() {
	for(auto i : _currentCPUTransfers) {
		if (i.second.toDevice == false){
			memcpy(i.second.origCPUAddr, i.second.pinnedCPUAddr, i.second.size);
		}
		FreeAllocation(i.second.pinnedCPUAddr);
	}
	_currentCPUTransfers.clear();
}

void * CPUMemoryLocation::GetAllocation(void * cpuDestAddr,uint64_t size, bool toDevice){
	auto it = _allocationMap.lower_bound(cpuDestAddr);
	if (it != _allocationMap.end()) {
		if(it->first == cpuDestAddr)
			return cpuDestAddr;
	}

	// Check if CPU Destination overlaps a prior tranfser
	// If the overlap is not exact, synchronize because we
	// will have a datarace otherwise.
	auto it2 = _currentCPUTransfers.lower_bound(cpuDestAddr);
	if (it2 != _currentCPUTransfers.end()) {
		bool sync = false;
		void * overlap = it2->second.GetOverlap(cpuDestAddr,size);
		if (overlap == cpuDestAddr){
			if(it2->second.toDevice == true)
				it2->second.toDevice = toDevice;
			return it2->second.pinnedCPUAddr;
		}else if (overlap != NULL) {
			sync = true;
			std::cerr << "OVERLAP DETECTED, PEFORMING SYNC" << std::endl;
			autocorr_synchronize_device();
		}
		if (sync == false) {
			auto it3 = std::next(it2);
			if (it3 != _currentCPUTransfers.end()){
				void * checkHigher = it3->second.GetOverlap(cpuDestAddr,size);
				if (checkHigher != NULL){
					std::cerr << "OVERLAP DETECTED, PEFORMING SYNC" << std::endl;
					autocorr_synchronize_device();
				}
			}
		}
	}

	// Grab a new memory allocation
	auto it4 = _freeMemory.find(size);
	if (it4 == _freeMemory.end()) {
		void * ptr = NULL;
		autocorr_cuMemAllocHost_v2_wrapper(&ptr, size);
		_freeMemory.insert(std::make_pair(size,ptr));
        RegisterAllocation(ptr, size, false);
		it4 = _freeMemory.find(size);
	}

	_currentCPUTransfers.emplace(cpuDestAddr, CPUAddressPair(cpuDestAddr,it4->second,size,toDevice));
	void * ret = it4->second;
    if(toDevice)
        memcpy(ret, cpuDestAddr, size);
	_freeMemory.erase(it4);
	return ret;
}

void CPUMemoryLocation::RegisterAllocation(void * ptr, uint64_t size, bool appAllocated) {
	_allocationMap.emplace(ptr, CPUMemoryAllocation(ptr, size, appAllocated));
}

void CPUMemoryLocation::UnregisterAllocation(void * ptr) {
    auto it = _allocationMap.find(ptr);
    if(_allocationMap.end() != it)
        _allocationMap.erase(it);
}
void CPUMemoryLocation::FreeAllocation(void * ptr) {
	auto it = _allocationMap.find(ptr);
	if (it == _allocationMap.end())
		return;
    if (it->second.appallocated == false)
	    _freeMemory.insert(std::make_pair(it->second.size, ptr));
}
