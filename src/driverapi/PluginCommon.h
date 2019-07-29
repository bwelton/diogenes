#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include <map>
#include <vector>
#include "cuda.h"
//#include "Parameters.h"
//#include "DriverWrapperBase.h"
enum PluginReturn {
	NO_ACTION = 0,
	NOTIFY_ON_CHANGE,	// Notify when the parameter has changed
	DO_NOT_PERFORM,		// Instruct driver to not perform the action
	PERFORMED_ACTION,	// This plugin performed the action, do not perform again
	MODIFIED_PARAMS,	// This plugin modified the parameters
	FAILED			// This plugin has failed
};

typedef enum {
	ID_cuGetErrorString = 0,
	ID_cuGetErrorName,
	ID_cuInit,
	ID_cuDriverGetVersion,
	ID_cuDeviceGet,
	ID_cuDeviceGetCount,
	ID_cuDeviceGetName,
	ID_cuDeviceTotalMem,
	ID_cuDeviceGetAttribute,
	ID_cuDeviceGetProperties,
	ID_cuDeviceComputeCapability,
	ID_cuDevicePrimaryCtxRetain,
	ID_cuDevicePrimaryCtxRelease,
	ID_cuDevicePrimaryCtxSetFlags,
	ID_cuDevicePrimaryCtxGetState,
	ID_cuDevicePrimaryCtxReset,
	ID_cuCtxCreate,
	ID_cuCtxDestroy,
	ID_cuCtxPushCurrent,
	ID_cuCtxPopCurrent,
	ID_cuCtxSetCurrent,
	ID_cuCtxGetCurrent,
	ID_cuCtxGetDevice,
	ID_cuCtxGetFlags,
	ID_cuCtxSynchronize,
	ID_cuCtxSetLimit,
	ID_cuCtxGetLimit,
	ID_cuCtxGetCacheConfig,
	ID_cuCtxSetCacheConfig,
	ID_cuCtxGetSharedMemConfig,
	ID_cuCtxSetSharedMemConfig,
	ID_cuCtxGetApiVersion,
	ID_cuCtxGetStreamPriorityRange,
	ID_cuCtxAttach,
	ID_cuCtxDetach,
	ID_cuModuleLoad,
	ID_cuModuleLoadData,
	ID_cuModuleLoadDataEx,
	ID_cuModuleLoadFatBinary,
	ID_cuModuleUnload,
	ID_cuModuleGetFunction,
	ID_cuModuleGetGlobal,
	ID_cuModuleGetTexRef,
	ID_cuModuleGetSurfRef,
	ID_cuLinkCreate,
	ID_cuLinkAddData,
	ID_cuLinkAddFile,
	ID_cuLinkComplete,
	ID_cuLinkDestroy,
	ID_cuMemGetInfo,
	ID_cuMemAlloc,
	ID_cuMemAllocPitch,
	ID_cuMemFree,
	ID_cuMemGetAddressRange,
	ID_cuMemAllocHost,
	ID_cuMemFreeHost,
	ID_cuMemHostAlloc,
	ID_cuMemHostGetDevicePointer,
	ID_cuMemHostGetFlags,
	ID_cuMemAllocManaged,
	ID_cuDeviceGetByPCIBusId,
	ID_cuDeviceGetPCIBusId,
	ID_cuIpcGetEventHandle,
	ID_cuIpcOpenEventHandle,
	ID_cuIpcGetMemHandle,
	ID_cuIpcOpenMemHandle,
	ID_cuIpcCloseMemHandle,
	ID_cuMemHostRegister,
	ID_cuMemHostUnregister,
	ID_cuMemcpy,
	ID_cuMemcpyPeer,
	ID_cuMemcpyHtoD,
	ID_cuMemcpyDtoH,
	ID_cuMemcpyDtoD,
	ID_cuMemcpyDtoA,
	ID_cuMemcpyAtoD,
	ID_cuMemcpyHtoA,
	ID_cuMemcpyAtoH,
	ID_cuMemcpyAtoA,
	ID_cuMemcpy2D,
	ID_cuMemcpy2DUnaligned,
	ID_cuMemcpy3D,
	ID_cuMemcpy3DPeer,
	ID_cuMemcpyAsync,
	ID_cuMemcpyPeerAsync,
	ID_cuMemcpyHtoDAsync,
	ID_cuMemcpyDtoHAsync,
	ID_cuMemcpyDtoDAsync,
	ID_cuMemcpyHtoAAsync,
	ID_cuMemcpyAtoHAsync,
	ID_cuMemcpy2DAsync,
	ID_cuMemcpy3DAsync,
	ID_cuMemcpy3DPeerAsync,
	ID_cuMemsetD8,
	ID_cuMemsetD16,
	ID_cuMemsetD32,
	ID_cuMemsetD2D8,
	ID_cuMemsetD2D16,
	ID_cuMemsetD2D32,
	ID_cuMemsetD8Async,
	ID_cuMemsetD16Async,
	ID_cuMemsetD32Async,
	ID_cuMemsetD2D8Async,
	ID_cuMemsetD2D16Async,
	ID_cuMemsetD2D32Async,
	ID_cuArrayCreate,
	ID_cuArrayGetDescriptor,
	ID_cuArrayDestroy,
	ID_cuArray3DCreate,
	ID_cuArray3DGetDescriptor,
	ID_cuMipmappedArrayCreate,
	ID_cuMipmappedArrayGetLevel,
	ID_cuMipmappedArrayDestroy,
	ID_cuPointerGetAttribute,
	ID_cuMemPrefetchAsync,
	ID_cuMemAdvise,
	ID_cuMemRangeGetAttribute,
	ID_cuMemRangeGetAttributes,
	ID_cuPointerSetAttribute,
	ID_cuPointerGetAttributes,
	ID_cuStreamCreate,
	ID_cuStreamCreateWithPriority,
	ID_cuStreamGetPriority,
	ID_cuStreamGetFlags,
	ID_cuStreamWaitEvent,
	ID_cuStreamAddCallback,
	ID_cuStreamAttachMemAsync,
	ID_cuStreamQuery,
	ID_cuStreamSynchronize,
	ID_cuStreamDestroy,
	ID_cuEventCreate,
	ID_cuEventRecord,
	ID_cuEventQuery,
	ID_cuEventSynchronize,
	ID_cuEventDestroy,
	ID_cuEventElapsedTime,
	ID_cuStreamWaitValue32,
	ID_cuStreamWriteValue32,
	ID_cuStreamBatchMemOp,
	ID_cuFuncGetAttribute,
	ID_cuFuncSetCacheConfig,
	ID_cuFuncSetSharedMemConfig,
	ID_cuLaunchKernel,
	ID_cuFuncSetBlockShape,
	ID_cuFuncSetSharedSize,
	ID_cuParamSetSize,
	ID_cuParamSeti,
	ID_cuParamSetf,
	ID_cuParamSetv,
	ID_cuLaunch,
	ID_cuLaunchGrid,
	ID_cuLaunchGridAsync,
	ID_cuParamSetTexRef,
	ID_cuOccupancyMaxActiveBlocksPerMultiprocessor,
	ID_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags,
	ID_cuOccupancyMaxPotentialBlockSize,
	ID_cuOccupancyMaxPotentialBlockSizeWithFlags,
	ID_cuTexRefSetArray,
	ID_cuTexRefSetMipmappedArray,
	ID_cuTexRefSetAddress,
	ID_cuTexRefSetAddress2D,
	ID_cuTexRefSetFormat,
	ID_cuTexRefSetAddressMode,
	ID_cuTexRefSetFilterMode,
	ID_cuTexRefSetMipmapFilterMode,
	ID_cuTexRefSetMipmapLevelBias,
	ID_cuTexRefSetMipmapLevelClamp,
	ID_cuTexRefSetMaxAnisotropy,
	ID_cuTexRefSetBorderColor,
	ID_cuTexRefSetFlags,
	ID_cuTexRefGetAddress,
	ID_cuTexRefGetArray,
	ID_cuTexRefGetMipmappedArray,
	ID_cuTexRefGetAddressMode,
	ID_cuTexRefGetFilterMode,
	ID_cuTexRefGetFormat,
	ID_cuTexRefGetMipmapFilterMode,
	ID_cuTexRefGetMipmapLevelBias,
	ID_cuTexRefGetMipmapLevelClamp,
	ID_cuTexRefGetMaxAnisotropy,
	ID_cuTexRefGetBorderColor,
	ID_cuTexRefGetFlags,
	ID_cuTexRefCreate,
	ID_cuTexRefDestroy,
	ID_cuSurfRefSetArray,
	ID_cuSurfRefGetArray,
	ID_cuTexObjectCreate,
	ID_cuTexObjectDestroy,
	ID_cuTexObjectGetResourceDesc,
	ID_cuTexObjectGetTextureDesc,
	ID_cuTexObjectGetResourceViewDesc,
	ID_cuSurfObjectCreate,
	ID_cuSurfObjectDestroy,
	ID_cuSurfObjectGetResourceDesc,
	ID_cuDeviceCanAccessPeer,
	ID_cuDeviceGetP2PAttribute,
	ID_cuCtxEnablePeerAccess,
	ID_cuCtxDisablePeerAccess,
	ID_cuGraphicsUnregisterResource,
	ID_cuGraphicsSubResourceGetMappedArray,
	ID_cuGraphicsResourceGetMappedMipmappedArray,
	ID_cuGraphicsResourceGetMappedPointer,
	ID_cuGraphicsResourceSetMapFlags,
	ID_cuGraphicsMapResources,
	ID_cuGraphicsUnmapResources,
	ID_cuGetExportTable,
	ID_cuTexRefSetAddress2D_v2,
	ID_cuMemcpyHtoD_v2,
	ID_cuMemcpyDtoH_v2,
	ID_cuMemcpyDtoD_v2,
	ID_cuMemcpyDtoA_v2,
	ID_cuMemcpyAtoD_v2,
	ID_cuMemcpyHtoA_v2,
	ID_cuMemcpyAtoH_v2,
	ID_cuMemcpyAtoA_v2,
	ID_cuMemcpyHtoAAsync_v2,
	ID_cuMemcpyAtoHAsync_v2,
	ID_cuMemcpy2D_v2,
	ID_cuMemcpy2DUnaligned_v2,
	ID_cuMemcpy3D_v2,
	ID_cuMemcpyHtoDAsync_v2,
	ID_cuMemcpyDtoHAsync_v2,
	ID_cuMemcpyDtoDAsync_v2,
	ID_cuMemcpy2DAsync_v2,
	ID_cuMemcpy3DAsync_v2,
	ID_cuMemsetD8_v2,
	ID_cuMemsetD16_v2,
	ID_cuMemsetD32_v2,
	ID_cuMemsetD2D8_v2,
	ID_cuMemsetD2D16_v2,
	ID_cuMemsetD2D32_v2,
	ID_InternalSynchronization
} CallID;

