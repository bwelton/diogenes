#include "Autocorrect.h"
#include <atomic> 

StackwalkInst * autocorr_globalWalker = NULL;
std::atomic<int> autocorr_disablememcapture(0);
volatile bool autocorr_exitinit = false;
volatile bool autocorr_ingetstack = false;
pthread_mutex_t simpleGPUMemLock; 
thread_local SimpleGPUMallocMap * local_SimpleGPUMap = NULL;
CPUMemoryLocation * local_CPUMEMLOCATION = NULL;

void autocorr_mutatee_exit_handler() {
	autocorr_exitinit = true;	
}

void AUTOCORR_SYNC_FINISHED() {
	if (autocorr_exitinit)
		return;
	// std::cerr << "Sync Finished" << std::endl;
	if(local_CPUMEMLOCATION != NULL)
		local_CPUMEMLOCATION->CopyToDestination();
}
bool autocorr_IsUnnecessary(uint64_t callerID) {
	if (autocorr_exitinit || autocorr_GlobalStacktree == NULL)
		return 0;
	uint64_t store[100];
	

#if defined(__powerpc64__) || defined(__POWERPC__) || defined(_ARCH_PPC64) || defined(_ARCH_PPC) || defined(__powerpc__) || defined(__powerpc)
	// Dyninst's stack frame will appear on PPC, we must skip it
	uint64_t stackSize = Stackwalk_GetStackID_GetGNUBtrace(store, 100);
	store[2] = callerID;
	std::cerr << "Start stack" << std::endl;
	for (int i = 2; i < stackSize; i++)
		std::cerr << std::hex << store[i] << std::endl;
	std::cerr << "IN PPC FILE" << std::endl;
	return autocorr_GlobalStacktree->Lookup(&(store[2]), stackSize - 2);
#else
	// insert caller id at top of stack, which is at 2 accounting for
	// our caller.
	uint64_t stackSize = Stackwalk_GetStackLibUnwind(store, 100);
	store[1] = callerID;
	return autocorr_GlobalStacktree->Lookup(&(store[1]), stackSize - 1);
#endif
}

int autocorr_cuMemAllocHost222_v2(void ** ptr, size_t size) {
	//std::cerr << "in memallochost" <<std::endl;
	int ret = autocorr_cuMemAllocHost222_v2_wrapper(ptr,size);
	if(local_CPUMEMLOCATION == NULL)
		local_CPUMEMLOCATION = new CPUMemoryLocation();
	
	local_CPUMEMLOCATION->RegisterAllocation(*ptr, size, true);
	return ret;
}
int autocorr_cuMemAlloc(void ** ptr, size_t size) {
	//std::cerr << "in memalloc" <<std::endl;
	// return autocorr_cuMemAlloc_wrapper(ptr,size);
	if(autocorr_exitinit)
		return autocorr_cuMemAlloc_wrapper(ptr,size);

	if(local_SimpleGPUMap == NULL)
		local_SimpleGPUMap = new SimpleGPUMallocMap();
	*ptr = local_SimpleGPUMap->GetAllocation(size);
	return (int)CUDA_SUCCESS;
}


int autocorr_cuMemFreeHost222(void * ptr) {
	//std::cerr << "in memfreehost" <<std::endl;
	if(local_CPUMEMLOCATION == NULL)
		local_CPUMEMLOCATION = new CPUMemoryLocation();
	
	local_CPUMEMLOCATION->UnregisterAllocation(ptr);
	return autocorr_cuMemFreeHost222_wrapper(ptr);
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

// #define AUTOCORR_CALLID_cuMemcpyDtoHAsync_v2 1
// #define AUTOCORR_CALLID_cuMemcpyHtoDAsync_v2 2
// #define AUTOCORR_CALLID_cuMemcpyHtoD_v2 3
// #define AUTOCORR_CALLID_cuMemcpyDtoH_v2 4
// #define AUTOCORR_CALLID_cuMemFree_v2 5
int autocorr_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount ) {
	if(local_CPUMEMLOCATION == NULL)
		local_CPUMEMLOCATION = new CPUMemoryLocation();
	//void * cpuPtr = local_CPUMEMLOCATION->GetAllocation((void*)srcHost,ByteCount,true);
	autocorr_cuMemcpyHtoD_v2_wrapper(dstDevice, srcHost, ByteCount);
	//std::cerr << "CPUPTR TO SEND: " << std::hex << cpuPtr << " ORIGINAL: " << std::hex << srcHost << std::endl;
	// if (!autocorr_IsUnnecessary(AUTOCORR_CALLID_cuMemcpyHtoD_v2)) {
	// 	//std::cerr << "cuMemcpyHtoD synchronization is required" << std::endl;
	// 	return autocorr_cuMemcpyHtoD_v2_wrapper(dstDevice, cpuPtr, ByteCount);
	// } else {
	// 	//std::cerr << "cuMemcpyHtoD synchronization is NOT required" << std::endl;
	// 	return autocorr_cuMemcpyHtoDAsync_v2_wrapper(dstDevice, cpuPtr, ByteCount,0);
	// }
	//return autocorr_cuMemcpyHtoD_v2_wrapper(dstDevice, srcHost, ByteCount);
}
	// void * GetAllocation(void * cpuDestAddr,uint64_t size, bool toDevice);
	// void RegisterAllocation(void * ptr, uint64_t size, bool appAllocated);
	// void FreeAllocation(void * ptr);
	// void CopyToDestination();
	// void UnregisterAllocation(void * ptr, uint64_t size);
int autocorr_cuMemcpyDtoH_v222(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount) {
	if(local_CPUMEMLOCATION == NULL)
		local_CPUMEMLOCATION = new CPUMemoryLocation();

	//if (!autocorr_IsUnnecessary(AUTOCORR_CALLID_cuMemcpyDtoH_v2)) {
		//std::cerr << "cuMemcpyDtoH synchronization is required" << std::endl;
		return autocorr_cuMemcpyDtoH_v222_wrapper(dstHost, srcDevice, ByteCount);
	//} else {
		//std::cerr << "cuMemcpyDotH synchronization is NOT required" << std::endl;
		//return autocorr_cuMemcpyDtoHAsync_v2_wrapper(dstHost, srcDevice, ByteCount,0);
	//}
	//return autocorr_cuMemcpyDtoH_v2_wrapper(dstHost, srcDevice, ByteCount);
}

int autocorr_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream) {

	return autocorr_cuMemcpyDtoHAsync_v2_wrapper(dstHost, srcDevice, ByteCount, hStream);
}
int autocorr_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream) {

	return autocorr_cuMemcpyHtoDAsync_v2_wrapper(dstDevice, srcHost, ByteCount, hStream);
}






