#include "MemGraph.h"
#include <atomic> 

StackwalkInst * memgraph_globalWalker = NULL;
std::atomic<int> memgraph_disablememcapture(0);
volatile bool memgraph_exitinit = false;
volatile bool memgraph_ingetstack = false;
void Memgraph_WriteStacktrieToFile(const char * fname, CPPStackTrie write) {
	if (write == NULL)
		return;
	FILE * out = fopen(fname, "wb");
	char * ptr;
	uint64_t size = 0;
	CPPStackTrie_ConvertTreeToStackKey(write,&ptr, &size);

	fwrite(ptr, 1, size, out);
	fclose(out);
}

inline int memgraph_CheckMemProtection() {
	return memgraph_disablememcapture;
}

inline void memgraph_IncrementMemProtection() {
	memgraph_disablememcapture++;
}

inline void memgraph_DecrementMemProtection() {
	if (memgraph_disablememcapture>0)
		memgraph_disablememcapture--;
}

void mutatee_exit_handler() {
	memgraph_exitinit = true;
	memgraph_ingetstack = true;
	CaptureProcMap_Write(MEMGRAPH_PROCMAP);
	if (memgraph_globalWalker != NULL)
		Memgraph_WriteStacktrieToFile(MEMGRAPH_STACKFILE,memgraph_globalWalker->tree);
	auto tracker = MemGraphBuild::GetGlobalTracker();
	uint64_t size = 0;
	char * writeOutput = tracker->SerializeData(size);
	char *outName = MEMGRAPH_GRAPHDATA;
	FILE * out = fopen(outName, "wb");
	fwrite(writeOutput, 1, size, out);
	fclose(out);
	
	memgraph_globalWalker = NULL;
	
}


uint64_t memgraph_GetStackID() {
	if (memgraph_exitinit || memgraph_ingetstack)
		return 0;
	memgraph_IncrementMemProtection();
	memgraph_ingetstack = true;
	if (memgraph_globalWalker == NULL){
		memgraph_globalWalker = Stackwalk_Init(malloc,free);
	}
	// puts("Getting Stackwalk!");
	uint64_t ret =  Stackwalk_GetStackIDLockUnwind(memgraph_globalWalker);
	// puts("Finished Stackwalk!");
	memgraph_ingetstack = false;
	memgraph_DecrementMemProtection();
	return ret;
}


void memgraph_handletransfer(uint64_t cpuAddress, uint64_t size) {
	if (memgraph_CheckMemProtection()>0 || memgraph_exitinit ==true)
		return;
	memgraph_IncrementMemProtection();
	auto tracker = MemGraphBuild::GetGlobalTracker();
	uint64_t stackId = memgraph_GetStackID();
	tracker->HandleTransfer(stackId, cpuAddress, size);
	memgraph_DecrementMemProtection();
}

void memgraph_handleallocation(uint64_t memaddr, uint64_t size) {
	if(memgraph_CheckMemProtection()>0 || memgraph_exitinit ==true)
		return;
	memgraph_IncrementMemProtection();
	auto tracker = MemGraphBuild::GetGlobalTracker();
	uint64_t stackId = memgraph_GetStackID();
	tracker->HandleAllocation(stackId, memaddr, size);	
	memgraph_DecrementMemProtection();
}

void memgraph_handlefree(uint64_t memaddr) {
	if (memgraph_CheckMemProtection()>0 || memgraph_exitinit == true)
		return;
	memgraph_IncrementMemProtection();
	auto tracker = MemGraphBuild::GetGlobalTracker();
	uint64_t stackId = memgraph_GetStackID();
	tracker->HandleFree(stackId, memaddr);	
	memgraph_DecrementMemProtection();	
}

void * memgraph_malloc(size_t size) {
	void * ret = memgraph_malloc_wrapper(size);
	if (ret != NULL)
		memgraph_handleallocation((uint64_t)ret, size);
	return ret;
}


int memgraph_cuMemAllocHost_v2(void ** ptr, size_t size) {
	int ret = memgraph_cuMemAllocHost_v2_wrapper(ptr,size);
	if (*ptr != NULL)
		memgraph_handleallocation((uint64_t)(*ptr), size);
	return ret;
}
int memgraph_cuMemAlloc(void ** ptr, size_t size) {
	int ret = memgraph_cuMemAlloc_wrapper(ptr,size);
	if (*ptr != NULL)
		memgraph_handleallocation((uint64_t)(*ptr), size);
	return ret;
}
int memgraph_cuMemFreeHost(void * ptr) {
	if(ptr!=NULL)
		memgraph_handlefree((uint64_t)ptr);
	return memgraph_cuMemFreeHost_wrapper(ptr);
}

void memgraph_free(void * ptr) {
	if(ptr!=NULL)
		memgraph_handlefree((uint64_t)ptr);
	memgraph_free_wrapper(ptr);
	return;
}

int memgraph_cuMemFree(void * ptr) {
	if(ptr!=NULL)
		memgraph_handlefree((uint64_t)ptr);
	int ret = memgraph_cuMemFree_wrapper(ptr);
	return ret;
}

int memgraph_cuMemAllocManaged(void ** ptr, size_t len, unsigned int flags) {
	int ret = memgraph_cuMemAllocManaged_wrapper(ptr,len,flags);
	if (*ptr != NULL)
		memgraph_handleallocation((uint64_t)(*ptr), len);
	return ret;
}



int memgraph_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount ) {
	memgraph_handletransfer((uint64_t)srcHost, ByteCount);
	return memgraph_cuMemcpyHtoD_v2_wrapper(dstDevice, srcHost, ByteCount);
}

int memgraph_cuMemcpyDtoH_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount) {
	memgraph_handletransfer((uint64_t)dstHost, ByteCount);
	return memgraph_cuMemcpyDtoH_v2_wrapper(dstHost, srcDevice, ByteCount);
}

int memgraph_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream) {
	memgraph_handletransfer((uint64_t)dstHost, ByteCount);
	return memgraph_cuMemcpyDtoHAsync_v2_wrapper(dstHost, srcDevice, ByteCount, hStream);
}
int memgraph_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream) {
	memgraph_handletransfer((uint64_t)srcHost, ByteCount);
	return memgraph_cuMemcpyHtoDAsync_v2_wrapper(dstDevice, srcHost, ByteCount, hStream);
}

int memgraph_cuMemcpyAsync(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream) {
	return memgraph_cuMemcpyAsync_wrapper(dst, src,ByteCount,hStream);
}