#define PT_cuGetErrorString CUresult* ,const char * ** 
#define PT_cuGetErrorName CUresult* ,const char * ** 
#define PT_cuInit unsigned int* 
#define PT_cuDriverGetVersion int ** 
#define PT_cuDeviceGet CUdevice ** ,int* 
#define PT_cuDeviceGetCount int ** 
#define PT_cuDeviceGetName char ** ,int* ,CUdevice* 
#define PT_cuDeviceTotalMem size_t ** ,CUdevice* 
#define PT_cuDeviceGetAttribute int ** ,CUdevice_attribute* ,CUdevice* 
#define PT_cuDeviceGetProperties CUdevprop ** ,CUdevice* 
#define PT_cuDeviceComputeCapability int ** ,int ** ,CUdevice* 
#define PT_cuDevicePrimaryCtxRetain CUcontext ** ,CUdevice* 
#define PT_cuDevicePrimaryCtxRelease CUdevice* 
#define PT_cuDevicePrimaryCtxSetFlags CUdevice* ,unsigned int* 
#define PT_cuDevicePrimaryCtxGetState CUdevice* ,unsigned int ** ,int ** 
#define PT_cuDevicePrimaryCtxReset CUdevice* 
#define PT_cuCtxCreate CUcontext ** ,unsigned int* ,CUdevice* 
#define PT_cuCtxDestroy CUcontext* 
#define PT_cuCtxPushCurrent CUcontext* 
#define PT_cuCtxPopCurrent CUcontext ** 
#define PT_cuCtxSetCurrent CUcontext* 
#define PT_cuCtxGetCurrent CUcontext ** 
#define PT_cuCtxGetDevice CUdevice ** 
#define PT_cuCtxGetFlags unsigned int ** 
#define PT_cuCtxSetLimit CUlimit* ,size_t* 
#define PT_cuCtxGetLimit size_t ** ,CUlimit* 
#define PT_cuCtxGetCacheConfig CUfunc_cache ** 
#define PT_cuCtxSetCacheConfig CUfunc_cache* 
#define PT_cuCtxGetSharedMemConfig CUsharedconfig ** 
#define PT_cuCtxSetSharedMemConfig CUsharedconfig* 
#define PT_cuCtxGetApiVersion CUcontext* ,unsigned int ** 
#define PT_cuCtxGetStreamPriorityRange int ** ,int ** 
#define PT_cuCtxAttach CUcontext ** ,unsigned int* 
#define PT_cuCtxDetach CUcontext* 
#define PT_cuModuleLoad CUmodule ** ,const char ** 
#define PT_cuModuleLoadData CUmodule ** ,const void ** 
#define PT_cuModuleLoadDataEx CUmodule ** ,const void ** ,unsigned int* ,CUjit_option ** ,void * ** 
#define PT_cuModuleLoadFatBinary CUmodule ** ,const void ** 
#define PT_cuModuleUnload CUmodule* 
#define PT_cuModuleGetFunction CUfunction ** ,CUmodule* ,const char ** 
#define PT_cuModuleGetGlobal CUdeviceptr ** ,size_t ** ,CUmodule* ,const char ** 
#define PT_cuModuleGetTexRef CUtexref ** ,CUmodule* ,const char ** 
#define PT_cuModuleGetSurfRef CUsurfref ** ,CUmodule* ,const char ** 
#define PT_cuLinkCreate unsigned int* ,CUjit_option ** ,void * ** ,CUlinkState ** 
#define PT_cuLinkAddData CUlinkState* ,CUjitInputType* ,void ** ,size_t* ,const char ** ,unsigned int* ,CUjit_option ** ,void * ** 
#define PT_cuLinkAddFile CUlinkState* ,CUjitInputType* ,const char ** ,unsigned int* ,CUjit_option ** ,void * ** 
#define PT_cuLinkComplete CUlinkState* ,void * ** ,size_t ** 
#define PT_cuLinkDestroy CUlinkState* 
#define PT_cuMemGetInfo size_t ** ,size_t ** 
#define PT_cuMemAlloc CUdeviceptr ** ,size_t* 
#define PT_cuMemAllocPitch CUdeviceptr ** ,size_t ** ,size_t* ,size_t* ,unsigned int* 
#define PT_cuMemFree CUdeviceptr* 
#define PT_cuMemGetAddressRange CUdeviceptr ** ,size_t ** ,CUdeviceptr* 
#define PT_cuMemAllocHost void * ** ,size_t* 
#define PT_cuMemFreeHost void ** 
#define PT_cuMemHostAlloc void * ** ,size_t* ,unsigned int* 
#define PT_cuMemHostGetDevicePointer CUdeviceptr ** ,void ** ,unsigned int* 
#define PT_cuMemHostGetFlags unsigned int ** ,void ** 
#define PT_cuMemAllocManaged CUdeviceptr ** ,size_t* ,unsigned int* 
#define PT_cuDeviceGetByPCIBusId CUdevice ** ,const char ** 
#define PT_cuDeviceGetPCIBusId char ** ,int* ,CUdevice* 
#define PT_cuIpcGetEventHandle CUipcEventHandle ** ,CUevent* 
#define PT_cuIpcOpenEventHandle CUevent ** ,CUipcEventHandle* 
#define PT_cuIpcGetMemHandle CUipcMemHandle ** ,CUdeviceptr* 
#define PT_cuIpcOpenMemHandle CUdeviceptr ** ,CUipcMemHandle* ,unsigned int* 
#define PT_cuIpcCloseMemHandle CUdeviceptr* 
#define PT_cuMemHostRegister void ** ,size_t* ,unsigned int* 
#define PT_cuMemHostUnregister void ** 
#define PT_cuMemcpy CUdeviceptr* ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyPeer CUdeviceptr* ,CUcontext* ,CUdeviceptr* ,CUcontext* ,size_t* 
#define PT_cuMemcpyHtoD CUdeviceptr* ,const void ** ,size_t* 
#define PT_cuMemcpyDtoH void ** ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyDtoD CUdeviceptr* ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyDtoA CUarray* ,size_t* ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyAtoD CUdeviceptr* ,CUarray* ,size_t* ,size_t* 
#define PT_cuMemcpyHtoA CUarray* ,size_t* ,const void ** ,size_t* 
#define PT_cuMemcpyAtoH void ** ,CUarray* ,size_t* ,size_t* 
#define PT_cuMemcpyAtoA CUarray* ,size_t* ,CUarray* ,size_t* ,size_t* 
#define PT_cuMemcpy2D const CUDA_MEMCPY2D ** 
#define PT_cuMemcpy2DUnaligned const CUDA_MEMCPY2D ** 
#define PT_cuMemcpy3D const CUDA_MEMCPY3D ** 
#define PT_cuMemcpy3DPeer const CUDA_MEMCPY3D_PEER ** 
#define PT_cuMemcpyAsync CUdeviceptr* ,CUdeviceptr* ,size_t* ,CUstream* 
#define PT_cuMemcpyPeerAsync CUdeviceptr* ,CUcontext* ,CUdeviceptr* ,CUcontext* ,size_t* ,CUstream* 
#define PT_cuMemcpyHtoDAsync CUdeviceptr* ,const void ** ,size_t* ,CUstream* 
#define PT_cuMemcpyDtoHAsync void ** ,CUdeviceptr* ,size_t* ,CUstream* 
#define PT_cuMemcpyDtoDAsync CUdeviceptr* ,CUdeviceptr* ,size_t* ,CUstream* 
#define PT_cuMemcpyHtoAAsync CUarray* ,size_t* ,const void ** ,size_t* ,CUstream* 
#define PT_cuMemcpyAtoHAsync void ** ,CUarray* ,size_t* ,size_t* ,CUstream* 
#define PT_cuMemcpy2DAsync const CUDA_MEMCPY2D ** ,CUstream* 
#define PT_cuMemcpy3DAsync const CUDA_MEMCPY3D ** ,CUstream* 
#define PT_cuMemcpy3DPeerAsync const CUDA_MEMCPY3D_PEER ** ,CUstream* 
#define PT_cuMemsetD8 CUdeviceptr* ,unsigned char* ,size_t* 
#define PT_cuMemsetD16 CUdeviceptr* ,unsigned short* ,size_t* 
#define PT_cuMemsetD32 CUdeviceptr* ,unsigned int* ,size_t* 
#define PT_cuMemsetD2D8 CUdeviceptr* ,size_t* ,unsigned char* ,size_t* ,size_t* 
#define PT_cuMemsetD2D16 CUdeviceptr* ,size_t* ,unsigned short* ,size_t* ,size_t* 
#define PT_cuMemsetD2D32 CUdeviceptr* ,size_t* ,unsigned int* ,size_t* ,size_t* 
#define PT_cuMemsetD8Async CUdeviceptr* ,unsigned char* ,size_t* ,CUstream* 
#define PT_cuMemsetD16Async CUdeviceptr* ,unsigned short* ,size_t* ,CUstream* 
#define PT_cuMemsetD32Async CUdeviceptr* ,unsigned int* ,size_t* ,CUstream* 
#define PT_cuMemsetD2D8Async CUdeviceptr* ,size_t* ,unsigned char* ,size_t* ,size_t* ,CUstream* 
#define PT_cuMemsetD2D16Async CUdeviceptr* ,size_t* ,unsigned short* ,size_t* ,size_t* ,CUstream* 
#define PT_cuMemsetD2D32Async CUdeviceptr* ,size_t* ,unsigned int* ,size_t* ,size_t* ,CUstream* 
#define PT_cuArrayCreate CUarray ** ,const CUDA_ARRAY_DESCRIPTOR ** 
#define PT_cuArrayGetDescriptor CUDA_ARRAY_DESCRIPTOR ** ,CUarray* 
#define PT_cuArrayDestroy CUarray* 
#define PT_cuArray3DCreate CUarray ** ,const CUDA_ARRAY3D_DESCRIPTOR ** 
#define PT_cuArray3DGetDescriptor CUDA_ARRAY3D_DESCRIPTOR ** ,CUarray* 
#define PT_cuMipmappedArrayCreate CUmipmappedArray ** ,const CUDA_ARRAY3D_DESCRIPTOR ** ,unsigned int* 
#define PT_cuMipmappedArrayGetLevel CUarray ** ,CUmipmappedArray* ,unsigned int* 
#define PT_cuMipmappedArrayDestroy CUmipmappedArray* 
#define PT_cuPointerGetAttribute void ** ,CUpointer_attribute* ,CUdeviceptr* 
#define PT_cuMemPrefetchAsync CUdeviceptr* ,size_t* ,CUdevice* ,CUstream* 
#define PT_cuMemAdvise CUdeviceptr* ,size_t* ,CUmem_advise* ,CUdevice* 
#define PT_cuMemRangeGetAttribute void ** ,size_t* ,CUmem_range_attribute* ,CUdeviceptr* ,size_t* 
#define PT_cuMemRangeGetAttributes void * ** ,size_t ** ,CUmem_range_attribute ** ,size_t* ,CUdeviceptr* ,size_t* 
#define PT_cuPointerSetAttribute const void ** ,CUpointer_attribute* ,CUdeviceptr* 
#define PT_cuPointerGetAttributes unsigned int* ,CUpointer_attribute ** ,void * ** ,CUdeviceptr* 
#define PT_cuStreamCreate CUstream ** ,unsigned int* 
#define PT_cuStreamCreateWithPriority CUstream ** ,unsigned int* ,int* 
#define PT_cuStreamGetPriority CUstream* ,int ** 
#define PT_cuStreamGetFlags CUstream* ,unsigned int ** 
#define PT_cuStreamWaitEvent CUstream* ,CUevent* ,unsigned int* 
#define PT_cuStreamAddCallback CUstream* ,CUstreamCallback* ,void ** ,unsigned int* 
#define PT_cuStreamAttachMemAsync CUstream* ,CUdeviceptr* ,size_t* ,unsigned int* 
#define PT_cuStreamQuery CUstream* 
#define PT_cuStreamSynchronize CUstream* 
#define PT_cuStreamDestroy CUstream* 
#define PT_cuEventCreate CUevent ** ,unsigned int* 
#define PT_cuEventRecord CUevent* ,CUstream* 
#define PT_cuEventQuery CUevent* 
#define PT_cuEventSynchronize CUevent* 
#define PT_cuEventDestroy CUevent* 
#define PT_cuEventElapsedTime float ** ,CUevent* ,CUevent* 
#define PT_cuStreamWaitValue32 CUstream* ,CUdeviceptr* ,cuuint32_t* ,unsigned int* 
#define PT_cuStreamWriteValue32 CUstream* ,CUdeviceptr* ,cuuint32_t* ,unsigned int* 
#define PT_cuStreamBatchMemOp CUstream* ,unsigned int* ,CUstreamBatchMemOpParams ** ,unsigned int* 
#define PT_cuFuncGetAttribute int ** ,CUfunction_attribute* ,CUfunction* 
#define PT_cuFuncSetCacheConfig CUfunction* ,CUfunc_cache* 
#define PT_cuFuncSetSharedMemConfig CUfunction* ,CUsharedconfig* 
#define PT_cuLaunchKernel CUfunction* ,unsigned int* ,unsigned int* ,unsigned int* ,unsigned int* ,unsigned int* ,unsigned int* ,unsigned int* ,CUstream* ,void * ** ,void * ** 
#define PT_cuFuncSetBlockShape CUfunction* ,int* ,int* ,int* 
#define PT_cuFuncSetSharedSize CUfunction* ,unsigned int* 
#define PT_cuParamSetSize CUfunction* ,unsigned int* 
#define PT_cuParamSeti CUfunction* ,int* ,unsigned int* 
#define PT_cuParamSetf CUfunction* ,int* ,float* 
#define PT_cuParamSetv CUfunction* ,int* ,void ** ,unsigned int* 
#define PT_cuLaunch CUfunction* 
#define PT_cuLaunchGrid CUfunction* ,int* ,int* 
#define PT_cuLaunchGridAsync CUfunction* ,int* ,int* ,CUstream* 
#define PT_cuParamSetTexRef CUfunction* ,int* ,CUtexref* 
#define PT_cuOccupancyMaxActiveBlocksPerMultiprocessor int ** ,CUfunction* ,int* ,size_t* 
#define PT_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags int ** ,CUfunction* ,int* ,size_t* ,unsigned int* 
#define PT_cuOccupancyMaxPotentialBlockSize int ** ,int ** ,CUfunction* ,CUoccupancyB2DSize* ,size_t* ,int* 
#define PT_cuOccupancyMaxPotentialBlockSizeWithFlags int ** ,int ** ,CUfunction* ,CUoccupancyB2DSize* ,size_t* ,int* ,unsigned int* 
#define PT_cuTexRefSetArray CUtexref* ,CUarray* ,unsigned int* 
#define PT_cuTexRefSetMipmappedArray CUtexref* ,CUmipmappedArray* ,unsigned int* 
#define PT_cuTexRefSetAddress size_t ** ,CUtexref* ,CUdeviceptr* ,size_t* 
#define PT_cuTexRefSetAddress2D CUtexref* ,const CUDA_ARRAY_DESCRIPTOR ** ,CUdeviceptr* ,size_t* 
#define PT_cuTexRefSetFormat CUtexref* ,CUarray_format* ,int* 
#define PT_cuTexRefSetAddressMode CUtexref* ,int* ,CUaddress_mode* 
#define PT_cuTexRefSetFilterMode CUtexref* ,CUfilter_mode* 
#define PT_cuTexRefSetMipmapFilterMode CUtexref* ,CUfilter_mode* 
#define PT_cuTexRefSetMipmapLevelBias CUtexref* ,float* 
#define PT_cuTexRefSetMipmapLevelClamp CUtexref* ,float* ,float* 
#define PT_cuTexRefSetMaxAnisotropy CUtexref* ,unsigned int* 
#define PT_cuTexRefSetBorderColor CUtexref* ,float ** 
#define PT_cuTexRefSetFlags CUtexref* ,unsigned int* 
#define PT_cuTexRefGetAddress CUdeviceptr ** ,CUtexref* 
#define PT_cuTexRefGetArray CUarray ** ,CUtexref* 
#define PT_cuTexRefGetMipmappedArray CUmipmappedArray ** ,CUtexref* 
#define PT_cuTexRefGetAddressMode CUaddress_mode ** ,CUtexref* ,int* 
#define PT_cuTexRefGetFilterMode CUfilter_mode ** ,CUtexref* 
#define PT_cuTexRefGetFormat CUarray_format ** ,int ** ,CUtexref* 
#define PT_cuTexRefGetMipmapFilterMode CUfilter_mode ** ,CUtexref* 
#define PT_cuTexRefGetMipmapLevelBias float ** ,CUtexref* 
#define PT_cuTexRefGetMipmapLevelClamp float ** ,float ** ,CUtexref* 
#define PT_cuTexRefGetMaxAnisotropy int ** ,CUtexref* 
#define PT_cuTexRefGetBorderColor float ** ,CUtexref* 
#define PT_cuTexRefGetFlags unsigned int ** ,CUtexref* 
#define PT_cuTexRefCreate CUtexref ** 
#define PT_cuTexRefDestroy CUtexref* 
#define PT_cuSurfRefSetArray CUsurfref* ,CUarray* ,unsigned int* 
#define PT_cuSurfRefGetArray CUarray ** ,CUsurfref* 
#define PT_cuTexObjectCreate CUtexObject ** ,const CUDA_RESOURCE_DESC ** ,const CUDA_TEXTURE_DESC ** ,const CUDA_RESOURCE_VIEW_DESC ** 
#define PT_cuTexObjectDestroy CUtexObject* 
#define PT_cuTexObjectGetResourceDesc CUDA_RESOURCE_DESC ** ,CUtexObject* 
#define PT_cuTexObjectGetTextureDesc CUDA_TEXTURE_DESC ** ,CUtexObject* 
#define PT_cuTexObjectGetResourceViewDesc CUDA_RESOURCE_VIEW_DESC ** ,CUtexObject* 
#define PT_cuSurfObjectCreate CUsurfObject ** ,const CUDA_RESOURCE_DESC ** 
#define PT_cuSurfObjectDestroy CUsurfObject* 
#define PT_cuSurfObjectGetResourceDesc CUDA_RESOURCE_DESC ** ,CUsurfObject* 
#define PT_cuDeviceCanAccessPeer int ** ,CUdevice* ,CUdevice* 
#define PT_cuDeviceGetP2PAttribute int ** ,CUdevice_P2PAttribute* ,CUdevice* ,CUdevice* 
#define PT_cuCtxEnablePeerAccess CUcontext* ,unsigned int* 
#define PT_cuCtxDisablePeerAccess CUcontext* 
#define PT_cuGraphicsUnregisterResource CUgraphicsResource* 
#define PT_cuGraphicsSubResourceGetMappedArray CUarray ** ,CUgraphicsResource* ,unsigned int* ,unsigned int* 
#define PT_cuGraphicsResourceGetMappedMipmappedArray CUmipmappedArray ** ,CUgraphicsResource* 
#define PT_cuGraphicsResourceGetMappedPointer CUdeviceptr ** ,size_t ** ,CUgraphicsResource* 
#define PT_cuGraphicsResourceSetMapFlags CUgraphicsResource* ,unsigned int* 
#define PT_cuGraphicsMapResources unsigned int* ,CUgraphicsResource ** ,CUstream* 
#define PT_cuGraphicsUnmapResources unsigned int* ,CUgraphicsResource ** ,CUstream* 
#define PT_cuGetExportTable const void * ** ,const CUuuid ** 
#define PT_cuTexRefSetAddress2D_v2 CUtexref* ,const CUDA_ARRAY_DESCRIPTOR ** ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyHtoD_v2 CUdeviceptr* ,const void ** ,size_t* 
#define PT_cuMemcpyDtoH_v2 void ** ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyDtoD_v2 CUdeviceptr* ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyDtoA_v2 CUarray* ,size_t* ,CUdeviceptr* ,size_t* 
#define PT_cuMemcpyAtoD_v2 CUdeviceptr* ,CUarray* ,size_t* ,size_t* 
#define PT_cuMemcpyHtoA_v2 CUarray* ,size_t* ,const void ** ,size_t* 
#define PT_cuMemcpyAtoH_v2 void ** ,CUarray* ,size_t* ,size_t* 
#define PT_cuMemcpyAtoA_v2 CUarray* ,size_t* ,CUarray* ,size_t* ,size_t* 
#define PT_cuMemcpyHtoAAsync_v2 CUarray* ,size_t* ,const void ** ,size_t* ,CUstream* 
#define PT_cuMemcpyAtoHAsync_v2 void ** ,CUarray* ,size_t* ,size_t* ,CUstream* 
#define PT_cuMemcpy2D_v2 const CUDA_MEMCPY2D ** 
#define PT_cuMemcpy2DUnaligned_v2 const CUDA_MEMCPY2D ** 
#define PT_cuMemcpy3D_v2 const CUDA_MEMCPY3D ** 
#define PT_cuMemcpyHtoDAsync_v2 CUdeviceptr* ,const void ** ,size_t* ,CUstream* 
#define PT_cuMemcpyDtoHAsync_v2 void ** ,CUdeviceptr* ,size_t* ,CUstream* 
#define PT_cuMemcpyDtoDAsync_v2 CUdeviceptr* ,CUdeviceptr* ,size_t* ,CUstream* 
#define PT_cuMemcpy2DAsync_v2 const CUDA_MEMCPY2D ** ,CUstream* 
#define PT_cuMemcpy3DAsync_v2 const CUDA_MEMCPY3D ** ,CUstream* 
#define PT_cuMemsetD8_v2 CUdeviceptr* ,unsigned char* ,size_t* 
#define PT_cuMemsetD16_v2 CUdeviceptr* ,unsigned short* ,size_t* 
#define PT_cuMemsetD32_v2 CUdeviceptr* ,unsigned int* ,size_t* 
#define PT_cuMemsetD2D8_v2 CUdeviceptr* ,size_t* ,unsigned char* ,size_t* ,size_t* 
#define PT_cuMemsetD2D16_v2 CUdeviceptr* ,size_t* ,unsigned short* ,size_t* ,size_t* 
#define PT_cuMemsetD2D32_v2 CUdeviceptr* ,size_t* ,unsigned int* ,size_t* ,size_t* 



