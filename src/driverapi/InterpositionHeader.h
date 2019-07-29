#pragma once
#include <dlfcn.h>
#include "Parameters.h"
#include "DriverWrapperBase.h"
extern "C"{
#define CUDAAPI


#define CUarray void *
#define CUtexObject void *
#define CUipcEventHandle void *
#define CUmodule void *
#define CUstreamCallback void *
#define CUtexref void *
#define CUdevice_attribute void *
#define CUdevice void *
#define CUdeviceptr void *
#define CUcontext void *
#define CUipcMemHandle void *
#define CUDA_MEMCPY3D_PEER void *
#define CUlimit void *
#define CUarray_format void *
#define CUDA_ARRAY3D_DESCRIPTOR void *
#define CUevent void *
#define CUDA_MEMCPY2D void *
#define CUpointer_attribute void *
#define CUDA_RESOURCE_VIEW_DESC void *
#define CUmipmappedArray void *
#define CUaddress_mode void *
#define CUfunc_cache void *
#define CUjit_option void *
#define CUfunction_attribute void *
#define CUgraphicsResource void *
#define CUresult int
#define CUlinkState void *
#define CUDA_RESOURCE_DESC void *
#define CUuuid void *
#define CUDA_ARRAY_DESCRIPTOR void *
#define CUsharedconfig void *
#define CUjitInputType void *
#define CUDA_MEMCPY3D void *
#define CUsurfref void *
#define CUfilter_mode void *
#define CUDA_TEXTURE_DESC void *
#define CUstream void *
#define CUfunction void *
#define CUsurfObject void *
#define CUoccupancyB2DSize void *
#define CUdevprop void *
CUresult CUDAAPI cuGetErrorString( CUresult error, const char * * pStr );
CUresult CUDAAPI cuGetErrorName( CUresult error, const char * * pStr );
CUresult CUDAAPI cuInit( unsigned int Flags );
CUresult CUDAAPI cuDriverGetVersion( int * driverVersion );
CUresult CUDAAPI cuDeviceGet( CUdevice * device, int ordinal );
CUresult CUDAAPI cuDeviceGetCount( int * count );
CUresult CUDAAPI cuDeviceGetName( char * name, int len, CUdevice dev );
CUresult CUDAAPI cuDeviceTotalMem( size_t * bytes, CUdevice dev );
CUresult CUDAAPI cuDeviceGetAttribute( int * pi, CUdevice_attribute attrib, CUdevice dev );
CUresult CUDAAPI cuDeviceGetProperties( CUdevprop * prop, CUdevice dev );
CUresult CUDAAPI cuDeviceComputeCapability( int * major, int * minor, CUdevice dev );
CUresult CUDAAPI cuDevicePrimaryCtxRetain( CUcontext * pctx, CUdevice dev );
CUresult CUDAAPI cuDevicePrimaryCtxRelease( CUdevice dev );
CUresult CUDAAPI cuDevicePrimaryCtxSetFlags( CUdevice dev, unsigned int flags );
CUresult CUDAAPI cuDevicePrimaryCtxGetState( CUdevice dev, unsigned int * flags, int * active );
CUresult CUDAAPI cuDevicePrimaryCtxReset( CUdevice dev );
CUresult CUDAAPI cuCtxCreate( CUcontext * pctx, unsigned int flags, CUdevice dev );
CUresult CUDAAPI cuCtxDestroy( CUcontext ctx );
CUresult CUDAAPI cuCtxPushCurrent( CUcontext ctx );
CUresult CUDAAPI cuCtxPopCurrent( CUcontext * pctx );
CUresult CUDAAPI cuCtxSetCurrent( CUcontext ctx );
CUresult CUDAAPI cuCtxGetCurrent( CUcontext * pctx );
CUresult CUDAAPI cuCtxGetDevice( CUdevice * device );
CUresult CUDAAPI cuCtxGetFlags( unsigned int * flags );
CUresult CUDAAPI cuCtxSynchronize( void  );
CUresult CUDAAPI cuCtxSetLimit( CUlimit limit, size_t value );
CUresult CUDAAPI cuCtxGetLimit( size_t * pvalue, CUlimit limit );
CUresult CUDAAPI cuCtxGetCacheConfig( CUfunc_cache * pconfig );
CUresult CUDAAPI cuCtxSetCacheConfig( CUfunc_cache config );
CUresult CUDAAPI cuCtxGetSharedMemConfig( CUsharedconfig * pConfig );
CUresult CUDAAPI cuCtxSetSharedMemConfig( CUsharedconfig config );
CUresult CUDAAPI cuCtxGetApiVersion( CUcontext ctx, unsigned int * version );
CUresult CUDAAPI cuCtxGetStreamPriorityRange( int * leastPriority, int * greatestPriority );
CUresult CUDAAPI cuCtxAttach( CUcontext * pctx, unsigned int flags );
CUresult CUDAAPI cuCtxDetach( CUcontext ctx );
CUresult CUDAAPI cuModuleLoad( CUmodule * module, const char * fname );
CUresult CUDAAPI cuModuleLoadData( CUmodule * module, const void * image );
CUresult CUDAAPI cuModuleLoadDataEx( CUmodule * module, const void * image, unsigned int numOptions, CUjit_option * options, void * * optionValues );
CUresult CUDAAPI cuModuleLoadFatBinary( CUmodule * module, const void * fatCubin );
CUresult CUDAAPI cuModuleUnload( CUmodule hmod );
CUresult CUDAAPI cuModuleGetFunction( CUfunction * hfunc, CUmodule hmod, const char * name );
CUresult CUDAAPI cuModuleGetGlobal( CUdeviceptr * dptr, size_t * bytes, CUmodule hmod, const char * name );
CUresult CUDAAPI cuModuleGetTexRef( CUtexref * pTexRef, CUmodule hmod, const char * name );
CUresult CUDAAPI cuModuleGetSurfRef( CUsurfref * pSurfRef, CUmodule hmod, const char * name );
CUresult CUDAAPI cuLinkCreate( unsigned int numOptions, CUjit_option * options, void * * optionValues, CUlinkState * stateOut );
CUresult CUDAAPI cuLinkAddData( CUlinkState state, CUjitInputType type, void * data, size_t size, const char * name, unsigned int numOptions, CUjit_option * options, void * * optionValues );
CUresult CUDAAPI cuLinkAddFile( CUlinkState state, CUjitInputType type, const char * path, unsigned int numOptions, CUjit_option * options, void * * optionValues );
CUresult CUDAAPI cuLinkComplete( CUlinkState state, void * * cubinOut, size_t * sizeOut );
CUresult CUDAAPI cuLinkDestroy( CUlinkState state );
CUresult CUDAAPI cuMemGetInfo( size_t * free, size_t * total );
CUresult CUDAAPI cuMemAlloc( CUdeviceptr * dptr, size_t bytesize );
CUresult CUDAAPI cuMemAllocPitch( CUdeviceptr * dptr, size_t * pPitch, size_t WidthInBytes, size_t Height, unsigned int ElementSizeBytes );
CUresult CUDAAPI cuMemFree( CUdeviceptr dptr );
CUresult CUDAAPI cuMemGetAddressRange( CUdeviceptr * pbase, size_t * psize, CUdeviceptr dptr );
CUresult CUDAAPI cuMemAllocHost( void * * pp, size_t bytesize );
CUresult CUDAAPI cuMemFreeHost( void * p );
CUresult CUDAAPI cuMemHostAlloc( void * * pp, size_t bytesize, unsigned int Flags );
CUresult CUDAAPI cuMemHostGetDevicePointer( CUdeviceptr * pdptr, void * p, unsigned int Flags );
CUresult CUDAAPI cuMemHostGetFlags( unsigned int * pFlags, void * p );
CUresult CUDAAPI cuMemAllocManaged( CUdeviceptr * dptr, size_t bytesize, unsigned int flags );
CUresult CUDAAPI cuDeviceGetByPCIBusId( CUdevice * dev, const char * pciBusId );
CUresult CUDAAPI cuDeviceGetPCIBusId( char * pciBusId, int len, CUdevice dev );
CUresult CUDAAPI cuIpcGetEventHandle( CUipcEventHandle * pHandle, CUevent event );
CUresult CUDAAPI cuIpcOpenEventHandle( CUevent * phEvent, CUipcEventHandle handle );
CUresult CUDAAPI cuIpcGetMemHandle( CUipcMemHandle * pHandle, CUdeviceptr dptr );
CUresult CUDAAPI cuIpcOpenMemHandle( CUdeviceptr * pdptr, CUipcMemHandle handle, unsigned int Flags );
CUresult CUDAAPI cuIpcCloseMemHandle( CUdeviceptr dptr );
CUresult CUDAAPI cuMemHostRegister( void * p, size_t bytesize, unsigned int Flags );
CUresult CUDAAPI cuMemHostUnregister( void * p );
CUresult CUDAAPI cuMemcpy( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount );
CUresult CUDAAPI cuMemcpyPeer( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount );
CUresult CUDAAPI cuMemcpyHtoD( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount );
CUresult CUDAAPI cuMemcpyDtoH( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount );
CUresult CUDAAPI cuMemcpyDtoD( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount );
CUresult CUDAAPI cuMemcpyDtoA( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount );
CUresult CUDAAPI cuMemcpyAtoD( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount );
CUresult CUDAAPI cuMemcpyHtoA( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount );
CUresult CUDAAPI cuMemcpyAtoH( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount );
CUresult CUDAAPI cuMemcpyAtoA( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount );
CUresult CUDAAPI cuMemcpy2D( const CUDA_MEMCPY2D * pCopy );
CUresult CUDAAPI cuMemcpy2DUnaligned( const CUDA_MEMCPY2D * pCopy );
CUresult CUDAAPI cuMemcpy3D( const CUDA_MEMCPY3D * pCopy );
CUresult CUDAAPI cuMemcpy3DPeer( const CUDA_MEMCPY3D_PEER * pCopy );
CUresult CUDAAPI cuMemcpyAsync( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyPeerAsync( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyHtoDAsync( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyDtoHAsync( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyDtoDAsync( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyHtoAAsync( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyAtoHAsync( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpy2DAsync( const CUDA_MEMCPY2D * pCopy, CUstream hStream );
CUresult CUDAAPI cuMemcpy3DAsync( const CUDA_MEMCPY3D * pCopy, CUstream hStream );
CUresult CUDAAPI cuMemcpy3DPeerAsync( const CUDA_MEMCPY3D_PEER * pCopy, CUstream hStream );
CUresult CUDAAPI cuMemsetD8( CUdeviceptr dstDevice, unsigned char uc, size_t N );
CUresult CUDAAPI cuMemsetD16( CUdeviceptr dstDevice, unsigned short us, size_t N );
CUresult CUDAAPI cuMemsetD32( CUdeviceptr dstDevice, unsigned int ui, size_t N );
CUresult CUDAAPI cuMemsetD2D8( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height );
CUresult CUDAAPI cuMemsetD2D16( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height );
CUresult CUDAAPI cuMemsetD2D32( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height );
CUresult CUDAAPI cuMemsetD8Async( CUdeviceptr dstDevice, unsigned char uc, size_t N, CUstream hStream );
CUresult CUDAAPI cuMemsetD16Async( CUdeviceptr dstDevice, unsigned short us, size_t N, CUstream hStream );
CUresult CUDAAPI cuMemsetD32Async( CUdeviceptr dstDevice, unsigned int ui, size_t N, CUstream hStream );
CUresult CUDAAPI cuMemsetD2D8Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height, CUstream hStream );
CUresult CUDAAPI cuMemsetD2D16Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height, CUstream hStream );
CUresult CUDAAPI cuMemsetD2D32Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height, CUstream hStream );
CUresult CUDAAPI cuArrayCreate( CUarray * pHandle, const CUDA_ARRAY_DESCRIPTOR * pAllocateArray );
CUresult CUDAAPI cuArrayGetDescriptor( CUDA_ARRAY_DESCRIPTOR * pArrayDescriptor, CUarray hArray );
CUresult CUDAAPI cuArrayDestroy( CUarray hArray );
CUresult CUDAAPI cuArray3DCreate( CUarray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pAllocateArray );
CUresult CUDAAPI cuArray3DGetDescriptor( CUDA_ARRAY3D_DESCRIPTOR * pArrayDescriptor, CUarray hArray );
CUresult CUDAAPI cuMipmappedArrayCreate( CUmipmappedArray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pMipmappedArrayDesc, unsigned int numMipmapLevels );
CUresult CUDAAPI cuMipmappedArrayGetLevel( CUarray * pLevelArray, CUmipmappedArray hMipmappedArray, unsigned int level );
CUresult CUDAAPI cuMipmappedArrayDestroy( CUmipmappedArray hMipmappedArray );
CUresult CUDAAPI cuPointerGetAttribute( void * data, CUpointer_attribute attribute, CUdeviceptr ptr );
CUresult CUDAAPI cuPointerSetAttribute( const void * value, CUpointer_attribute attribute, CUdeviceptr ptr );
CUresult CUDAAPI cuPointerGetAttributes( unsigned int numAttributes, CUpointer_attribute * attributes, void * * data, CUdeviceptr ptr );
CUresult CUDAAPI cuStreamCreate( CUstream * phStream, unsigned int Flags );
CUresult CUDAAPI cuStreamCreateWithPriority( CUstream * phStream, unsigned int flags, int priority );
CUresult CUDAAPI cuStreamGetPriority( CUstream hStream, int * priority );
CUresult CUDAAPI cuStreamGetFlags( CUstream hStream, unsigned int * flags );
CUresult CUDAAPI cuStreamWaitEvent( CUstream hStream, CUevent hEvent, unsigned int Flags );
CUresult CUDAAPI cuStreamAddCallback( CUstream hStream, CUstreamCallback callback, void * userData, unsigned int flags );
CUresult CUDAAPI cuStreamAttachMemAsync( CUstream hStream, CUdeviceptr dptr, size_t length, unsigned int flags );
CUresult CUDAAPI cuStreamQuery( CUstream hStream );
CUresult CUDAAPI cuStreamSynchronize( CUstream hStream );
CUresult CUDAAPI cuStreamDestroy( CUstream hStream );
CUresult CUDAAPI cuEventCreate( CUevent * phEvent, unsigned int Flags );
CUresult CUDAAPI cuEventRecord( CUevent hEvent, CUstream hStream );
CUresult CUDAAPI cuEventQuery( CUevent hEvent );
CUresult CUDAAPI cuEventSynchronize( CUevent hEvent );
CUresult CUDAAPI cuEventDestroy( CUevent hEvent );
CUresult CUDAAPI cuEventElapsedTime( float * pMilliseconds, CUevent hStart, CUevent hEnd );
CUresult CUDAAPI cuFuncGetAttribute( int * pi, CUfunction_attribute attrib, CUfunction hfunc );
CUresult CUDAAPI cuFuncSetCacheConfig( CUfunction hfunc, CUfunc_cache config );
CUresult CUDAAPI cuFuncSetSharedMemConfig( CUfunction hfunc, CUsharedconfig config );
CUresult CUDAAPI cuLaunchKernel( CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes, CUstream hStream, void * * kernelParams, void * * extra );
CUresult CUDAAPI cuFuncSetBlockShape( CUfunction hfunc, int x, int y, int z );
CUresult CUDAAPI cuFuncSetSharedSize( CUfunction hfunc, unsigned int bytes );
CUresult CUDAAPI cuParamSetSize( CUfunction hfunc, unsigned int numbytes );
CUresult CUDAAPI cuParamSeti( CUfunction hfunc, int offset, unsigned int value );
CUresult CUDAAPI cuParamSetf( CUfunction hfunc, int offset, float value );
CUresult CUDAAPI cuParamSetv( CUfunction hfunc, int offset, void * ptr, unsigned int numbytes );
CUresult CUDAAPI cuLaunch( CUfunction f );
CUresult CUDAAPI cuLaunchGrid( CUfunction f, int grid_width, int grid_height );
CUresult CUDAAPI cuLaunchGridAsync( CUfunction f, int grid_width, int grid_height, CUstream hStream );
CUresult CUDAAPI cuParamSetTexRef( CUfunction hfunc, int texunit, CUtexref hTexRef );
CUresult CUDAAPI cuOccupancyMaxActiveBlocksPerMultiprocessor( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize );
CUresult CUDAAPI cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize, unsigned int flags );
CUresult CUDAAPI cuOccupancyMaxPotentialBlockSize( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit );
CUresult CUDAAPI cuOccupancyMaxPotentialBlockSizeWithFlags( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit, unsigned int flags );
CUresult CUDAAPI cuTexRefSetArray( CUtexref hTexRef, CUarray hArray, unsigned int Flags );
CUresult CUDAAPI cuTexRefSetMipmappedArray( CUtexref hTexRef, CUmipmappedArray hMipmappedArray, unsigned int Flags );
CUresult CUDAAPI cuTexRefSetAddress( size_t * ByteOffset, CUtexref hTexRef, CUdeviceptr dptr, size_t bytes );
CUresult CUDAAPI cuTexRefSetAddress2D( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch );
CUresult CUDAAPI cuTexRefSetFormat( CUtexref hTexRef, CUarray_format fmt, int NumPackedComponents );
CUresult CUDAAPI cuTexRefSetAddressMode( CUtexref hTexRef, int dim, CUaddress_mode am );
CUresult CUDAAPI cuTexRefSetFilterMode( CUtexref hTexRef, CUfilter_mode fm );
CUresult CUDAAPI cuTexRefSetMipmapFilterMode( CUtexref hTexRef, CUfilter_mode fm );
CUresult CUDAAPI cuTexRefSetMipmapLevelBias( CUtexref hTexRef, float bias );
CUresult CUDAAPI cuTexRefSetMipmapLevelClamp( CUtexref hTexRef, float minMipmapLevelClamp, float maxMipmapLevelClamp );
CUresult CUDAAPI cuTexRefSetMaxAnisotropy( CUtexref hTexRef, unsigned int maxAniso );
CUresult CUDAAPI cuTexRefSetFlags( CUtexref hTexRef, unsigned int Flags );
CUresult CUDAAPI cuTexRefGetAddress( CUdeviceptr * pdptr, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetArray( CUarray * phArray, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetMipmappedArray( CUmipmappedArray * phMipmappedArray, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetAddressMode( CUaddress_mode * pam, CUtexref hTexRef, int dim );
CUresult CUDAAPI cuTexRefGetFilterMode( CUfilter_mode * pfm, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetFormat( CUarray_format * pFormat, int * pNumChannels, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetMipmapFilterMode( CUfilter_mode * pfm, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetMipmapLevelBias( float * pbias, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetMipmapLevelClamp( float * pminMipmapLevelClamp, float * pmaxMipmapLevelClamp, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetMaxAnisotropy( int * pmaxAniso, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefGetFlags( unsigned int * pFlags, CUtexref hTexRef );
CUresult CUDAAPI cuTexRefCreate( CUtexref * pTexRef );
CUresult CUDAAPI cuTexRefDestroy( CUtexref hTexRef );
CUresult CUDAAPI cuSurfRefSetArray( CUsurfref hSurfRef, CUarray hArray, unsigned int Flags );
CUresult CUDAAPI cuSurfRefGetArray( CUarray * phArray, CUsurfref hSurfRef );
CUresult CUDAAPI cuTexObjectCreate( CUtexObject * pTexObject, const CUDA_RESOURCE_DESC * pResDesc, const CUDA_TEXTURE_DESC * pTexDesc, const CUDA_RESOURCE_VIEW_DESC * pResViewDesc );
CUresult CUDAAPI cuTexObjectDestroy( CUtexObject texObject );
CUresult CUDAAPI cuTexObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUtexObject texObject );
CUresult CUDAAPI cuTexObjectGetTextureDesc( CUDA_TEXTURE_DESC * pTexDesc, CUtexObject texObject );
CUresult CUDAAPI cuTexObjectGetResourceViewDesc( CUDA_RESOURCE_VIEW_DESC * pResViewDesc, CUtexObject texObject );
CUresult CUDAAPI cuSurfObjectCreate( CUsurfObject * pSurfObject, const CUDA_RESOURCE_DESC * pResDesc );
CUresult CUDAAPI cuSurfObjectDestroy( CUsurfObject surfObject );
CUresult CUDAAPI cuSurfObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUsurfObject surfObject );
CUresult CUDAAPI cuDeviceCanAccessPeer( int * canAccessPeer, CUdevice dev, CUdevice peerDev );
CUresult CUDAAPI cuCtxEnablePeerAccess( CUcontext peerContext, unsigned int Flags );
CUresult CUDAAPI cuCtxDisablePeerAccess( CUcontext peerContext );
CUresult CUDAAPI cuGraphicsUnregisterResource( CUgraphicsResource resource );
CUresult CUDAAPI cuGraphicsSubResourceGetMappedArray( CUarray * pArray, CUgraphicsResource resource, unsigned int arrayIndex, unsigned int mipLevel );
CUresult CUDAAPI cuGraphicsResourceGetMappedMipmappedArray( CUmipmappedArray * pMipmappedArray, CUgraphicsResource resource );
CUresult CUDAAPI cuGraphicsResourceGetMappedPointer( CUdeviceptr * pDevPtr, size_t * pSize, CUgraphicsResource resource );
CUresult CUDAAPI cuGraphicsResourceSetMapFlags( CUgraphicsResource resource, unsigned int flags );
CUresult CUDAAPI cuGraphicsMapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream );
CUresult CUDAAPI cuGraphicsUnmapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream );
CUresult CUDAAPI cuGetExportTable( const void * * ppExportTable, const CUuuid * pExportTableId );
CUresult CUDAAPI cuTexRefSetAddress2D_v2( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch );
CUresult CUDAAPI cuMemcpyHtoD_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount );
CUresult CUDAAPI cuMemcpyDtoH_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount );
CUresult CUDAAPI cuMemcpyDtoD_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount );
CUresult CUDAAPI cuMemcpyDtoA_v2( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount );
CUresult CUDAAPI cuMemcpyAtoD_v2( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount );
CUresult CUDAAPI cuMemcpyHtoA_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount );
CUresult CUDAAPI cuMemcpyAtoH_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount );
CUresult CUDAAPI cuMemcpyAtoA_v2( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount );
CUresult CUDAAPI cuMemcpyHtoAAsync_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyAtoHAsync_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpy2D_v2( const CUDA_MEMCPY2D * pCopy );
CUresult CUDAAPI cuMemcpy2DUnaligned_v2( const CUDA_MEMCPY2D * pCopy );
CUresult CUDAAPI cuMemcpy3D_v2( const CUDA_MEMCPY3D * pCopy );
CUresult CUDAAPI cuMemcpyHtoDAsync_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyDtoHAsync_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpyDtoDAsync_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream );
CUresult CUDAAPI cuMemcpy2DAsync_v2( const CUDA_MEMCPY2D * pCopy, CUstream hStream );
CUresult CUDAAPI cuMemcpy3DAsync_v2( const CUDA_MEMCPY3D * pCopy, CUstream hStream );
CUresult CUDAAPI cuMemsetD8_v2( CUdeviceptr dstDevice, unsigned char uc, size_t N );
CUresult CUDAAPI cuMemsetD16_v2( CUdeviceptr dstDevice, unsigned short us, size_t N );
CUresult CUDAAPI cuMemsetD32_v2( CUdeviceptr dstDevice, unsigned int ui, size_t N );
CUresult CUDAAPI cuMemsetD2D8_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height );
CUresult CUDAAPI cuMemsetD2D16_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height );
CUresult CUDAAPI cuMemsetD2D32_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height );

};
