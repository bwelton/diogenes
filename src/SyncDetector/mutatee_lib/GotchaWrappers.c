#include "SyncDetector.h"

StackwalkInst * globalWalker = NULL;

size_t pagesize = 0;
volatile bool syncdetect_exitinit = false;

void mutatee_exit_handler() {
	syncdetect_exitinit = true;
}

uint64_t GetStackID() {
	if (globalWalker == NULL){
		globalWalker = Stackwalk_Init(syncdetect_malloc_wrapper,syncdetect_free_wrapper);
	}
	return Stackwalk_GetStackID(globalWalker);
}

void DIOG_Synchronization_Post() {
	uint64_t stackID = GetStackID();
	PageLocker * local_locker = PageLocker_GetThreadSpecific();
	PageLocker_LockMemory(local_locker);
}

size_t GetPageRound(size_t size) {
	if (pagesize == 0)
		pagesize = getpagesize();
    size_t ret = ((size + pagesize-1) & ~(pagesize-1));//size + (4096 % size);
    // fprintf(stderr, "Allocation size prior= %llu After= %llu\n", size, ret);
    return ret;
}

int syncdetect_cuMemAllocHost_v2(void ** ptr, size_t size) {
    fprintf(stderr, "in cuMemallocHost\n");
    *ptr = malloc(size);
    return cuMemHostRegister(*ptr, size, 0);
}
void * syncdetect_malloc(size_t size) {
	if (pagesize == 0)
		pagesize = getpagesize();
	//void * ret= syncdetect_malloc_wrapper(size);
    void * ret= memalign(pagesize,size);
/*    if(!syncdetect_exitinit)
    	PageLocker_AddMemoryAllocation(ret, size);*/
    return ret;
}

void syncdetect_free(void * ptr) {
/*	if(!syncdetect_exitinit)
		PageLocker_FreeMemoryAllocation(ptr);
		*/
    return syncdetect_free_wrapper(ptr);
}

int syncdetect_cuMemAllocManaged(void ** ptr, size_t len, unsigned int flags) {
	fprintf(stderr , "In Malloc Managed\n");
    return syncdetect_cuMemAllocManaged_wrapper(ptr,GetPageRound(len),flags);	    
}

inline RelockIndex syncdetect_Pretransfer(void * memAddr, size_t size) {
	if (!syncdetect_exitinit){
		PageLocker * local_locker = PageLocker_GetThreadSpecific();
		return PageLocker_TempUnlockAddress(local_locker, memAddr,size);
	}
	return -1;
}


inline void syncdetect_LockAddress(void * memAddr, size_t size, RelockIndex index) {
	if (!syncdetect_exitinit) {
		PageLocker * local_locker = PageLocker_GetThreadSpecific();
		if (index != -1){
			PageLocker_RelockIndex(local_locker, index);
			return;
		} else {
			PageLocker_AddTransferPage(local_locker,memAddr,size);
		}
	}
}


int syncdetect_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount ) {
	int ret;
	RelockIndex tmp = syncdetect_Pretransfer((void*)srcHost, ByteCount);
	ret = syncdetect_cuMemcpyHtoD_v2_wrapper(dstDevice,  srcHost, ByteCount);
	syncdetect_LockAddress((void*)srcHost, ByteCount, tmp);
	return ret;
}

int syncdetect_cuMemcpyDtoH_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount) {
	int ret;
	RelockIndex tmp = syncdetect_Pretransfer((void*)dstHost, ByteCount);
	ret = syncdetect_cuMemcpyDtoH_v2_wrapper(dstHost, srcDevice, ByteCount);
	syncdetect_LockAddress((void*)dstHost, ByteCount, tmp);
	return ret;
}


int syncdetect_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream) {
	int ret;
	RelockIndex tmp = syncdetect_Pretransfer((void*)dstHost, ByteCount);
    ret = syncdetect_cuMemcpyDtoHAsync_v2_wrapper(dstHost, srcDevice, ByteCount, hStream);
    syncdetect_LockAddress((void*)dstHost, ByteCount, tmp);
    return ret;
    //return syncdetect_cuMemcpyDtoHAsync_v2_wrapper;
}
int syncdetect_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream) {
	int ret;
	RelockIndex tmp = syncdetect_Pretransfer((void*)srcHost, ByteCount);
	ret =  syncdetect_cuMemcpyHtoDAsync_v2_wrapper(dstDevice, srcHost, ByteCount, hStream);
	syncdetect_LockAddress((void*)srcHost, ByteCount, tmp);
	return ret;
    //return syncdetect_cuMemcpyHtoDAsync_v2_wrapper;
}
int syncdetect_cuMemcpyAsync(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream) {
	int ret;
	RelockIndex tmp1 = syncdetect_Pretransfer((void*)dst, ByteCount);
	ret = syncdetect_cuMemcpyAsync_wrapper(dst, src, ByteCount, hStream);
	syncdetect_LockAddress((void*)dst, ByteCount, tmp1);
	return ret;
    //return syncdetect_cuMemcpyAsync_wrapper;
}