#ifndef EXTERN_FLAG
#ifndef DEFINED_TEMPLATES
#define EXTERN_FLAG extern
#else
#define EXTERN_FLAG 
#endif
#endif
EXTERN_FLAG std::function<int(CUresult,const char * *)> Bound_cuGetErrorString;
EXTERN_FLAG std::function<int(CUresult,const char * *)> Bound_cuGetErrorName;
EXTERN_FLAG std::function<int(unsigned int)> Bound_cuInit;
EXTERN_FLAG std::function<int(int *)> Bound_cuDriverGetVersion;
EXTERN_FLAG std::function<int(CUdevice *,int)> Bound_cuDeviceGet;
EXTERN_FLAG std::function<int(int *)> Bound_cuDeviceGetCount;
EXTERN_FLAG std::function<int(char *,int,CUdevice)> Bound_cuDeviceGetName;
EXTERN_FLAG std::function<int(size_t *,CUdevice)> Bound_cuDeviceTotalMem;
EXTERN_FLAG std::function<int(int *,CUdevice_attribute,CUdevice)> Bound_cuDeviceGetAttribute;
EXTERN_FLAG std::function<int(CUdevprop *,CUdevice)> Bound_cuDeviceGetProperties;
EXTERN_FLAG std::function<int(int *,int *,CUdevice)> Bound_cuDeviceComputeCapability;
EXTERN_FLAG std::function<int(CUcontext *,CUdevice)> Bound_cuDevicePrimaryCtxRetain;
EXTERN_FLAG std::function<int(CUdevice)> Bound_cuDevicePrimaryCtxRelease;
EXTERN_FLAG std::function<int(CUdevice,unsigned int)> Bound_cuDevicePrimaryCtxSetFlags;
EXTERN_FLAG std::function<int(CUdevice,unsigned int *,int *)> Bound_cuDevicePrimaryCtxGetState;
EXTERN_FLAG std::function<int(CUdevice)> Bound_cuDevicePrimaryCtxReset;
EXTERN_FLAG std::function<int(CUcontext *,unsigned int,CUdevice)> Bound_cuCtxCreate;
EXTERN_FLAG std::function<int(CUcontext)> Bound_cuCtxDestroy;
EXTERN_FLAG std::function<int(CUcontext)> Bound_cuCtxPushCurrent;
EXTERN_FLAG std::function<int(CUcontext *)> Bound_cuCtxPopCurrent;
EXTERN_FLAG std::function<int(CUcontext)> Bound_cuCtxSetCurrent;
EXTERN_FLAG std::function<int(CUcontext *)> Bound_cuCtxGetCurrent;
EXTERN_FLAG std::function<int(CUdevice *)> Bound_cuCtxGetDevice;
EXTERN_FLAG std::function<int(unsigned int *)> Bound_cuCtxGetFlags;
EXTERN_FLAG std::function<int(void)> Bound_cuCtxSynchronize;
EXTERN_FLAG std::function<int(CUlimit,size_t)> Bound_cuCtxSetLimit;
EXTERN_FLAG std::function<int(size_t *,CUlimit)> Bound_cuCtxGetLimit;
EXTERN_FLAG std::function<int(CUfunc_cache *)> Bound_cuCtxGetCacheConfig;
EXTERN_FLAG std::function<int(CUfunc_cache)> Bound_cuCtxSetCacheConfig;
EXTERN_FLAG std::function<int(CUsharedconfig *)> Bound_cuCtxGetSharedMemConfig;
EXTERN_FLAG std::function<int(CUsharedconfig)> Bound_cuCtxSetSharedMemConfig;
EXTERN_FLAG std::function<int(CUcontext,unsigned int *)> Bound_cuCtxGetApiVersion;
EXTERN_FLAG std::function<int(int *,int *)> Bound_cuCtxGetStreamPriorityRange;
EXTERN_FLAG std::function<int(CUcontext *,unsigned int)> Bound_cuCtxAttach;
EXTERN_FLAG std::function<int(CUcontext)> Bound_cuCtxDetach;
EXTERN_FLAG std::function<int(CUmodule *,const char *)> Bound_cuModuleLoad;
EXTERN_FLAG std::function<int(CUmodule *,const void *)> Bound_cuModuleLoadData;
EXTERN_FLAG std::function<int(CUmodule *,const void *,unsigned int,CUjit_option *,void * *)> Bound_cuModuleLoadDataEx;
EXTERN_FLAG std::function<int(CUmodule *,const void *)> Bound_cuModuleLoadFatBinary;
EXTERN_FLAG std::function<int(CUmodule)> Bound_cuModuleUnload;
EXTERN_FLAG std::function<int(CUfunction *,CUmodule,const char *)> Bound_cuModuleGetFunction;
EXTERN_FLAG std::function<int(CUdeviceptr *,size_t *,CUmodule,const char *)> Bound_cuModuleGetGlobal;
EXTERN_FLAG std::function<int(CUtexref *,CUmodule,const char *)> Bound_cuModuleGetTexRef;
EXTERN_FLAG std::function<int(CUsurfref *,CUmodule,const char *)> Bound_cuModuleGetSurfRef;
EXTERN_FLAG std::function<int(unsigned int,CUjit_option *,void * *,CUlinkState *)> Bound_cuLinkCreate;
EXTERN_FLAG std::function<int(CUlinkState,CUjitInputType,void *,size_t,const char *,unsigned int,CUjit_option *,void * *)> Bound_cuLinkAddData;
EXTERN_FLAG std::function<int(CUlinkState,CUjitInputType,const char *,unsigned int,CUjit_option *,void * *)> Bound_cuLinkAddFile;
EXTERN_FLAG std::function<int(CUlinkState,void * *,size_t *)> Bound_cuLinkComplete;
EXTERN_FLAG std::function<int(CUlinkState)> Bound_cuLinkDestroy;
EXTERN_FLAG std::function<int(size_t *,size_t *)> Bound_cuMemGetInfo;
EXTERN_FLAG std::function<int(CUdeviceptr *,size_t)> Bound_cuMemAlloc;
EXTERN_FLAG std::function<int(CUdeviceptr *,size_t *,size_t,size_t,unsigned int)> Bound_cuMemAllocPitch;
EXTERN_FLAG std::function<int(CUdeviceptr)> Bound_cuMemFree;
EXTERN_FLAG std::function<int(CUdeviceptr *,size_t *,CUdeviceptr)> Bound_cuMemGetAddressRange;
EXTERN_FLAG std::function<int(void * *,size_t)> Bound_cuMemAllocHost;
EXTERN_FLAG std::function<int(void *)> Bound_cuMemFreeHost;
EXTERN_FLAG std::function<int(void * *,size_t,unsigned int)> Bound_cuMemHostAlloc;
EXTERN_FLAG std::function<int(CUdeviceptr *,void *,unsigned int)> Bound_cuMemHostGetDevicePointer;
EXTERN_FLAG std::function<int(unsigned int *,void *)> Bound_cuMemHostGetFlags;
EXTERN_FLAG std::function<int(CUdeviceptr *,size_t,unsigned int)> Bound_cuMemAllocManaged;
EXTERN_FLAG std::function<int(CUdevice *,const char *)> Bound_cuDeviceGetByPCIBusId;
EXTERN_FLAG std::function<int(char *,int,CUdevice)> Bound_cuDeviceGetPCIBusId;
EXTERN_FLAG std::function<int(CUipcEventHandle *,CUevent)> Bound_cuIpcGetEventHandle;
EXTERN_FLAG std::function<int(CUevent *,CUipcEventHandle)> Bound_cuIpcOpenEventHandle;
EXTERN_FLAG std::function<int(CUipcMemHandle *,CUdeviceptr)> Bound_cuIpcGetMemHandle;
EXTERN_FLAG std::function<int(CUdeviceptr *,CUipcMemHandle,unsigned int)> Bound_cuIpcOpenMemHandle;
EXTERN_FLAG std::function<int(CUdeviceptr)> Bound_cuIpcCloseMemHandle;
EXTERN_FLAG std::function<int(void *,size_t,unsigned int)> Bound_cuMemHostRegister;
EXTERN_FLAG std::function<int(void *)> Bound_cuMemHostUnregister;
EXTERN_FLAG std::function<int(CUdeviceptr,CUdeviceptr,size_t)> Bound_cuMemcpy;
EXTERN_FLAG std::function<int(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t)> Bound_cuMemcpyPeer;
EXTERN_FLAG std::function<int(CUdeviceptr,const void *,size_t)> Bound_cuMemcpyHtoD;
EXTERN_FLAG std::function<int(void *,CUdeviceptr,size_t)> Bound_cuMemcpyDtoH;
EXTERN_FLAG std::function<int(CUdeviceptr,CUdeviceptr,size_t)> Bound_cuMemcpyDtoD;
EXTERN_FLAG std::function<int(CUarray,size_t,CUdeviceptr,size_t)> Bound_cuMemcpyDtoA;
EXTERN_FLAG std::function<int(CUdeviceptr,CUarray,size_t,size_t)> Bound_cuMemcpyAtoD;
EXTERN_FLAG std::function<int(CUarray,size_t,const void *,size_t)> Bound_cuMemcpyHtoA;
EXTERN_FLAG std::function<int(void *,CUarray,size_t,size_t)> Bound_cuMemcpyAtoH;
EXTERN_FLAG std::function<int(CUarray,size_t,CUarray,size_t,size_t)> Bound_cuMemcpyAtoA;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY2D *)> Bound_cuMemcpy2D;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY2D *)> Bound_cuMemcpy2DUnaligned;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY3D *)> Bound_cuMemcpy3D;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY3D_PEER *)> Bound_cuMemcpy3DPeer;
EXTERN_FLAG std::function<int(CUdeviceptr,CUdeviceptr,size_t,CUstream)> Bound_cuMemcpyAsync;
EXTERN_FLAG std::function<int(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t,CUstream)> Bound_cuMemcpyPeerAsync;
EXTERN_FLAG std::function<int(CUdeviceptr,const void *,size_t,CUstream)> Bound_cuMemcpyHtoDAsync;
EXTERN_FLAG std::function<int(void *,CUdeviceptr,size_t,CUstream)> Bound_cuMemcpyDtoHAsync;
EXTERN_FLAG std::function<int(CUdeviceptr,CUdeviceptr,size_t,CUstream)> Bound_cuMemcpyDtoDAsync;
EXTERN_FLAG std::function<int(CUarray,size_t,const void *,size_t,CUstream)> Bound_cuMemcpyHtoAAsync;
EXTERN_FLAG std::function<int(void *,CUarray,size_t,size_t,CUstream)> Bound_cuMemcpyAtoHAsync;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY2D *,CUstream)> Bound_cuMemcpy2DAsync;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY3D *,CUstream)> Bound_cuMemcpy3DAsync;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY3D_PEER *,CUstream)> Bound_cuMemcpy3DPeerAsync;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned char,size_t)> Bound_cuMemsetD8;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned short,size_t)> Bound_cuMemsetD16;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned int,size_t)> Bound_cuMemsetD32;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned char,size_t,size_t)> Bound_cuMemsetD2D8;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned short,size_t,size_t)> Bound_cuMemsetD2D16;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned int,size_t,size_t)> Bound_cuMemsetD2D32;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned char,size_t,CUstream)> Bound_cuMemsetD8Async;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned short,size_t,CUstream)> Bound_cuMemsetD16Async;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned int,size_t,CUstream)> Bound_cuMemsetD32Async;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned char,size_t,size_t,CUstream)> Bound_cuMemsetD2D8Async;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned short,size_t,size_t,CUstream)> Bound_cuMemsetD2D16Async;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned int,size_t,size_t,CUstream)> Bound_cuMemsetD2D32Async;
EXTERN_FLAG std::function<int(CUarray *,const CUDA_ARRAY_DESCRIPTOR *)> Bound_cuArrayCreate;
EXTERN_FLAG std::function<int(CUDA_ARRAY_DESCRIPTOR *,CUarray)> Bound_cuArrayGetDescriptor;
EXTERN_FLAG std::function<int(CUarray)> Bound_cuArrayDestroy;
EXTERN_FLAG std::function<int(CUarray *,const CUDA_ARRAY3D_DESCRIPTOR *)> Bound_cuArray3DCreate;
EXTERN_FLAG std::function<int(CUDA_ARRAY3D_DESCRIPTOR *,CUarray)> Bound_cuArray3DGetDescriptor;
EXTERN_FLAG std::function<int(CUmipmappedArray *,const CUDA_ARRAY3D_DESCRIPTOR *,unsigned int)> Bound_cuMipmappedArrayCreate;
EXTERN_FLAG std::function<int(CUarray *,CUmipmappedArray,unsigned int)> Bound_cuMipmappedArrayGetLevel;
EXTERN_FLAG std::function<int(CUmipmappedArray)> Bound_cuMipmappedArrayDestroy;
EXTERN_FLAG std::function<int(void *,CUpointer_attribute,CUdeviceptr)> Bound_cuPointerGetAttribute;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,CUdevice,CUstream)> Bound_cuMemPrefetchAsync;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,CUmem_advise,CUdevice)> Bound_cuMemAdvise;
EXTERN_FLAG std::function<int(void *,size_t,CUmem_range_attribute,CUdeviceptr,size_t)> Bound_cuMemRangeGetAttribute;
EXTERN_FLAG std::function<int(void * *,size_t *,CUmem_range_attribute *,size_t,CUdeviceptr,size_t)> Bound_cuMemRangeGetAttributes;
EXTERN_FLAG std::function<int(const void *,CUpointer_attribute,CUdeviceptr)> Bound_cuPointerSetAttribute;
EXTERN_FLAG std::function<int(unsigned int,CUpointer_attribute *,void * *,CUdeviceptr)> Bound_cuPointerGetAttributes;
EXTERN_FLAG std::function<int(CUstream *,unsigned int)> Bound_cuStreamCreate;
EXTERN_FLAG std::function<int(CUstream *,unsigned int,int)> Bound_cuStreamCreateWithPriority;
EXTERN_FLAG std::function<int(CUstream,int *)> Bound_cuStreamGetPriority;
EXTERN_FLAG std::function<int(CUstream,unsigned int *)> Bound_cuStreamGetFlags;
EXTERN_FLAG std::function<int(CUstream,CUevent,unsigned int)> Bound_cuStreamWaitEvent;
EXTERN_FLAG std::function<int(CUstream,CUstreamCallback,void *,unsigned int)> Bound_cuStreamAddCallback;
EXTERN_FLAG std::function<int(CUstream,CUdeviceptr,size_t,unsigned int)> Bound_cuStreamAttachMemAsync;
EXTERN_FLAG std::function<int(CUstream)> Bound_cuStreamQuery;
EXTERN_FLAG std::function<int(CUstream)> Bound_cuStreamSynchronize;
EXTERN_FLAG std::function<int(CUstream)> Bound_cuStreamDestroy;
EXTERN_FLAG std::function<int(CUevent *,unsigned int)> Bound_cuEventCreate;
EXTERN_FLAG std::function<int(CUevent,CUstream)> Bound_cuEventRecord;
EXTERN_FLAG std::function<int(CUevent)> Bound_cuEventQuery;
EXTERN_FLAG std::function<int(CUevent)> Bound_cuEventSynchronize;
EXTERN_FLAG std::function<int(CUevent)> Bound_cuEventDestroy;
EXTERN_FLAG std::function<int(float *,CUevent,CUevent)> Bound_cuEventElapsedTime;
EXTERN_FLAG std::function<int(CUstream,CUdeviceptr,cuuint32_t,unsigned int)> Bound_cuStreamWaitValue32;
EXTERN_FLAG std::function<int(CUstream,CUdeviceptr,cuuint32_t,unsigned int)> Bound_cuStreamWriteValue32;
EXTERN_FLAG std::function<int(CUstream,unsigned int,CUstreamBatchMemOpParams *,unsigned int)> Bound_cuStreamBatchMemOp;
EXTERN_FLAG std::function<int(int *,CUfunction_attribute,CUfunction)> Bound_cuFuncGetAttribute;
EXTERN_FLAG std::function<int(CUfunction,CUfunc_cache)> Bound_cuFuncSetCacheConfig;
EXTERN_FLAG std::function<int(CUfunction,CUsharedconfig)> Bound_cuFuncSetSharedMemConfig;
EXTERN_FLAG std::function<int(CUfunction,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,CUstream,void * *,void * *)> Bound_cuLaunchKernel;
EXTERN_FLAG std::function<int(CUfunction,int,int,int)> Bound_cuFuncSetBlockShape;
EXTERN_FLAG std::function<int(CUfunction,unsigned int)> Bound_cuFuncSetSharedSize;
EXTERN_FLAG std::function<int(CUfunction,unsigned int)> Bound_cuParamSetSize;
EXTERN_FLAG std::function<int(CUfunction,int,unsigned int)> Bound_cuParamSeti;
EXTERN_FLAG std::function<int(CUfunction,int,float)> Bound_cuParamSetf;
EXTERN_FLAG std::function<int(CUfunction,int,void *,unsigned int)> Bound_cuParamSetv;
EXTERN_FLAG std::function<int(CUfunction)> Bound_cuLaunch;
EXTERN_FLAG std::function<int(CUfunction,int,int)> Bound_cuLaunchGrid;
EXTERN_FLAG std::function<int(CUfunction,int,int,CUstream)> Bound_cuLaunchGridAsync;
EXTERN_FLAG std::function<int(CUfunction,int,CUtexref)> Bound_cuParamSetTexRef;
EXTERN_FLAG std::function<int(int *,CUfunction,int,size_t)> Bound_cuOccupancyMaxActiveBlocksPerMultiprocessor;
EXTERN_FLAG std::function<int(int *,CUfunction,int,size_t,unsigned int)> Bound_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags;
EXTERN_FLAG std::function<int(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int)> Bound_cuOccupancyMaxPotentialBlockSize;
EXTERN_FLAG std::function<int(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int,unsigned int)> Bound_cuOccupancyMaxPotentialBlockSizeWithFlags;
EXTERN_FLAG std::function<int(CUtexref,CUarray,unsigned int)> Bound_cuTexRefSetArray;
EXTERN_FLAG std::function<int(CUtexref,CUmipmappedArray,unsigned int)> Bound_cuTexRefSetMipmappedArray;
EXTERN_FLAG std::function<int(size_t *,CUtexref,CUdeviceptr,size_t)> Bound_cuTexRefSetAddress;
EXTERN_FLAG std::function<int(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t)> Bound_cuTexRefSetAddress2D;
EXTERN_FLAG std::function<int(CUtexref,CUarray_format,int)> Bound_cuTexRefSetFormat;
EXTERN_FLAG std::function<int(CUtexref,int,CUaddress_mode)> Bound_cuTexRefSetAddressMode;
EXTERN_FLAG std::function<int(CUtexref,CUfilter_mode)> Bound_cuTexRefSetFilterMode;
EXTERN_FLAG std::function<int(CUtexref,CUfilter_mode)> Bound_cuTexRefSetMipmapFilterMode;
EXTERN_FLAG std::function<int(CUtexref,float)> Bound_cuTexRefSetMipmapLevelBias;
EXTERN_FLAG std::function<int(CUtexref,float,float)> Bound_cuTexRefSetMipmapLevelClamp;
EXTERN_FLAG std::function<int(CUtexref,unsigned int)> Bound_cuTexRefSetMaxAnisotropy;
EXTERN_FLAG std::function<int(CUtexref,float *)> Bound_cuTexRefSetBorderColor;
EXTERN_FLAG std::function<int(CUtexref,unsigned int)> Bound_cuTexRefSetFlags;
EXTERN_FLAG std::function<int(CUdeviceptr *,CUtexref)> Bound_cuTexRefGetAddress;
EXTERN_FLAG std::function<int(CUarray *,CUtexref)> Bound_cuTexRefGetArray;
EXTERN_FLAG std::function<int(CUmipmappedArray *,CUtexref)> Bound_cuTexRefGetMipmappedArray;
EXTERN_FLAG std::function<int(CUaddress_mode *,CUtexref,int)> Bound_cuTexRefGetAddressMode;
EXTERN_FLAG std::function<int(CUfilter_mode *,CUtexref)> Bound_cuTexRefGetFilterMode;
EXTERN_FLAG std::function<int(CUarray_format *,int *,CUtexref)> Bound_cuTexRefGetFormat;
EXTERN_FLAG std::function<int(CUfilter_mode *,CUtexref)> Bound_cuTexRefGetMipmapFilterMode;
EXTERN_FLAG std::function<int(float *,CUtexref)> Bound_cuTexRefGetMipmapLevelBias;
EXTERN_FLAG std::function<int(float *,float *,CUtexref)> Bound_cuTexRefGetMipmapLevelClamp;
EXTERN_FLAG std::function<int(int *,CUtexref)> Bound_cuTexRefGetMaxAnisotropy;
EXTERN_FLAG std::function<int(float *,CUtexref)> Bound_cuTexRefGetBorderColor;
EXTERN_FLAG std::function<int(unsigned int *,CUtexref)> Bound_cuTexRefGetFlags;
EXTERN_FLAG std::function<int(CUtexref *)> Bound_cuTexRefCreate;
EXTERN_FLAG std::function<int(CUtexref)> Bound_cuTexRefDestroy;
EXTERN_FLAG std::function<int(CUsurfref,CUarray,unsigned int)> Bound_cuSurfRefSetArray;
EXTERN_FLAG std::function<int(CUarray *,CUsurfref)> Bound_cuSurfRefGetArray;
EXTERN_FLAG std::function<int(CUtexObject *,const CUDA_RESOURCE_DESC *,const CUDA_TEXTURE_DESC *,const CUDA_RESOURCE_VIEW_DESC *)> Bound_cuTexObjectCreate;
EXTERN_FLAG std::function<int(CUtexObject)> Bound_cuTexObjectDestroy;
EXTERN_FLAG std::function<int(CUDA_RESOURCE_DESC *,CUtexObject)> Bound_cuTexObjectGetResourceDesc;
EXTERN_FLAG std::function<int(CUDA_TEXTURE_DESC *,CUtexObject)> Bound_cuTexObjectGetTextureDesc;
EXTERN_FLAG std::function<int(CUDA_RESOURCE_VIEW_DESC *,CUtexObject)> Bound_cuTexObjectGetResourceViewDesc;
EXTERN_FLAG std::function<int(CUsurfObject *,const CUDA_RESOURCE_DESC *)> Bound_cuSurfObjectCreate;
EXTERN_FLAG std::function<int(CUsurfObject)> Bound_cuSurfObjectDestroy;
EXTERN_FLAG std::function<int(CUDA_RESOURCE_DESC *,CUsurfObject)> Bound_cuSurfObjectGetResourceDesc;
EXTERN_FLAG std::function<int(int *,CUdevice,CUdevice)> Bound_cuDeviceCanAccessPeer;
EXTERN_FLAG std::function<int(int *,CUdevice_P2PAttribute,CUdevice,CUdevice)> Bound_cuDeviceGetP2PAttribute;
EXTERN_FLAG std::function<int(CUcontext,unsigned int)> Bound_cuCtxEnablePeerAccess;
EXTERN_FLAG std::function<int(CUcontext)> Bound_cuCtxDisablePeerAccess;
EXTERN_FLAG std::function<int(CUgraphicsResource)> Bound_cuGraphicsUnregisterResource;
EXTERN_FLAG std::function<int(CUarray *,CUgraphicsResource,unsigned int,unsigned int)> Bound_cuGraphicsSubResourceGetMappedArray;
EXTERN_FLAG std::function<int(CUmipmappedArray *,CUgraphicsResource)> Bound_cuGraphicsResourceGetMappedMipmappedArray;
EXTERN_FLAG std::function<int(CUdeviceptr *,size_t *,CUgraphicsResource)> Bound_cuGraphicsResourceGetMappedPointer;
EXTERN_FLAG std::function<int(CUgraphicsResource,unsigned int)> Bound_cuGraphicsResourceSetMapFlags;
EXTERN_FLAG std::function<int(unsigned int,CUgraphicsResource *,CUstream)> Bound_cuGraphicsMapResources;
EXTERN_FLAG std::function<int(unsigned int,CUgraphicsResource *,CUstream)> Bound_cuGraphicsUnmapResources;
EXTERN_FLAG std::function<int(const void * *,const CUuuid *)> Bound_cuGetExportTable;
EXTERN_FLAG std::function<int(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t)> Bound_cuTexRefSetAddress2D_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,const void *,size_t)> Bound_cuMemcpyHtoD_v2;
EXTERN_FLAG std::function<int(void *,CUdeviceptr,size_t)> Bound_cuMemcpyDtoH_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,CUdeviceptr,size_t)> Bound_cuMemcpyDtoD_v2;
EXTERN_FLAG std::function<int(CUarray,size_t,CUdeviceptr,size_t)> Bound_cuMemcpyDtoA_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,CUarray,size_t,size_t)> Bound_cuMemcpyAtoD_v2;
EXTERN_FLAG std::function<int(CUarray,size_t,const void *,size_t)> Bound_cuMemcpyHtoA_v2;
EXTERN_FLAG std::function<int(void *,CUarray,size_t,size_t)> Bound_cuMemcpyAtoH_v2;
EXTERN_FLAG std::function<int(CUarray,size_t,CUarray,size_t,size_t)> Bound_cuMemcpyAtoA_v2;
EXTERN_FLAG std::function<int(CUarray,size_t,const void *,size_t,CUstream)> Bound_cuMemcpyHtoAAsync_v2;
EXTERN_FLAG std::function<int(void *,CUarray,size_t,size_t,CUstream)> Bound_cuMemcpyAtoHAsync_v2;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY2D *)> Bound_cuMemcpy2D_v2;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY2D *)> Bound_cuMemcpy2DUnaligned_v2;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY3D *)> Bound_cuMemcpy3D_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,const void *,size_t,CUstream)> Bound_cuMemcpyHtoDAsync_v2;
EXTERN_FLAG std::function<int(void *,CUdeviceptr,size_t,CUstream)> Bound_cuMemcpyDtoHAsync_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,CUdeviceptr,size_t,CUstream)> Bound_cuMemcpyDtoDAsync_v2;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY2D *,CUstream)> Bound_cuMemcpy2DAsync_v2;
EXTERN_FLAG std::function<int(const CUDA_MEMCPY3D *,CUstream)> Bound_cuMemcpy3DAsync_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned char,size_t)> Bound_cuMemsetD8_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned short,size_t)> Bound_cuMemsetD16_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,unsigned int,size_t)> Bound_cuMemsetD32_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned char,size_t,size_t)> Bound_cuMemsetD2D8_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned short,size_t,size_t)> Bound_cuMemsetD2D16_v2;
EXTERN_FLAG std::function<int(CUdeviceptr,size_t,unsigned int,size_t,size_t)> Bound_cuMemsetD2D32_v2;

// MANUALLY ADDED
EXTERN_FLAG std::function<int(void *, void *, void *)> Bound_InternalSynchronization;