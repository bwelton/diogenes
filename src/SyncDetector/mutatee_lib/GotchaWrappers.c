#include "SyncDetector.h"

StackwalkInst * globalWalker = NULL;

volatile bool syncdetect_disablememcapture = false;
volatile uint64_t currentStackID = 0;
size_t pagesize = 0;
volatile bool syncdetect_exitinit = false;
StackTrie * syncdetect_necessary_syncs = NULL;

void WriteStacktrieToFile(const char * fname, StackTrie * write) {
	if (write == NULL)
		return;
	FILE * out = fopen(fname, "wb");
	CVector * resultVec = CVector_Init(syncdetect_malloc_wrapper, syncdetect_free_wrapper, 100000);
	StackTrie_ConvertTreeToStackKey(write, resultVec, NULL);
	size_t size = 0;
	char * ptr = (char *)CVector_GetData(resultVec,&size);
	ptr[size] = '\000';
	fwrite(ptr, 1, size, out);
	fclose(out);
}


void mutatee_exit_handler() {
	syncdetect_exitinit = true;
	CaptureProcMap_Write("ProcMap.txt");
	WriteStacktrieToFile(SYNC_STACKCOLLISION_FILE, syncdetect_necessary_syncs);
	if (globalWalker != NULL)
		WriteStacktrieToFile(SYNC_STACKCAPTURE_FILE,globalWalker->tree);
	globalWalker = NULL;
	syncdetect_necessary_syncs = NULL;
}

uint64_t GetStackID() {
	if (syncdetect_exitinit)
		return 0;
	if (globalWalker == NULL){
		globalWalker = Stackwalk_Init(syncdetect_malloc_wrapper,syncdetect_free_wrapper);
	}
	if (syncdetect_necessary_syncs == NULL)
		syncdetect_necessary_syncs = StackTrie_Initalize(0, NULL, syncdetect_malloc_wrapper, syncdetect_free_wrapper);
	syncdetect_disablememcapture = true;
	uint64_t ret =  Stackwalk_GetStackID(globalWalker);
	syncdetect_disablememcapture = false;
	return ret;
}

void DIOG_Synchronization_Post() {
	if(syncdetect_exitinit)
		return;
	
	currentStackID = GetStackID();
	fprintf(stderr, "Inside Synchronization - ID = %"PRIu64"\n", currentStackID);
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
	//fprintf(stderr,"IN MALLOC\n");
	if (syncdetect_disablememcapture)
		return syncdetect_malloc_wrapper(size);
	//void * ret= syncdetect_malloc_wrapper(size);
    void * ret= memalign(pagesize,GetPageRound(size));
    if(!syncdetect_exitinit)
    	PageLocker_AddMemoryAllocation(ret, GetPageRound(size));
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
	fprintf(stderr, "In htod_v2\n");
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

