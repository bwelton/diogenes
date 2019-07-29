#include "Parameters.h"
#include <cassert>

typedef int (*TPVoid0)(void);
typedef int (*TPVoid1)(void *);
typedef int (*TPVoid2)(void *,void *);
typedef int (*TPVoid3)(void *,void *,void *);
typedef int (*TPVoid4)(void *,void *,void *,void *);
typedef int (*TPVoid5)(void *,void *,void *,void *,void *);
typedef int (*TPVoid6)(void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid7)(void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid8)(void *,void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid9)(void *,void *,void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid10)(void *,void *,void *,void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid11)(void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid12)(void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid13)(void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid14)(void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *);
typedef int (*TPVoid15)(void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *);

static const std::vector<const char *> CallVector = {"cuGetErrorString","cuGetErrorName","cuInit","cuDriverGetVersion","cuDeviceGet","cuDeviceGetCount","cuDeviceGetName","cuDeviceTotalMem","cuDeviceGetAttribute","cuDeviceGetProperties","cuDeviceComputeCapability","cuDevicePrimaryCtxRetain","cuDevicePrimaryCtxRelease","cuDevicePrimaryCtxSetFlags","cuDevicePrimaryCtxGetState","cuDevicePrimaryCtxReset","cuCtxCreate","cuCtxDestroy","cuCtxPushCurrent","cuCtxPopCurrent","cuCtxSetCurrent","cuCtxGetCurrent","cuCtxGetDevice","cuCtxGetFlags","cuCtxSynchronize","cuCtxSetLimit","cuCtxGetLimit","cuCtxGetCacheConfig","cuCtxSetCacheConfig","cuCtxGetSharedMemConfig","cuCtxSetSharedMemConfig","cuCtxGetApiVersion","cuCtxGetStreamPriorityRange","cuCtxAttach","cuCtxDetach","cuModuleLoad","cuModuleLoadData","cuModuleLoadDataEx","cuModuleLoadFatBinary","cuModuleUnload","cuModuleGetFunction","cuModuleGetGlobal","cuModuleGetTexRef","cuModuleGetSurfRef","cuLinkCreate","cuLinkAddData","cuLinkAddFile","cuLinkComplete","cuLinkDestroy","cuMemGetInfo","cuMemAlloc","cuMemAllocPitch","cuMemFree","cuMemGetAddressRange","cuMemAllocHost","cuMemFreeHost","cuMemHostAlloc","cuMemHostGetDevicePointer","cuMemHostGetFlags","cuMemAllocManaged","cuDeviceGetByPCIBusId","cuDeviceGetPCIBusId","cuIpcGetEventHandle","cuIpcOpenEventHandle","cuIpcGetMemHandle","cuIpcOpenMemHandle","cuIpcCloseMemHandle","cuMemHostRegister","cuMemHostUnregister","cuMemcpy","cuMemcpyPeer","cuMemcpyHtoD","cuMemcpyDtoH","cuMemcpyDtoD","cuMemcpyDtoA","cuMemcpyAtoD","cuMemcpyHtoA","cuMemcpyAtoH","cuMemcpyAtoA","cuMemcpy2D","cuMemcpy2DUnaligned","cuMemcpy3D","cuMemcpy3DPeer","cuMemcpyAsync","cuMemcpyPeerAsync","cuMemcpyHtoDAsync","cuMemcpyDtoHAsync","cuMemcpyDtoDAsync","cuMemcpyHtoAAsync","cuMemcpyAtoHAsync","cuMemcpy2DAsync","cuMemcpy3DAsync","cuMemcpy3DPeerAsync","cuMemsetD8","cuMemsetD16","cuMemsetD32","cuMemsetD2D8","cuMemsetD2D16","cuMemsetD2D32","cuMemsetD8Async","cuMemsetD16Async","cuMemsetD32Async","cuMemsetD2D8Async","cuMemsetD2D16Async","cuMemsetD2D32Async","cuArrayCreate","cuArrayGetDescriptor","cuArrayDestroy","cuArray3DCreate","cuArray3DGetDescriptor","cuMipmappedArrayCreate","cuMipmappedArrayGetLevel","cuMipmappedArrayDestroy","cuPointerGetAttribute","cuMemPrefetchAsync","cuMemAdvise","cuMemRangeGetAttribute","cuMemRangeGetAttributes","cuPointerSetAttribute","cuPointerGetAttributes","cuStreamCreate","cuStreamCreateWithPriority","cuStreamGetPriority","cuStreamGetFlags","cuStreamWaitEvent","cuStreamAddCallback","cuStreamAttachMemAsync","cuStreamQuery","cuStreamSynchronize","cuStreamDestroy","cuEventCreate","cuEventRecord","cuEventQuery","cuEventSynchronize","cuEventDestroy","cuEventElapsedTime","cuStreamWaitValue32","cuStreamWriteValue32","cuStreamBatchMemOp","cuFuncGetAttribute","cuFuncSetCacheConfig","cuFuncSetSharedMemConfig","cuLaunchKernel","cuFuncSetBlockShape","cuFuncSetSharedSize","cuParamSetSize","cuParamSeti","cuParamSetf","cuParamSetv","cuLaunch","cuLaunchGrid","cuLaunchGridAsync","cuParamSetTexRef","cuOccupancyMaxActiveBlocksPerMultiprocessor","cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags","cuOccupancyMaxPotentialBlockSize","cuOccupancyMaxPotentialBlockSizeWithFlags","cuTexRefSetArray","cuTexRefSetMipmappedArray","cuTexRefSetAddress","cuTexRefSetAddress2D","cuTexRefSetFormat","cuTexRefSetAddressMode","cuTexRefSetFilterMode","cuTexRefSetMipmapFilterMode","cuTexRefSetMipmapLevelBias","cuTexRefSetMipmapLevelClamp","cuTexRefSetMaxAnisotropy","cuTexRefSetBorderColor","cuTexRefSetFlags","cuTexRefGetAddress","cuTexRefGetArray","cuTexRefGetMipmappedArray","cuTexRefGetAddressMode","cuTexRefGetFilterMode","cuTexRefGetFormat","cuTexRefGetMipmapFilterMode","cuTexRefGetMipmapLevelBias","cuTexRefGetMipmapLevelClamp","cuTexRefGetMaxAnisotropy","cuTexRefGetBorderColor","cuTexRefGetFlags","cuTexRefCreate","cuTexRefDestroy","cuSurfRefSetArray","cuSurfRefGetArray","cuTexObjectCreate","cuTexObjectDestroy","cuTexObjectGetResourceDesc","cuTexObjectGetTextureDesc","cuTexObjectGetResourceViewDesc","cuSurfObjectCreate","cuSurfObjectDestroy","cuSurfObjectGetResourceDesc","cuDeviceCanAccessPeer","cuDeviceGetP2PAttribute","cuCtxEnablePeerAccess","cuCtxDisablePeerAccess","cuGraphicsUnregisterResource","cuGraphicsSubResourceGetMappedArray","cuGraphicsResourceGetMappedMipmappedArray","cuGraphicsResourceGetMappedPointer","cuGraphicsResourceSetMapFlags","cuGraphicsMapResources","cuGraphicsUnmapResources","cuGetExportTable","cuTexRefSetAddress2D_v2","cuMemcpyHtoD_v2","cuMemcpyDtoH_v2","cuMemcpyDtoD_v2","cuMemcpyDtoA_v2","cuMemcpyAtoD_v2","cuMemcpyHtoA_v2","cuMemcpyAtoH_v2","cuMemcpyAtoA_v2","cuMemcpyHtoAAsync_v2","cuMemcpyAtoHAsync_v2","cuMemcpy2D_v2","cuMemcpy2DUnaligned_v2","cuMemcpy3D_v2","cuMemcpyHtoDAsync_v2","cuMemcpyDtoHAsync_v2","cuMemcpyDtoDAsync_v2","cuMemcpy2DAsync_v2","cuMemcpy3DAsync_v2","cuMemsetD8_v2","cuMemsetD16_v2","cuMemsetD32_v2","cuMemsetD2D8_v2","cuMemsetD2D16_v2","cuMemsetD2D32_v2", "InternalSynchronization"};
std::set<CallID> * TransferCallIDs;
std::set<CallID> * StandardCopies;
std::set<CallID> * ArrayCopies;
std::set<CallID> * UnsupportedCopies;
extern "C" void InitParameterData() {
	// Initalize Transfer Call Data
	std::vector<CallID> tmp = TRANSFER_CALLS
	std::vector<CallID> stdCopies = STANDARD_COPIES
	std::vector<CallID> arrayCopies = ARRAY_COPIES
	std::vector<CallID> unsupportedCopies = UNSUPPORTED_TRANSFERS

	TransferCallIDs = new std::set<CallID>();
	for (auto i : tmp)
		TransferCallIDs->insert(i);

	StandardCopies =  new std::set<CallID>();
	for (auto i : stdCopies)
		StandardCopies->insert(i);	

	ArrayCopies =  new std::set<CallID>();
	for (auto i : arrayCopies)
		ArrayCopies->insert(i);		

	UnsupportedCopies = new std::set<CallID>();
	for (auto i : unsupportedCopies)
		UnsupportedCopies->insert(i);			
}

