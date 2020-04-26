#include "Autocorrect.h"
void * SimpleGPUMallocMap::GetAllocation(uint64_t size) {
	//pthread_mutex_lock(&simpleGPUMemLock); 
	void * ret;
	auto it = _sizeToAddress.find(size);
	if (it == _sizeToAddress.end()) {
		autocorr_cuMemAlloc_wrapper(&ret, size);
		_addrToSize[ret] = size;
	} else {
		ret = it->second;
		_sizeToAddress.erase(it);
	}
	//pthread_mutex_unlock(&simpleGPUMemLock); 
	return ret;
}
void SimpleGPUMallocMap::FreeAllocation(void * addr) {
	//pthread_mutex_lock(&simpleGPUMemLock); 
	if (_addrToSize.find(addr) != _addrToSize.end())
		_sizeToAddress.insert(std::make_pair(_addrToSize[addr], addr));
	else {
		std::cerr << "Could not find allocation!" << std::endl;
		autocorr_cuMemFree22_wrapper(addr);
	}
	//pthread_mutex_unlock(&simpleGPUMemLock); 	
}

