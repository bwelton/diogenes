#ifndef SYNCDETECTOR_MUTATEE_HEADER
#define SYNCDETECTOR_MUTATEE_HEADER 1
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include <unordered_map>
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


// Exit handlers
void autocorr_mutatee_exit_handler();
// Wrappers
int autocorr_cuMemAllocHost_v2(void ** ptr, size_t size);
int autocorr_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount);
int autocorr_cuMemcpyDtoH_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount);
int autocorr_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream);
int autocorr_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream);
int autocorr_cuMemFreeHost(void * ptr);
int autocorr_cuMemAlloc(void ** ptr, size_t size) ;
int autocorr_cuMemFree22(void* ptr);
int autocorr_synchronize_device();

//uint64_t GetStackID();


struct SimpleGPUMallocMap {
	std::unordered_multimap<uint64_t, void*> _sizeToAddress;
	std::unordered_map<void*, uint64_t> _addrToSize;
	void * GetAllocation(uint64_t size);
	void FreeAllocation(void * addr);
};
// Global wrapper variables
extern typeof(&autocorr_cuMemAllocHost_v2) autocorr_cuMemAllocHost_v2_wrapper;
extern typeof(&autocorr_cuMemcpyHtoD_v2) autocorr_cuMemcpyHtoD_v2_wrapper;
extern typeof(&autocorr_cuMemcpyDtoH_v2) autocorr_cuMemcpyDtoH_v2_wrapper;
extern typeof(&autocorr_cuMemcpyDtoHAsync_v2) autocorr_cuMemcpyDtoHAsync_v2_wrapper;
extern typeof(&autocorr_cuMemcpyHtoDAsync_v2) autocorr_cuMemcpyHtoDAsync_v2_wrapper;
extern typeof(&autocorr_cuMemFreeHost) autocorr_cuMemFreeHost_wrapper;
extern typeof(&autocorr_cuMemAlloc) autocorr_cuMemAlloc_wrapper;
extern typeof(&autocorr_cuMemFree22) autocorr_cuMemFree22_wrapper;
extern typeof(&autocorr_synchronize_device) autocorr_ctxSynchronize_wrapper;
extern volatile bool autocorr_exitinit;
extern FastStackTree * autocorr_GlobalStacktree;
#endif