MemoryTransfer * Parameters::GetMemtrans() {
	return _memTrans;
}
Parameters::Parameters(CallID id, void * func, std::vector<void**> v) : _callId(id), _func(func), values(v) {
	_hash = 0;
	_called = false;
	_memTrans = new MemoryTransfer(this);
}
const char * Parameters::GetName() {return CallVector[(int)_callId];}

Parameters::~Parameters(){
	delete _memTrans;
}


CallID Parameters::GetID() {
	return _callId;
}

bool Parameters::Called(){
	return _called;
}

uint32_t Parameters::GetHash(){
	return _hash;
}


void Parameters::SetHash(uint32_t hash){
	_hash = hash;
}
void Parameters::SetInstID(size_t i){
	_instId = i;
}
size_t Parameters::GetInstID(){
	return _instId;
}
int Parameters::Call(){
	if(_called == false){
		_ret = CallUnderlying();
		_called = true;
	}
	return _ret;

}

int Parameters::GetReturn() {
	if (_called == false)
		return Call();
	return _ret;
}

int Parameters::CallUnderlying(){
	switch(values.size()) {
		case 0: return ((TPVoid0)_func)();
		case 1: return ((TPVoid1)_func)(*values[0]);
		case 2: return ((TPVoid2)_func)(*values[0],*values[1]);
		case 3: return ((TPVoid3)_func)(*values[0],*values[1],*values[2]);
		case 4: return ((TPVoid4)_func)(*values[0],*values[1],*values[2],*values[3]);
		case 5: return ((TPVoid5)_func)(*values[0],*values[1],*values[2],*values[3],*values[4]);
		case 6: return ((TPVoid6)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5]);
		case 7: return ((TPVoid7)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6]);
		case 8: return ((TPVoid8)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7]);
		case 9: return ((TPVoid9)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7],*values[8]);
		case 10: return ((TPVoid10)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7],*values[8],*values[9]);
		case 11: return ((TPVoid11)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7],*values[8],*values[9],*values[10]);
		case 12: return ((TPVoid12)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7],*values[8],*values[9],*values[10],*values[11]);
		case 13: return ((TPVoid13)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7],*values[8],*values[9],*values[10],*values[11],*values[12]);
		case 14: return ((TPVoid14)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7],*values[8],*values[9],*values[10],*values[11],*values[12],*values[13]);
		case 15: return ((TPVoid15)_func)(*values[0],*values[1],*values[2],*values[3],*values[4],*values[5],*values[6],*values[7],*values[8],*values[9],*values[10],*values[11],*values[12],*values[13],*values[14]);
	}	
	assert (1 == 0);
	return -1;
}

void * Parameters::GetParameter(int pos) {
	return (void*)(values[pos]);
}
size_t Parameters::GetLen() {
	return values.size();
}


