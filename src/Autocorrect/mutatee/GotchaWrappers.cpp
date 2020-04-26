#include "Autocorrect.h"
#include <atomic> 

StackwalkInst * autocorr_globalWalker = NULL;
std::atomic<int> autocorr_disablememcapture(0);
volatile bool autocorr_exitinit = false;
volatile bool autocorr_ingetstack = false;
void autocorr_mutatee_exit_handler() {
	autocorr_exitinit = true;	
}
pthread_mutex_t simpleGPUMemLock; 
thread_local SimpleGPUMallocMap * local_SimpleGPUMap;

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


bool autocorr_IsUnnecessary(uint64_t callerID) {
	if (autocorr_exitinit || autocorr_GlobalStacktree == NULL)
		return 0;
	uint64_t store[100];
	uint64_t stackSize = Stackwalk_GetStackLibUnwind(store, 100);

	// insert caller id at top of stack, which is at 2 accounting for
	// our caller.
	store[1] = callerID;
	return autocorr_GlobalStacktree->Lookup(&(store[1]), stackSize - 1);
}


int autocorr_cuMemAllocHost_v2(void ** ptr, size_t size) {
	int ret = autocorr_cuMemAllocHost_v2_wrapper(ptr,size);
	return ret;
}
int autocorr_cuMemAlloc(void ** ptr, size_t size) {
	if(autocorr_exitinit)
		return autocorr_cuMemAlloc_wrapper(ptr,size);

	if(local_SimpleGPUMap == NULL)
		local_SimpleGPUMap = new SimpleGPUMallocMap();
	*ptr = local_SimpleGPUMap->GetAllocation(size);
	return (int)CUDA_SUCCESS;
}
int autocorr_cuMemFreeHost(void * ptr) {
	return autocorr_cuMemFreeHost_wrapper(ptr);
}


int autocorr_synchronize_device()  {
	return autocorr_ctxSynchronize_wrapper();
}
int autocorr_cuMemFree22(void * ptr) {
	if(autocorr_exitinit)
		return autocorr_cuMemFree22_wrapper(ptr);

	if(local_SimpleGPUMap == NULL)
		local_SimpleGPUMap = new SimpleGPUMallocMap();

	local_SimpleGPUMap->FreeAllocation(ptr);
	if (!autocorr_IsUnnecessary(AUTOCORR_CALLID_cuMemFree_v2)) {
		std::cerr << "Cuda free synchronization necessary!" << std::endl;
		return autocorr_synchronize_device();
	} 
	return (int)CUDA_SUCCESS;
	// if (autocorr_IsUnnecessary(AUTOCORR_CALLID_cuMemFree_v2))
	// 	std::cerr << "Cuda free synchronization unnecessary" << std::endl;
	// else 
	// 	std::cerr << "Cuda free synchronization necessary!" << std::endl;
	// int ret = autocorr_cuMemFree22_wrapper(ptr);
	
}

int autocorr_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount ) {
	return autocorr_cuMemcpyHtoD_v2_wrapper(dstDevice, srcHost, ByteCount);
}

int autocorr_cuMemcpyDtoH_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount) {
	return autocorr_cuMemcpyDtoH_v2_wrapper(dstHost, srcDevice, ByteCount);
}

int autocorr_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream) {

	return autocorr_cuMemcpyDtoHAsync_v2_wrapper(dstHost, srcDevice, ByteCount, hStream);
}
int autocorr_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream) {

	return autocorr_cuMemcpyHtoDAsync_v2_wrapper(dstDevice, srcHost, ByteCount, hStream);
}






