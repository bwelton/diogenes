#include "cuda.h"
#include <stdio.h>
#include <cassert>
extern "C" {
int ORIGINAL_InternalSynchronization( void * a, void * b, void * c) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0);}
int ORIGINAL_cuGetErrorString( CUresult error, const char * * pStr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGetErrorName( CUresult error, const char * * pStr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuInit( unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDriverGetVersion( int * driverVersion ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGet( CUdevice * device, int ordinal ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGetCount( int * count ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGetName( char * name, int len, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceTotalMem( size_t * bytes, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGetAttribute( int * pi, CUdevice_attribute attrib, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGetProperties( CUdevprop * prop, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceComputeCapability( int * major, int * minor, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDevicePrimaryCtxRetain( CUcontext * pctx, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDevicePrimaryCtxRelease( CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDevicePrimaryCtxSetFlags( CUdevice dev, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDevicePrimaryCtxGetState( CUdevice dev, unsigned int * flags, int * active ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDevicePrimaryCtxReset( CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxCreate( CUcontext * pctx, unsigned int flags, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxDestroy( CUcontext ctx ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxPushCurrent( CUcontext ctx ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxPopCurrent( CUcontext * pctx ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxSetCurrent( CUcontext ctx ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetCurrent( CUcontext * pctx ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetDevice( CUdevice * device ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetFlags( unsigned int * flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxSynchronize( void  ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxSetLimit( CUlimit limit, size_t value ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetLimit( size_t * pvalue, CUlimit limit ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetCacheConfig( CUfunc_cache * pconfig ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxSetCacheConfig( CUfunc_cache config ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetSharedMemConfig( CUsharedconfig * pConfig ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxSetSharedMemConfig( CUsharedconfig config ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetApiVersion( CUcontext ctx, unsigned int * version ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxGetStreamPriorityRange( int * leastPriority, int * greatestPriority ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxAttach( CUcontext * pctx, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxDetach( CUcontext ctx ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleLoad( CUmodule * module, const char * fname ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleLoadData( CUmodule * module, const void * image ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleLoadDataEx( CUmodule * module, const void * image, unsigned int numOptions, CUjit_option * options, void * * optionValues ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleLoadFatBinary( CUmodule * module, const void * fatCubin ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleUnload( CUmodule hmod ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleGetFunction( CUfunction * hfunc, CUmodule hmod, const char * name ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleGetGlobal( CUdeviceptr * dptr, size_t * bytes, CUmodule hmod, const char * name ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleGetTexRef( CUtexref * pTexRef, CUmodule hmod, const char * name ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuModuleGetSurfRef( CUsurfref * pSurfRef, CUmodule hmod, const char * name ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLinkCreate( unsigned int numOptions, CUjit_option * options, void * * optionValues, CUlinkState * stateOut ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLinkAddData( CUlinkState state, CUjitInputType type, void * data, size_t size, const char * name, unsigned int numOptions, CUjit_option * options, void * * optionValues ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLinkAddFile( CUlinkState state, CUjitInputType type, const char * path, unsigned int numOptions, CUjit_option * options, void * * optionValues ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLinkComplete( CUlinkState state, void * * cubinOut, size_t * sizeOut ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLinkDestroy( CUlinkState state ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemGetInfo( size_t * free, size_t * total ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemAlloc( CUdeviceptr * dptr, size_t bytesize ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemAllocPitch( CUdeviceptr * dptr, size_t * pPitch, size_t WidthInBytes, size_t Height, unsigned int ElementSizeBytes ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemFree( CUdeviceptr dptr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemGetAddressRange( CUdeviceptr * pbase, size_t * psize, CUdeviceptr dptr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemAllocHost( void * * pp, size_t bytesize ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemFreeHost( void * p ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemHostAlloc( void * * pp, size_t bytesize, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemHostGetDevicePointer( CUdeviceptr * pdptr, void * p, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemHostGetFlags( unsigned int * pFlags, void * p ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemAllocManaged( CUdeviceptr * dptr, size_t bytesize, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGetByPCIBusId( CUdevice * dev, const char * pciBusId ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGetPCIBusId( char * pciBusId, int len, CUdevice dev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuIpcGetEventHandle( CUipcEventHandle * pHandle, CUevent event ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuIpcOpenEventHandle( CUevent * phEvent, CUipcEventHandle handle ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuIpcGetMemHandle( CUipcMemHandle * pHandle, CUdeviceptr dptr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuIpcOpenMemHandle( CUdeviceptr * pdptr, CUipcMemHandle handle, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuIpcCloseMemHandle( CUdeviceptr dptr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemHostRegister( void * p, size_t bytesize, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemHostUnregister( void * p ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyPeer( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoD( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoH( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoD( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoA( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoD( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoA( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoH( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoA( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy2D( const CUDA_MEMCPY2D * pCopy ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy2DUnaligned( const CUDA_MEMCPY2D * pCopy ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy3D( const CUDA_MEMCPY3D * pCopy ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy3DPeer( const CUDA_MEMCPY3D_PEER * pCopy ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAsync( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyPeerAsync( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoDAsync( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoHAsync( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoDAsync( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoAAsync( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoHAsync( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy2DAsync( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy3DAsync( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy3DPeerAsync( const CUDA_MEMCPY3D_PEER * pCopy, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD8( CUdeviceptr dstDevice, unsigned char uc, size_t N ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD16( CUdeviceptr dstDevice, unsigned short us, size_t N ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD32( CUdeviceptr dstDevice, unsigned int ui, size_t N ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D8( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D16( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D32( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD8Async( CUdeviceptr dstDevice, unsigned char uc, size_t N, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD16Async( CUdeviceptr dstDevice, unsigned short us, size_t N, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD32Async( CUdeviceptr dstDevice, unsigned int ui, size_t N, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D8Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D16Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D32Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuArrayCreate( CUarray * pHandle, const CUDA_ARRAY_DESCRIPTOR * pAllocateArray ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuArrayGetDescriptor( CUDA_ARRAY_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuArrayDestroy( CUarray hArray ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuArray3DCreate( CUarray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pAllocateArray ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuArray3DGetDescriptor( CUDA_ARRAY3D_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMipmappedArrayCreate( CUmipmappedArray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pMipmappedArrayDesc, unsigned int numMipmapLevels ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMipmappedArrayGetLevel( CUarray * pLevelArray, CUmipmappedArray hMipmappedArray, unsigned int level ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMipmappedArrayDestroy( CUmipmappedArray hMipmappedArray ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuPointerGetAttribute( void * data, CUpointer_attribute attribute, CUdeviceptr ptr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemPrefetchAsync( CUdeviceptr devPtr, size_t count, CUdevice dstDevice, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemAdvise( CUdeviceptr devPtr, size_t count, CUmem_advise advice, CUdevice device ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemRangeGetAttribute( void * data, size_t dataSize, CUmem_range_attribute attribute, CUdeviceptr devPtr, size_t count ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemRangeGetAttributes( void * * data, size_t * dataSizes, CUmem_range_attribute * attributes, size_t numAttributes, CUdeviceptr devPtr, size_t count ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuPointerSetAttribute( const void * value, CUpointer_attribute attribute, CUdeviceptr ptr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuPointerGetAttributes( unsigned int numAttributes, CUpointer_attribute * attributes, void * * data, CUdeviceptr ptr ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamCreate( CUstream * phStream, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamCreateWithPriority( CUstream * phStream, unsigned int flags, int priority ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamGetPriority( CUstream hStream, int * priority ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamGetFlags( CUstream hStream, unsigned int * flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamWaitEvent( CUstream hStream, CUevent hEvent, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamAddCallback( CUstream hStream, CUstreamCallback callback, void * userData, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamAttachMemAsync( CUstream hStream, CUdeviceptr dptr, size_t length, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamQuery( CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamSynchronize( CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamDestroy( CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuEventCreate( CUevent * phEvent, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuEventRecord( CUevent hEvent, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuEventQuery( CUevent hEvent ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuEventSynchronize( CUevent hEvent ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuEventDestroy( CUevent hEvent ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuEventElapsedTime( float * pMilliseconds, CUevent hStart, CUevent hEnd ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamWaitValue32( CUstream stream, CUdeviceptr addr, cuuint32_t value, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamWriteValue32( CUstream stream, CUdeviceptr addr, cuuint32_t value, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuStreamBatchMemOp( CUstream stream, unsigned int count, CUstreamBatchMemOpParams * paramArray, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuFuncGetAttribute( int * pi, CUfunction_attribute attrib, CUfunction hfunc ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuFuncSetCacheConfig( CUfunction hfunc, CUfunc_cache config ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuFuncSetSharedMemConfig( CUfunction hfunc, CUsharedconfig config ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLaunchKernel( CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes, CUstream hStream, void * * kernelParams, void * * extra ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuFuncSetBlockShape( CUfunction hfunc, int x, int y, int z ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuFuncSetSharedSize( CUfunction hfunc, unsigned int bytes ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuParamSetSize( CUfunction hfunc, unsigned int numbytes ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuParamSeti( CUfunction hfunc, int offset, unsigned int value ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuParamSetf( CUfunction hfunc, int offset, float value ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuParamSetv( CUfunction hfunc, int offset, void * ptr, unsigned int numbytes ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLaunch( CUfunction f ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLaunchGrid( CUfunction f, int grid_width, int grid_height ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuLaunchGridAsync( CUfunction f, int grid_width, int grid_height, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuParamSetTexRef( CUfunction hfunc, int texunit, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessor( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuOccupancyMaxPotentialBlockSize( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuOccupancyMaxPotentialBlockSizeWithFlags( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetArray( CUtexref hTexRef, CUarray hArray, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetMipmappedArray( CUtexref hTexRef, CUmipmappedArray hMipmappedArray, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetAddress( size_t * ByteOffset, CUtexref hTexRef, CUdeviceptr dptr, size_t bytes ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetAddress2D( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetFormat( CUtexref hTexRef, CUarray_format fmt, int NumPackedComponents ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetAddressMode( CUtexref hTexRef, int dim, CUaddress_mode am ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetFilterMode( CUtexref hTexRef, CUfilter_mode fm ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetMipmapFilterMode( CUtexref hTexRef, CUfilter_mode fm ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetMipmapLevelBias( CUtexref hTexRef, float bias ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetMipmapLevelClamp( CUtexref hTexRef, float minMipmapLevelClamp, float maxMipmapLevelClamp ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetMaxAnisotropy( CUtexref hTexRef, unsigned int maxAniso ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetBorderColor( CUtexref hTexRef, float * pBorderColor ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetFlags( CUtexref hTexRef, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetAddress( CUdeviceptr * pdptr, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetArray( CUarray * phArray, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetMipmappedArray( CUmipmappedArray * phMipmappedArray, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetAddressMode( CUaddress_mode * pam, CUtexref hTexRef, int dim ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetFormat( CUarray_format * pFormat, int * pNumChannels, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetMipmapFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetMipmapLevelBias( float * pbias, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetMipmapLevelClamp( float * pminMipmapLevelClamp, float * pmaxMipmapLevelClamp, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetMaxAnisotropy( int * pmaxAniso, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetBorderColor( float * pBorderColor, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefGetFlags( unsigned int * pFlags, CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefCreate( CUtexref * pTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefDestroy( CUtexref hTexRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuSurfRefSetArray( CUsurfref hSurfRef, CUarray hArray, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuSurfRefGetArray( CUarray * phArray, CUsurfref hSurfRef ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexObjectCreate( CUtexObject * pTexObject, const CUDA_RESOURCE_DESC * pResDesc, const CUDA_TEXTURE_DESC * pTexDesc, const CUDA_RESOURCE_VIEW_DESC * pResViewDesc ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexObjectDestroy( CUtexObject texObject ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUtexObject texObject ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexObjectGetTextureDesc( CUDA_TEXTURE_DESC * pTexDesc, CUtexObject texObject ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexObjectGetResourceViewDesc( CUDA_RESOURCE_VIEW_DESC * pResViewDesc, CUtexObject texObject ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuSurfObjectCreate( CUsurfObject * pSurfObject, const CUDA_RESOURCE_DESC * pResDesc ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuSurfObjectDestroy( CUsurfObject surfObject ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuSurfObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUsurfObject surfObject ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceCanAccessPeer( int * canAccessPeer, CUdevice dev, CUdevice peerDev ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuDeviceGetP2PAttribute( int * value, CUdevice_P2PAttribute attrib, CUdevice srcDevice, CUdevice dstDevice ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxEnablePeerAccess( CUcontext peerContext, unsigned int Flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuCtxDisablePeerAccess( CUcontext peerContext ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGraphicsUnregisterResource( CUgraphicsResource resource ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGraphicsSubResourceGetMappedArray( CUarray * pArray, CUgraphicsResource resource, unsigned int arrayIndex, unsigned int mipLevel ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGraphicsResourceGetMappedMipmappedArray( CUmipmappedArray * pMipmappedArray, CUgraphicsResource resource ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGraphicsResourceGetMappedPointer( CUdeviceptr * pDevPtr, size_t * pSize, CUgraphicsResource resource ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGraphicsResourceSetMapFlags( CUgraphicsResource resource, unsigned int flags ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGraphicsMapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGraphicsUnmapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuGetExportTable( const void * * ppExportTable, const CUuuid * pExportTableId ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuTexRefSetAddress2D_v2( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoD_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoH_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoD_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoA_v2( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoD_v2( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoA_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoH_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoA_v2( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoAAsync_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyAtoHAsync_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy2D_v2( const CUDA_MEMCPY2D * pCopy ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy2DUnaligned_v2( const CUDA_MEMCPY2D * pCopy ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy3D_v2( const CUDA_MEMCPY3D * pCopy ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyHtoDAsync_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoHAsync_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpyDtoDAsync_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy2DAsync_v2( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemcpy3DAsync_v2( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD8_v2( CUdeviceptr dstDevice, unsigned char uc, size_t N ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD16_v2( CUdeviceptr dstDevice, unsigned short us, size_t N ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD32_v2( CUdeviceptr dstDevice, unsigned int ui, size_t N ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D8_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D16_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }
int ORIGINAL_cuMemsetD2D32_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) { fprintf(stderr, "We are calling the stub wrapper function, this is wrong, exit now\n"); assert(1 == 0); }

}