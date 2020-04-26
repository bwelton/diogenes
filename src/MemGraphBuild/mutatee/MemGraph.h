#ifndef SYNCDETECTOR_MUTATEE_HEADER
#define SYNCDETECTOR_MUTATEE_HEADER 1
#include <sys/types.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include "gotcha/gotcha.h"
#include <pthread.h> 
#include <cuda.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <execinfo.h>
#include "AllocationTracker.h"
#include "PageLocker.h"
#include "Stackwalk.h"
#include "StackTrie.h"
#include "MemGraphFilenames.h"
#include "CaptureProcMap.h"


typedef struct NecessarySyncs{
	uint64_t sync;
	uint64_t use;
} NecessarySyncs;

// Exit handlers
void mutatee_exit_handler();
// Wrappers
int memgraph_cuMemAllocHost_v2(void ** ptr, size_t size);
void * memgraph_malloc(size_t size);
int memgraph_cuMemAllocManaged(void ** ptr, size_t len, unsigned int flags);
int memgraph_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount);
int memgraph_cuMemcpyDtoH_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount);
int memgraph_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream);
int memgraph_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream);
int memgraph_cuMemcpyAsync(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream);
int memgraph_cuMemFreeHost(void * ptr);
void memgraph_free(void * ptr);
int memgraph_cuMemAlloc(void ** ptr, size_t size) ;
int memgraph_cuMemFree(void* ptr);
uint64_t GetStackID();

// Global wrapper variables
extern typeof(&memgraph_cuMemAllocHost_v2) memgraph_cuMemAllocHost_v2_wrapper;
extern typeof(&memgraph_malloc) memgraph_malloc_wrapper;
extern typeof(&memgraph_cuMemAllocManaged) memgraph_cuMemAllocManaged_wrapper;
extern typeof(&memgraph_cuMemcpyHtoD_v2) memgraph_cuMemcpyHtoD_v2_wrapper;
extern typeof(&memgraph_cuMemcpyDtoH_v2) memgraph_cuMemcpyDtoH_v2_wrapper;
extern typeof(&memgraph_free) memgraph_free_wrapper;
extern typeof(&memgraph_cuMemcpyDtoHAsync_v2) memgraph_cuMemcpyDtoHAsync_v2_wrapper;
extern typeof(&memgraph_cuMemcpyHtoDAsync_v2) memgraph_cuMemcpyHtoDAsync_v2_wrapper;
extern typeof(&memgraph_cuMemcpyAsync) memgraph_cuMemcpyAsync_wrapper;
extern typeof(&memgraph_cuMemFreeHost) memgraph_cuMemFreeHost_wrapper;
extern typeof(&memgraph_cuMemAlloc) memgraph_cuMemAlloc_wrapper;
extern typeof(&memgraph_cuMemFree) memgraph_cuMemFree_wrapper;
extern StackwalkInst * memgraph_globalWalker;
extern volatile uint64_t currentStackID;
extern volatile bool memgraph_exitinit;
extern StackTrie * memgraph_necessary_syncs;

extern pthread_t memgraph_doInst;
#endif