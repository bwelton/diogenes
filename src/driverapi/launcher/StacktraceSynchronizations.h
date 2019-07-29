#pragma once
#include "DyninstProcess.h"
#include "Constants.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <iostream>
#include <memory>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>

#include "DynOpsClass.h"
#include "Common.h"

// Dyninst includes
#include "CodeObject.h"
#include "CFG.h"
#include "PatchObject.h"
#include "PatchMgr.h"
#include "Point.h"
#include "BPatch_object.h"
#include "BPatch_snippet.h"
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_image.h"
#include "BPatch_function.h"
#include "BPatch_Vector.h"
#include "BPatch_point.h"
#include "BPatch_addressSpace.h"
#include "BPatch_statement.h"
#include "BPatch_basicBlock.h"
#include "dynC.h"
#include "set"
#include "LogInfo.h"
#include "Constants.h"
#include "StackPoint.h"
#include "StackStorage.h"
#include "ReadStackKeys.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

#define CUDACALLLIST {"cuMemGetAttribute_v2","cuTexRefSetAddress2D_v2","cuMemcpyHtoD_v2","cuMemcpyDtoH_v2","cuMemcpyDtoD_v2","cuMemcpyDtoA_v2","cuMemcpyAtoD_v2","cuMemcpyHtoA_v2","cuMemcpyAtoH_v2","cuMemcpyAtoA_v2","cuMemcpyHtoAAsync_v2","cuMemcpyAtoHAsync_v2","cuMemcpy2D_v2","cuMemcpy2DUnaligned_v2","cuMemcpy3D_v2","cuMemcpyHtoDAsync_v2","cuMemcpyDtoHAsync_v2","cuMemcpyDtoDAsync_v2","cuMemcpy2DAsync_v2","cuMemcpy3DAsync_v2","cuMemsetD8_v2","cuMemsetD16_v2","cuMemsetD32_v2","cuMemsetD2D8_v2","cuMemsetD2D16_v2","cuMemsetD2D32_v2","cuInit","cuDriverGetVersion","cuDeviceGet","cuDeviceGetCount","cuDeviceGetName","cuDeviceTotalMem","cuDeviceGetAttribute","cuDeviceGetProperties","cuDeviceComputeCapability","cuDevicePrimaryCtxRetain","cuDevicePrimaryCtxRelease","cuDevicePrimaryCtxSetFlags","cuDevicePrimaryCtxGetState","cuDevicePrimaryCtxReset","cuCtxCreate","cuCtxDestroy","cuCtxPushCurrent","cuCtxPopCurrent","cuCtxSetCurrent","cuCtxGetCurrent","cuCtxGetFlags","cuCtxSynchronize","cuCtxSetLimit","cuCtxGetLimit","cuCtxGetCacheConfig","cuCtxSetCacheConfig","cuCtxGetSharedMemConfig","cuCtxSetSharedMemConfig","cuCtxGetApiVersion","cuCtxGetStreamPriorityRange","cuCtxAttach","cuCtxDetach","cuModuleLoad","cuModuleLoadData","cuModuleLoadDataEx","cuModuleLoadFatBinary","cuModuleUnload","cuModuleGetFunction","cuModuleGetGlobal","cuModuleGetTexRef","cuModuleGetSurfRef","cuLinkCreate","cuLinkAddFile","cuLinkComplete","cuLinkDestroy","cuMemGetInfo","cuMemAlloc","cuMemAllocPitch","cuMemFree_v2","cuMemGetAddressRange","cuMemAllocHost","cuMemFreeHost","cuMemHostAlloc","cuMemHostGetDevicePointer","cuMemHostGetFlags","cuMemAllocManaged","cuDeviceGetByPCIBusId","cuDeviceGetPCIBusId","cuIpcGetEventHandle","cuIpcOpenEventHandle","cuIpcGetMemHandle","cuIpcOpenMemHandle","cuIpcCloseMemHandle","cuMemHostRegister","cuMemHostUnregister","cuMemcpy","cuMemcpyPeer","cuMemcpyHtoD","cuMemcpyDtoH","cuMemcpyDtoD","cuMemcpyDtoA","cuMemcpyAtoD","cuMemcpyHtoA","cuMemcpyAtoH","cuMemcpyAtoA","cuMemcpy2D","cuMemcpy2DUnaligned","cuMemcpy3D","cuMemcpy3DPeer","cuMemcpyAsync","cuMemcpyPeerAsync","cuMemcpyHtoDAsync","cuMemcpyDtoHAsync","cuMemcpyDtoDAsync","cuMemcpyHtoAAsync","cuMemcpyAtoHAsync","cuMemcpy2DAsync","cuMemcpy3DAsync","cuMemcpy3DPeerAsync","cuMemsetD8","cuMemsetD16","cuMemsetD32","cuMemsetD2D8","cuMemsetD2D16","cuMemsetD2D32","cuMemsetD8Async","cuMemsetD16Async","cuMemsetD32Async","cuMemsetD2D8Async","cuMemsetD2D16Async","cuMemsetD2D32Async","cuArrayCreate","cuArrayGetDescriptor","cuArrayDestroy","cuArray3DCreate","cuArray3DGetDescriptor","cuMipmappedArrayCreate","cuMipmappedArrayGetLevel","cuMipmappedArrayDestroy","cuPointerGetAttribute","cuMemPrefetchAsync","cuMemAdvise","cuMemRangeGetAttribute","cuMemRangeGetAttributes","cuPointerSetAttribute","cuPointerGetAttributes","cuStreamCreate","cuStreamCreateWithPriority","cuStreamGetPriority","cuStreamGetFlags","cuStreamWaitEvent","cuStreamAddCallback","cuStreamAttachMemAsync","cuStreamQuery","cuStreamSynchronize","cuStreamDestroy","cuEventCreate","cuEventRecord","cuEventQuery","cuEventSynchronize","cuEventDestroy","cuEventElapsedTime","cuStreamWaitValue32","cuStreamWriteValue32","cuStreamBatchMemOp","cuFuncSetCacheConfig","cuFuncSetSharedMemConfig","cuFuncSetBlockShape","cuFuncSetSharedSize","cuParamSetSize","cuParamSeti","cuParamSetf","cuParamSetv","cuLaunchGrid","cuLaunchGridAsync","cuParamSetTexRef","cuOccupancyMaxActiveBlocksPerMultiprocessor","cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags","cuOccupancyMaxPotentialBlockSize*$blockSize","cuOccupancyMaxPotentialBlockSizeWithFlags*$blockSize","cuTexRefSetArray","cuTexRefSetMipmappedArray","cuTexRefSetAddress","cuTexRefSetAddress2D","cuTexRefSetFormat","cuTexRefSetAddressMode","cuTexRefSetFilterMode","cuTexRefSetMipmapFilterMode","cuTexRefSetMipmapLevelBias","cuTexRefSetMipmapLevelClamp","cuTexRefSetMaxAnisotropy","cuTexRefSetBorderColor","cuTexRefSetFlags","cuTexRefGetAddress","cuTexRefGetArray","cuTexRefGetMipmappedArray","cuTexRefGetAddressMode","cuTexRefGetFilterMode","cuTexRefGetFormat","cuTexRefGetMipmapFilterMode","cuTexRefGetMipmapLevelBias","cuTexRefGetMipmapLevelClamp","cuTexRefGetMaxAnisotropy","cuTexRefGetBorderColor","cuTexRefGetFlags","cuTexRefCreate","cuTexRefDestroy","cuSurfRefSetArray","cuSurfRefGetArray","cuTexObjectCreate*$pResViewDesc","cuTexObjectDestroy","cuTexObjectGetResourceDesc","cuTexObjectGetTextureDesc","cuTexObjectGetResourceViewDesc","cuSurfObjectCreate","cuSurfObjectDestroy","cuSurfObjectGetResourceDesc","cuDeviceCanAccessPeer","cuDeviceGetP2PAttribute","cuCtxEnablePeerAccess","cuCtxDisablePeerAccess","cuGraphicsUnregisterResource","cuGraphicsSubResourceGetMappedArray","cuGraphicsResourceGetMappedMipmappedArray","cuGraphicsResourceGetMappedPointer","cuGraphicsResourceSetMapFlags","cuGraphicsMapResources","cuGraphicsUnmapResources","cuGetExportTable","cuMemHostRegister","cuGraphicsResourceSetMapFlags","cuLinkCreate","cuLinkAddData* *$optionValues","cuLinkAddFile","cuTexRefSetAddress2D_v2","cuDeviceTotalMem","cuCtxCreate","cuModuleGetGlobal","cuMemGetInfo","cuMemAlloc","cuMemAllocPitch","cuMemFree","cuMemGetAddressRange","cuMemAllocHost","cuMemHostGetDevicePointer","cuMemcpyHtoD","cuMemcpyDtoH","cuMemcpyDtoD","cuMemcpyDtoA","cuMemcpyAtoD","cuMemcpyHtoA","cuMemcpyAtoH","cuMemcpyAtoA","cuMemcpyHtoAAsync","cuMemcpyAtoHAsync","cuMemcpy2D","cuMemcpy2DUnaligned","cuMemcpy3D","cuMemcpyHtoDAsync","cuMemcpyDtoHAsync","cuMemcpyDtoDAsync","cuMemcpy2DAsync","cuMemcpy3DAsync","cuMemsetD8","cuMemsetD16","cuMemsetD32","cuMemsetD2D8","cuMemsetD2D16","cuMemsetD2D32","cuArrayCreate","cuArrayGetDescriptor","cuArray3DCreate","cuArray3DGetDescriptor","cuTexRefSetAddress","cuTexRefSetAddress2D","cuTexRefGetAddress","cuGraphicsResourceGetMappedPointer","cuCtxDestroy","cuCtxPopCurrent","cuCtxPushCurrent","cuStreamDestroy","cuEventDestroy","cuMemcpyHtoD_v2","cuMemcpyDtoH_v2","cuMemcpyDtoD_v2","cuMemcpyDtoA_v2","cuMemcpyAtoD_v2","cuMemcpyHtoA_v2","cuMemcpyAtoH_v2","cuMemcpyAtoA_v2","cuMemcpyHtoAAsync_v2","cuMemcpyAtoHAsync_v2","cuMemcpy2D_v2","cuMemcpy2DUnaligned_v2","cuMemcpy3D_v2","cuMemcpyHtoDAsync_v2","cuMemcpyDtoHAsync_v2","cuMemcpyDtoDAsync_v2","cuMemcpy2DAsync_v2","cuMemcpy3DAsync_v2","cuMemsetD8_v2","cuMemsetD16_v2","cuMemsetD32_v2","cuMemsetD2D8_v2","cuMemsetD2D16_v2","cuMemsetD2D32_v2","cuMemcpy","cuMemcpyAsync","cuMemcpyPeer","cuMemcpyPeerAsync","cuMemcpy3DPeer","cuMemcpy3DPeerAsync","cuMemsetD8Async","cuMemsetD16Async","cuMemsetD32Async","cuMemsetD2D8Async","cuMemsetD2D16Async","cuMemsetD2D32Async","cuStreamGetPriority","cuStreamGetFlags","cuStreamWaitEvent","cuStreamAddCallback","cuStreamAttachMemAsync","cuStreamQuery","cuStreamSynchronize","cuEventRecord","cuGraphicsMapResources","cuGraphicsUnmapResources","cuMemPrefetchAsync","cuStreamWriteValue32","cuStreamWaitValue32","cuStreamBatchMemOp"}
class StacktraceSynchronizations { 
public:
	StacktraceSynchronizations(std::shared_ptr<DyninstProcess> proc);
	void InsertStacktracing();
	void InsertEntryInst();
	void ReadResults(StackRecMap & recs);
private:
	void Setup();
	std::shared_ptr<DyninstProcess> _proc;
	std::map<int, StackPoint> _idToPoint;
	BPatch_object * _libcuda;
	BPatch_function * _wrapperFunc;
	BPatch_function * _cudaSync;
	BPatch_function * _wrapSym;
};