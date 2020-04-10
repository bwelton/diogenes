#ifndef SYNCDETECTOR_MUTATEE_HEADER
#define SYNCDETECTOR_MUTATEE_HEADER 1
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
#include "PageLocker.h"
#include "Stackwalk.h"
#include "StackTrie.h"
#include "SyncdetectFilenames.h"

typedef struct NecessarySyncs{
	uint64_t sync;
	uint64_t use;
} NecessarySyncs;

// Exit handlers
void mutatee_exit_handler();
void DIOG_Synchronization_Post();

// Wrappers
void syncdetect_catchSegFault(int sig, siginfo_t *siginfo, void *context);
int syncdetect_cuMemAllocHost_v2(void ** ptr, size_t size);
void * syncdetect_malloc(size_t size);
int syncdetect_cuMemAllocManaged(void ** ptr, size_t len, unsigned int flags);
int syncdetect_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount);
int syncdetect_cuMemcpyDtoH_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount);
int syncdetect_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream);
int syncdetect_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream);
int syncdetect_cuMemcpyAsync(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream);
void syncdetect_free(void * ptr);
uint64_t GetStackID();

// Global wrapper variables
extern typeof(&syncdetect_cuMemAllocHost_v2) syncdetect_cuMemAllocHost_v2_wrapper;
extern typeof(&syncdetect_malloc) syncdetect_malloc_wrapper;
extern typeof(&syncdetect_cuMemAllocManaged) syncdetect_cuMemAllocManaged_wrapper;
extern typeof(&syncdetect_cuMemcpyHtoD_v2) syncdetect_cuMemcpyHtoD_v2_wrapper;
extern typeof(&syncdetect_cuMemcpyDtoH_v2) syncdetect_cuMemcpyDtoH_v2_wrapper;
extern typeof(&syncdetect_free) syncdetect_free_wrapper;
extern typeof(&syncdetect_cuMemcpyDtoHAsync_v2) syncdetect_cuMemcpyDtoHAsync_v2_wrapper;
extern typeof(&syncdetect_cuMemcpyHtoDAsync_v2) syncdetect_cuMemcpyHtoDAsync_v2_wrapper;
extern typeof(&syncdetect_cuMemcpyAsync) syncdetect_cuMemcpyAsync_wrapper;
extern StackwalkInst * globalWalker;
extern volatile uint64_t currentStackID;
extern volatile bool syncdetect_exitinit;
extern StackTrie * syncdetect_necessary_syncs;
#endif