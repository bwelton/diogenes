#include "SyncDetector.h"
#include "CPPLocker.h"
#include "CPPStackTree.h"
StackwalkInst * globalWalker = NULL;

volatile bool syncdetect_disablememcapture = false;
volatile uint64_t currentStackID = 0;
size_t pagesize = 0;
volatile bool syncdetect_exitinit = false;
CPPStackTrie syncdetect_necessary_syncs = NULL;

void WriteStacktrieToFile(const char * fname, CPPStackTrie write) {
	if (write == NULL)
		return;
	FILE * out = fopen(fname, "wb");
	char * ptr;
	uint64_t size = 0;
	CPPStackTrie_ConvertTreeToStackKey(write,&ptr, &size);
/*
	CVector * resultVec = CVector_Init(syncdetect_malloc_wrapper, syncdetect_free_wrapper, 100000);
	StackTrie_ConvertTreeToStackKey(write, resultVec, NULL);
	size_t size = 0;
	char * ptr = (char *)CVector_GetData(resultVec,&size);
	ptr[size] = '\000';
*/
	fwrite(ptr, 1, size, out);
	fclose(out);
}


void mutatee_exit_handler() {
	syncdetect_exitinit = true;
	syncdetect_disablememcapture = true;
	CaptureProcMap_Write("ProcMap.txt");
	WriteStacktrieToFile(SYNC_STACKCOLLISION_FILE, syncdetect_necessary_syncs);
	if (globalWalker != NULL)
		WriteStacktrieToFile(SYNC_STACKCAPTURE_FILE,globalWalker->tree);
	globalWalker = NULL;
	syncdetect_necessary_syncs = NULL;
}

uint64_t GetStackID() {
	syncdetect_disablememcapture = true;
	if (syncdetect_exitinit)
		return 0;
	if (globalWalker == NULL){
		globalWalker = Stackwalk_Init(syncdetect_malloc_wrapper,syncdetect_free_wrapper);
	}
	if (syncdetect_necessary_syncs == NULL)
		syncdetect_necessary_syncs = CPPStackTrie_Initalize();// StackTrie_Initalize(0, NULL, syncdetect_malloc_wrapper, syncdetect_free_wrapper);
	uint64_t ret =  Stackwalk_GetStackID(globalWalker);
	syncdetect_disablememcapture = false;
	return ret;
}

void DIOG_Synchronization_Post() {
	if(syncdetect_exitinit)
		return;
	
	currentStackID = GetStackID();
	//fprintf(stderr, "Inside Synchronization - ID = %"PRIu64"\n", currentStackID);
	CPPLocker local_locker = CPPPageLocker_GetThreadSpecific();
	if (CPPPageLocker_LockMemory(local_locker) == false) {
		// Assume we have an address that is unlockable, this means
		// that we cannot detect potential uses and we should assume
		// that the synchronization is required
		CPPPageLocker_UnlockMemory(local_locker);
		CPPPageLocker_ClearToLockPages(local_locker);
		syncdetect_WriteNecessarySync(currentStackID,currentStackID);
	}
}

size_t GetPageRound(size_t size) {
	if (pagesize == 0)
		pagesize = getpagesize();
    size_t ret = ((size + pagesize-1) & ~(pagesize-1));//size + (4096 % size);
    // fprintf(stderr, "Allocation size prior= %llu After= %llu\n", size, ret);
    return ret;
}

int syncdetect_cuMemAllocHost_v2(void ** ptr, size_t size) {
    //fprintf(stderr, "in cuMemallocHost\n");
    *ptr = malloc(size);
    return cuMemHostRegister(*ptr, size, 0);
}
void * syncdetect_malloc(size_t size) {
	if (pagesize == 0)
		pagesize = getpagesize();
	//fprintf(stderr,"IN MALLOC\n");
	if (syncdetect_disablememcapture){
		return syncdetect_malloc_wrapper(size);
	}
	//void * ret= syncdetect_malloc_wrapper(size);
    void * ret= memalign(pagesize,GetPageRound(size));
    if(!syncdetect_exitinit)
    	CPPPageLocker_AddMemoryAllocation(CPPPageLocker_GetThreadSpecific(), ret, GetPageRound(size));
    return ret;
}

void syncdetect_free(void * ptr) {
	if(!syncdetect_exitinit && syncdetect_disablememcapture == false)
		CPPPageLocker_FreeMemoryAllocation(CPPPageLocker_GetThreadSpecific(),ptr);
    return syncdetect_free_wrapper(ptr);
}
int syncdetect_cuMemFreeHost(void * ptr) {
	cuMemHostUnregister(ptr);
	syncdetect_free(ptr);
	return (int)CUDA_SUCCESS;
}



int syncdetect_cuMemAllocManaged(void ** ptr, size_t len, unsigned int flags) {
	//fprintf(stderr , "In Malloc Managed\n");
    return syncdetect_cuMemAllocManaged_wrapper(ptr,GetPageRound(len),flags);	    
}

inline RelockIndex syncdetect_Pretransfer(void * memAddr, size_t size) {
	if (!syncdetect_exitinit){
		CPPLocker local_locker = CPPPageLocker_GetThreadSpecific();
		return CPPPageLocker_TempUnlockAddress(local_locker, memAddr,size);
	}
	return -1;
}


inline void syncdetect_LockAddress(void * memAddr, size_t size, RelockIndex index) {
	if (!syncdetect_exitinit) {
		CPPLocker local_locker = CPPPageLocker_GetThreadSpecific();
		if (index != -1){
			CPPPageLocker_RelockIndex(local_locker, index);
			return;
		} else {
			CPPPageLocker_AddTransferPage(local_locker,memAddr,size);
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

