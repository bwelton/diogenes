#ifndef SYNCDETECTOR_MUTATEE_HEADER
#define SYNCDETECTOR_MUTATEE_HEADER 1
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include <unordered_map>
#include <map>
#include "gotcha/gotcha.h"
#include <pthread.h> 
#include <cuda.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <execinfo.h>
#include "AllocationTracker.h"
#include "AutocorrectFilenames.h"
#include "PageLocker.h"
#include "Stackwalk.h"
#include "StackTrie.h"
#include "FastStackTree.h"

#define AUTOCORR_CALLID_cuMemcpyDtoHAsync_v2 1
#define AUTOCORR_CALLID_cuMemcpyHtoDAsync_v2 2
#define AUTOCORR_CALLID_cuMemcpyHtoD_v2 3
#define AUTOCORR_CALLID_cuMemcpyDtoH_v2 4
#define AUTOCORR_CALLID_cuMemFree_v2 5

struct CPUAddressPair {
	void * origCPUAddr;
	void * pinnedCPUAddr;
	bool toDevice;
	uint64_t size;
	CPUAddressPair(void * c, void * p, uint64_t s, bool dev);
	inline void * GetOverlap(void * c, uint64_t s);
};

struct CPUMemoryAllocation {
	void * ptr;
	uint64_t size;
	bool appallocated; 
	CPUMemoryAllocation(void * p, uint64_t s, bool appAlloc);
};
class CPUMemoryLocation {
public:
	//void * GetAllocation(void * ptr, uint64_t size);
	void * GetAllocation(void * cpuDestAddr,uint64_t size, bool toDevice);
	void RegisterAllocation(void * ptr, uint64_t size, bool appAllocated);
	void FreeAllocation(void * ptr);
	void CopyToDestination();
	void UnregisterAllocation(void * ptr);
	std::unordered_multimap<uint64_t, void*> _freeMemory;
	std::map<void *, CPUAddressPair> _currentCPUTransfers;
	std::map<void *, CPUMemoryAllocation> _activeTransfers;
	std::map<void *, CPUMemoryAllocation> _allocationMap;
	std::map<void *, CPUMemoryAllocation> _activeAllocations;
};

// Exit handlers
void autocorr_mutatee_exit_handler();
// Wrappers
int autocorr_cuMemAllocHost_v2(void ** ptr, size_t size);
int autocorr_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount);
int autocorr_cuMemcpyDtoH_v222(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount);
int autocorr_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream);
int autocorr_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream);
int autocorr_cuMemFreeHost222(void * ptr);
int autocorr_cuMemAlloc(void ** ptr, size_t size) ;
int autocorr_cuMemFree22(void* ptr);
int autocorr_synchronize_device();
void AUTOCORR_SYNC_FINISHED();
//uint64_t GetStackID();


struct SimpleGPUMallocMap {
	std::multimap<uint64_t, void*> _sizeToAddress;
	//std::unordered_multimap<uint64_t, void*> _sizeToAddress;
	std::unordered_map<void*, uint64_t> _addrToSize;
	void * GetAllocation(uint64_t size);
	void FreeAllocation(void * addr);
};
// Global wrapper variables
extern typeof(&autocorr_cuMemAllocHost_v2) autocorr_cuMemAllocHost_v2_wrapper;
extern typeof(&autocorr_cuMemcpyHtoD_v2) autocorr_cuMemcpyHtoD_v2_wrapper;
extern typeof(&autocorr_cuMemcpyDtoH_v222) autocorr_cuMemcpyDtoH_v222_wrapper;
extern typeof(&autocorr_cuMemcpyDtoHAsync_v2) autocorr_cuMemcpyDtoHAsync_v2_wrapper;
extern typeof(&autocorr_cuMemcpyHtoDAsync_v2) autocorr_cuMemcpyHtoDAsync_v2_wrapper;
extern typeof(&autocorr_cuMemFreeHost222) autocorr_cuMemFreeHost222_wrapper;
extern typeof(&autocorr_cuMemAlloc) autocorr_cuMemAlloc_wrapper;
extern typeof(&autocorr_cuMemFree22) autocorr_cuMemFree22_wrapper;
extern typeof(&autocorr_synchronize_device) autocorr_ctxSynchronize_wrapper;
extern volatile bool autocorr_exitinit;
extern FastStackTree * autocorr_GlobalStacktree;
#endif