#include "SyncDetector.h"

StackwalkInst * globalWalker = NULL;
volatile bool memgraph_disablememcapture = false;
volatile bool memgraph_exitinit = false;


void mutatee_exit_handler() {
	memgraph_exitinit = true;
	globalWalker = NULL;
}


int memgraph_cuMemAllocHost_v2(void ** ptr, size_t size) {

}
void * memgraph_malloc(size_t size) {

}

int memgraph_cuMemFreeHost(void * ptr) {

}

void memgraph_free(void * ptr) {

}

int memgraph_cuMemAllocManaged(void ** ptr, size_t len, unsigned int flags) {
  
}

int memgraph_cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount ) {

}

int memgraph_cuMemcpyDtoH_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount) {

}


int memgraph_cuMemcpyDtoHAsync_v2(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream) {

}
int memgraph_cuMemcpyHtoDAsync_v2(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount, CUstream hStream) {

}
int memgraph_cuMemcpyAsync(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream) {

}

