#include "InterpositionHeader.h"
#include <tuple>
#include "DriverWrapperBase.h"
#include "DriverWrapperFactory.h"
std::shared_ptr<DriverWrapperFactory> DriverFactory;
extern "C" {// This is the call that will take the place of the original
CUresult CUDAAPI cuGetErrorString( CUresult error, const char * * pStr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&error,(void *)&pStr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(0, &params));
	CUresult CUDAAPI (*REAL_cuGetErrorString)(CUresult,const char * *);
	*(void **)(*REAL_cuGetErrorString) = dlsym(RTLD_NEXT, "cuGetErrorString");
	DriverAPICall call = std::bind(REAL_cuGetErrorString ,error,pStr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGetErrorName( CUresult error, const char * * pStr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&error,(void *)&pStr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(1, &params));
	CUresult CUDAAPI (*REAL_cuGetErrorName)(CUresult,const char * *);
	*(void **)(*REAL_cuGetErrorName) = dlsym(RTLD_NEXT, "cuGetErrorName");
	DriverAPICall call = std::bind(REAL_cuGetErrorName ,error,pStr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuInit( unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(2, &params));
	CUresult CUDAAPI (*REAL_cuInit)(unsigned int);
	*(void **)(*REAL_cuInit) = dlsym(RTLD_NEXT, "cuInit");
	DriverAPICall call = std::bind(REAL_cuInit ,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDriverGetVersion( int * driverVersion ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&driverVersion };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(3, &params));
	CUresult CUDAAPI (*REAL_cuDriverGetVersion)(int *);
	*(void **)(*REAL_cuDriverGetVersion) = dlsym(RTLD_NEXT, "cuDriverGetVersion");
	DriverAPICall call = std::bind(REAL_cuDriverGetVersion ,driverVersion);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceGet( CUdevice * device, int ordinal ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&device,(void *)&ordinal };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(4, &params));
	CUresult CUDAAPI (*REAL_cuDeviceGet)(CUdevice *,int);
	*(void **)(*REAL_cuDeviceGet) = dlsym(RTLD_NEXT, "cuDeviceGet");
	DriverAPICall call = std::bind(REAL_cuDeviceGet ,device,ordinal);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceGetCount( int * count ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&count };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(5, &params));
	CUresult CUDAAPI (*REAL_cuDeviceGetCount)(int *);
	*(void **)(*REAL_cuDeviceGetCount) = dlsym(RTLD_NEXT, "cuDeviceGetCount");
	DriverAPICall call = std::bind(REAL_cuDeviceGetCount ,count);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceGetName( char * name, int len, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&name,(void *)&len,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(6, &params));
	CUresult CUDAAPI (*REAL_cuDeviceGetName)(char *,int,CUdevice);
	*(void **)(*REAL_cuDeviceGetName) = dlsym(RTLD_NEXT, "cuDeviceGetName");
	DriverAPICall call = std::bind(REAL_cuDeviceGetName ,name,len,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceTotalMem( size_t * bytes, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&bytes,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(7, &params));
	CUresult CUDAAPI (*REAL_cuDeviceTotalMem)(size_t *,CUdevice);
	*(void **)(*REAL_cuDeviceTotalMem) = dlsym(RTLD_NEXT, "cuDeviceTotalMem");
	DriverAPICall call = std::bind(REAL_cuDeviceTotalMem ,bytes,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceGetAttribute( int * pi, CUdevice_attribute attrib, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pi,(void *)&attrib,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(8, &params));
	CUresult CUDAAPI (*REAL_cuDeviceGetAttribute)(int *,CUdevice_attribute,CUdevice);
	*(void **)(*REAL_cuDeviceGetAttribute) = dlsym(RTLD_NEXT, "cuDeviceGetAttribute");
	DriverAPICall call = std::bind(REAL_cuDeviceGetAttribute ,pi,attrib,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceGetProperties( CUdevprop * prop, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&prop,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(9, &params));
	CUresult CUDAAPI (*REAL_cuDeviceGetProperties)(CUdevprop *,CUdevice);
	*(void **)(*REAL_cuDeviceGetProperties) = dlsym(RTLD_NEXT, "cuDeviceGetProperties");
	DriverAPICall call = std::bind(REAL_cuDeviceGetProperties ,prop,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceComputeCapability( int * major, int * minor, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&major,(void *)&minor,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(10, &params));
	CUresult CUDAAPI (*REAL_cuDeviceComputeCapability)(int *,int *,CUdevice);
	*(void **)(*REAL_cuDeviceComputeCapability) = dlsym(RTLD_NEXT, "cuDeviceComputeCapability");
	DriverAPICall call = std::bind(REAL_cuDeviceComputeCapability ,major,minor,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDevicePrimaryCtxRetain( CUcontext * pctx, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pctx,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(11, &params));
	CUresult CUDAAPI (*REAL_cuDevicePrimaryCtxRetain)(CUcontext *,CUdevice);
	*(void **)(*REAL_cuDevicePrimaryCtxRetain) = dlsym(RTLD_NEXT, "cuDevicePrimaryCtxRetain");
	DriverAPICall call = std::bind(REAL_cuDevicePrimaryCtxRetain ,pctx,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDevicePrimaryCtxRelease( CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(12, &params));
	CUresult CUDAAPI (*REAL_cuDevicePrimaryCtxRelease)(CUdevice);
	*(void **)(*REAL_cuDevicePrimaryCtxRelease) = dlsym(RTLD_NEXT, "cuDevicePrimaryCtxRelease");
	DriverAPICall call = std::bind(REAL_cuDevicePrimaryCtxRelease ,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDevicePrimaryCtxSetFlags( CUdevice dev, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dev,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(13, &params));
	CUresult CUDAAPI (*REAL_cuDevicePrimaryCtxSetFlags)(CUdevice,unsigned int);
	*(void **)(*REAL_cuDevicePrimaryCtxSetFlags) = dlsym(RTLD_NEXT, "cuDevicePrimaryCtxSetFlags");
	DriverAPICall call = std::bind(REAL_cuDevicePrimaryCtxSetFlags ,dev,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDevicePrimaryCtxGetState( CUdevice dev, unsigned int * flags, int * active ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dev,(void *)&flags,(void *)&active };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(14, &params));
	CUresult CUDAAPI (*REAL_cuDevicePrimaryCtxGetState)(CUdevice,unsigned int *,int *);
	*(void **)(*REAL_cuDevicePrimaryCtxGetState) = dlsym(RTLD_NEXT, "cuDevicePrimaryCtxGetState");
	DriverAPICall call = std::bind(REAL_cuDevicePrimaryCtxGetState ,dev,flags,active);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDevicePrimaryCtxReset( CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(15, &params));
	CUresult CUDAAPI (*REAL_cuDevicePrimaryCtxReset)(CUdevice);
	*(void **)(*REAL_cuDevicePrimaryCtxReset) = dlsym(RTLD_NEXT, "cuDevicePrimaryCtxReset");
	DriverAPICall call = std::bind(REAL_cuDevicePrimaryCtxReset ,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxCreate( CUcontext * pctx, unsigned int flags, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pctx,(void *)&flags,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(16, &params));
	CUresult CUDAAPI (*REAL_cuCtxCreate)(CUcontext *,unsigned int,CUdevice);
	*(void **)(*REAL_cuCtxCreate) = dlsym(RTLD_NEXT, "cuCtxCreate");
	DriverAPICall call = std::bind(REAL_cuCtxCreate ,pctx,flags,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxDestroy( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&ctx };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(17, &params));
	CUresult CUDAAPI (*REAL_cuCtxDestroy)(CUcontext);
	*(void **)(*REAL_cuCtxDestroy) = dlsym(RTLD_NEXT, "cuCtxDestroy");
	DriverAPICall call = std::bind(REAL_cuCtxDestroy ,ctx);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxPushCurrent( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&ctx };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(18, &params));
	CUresult CUDAAPI (*REAL_cuCtxPushCurrent)(CUcontext);
	*(void **)(*REAL_cuCtxPushCurrent) = dlsym(RTLD_NEXT, "cuCtxPushCurrent");
	DriverAPICall call = std::bind(REAL_cuCtxPushCurrent ,ctx);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxPopCurrent( CUcontext * pctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pctx };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(19, &params));
	CUresult CUDAAPI (*REAL_cuCtxPopCurrent)(CUcontext *);
	*(void **)(*REAL_cuCtxPopCurrent) = dlsym(RTLD_NEXT, "cuCtxPopCurrent");
	DriverAPICall call = std::bind(REAL_cuCtxPopCurrent ,pctx);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxSetCurrent( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&ctx };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(20, &params));
	CUresult CUDAAPI (*REAL_cuCtxSetCurrent)(CUcontext);
	*(void **)(*REAL_cuCtxSetCurrent) = dlsym(RTLD_NEXT, "cuCtxSetCurrent");
	DriverAPICall call = std::bind(REAL_cuCtxSetCurrent ,ctx);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetCurrent( CUcontext * pctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pctx };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(21, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetCurrent)(CUcontext *);
	*(void **)(*REAL_cuCtxGetCurrent) = dlsym(RTLD_NEXT, "cuCtxGetCurrent");
	DriverAPICall call = std::bind(REAL_cuCtxGetCurrent ,pctx);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetDevice( CUdevice * device ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&device };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(22, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetDevice)(CUdevice *);
	*(void **)(*REAL_cuCtxGetDevice) = dlsym(RTLD_NEXT, "cuCtxGetDevice");
	DriverAPICall call = std::bind(REAL_cuCtxGetDevice ,device);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetFlags( unsigned int * flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(23, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetFlags)(unsigned int *);
	*(void **)(*REAL_cuCtxGetFlags) = dlsym(RTLD_NEXT, "cuCtxGetFlags");
	DriverAPICall call = std::bind(REAL_cuCtxGetFlags ,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxSynchronize( void  ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = {  };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(24, &params));
	CUresult CUDAAPI (*REAL_cuCtxSynchronize)(void);
	*(void **)(*REAL_cuCtxSynchronize) = dlsym(RTLD_NEXT, "cuCtxSynchronize");
	DriverAPICall call = std::bind(REAL_cuCtxSynchronize );
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxSetLimit( CUlimit limit, size_t value ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&limit,(void *)&value };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(25, &params));
	CUresult CUDAAPI (*REAL_cuCtxSetLimit)(CUlimit,size_t);
	*(void **)(*REAL_cuCtxSetLimit) = dlsym(RTLD_NEXT, "cuCtxSetLimit");
	DriverAPICall call = std::bind(REAL_cuCtxSetLimit ,limit,value);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetLimit( size_t * pvalue, CUlimit limit ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pvalue,(void *)&limit };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(26, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetLimit)(size_t *,CUlimit);
	*(void **)(*REAL_cuCtxGetLimit) = dlsym(RTLD_NEXT, "cuCtxGetLimit");
	DriverAPICall call = std::bind(REAL_cuCtxGetLimit ,pvalue,limit);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetCacheConfig( CUfunc_cache * pconfig ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pconfig };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(27, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetCacheConfig)(CUfunc_cache *);
	*(void **)(*REAL_cuCtxGetCacheConfig) = dlsym(RTLD_NEXT, "cuCtxGetCacheConfig");
	DriverAPICall call = std::bind(REAL_cuCtxGetCacheConfig ,pconfig);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxSetCacheConfig( CUfunc_cache config ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&config };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(28, &params));
	CUresult CUDAAPI (*REAL_cuCtxSetCacheConfig)(CUfunc_cache);
	*(void **)(*REAL_cuCtxSetCacheConfig) = dlsym(RTLD_NEXT, "cuCtxSetCacheConfig");
	DriverAPICall call = std::bind(REAL_cuCtxSetCacheConfig ,config);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetSharedMemConfig( CUsharedconfig * pConfig ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pConfig };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(29, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetSharedMemConfig)(CUsharedconfig *);
	*(void **)(*REAL_cuCtxGetSharedMemConfig) = dlsym(RTLD_NEXT, "cuCtxGetSharedMemConfig");
	DriverAPICall call = std::bind(REAL_cuCtxGetSharedMemConfig ,pConfig);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxSetSharedMemConfig( CUsharedconfig config ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&config };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(30, &params));
	CUresult CUDAAPI (*REAL_cuCtxSetSharedMemConfig)(CUsharedconfig);
	*(void **)(*REAL_cuCtxSetSharedMemConfig) = dlsym(RTLD_NEXT, "cuCtxSetSharedMemConfig");
	DriverAPICall call = std::bind(REAL_cuCtxSetSharedMemConfig ,config);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetApiVersion( CUcontext ctx, unsigned int * version ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&ctx,(void *)&version };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(31, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetApiVersion)(CUcontext,unsigned int *);
	*(void **)(*REAL_cuCtxGetApiVersion) = dlsym(RTLD_NEXT, "cuCtxGetApiVersion");
	DriverAPICall call = std::bind(REAL_cuCtxGetApiVersion ,ctx,version);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxGetStreamPriorityRange( int * leastPriority, int * greatestPriority ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&leastPriority,(void *)&greatestPriority };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(32, &params));
	CUresult CUDAAPI (*REAL_cuCtxGetStreamPriorityRange)(int *,int *);
	*(void **)(*REAL_cuCtxGetStreamPriorityRange) = dlsym(RTLD_NEXT, "cuCtxGetStreamPriorityRange");
	DriverAPICall call = std::bind(REAL_cuCtxGetStreamPriorityRange ,leastPriority,greatestPriority);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxAttach( CUcontext * pctx, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pctx,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(33, &params));
	CUresult CUDAAPI (*REAL_cuCtxAttach)(CUcontext *,unsigned int);
	*(void **)(*REAL_cuCtxAttach) = dlsym(RTLD_NEXT, "cuCtxAttach");
	DriverAPICall call = std::bind(REAL_cuCtxAttach ,pctx,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxDetach( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&ctx };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(34, &params));
	CUresult CUDAAPI (*REAL_cuCtxDetach)(CUcontext);
	*(void **)(*REAL_cuCtxDetach) = dlsym(RTLD_NEXT, "cuCtxDetach");
	DriverAPICall call = std::bind(REAL_cuCtxDetach ,ctx);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleLoad( CUmodule * module, const char * fname ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&module,(void *)&fname };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(35, &params));
	CUresult CUDAAPI (*REAL_cuModuleLoad)(CUmodule *,const char *);
	*(void **)(*REAL_cuModuleLoad) = dlsym(RTLD_NEXT, "cuModuleLoad");
	DriverAPICall call = std::bind(REAL_cuModuleLoad ,module,fname);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleLoadData( CUmodule * module, const void * image ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&module,(void *)&image };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(36, &params));
	CUresult CUDAAPI (*REAL_cuModuleLoadData)(CUmodule *,const void *);
	*(void **)(*REAL_cuModuleLoadData) = dlsym(RTLD_NEXT, "cuModuleLoadData");
	DriverAPICall call = std::bind(REAL_cuModuleLoadData ,module,image);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleLoadDataEx( CUmodule * module, const void * image, unsigned int numOptions, CUjit_option * options, void * * optionValues ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&module,(void *)&image,(void *)&numOptions,(void *)&options,(void *)&optionValues };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(37, &params));
	CUresult CUDAAPI (*REAL_cuModuleLoadDataEx)(CUmodule *,const void *,unsigned int,CUjit_option *,void * *);
	*(void **)(*REAL_cuModuleLoadDataEx) = dlsym(RTLD_NEXT, "cuModuleLoadDataEx");
	DriverAPICall call = std::bind(REAL_cuModuleLoadDataEx ,module,image,numOptions,options,optionValues);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleLoadFatBinary( CUmodule * module, const void * fatCubin ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&module,(void *)&fatCubin };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(38, &params));
	CUresult CUDAAPI (*REAL_cuModuleLoadFatBinary)(CUmodule *,const void *);
	*(void **)(*REAL_cuModuleLoadFatBinary) = dlsym(RTLD_NEXT, "cuModuleLoadFatBinary");
	DriverAPICall call = std::bind(REAL_cuModuleLoadFatBinary ,module,fatCubin);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleUnload( CUmodule hmod ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hmod };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(39, &params));
	CUresult CUDAAPI (*REAL_cuModuleUnload)(CUmodule);
	*(void **)(*REAL_cuModuleUnload) = dlsym(RTLD_NEXT, "cuModuleUnload");
	DriverAPICall call = std::bind(REAL_cuModuleUnload ,hmod);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleGetFunction( CUfunction * hfunc, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&hmod,(void *)&name };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(40, &params));
	CUresult CUDAAPI (*REAL_cuModuleGetFunction)(CUfunction *,CUmodule,const char *);
	*(void **)(*REAL_cuModuleGetFunction) = dlsym(RTLD_NEXT, "cuModuleGetFunction");
	DriverAPICall call = std::bind(REAL_cuModuleGetFunction ,hfunc,hmod,name);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleGetGlobal( CUdeviceptr * dptr, size_t * bytes, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dptr,(void *)&bytes,(void *)&hmod,(void *)&name };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(41, &params));
	CUresult CUDAAPI (*REAL_cuModuleGetGlobal)(CUdeviceptr *,size_t *,CUmodule,const char *);
	*(void **)(*REAL_cuModuleGetGlobal) = dlsym(RTLD_NEXT, "cuModuleGetGlobal");
	DriverAPICall call = std::bind(REAL_cuModuleGetGlobal ,dptr,bytes,hmod,name);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleGetTexRef( CUtexref * pTexRef, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pTexRef,(void *)&hmod,(void *)&name };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(42, &params));
	CUresult CUDAAPI (*REAL_cuModuleGetTexRef)(CUtexref *,CUmodule,const char *);
	*(void **)(*REAL_cuModuleGetTexRef) = dlsym(RTLD_NEXT, "cuModuleGetTexRef");
	DriverAPICall call = std::bind(REAL_cuModuleGetTexRef ,pTexRef,hmod,name);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuModuleGetSurfRef( CUsurfref * pSurfRef, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pSurfRef,(void *)&hmod,(void *)&name };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(43, &params));
	CUresult CUDAAPI (*REAL_cuModuleGetSurfRef)(CUsurfref *,CUmodule,const char *);
	*(void **)(*REAL_cuModuleGetSurfRef) = dlsym(RTLD_NEXT, "cuModuleGetSurfRef");
	DriverAPICall call = std::bind(REAL_cuModuleGetSurfRef ,pSurfRef,hmod,name);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLinkCreate( unsigned int numOptions, CUjit_option * options, void * * optionValues, CUlinkState * stateOut ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&numOptions,(void *)&options,(void *)&optionValues,(void *)&stateOut };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(44, &params));
	CUresult CUDAAPI (*REAL_cuLinkCreate)(unsigned int,CUjit_option *,void * *,CUlinkState *);
	*(void **)(*REAL_cuLinkCreate) = dlsym(RTLD_NEXT, "cuLinkCreate");
	DriverAPICall call = std::bind(REAL_cuLinkCreate ,numOptions,options,optionValues,stateOut);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLinkAddData( CUlinkState state, CUjitInputType type, void * data, size_t size, const char * name, unsigned int numOptions, CUjit_option * options, void * * optionValues ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&state,(void *)&type,(void *)&data,(void *)&size,(void *)&name,(void *)&numOptions,(void *)&options,(void *)&optionValues };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(45, &params));
	CUresult CUDAAPI (*REAL_cuLinkAddData)(CUlinkState,CUjitInputType,void *,size_t,const char *,unsigned int,CUjit_option *,void * *);
	*(void **)(*REAL_cuLinkAddData) = dlsym(RTLD_NEXT, "cuLinkAddData");
	DriverAPICall call = std::bind(REAL_cuLinkAddData ,state,type,data,size,name,numOptions,options,optionValues);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLinkAddFile( CUlinkState state, CUjitInputType type, const char * path, unsigned int numOptions, CUjit_option * options, void * * optionValues ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&state,(void *)&type,(void *)&path,(void *)&numOptions,(void *)&options,(void *)&optionValues };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(46, &params));
	CUresult CUDAAPI (*REAL_cuLinkAddFile)(CUlinkState,CUjitInputType,const char *,unsigned int,CUjit_option *,void * *);
	*(void **)(*REAL_cuLinkAddFile) = dlsym(RTLD_NEXT, "cuLinkAddFile");
	DriverAPICall call = std::bind(REAL_cuLinkAddFile ,state,type,path,numOptions,options,optionValues);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLinkComplete( CUlinkState state, void * * cubinOut, size_t * sizeOut ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&state,(void *)&cubinOut,(void *)&sizeOut };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(47, &params));
	CUresult CUDAAPI (*REAL_cuLinkComplete)(CUlinkState,void * *,size_t *);
	*(void **)(*REAL_cuLinkComplete) = dlsym(RTLD_NEXT, "cuLinkComplete");
	DriverAPICall call = std::bind(REAL_cuLinkComplete ,state,cubinOut,sizeOut);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLinkDestroy( CUlinkState state ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&state };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(48, &params));
	CUresult CUDAAPI (*REAL_cuLinkDestroy)(CUlinkState);
	*(void **)(*REAL_cuLinkDestroy) = dlsym(RTLD_NEXT, "cuLinkDestroy");
	DriverAPICall call = std::bind(REAL_cuLinkDestroy ,state);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemGetInfo( size_t * free, size_t * total ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&free,(void *)&total };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(49, &params));
	CUresult CUDAAPI (*REAL_cuMemGetInfo)(size_t *,size_t *);
	*(void **)(*REAL_cuMemGetInfo) = dlsym(RTLD_NEXT, "cuMemGetInfo");
	DriverAPICall call = std::bind(REAL_cuMemGetInfo ,free,total);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemAlloc( CUdeviceptr * dptr, size_t bytesize ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dptr,(void *)&bytesize };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(50, &params));
	CUresult CUDAAPI (*REAL_cuMemAlloc)(CUdeviceptr *,size_t);
	*(void **)(*REAL_cuMemAlloc) = dlsym(RTLD_NEXT, "cuMemAlloc");
	DriverAPICall call = std::bind(REAL_cuMemAlloc ,dptr,bytesize);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemAllocPitch( CUdeviceptr * dptr, size_t * pPitch, size_t WidthInBytes, size_t Height, unsigned int ElementSizeBytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dptr,(void *)&pPitch,(void *)&WidthInBytes,(void *)&Height,(void *)&ElementSizeBytes };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(51, &params));
	CUresult CUDAAPI (*REAL_cuMemAllocPitch)(CUdeviceptr *,size_t *,size_t,size_t,unsigned int);
	*(void **)(*REAL_cuMemAllocPitch) = dlsym(RTLD_NEXT, "cuMemAllocPitch");
	DriverAPICall call = std::bind(REAL_cuMemAllocPitch ,dptr,pPitch,WidthInBytes,Height,ElementSizeBytes);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemFree( CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dptr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(52, &params));
	CUresult CUDAAPI (*REAL_cuMemFree)(CUdeviceptr);
	*(void **)(*REAL_cuMemFree) = dlsym(RTLD_NEXT, "cuMemFree");
	DriverAPICall call = std::bind(REAL_cuMemFree ,dptr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemGetAddressRange( CUdeviceptr * pbase, size_t * psize, CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pbase,(void *)&psize,(void *)&dptr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(53, &params));
	CUresult CUDAAPI (*REAL_cuMemGetAddressRange)(CUdeviceptr *,size_t *,CUdeviceptr);
	*(void **)(*REAL_cuMemGetAddressRange) = dlsym(RTLD_NEXT, "cuMemGetAddressRange");
	DriverAPICall call = std::bind(REAL_cuMemGetAddressRange ,pbase,psize,dptr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemAllocHost( void * * pp, size_t bytesize ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pp,(void *)&bytesize };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(54, &params));
	CUresult CUDAAPI (*REAL_cuMemAllocHost)(void * *,size_t);
	*(void **)(*REAL_cuMemAllocHost) = dlsym(RTLD_NEXT, "cuMemAllocHost");
	DriverAPICall call = std::bind(REAL_cuMemAllocHost ,pp,bytesize);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemFreeHost( void * p ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&p };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(55, &params));
	CUresult CUDAAPI (*REAL_cuMemFreeHost)(void *);
	*(void **)(*REAL_cuMemFreeHost) = dlsym(RTLD_NEXT, "cuMemFreeHost");
	DriverAPICall call = std::bind(REAL_cuMemFreeHost ,p);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemHostAlloc( void * * pp, size_t bytesize, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pp,(void *)&bytesize,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(56, &params));
	CUresult CUDAAPI (*REAL_cuMemHostAlloc)(void * *,size_t,unsigned int);
	*(void **)(*REAL_cuMemHostAlloc) = dlsym(RTLD_NEXT, "cuMemHostAlloc");
	DriverAPICall call = std::bind(REAL_cuMemHostAlloc ,pp,bytesize,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemHostGetDevicePointer( CUdeviceptr * pdptr, void * p, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pdptr,(void *)&p,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(57, &params));
	CUresult CUDAAPI (*REAL_cuMemHostGetDevicePointer)(CUdeviceptr *,void *,unsigned int);
	*(void **)(*REAL_cuMemHostGetDevicePointer) = dlsym(RTLD_NEXT, "cuMemHostGetDevicePointer");
	DriverAPICall call = std::bind(REAL_cuMemHostGetDevicePointer ,pdptr,p,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemHostGetFlags( unsigned int * pFlags, void * p ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pFlags,(void *)&p };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(58, &params));
	CUresult CUDAAPI (*REAL_cuMemHostGetFlags)(unsigned int *,void *);
	*(void **)(*REAL_cuMemHostGetFlags) = dlsym(RTLD_NEXT, "cuMemHostGetFlags");
	DriverAPICall call = std::bind(REAL_cuMemHostGetFlags ,pFlags,p);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemAllocManaged( CUdeviceptr * dptr, size_t bytesize, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dptr,(void *)&bytesize,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(59, &params));
	CUresult CUDAAPI (*REAL_cuMemAllocManaged)(CUdeviceptr *,size_t,unsigned int);
	*(void **)(*REAL_cuMemAllocManaged) = dlsym(RTLD_NEXT, "cuMemAllocManaged");
	DriverAPICall call = std::bind(REAL_cuMemAllocManaged ,dptr,bytesize,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceGetByPCIBusId( CUdevice * dev, const char * pciBusId ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dev,(void *)&pciBusId };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(60, &params));
	CUresult CUDAAPI (*REAL_cuDeviceGetByPCIBusId)(CUdevice *,const char *);
	*(void **)(*REAL_cuDeviceGetByPCIBusId) = dlsym(RTLD_NEXT, "cuDeviceGetByPCIBusId");
	DriverAPICall call = std::bind(REAL_cuDeviceGetByPCIBusId ,dev,pciBusId);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceGetPCIBusId( char * pciBusId, int len, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pciBusId,(void *)&len,(void *)&dev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(61, &params));
	CUresult CUDAAPI (*REAL_cuDeviceGetPCIBusId)(char *,int,CUdevice);
	*(void **)(*REAL_cuDeviceGetPCIBusId) = dlsym(RTLD_NEXT, "cuDeviceGetPCIBusId");
	DriverAPICall call = std::bind(REAL_cuDeviceGetPCIBusId ,pciBusId,len,dev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuIpcGetEventHandle( CUipcEventHandle * pHandle, CUevent event ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pHandle,(void *)&event };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(62, &params));
	CUresult CUDAAPI (*REAL_cuIpcGetEventHandle)(CUipcEventHandle *,CUevent);
	*(void **)(*REAL_cuIpcGetEventHandle) = dlsym(RTLD_NEXT, "cuIpcGetEventHandle");
	DriverAPICall call = std::bind(REAL_cuIpcGetEventHandle ,pHandle,event);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuIpcOpenEventHandle( CUevent * phEvent, CUipcEventHandle handle ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&phEvent,(void *)&handle };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(63, &params));
	CUresult CUDAAPI (*REAL_cuIpcOpenEventHandle)(CUevent *,CUipcEventHandle);
	*(void **)(*REAL_cuIpcOpenEventHandle) = dlsym(RTLD_NEXT, "cuIpcOpenEventHandle");
	DriverAPICall call = std::bind(REAL_cuIpcOpenEventHandle ,phEvent,handle);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuIpcGetMemHandle( CUipcMemHandle * pHandle, CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pHandle,(void *)&dptr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(64, &params));
	CUresult CUDAAPI (*REAL_cuIpcGetMemHandle)(CUipcMemHandle *,CUdeviceptr);
	*(void **)(*REAL_cuIpcGetMemHandle) = dlsym(RTLD_NEXT, "cuIpcGetMemHandle");
	DriverAPICall call = std::bind(REAL_cuIpcGetMemHandle ,pHandle,dptr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuIpcOpenMemHandle( CUdeviceptr * pdptr, CUipcMemHandle handle, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pdptr,(void *)&handle,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(65, &params));
	CUresult CUDAAPI (*REAL_cuIpcOpenMemHandle)(CUdeviceptr *,CUipcMemHandle,unsigned int);
	*(void **)(*REAL_cuIpcOpenMemHandle) = dlsym(RTLD_NEXT, "cuIpcOpenMemHandle");
	DriverAPICall call = std::bind(REAL_cuIpcOpenMemHandle ,pdptr,handle,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuIpcCloseMemHandle( CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dptr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(66, &params));
	CUresult CUDAAPI (*REAL_cuIpcCloseMemHandle)(CUdeviceptr);
	*(void **)(*REAL_cuIpcCloseMemHandle) = dlsym(RTLD_NEXT, "cuIpcCloseMemHandle");
	DriverAPICall call = std::bind(REAL_cuIpcCloseMemHandle ,dptr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemHostRegister( void * p, size_t bytesize, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&p,(void *)&bytesize,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(67, &params));
	CUresult CUDAAPI (*REAL_cuMemHostRegister)(void *,size_t,unsigned int);
	*(void **)(*REAL_cuMemHostRegister) = dlsym(RTLD_NEXT, "cuMemHostRegister");
	DriverAPICall call = std::bind(REAL_cuMemHostRegister ,p,bytesize,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemHostUnregister( void * p ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&p };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(68, &params));
	CUresult CUDAAPI (*REAL_cuMemHostUnregister)(void *);
	*(void **)(*REAL_cuMemHostUnregister) = dlsym(RTLD_NEXT, "cuMemHostUnregister");
	DriverAPICall call = std::bind(REAL_cuMemHostUnregister ,p);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dst,(void *)&src,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(69, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy)(CUdeviceptr,CUdeviceptr,size_t);
	*(void **)(*REAL_cuMemcpy) = dlsym(RTLD_NEXT, "cuMemcpy");
	DriverAPICall call = std::bind(REAL_cuMemcpy ,dst,src,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyPeer( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstContext,(void *)&srcDevice,(void *)&srcContext,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(70, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyPeer)(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t);
	*(void **)(*REAL_cuMemcpyPeer) = dlsym(RTLD_NEXT, "cuMemcpyPeer");
	DriverAPICall call = std::bind(REAL_cuMemcpyPeer ,dstDevice,dstContext,srcDevice,srcContext,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoD( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcHost,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(71, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoD)(CUdeviceptr,const void *,size_t);
	*(void **)(*REAL_cuMemcpyHtoD) = dlsym(RTLD_NEXT, "cuMemcpyHtoD");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoD ,dstDevice,srcHost,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoH( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcDevice,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(72, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoH)(void *,CUdeviceptr,size_t);
	*(void **)(*REAL_cuMemcpyDtoH) = dlsym(RTLD_NEXT, "cuMemcpyDtoH");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoH ,dstHost,srcDevice,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoD( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcDevice,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(73, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoD)(CUdeviceptr,CUdeviceptr,size_t);
	*(void **)(*REAL_cuMemcpyDtoD) = dlsym(RTLD_NEXT, "cuMemcpyDtoD");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoD ,dstDevice,srcDevice,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoA( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcDevice,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(74, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoA)(CUarray,size_t,CUdeviceptr,size_t);
	*(void **)(*REAL_cuMemcpyDtoA) = dlsym(RTLD_NEXT, "cuMemcpyDtoA");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoA ,dstArray,dstOffset,srcDevice,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoD( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(75, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoD)(CUdeviceptr,CUarray,size_t,size_t);
	*(void **)(*REAL_cuMemcpyAtoD) = dlsym(RTLD_NEXT, "cuMemcpyAtoD");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoD ,dstDevice,srcArray,srcOffset,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoA( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcHost,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(76, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoA)(CUarray,size_t,const void *,size_t);
	*(void **)(*REAL_cuMemcpyHtoA) = dlsym(RTLD_NEXT, "cuMemcpyHtoA");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoA ,dstArray,dstOffset,srcHost,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoH( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(77, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoH)(void *,CUarray,size_t,size_t);
	*(void **)(*REAL_cuMemcpyAtoH) = dlsym(RTLD_NEXT, "cuMemcpyAtoH");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoH ,dstHost,srcArray,srcOffset,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoA( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(78, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoA)(CUarray,size_t,CUarray,size_t,size_t);
	*(void **)(*REAL_cuMemcpyAtoA) = dlsym(RTLD_NEXT, "cuMemcpyAtoA");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoA ,dstArray,dstOffset,srcArray,srcOffset,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy2D( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(79, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy2D)(const CUDA_MEMCPY2D *);
	*(void **)(*REAL_cuMemcpy2D) = dlsym(RTLD_NEXT, "cuMemcpy2D");
	DriverAPICall call = std::bind(REAL_cuMemcpy2D ,pCopy);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy2DUnaligned( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(80, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy2DUnaligned)(const CUDA_MEMCPY2D *);
	*(void **)(*REAL_cuMemcpy2DUnaligned) = dlsym(RTLD_NEXT, "cuMemcpy2DUnaligned");
	DriverAPICall call = std::bind(REAL_cuMemcpy2DUnaligned ,pCopy);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy3D( const CUDA_MEMCPY3D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(81, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy3D)(const CUDA_MEMCPY3D *);
	*(void **)(*REAL_cuMemcpy3D) = dlsym(RTLD_NEXT, "cuMemcpy3D");
	DriverAPICall call = std::bind(REAL_cuMemcpy3D ,pCopy);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy3DPeer( const CUDA_MEMCPY3D_PEER * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(82, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy3DPeer)(const CUDA_MEMCPY3D_PEER *);
	*(void **)(*REAL_cuMemcpy3DPeer) = dlsym(RTLD_NEXT, "cuMemcpy3DPeer");
	DriverAPICall call = std::bind(REAL_cuMemcpy3DPeer ,pCopy);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAsync( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dst,(void *)&src,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(83, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAsync)(CUdeviceptr,CUdeviceptr,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyAsync) = dlsym(RTLD_NEXT, "cuMemcpyAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpyAsync ,dst,src,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyPeerAsync( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstContext,(void *)&srcDevice,(void *)&srcContext,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(84, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyPeerAsync)(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyPeerAsync) = dlsym(RTLD_NEXT, "cuMemcpyPeerAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpyPeerAsync ,dstDevice,dstContext,srcDevice,srcContext,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoDAsync( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcHost,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(85, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoDAsync)(CUdeviceptr,const void *,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyHtoDAsync) = dlsym(RTLD_NEXT, "cuMemcpyHtoDAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoDAsync ,dstDevice,srcHost,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoHAsync( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcDevice,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(86, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoHAsync)(void *,CUdeviceptr,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyDtoHAsync) = dlsym(RTLD_NEXT, "cuMemcpyDtoHAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoHAsync ,dstHost,srcDevice,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoDAsync( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcDevice,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(87, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoDAsync)(CUdeviceptr,CUdeviceptr,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyDtoDAsync) = dlsym(RTLD_NEXT, "cuMemcpyDtoDAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoDAsync ,dstDevice,srcDevice,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoAAsync( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcHost,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(88, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoAAsync)(CUarray,size_t,const void *,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyHtoAAsync) = dlsym(RTLD_NEXT, "cuMemcpyHtoAAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoAAsync ,dstArray,dstOffset,srcHost,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoHAsync( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(89, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoHAsync)(void *,CUarray,size_t,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyAtoHAsync) = dlsym(RTLD_NEXT, "cuMemcpyAtoHAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoHAsync ,dstHost,srcArray,srcOffset,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy2DAsync( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(90, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy2DAsync)(const CUDA_MEMCPY2D *,CUstream);
	*(void **)(*REAL_cuMemcpy2DAsync) = dlsym(RTLD_NEXT, "cuMemcpy2DAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpy2DAsync ,pCopy,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy3DAsync( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(91, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy3DAsync)(const CUDA_MEMCPY3D *,CUstream);
	*(void **)(*REAL_cuMemcpy3DAsync) = dlsym(RTLD_NEXT, "cuMemcpy3DAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpy3DAsync ,pCopy,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy3DPeerAsync( const CUDA_MEMCPY3D_PEER * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(92, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy3DPeerAsync)(const CUDA_MEMCPY3D_PEER *,CUstream);
	*(void **)(*REAL_cuMemcpy3DPeerAsync) = dlsym(RTLD_NEXT, "cuMemcpy3DPeerAsync");
	DriverAPICall call = std::bind(REAL_cuMemcpy3DPeerAsync ,pCopy,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD8( CUdeviceptr dstDevice, unsigned char uc, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&uc,(void *)&N };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(93, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD8)(CUdeviceptr,unsigned char,size_t);
	*(void **)(*REAL_cuMemsetD8) = dlsym(RTLD_NEXT, "cuMemsetD8");
	DriverAPICall call = std::bind(REAL_cuMemsetD8 ,dstDevice,uc,N);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD16( CUdeviceptr dstDevice, unsigned short us, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&us,(void *)&N };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(94, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD16)(CUdeviceptr,unsigned short,size_t);
	*(void **)(*REAL_cuMemsetD16) = dlsym(RTLD_NEXT, "cuMemsetD16");
	DriverAPICall call = std::bind(REAL_cuMemsetD16 ,dstDevice,us,N);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD32( CUdeviceptr dstDevice, unsigned int ui, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&ui,(void *)&N };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(95, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD32)(CUdeviceptr,unsigned int,size_t);
	*(void **)(*REAL_cuMemsetD32) = dlsym(RTLD_NEXT, "cuMemsetD32");
	DriverAPICall call = std::bind(REAL_cuMemsetD32 ,dstDevice,ui,N);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D8( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&uc,(void *)&Width,(void *)&Height };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(96, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D8)(CUdeviceptr,size_t,unsigned char,size_t,size_t);
	*(void **)(*REAL_cuMemsetD2D8) = dlsym(RTLD_NEXT, "cuMemsetD2D8");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D8 ,dstDevice,dstPitch,uc,Width,Height);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D16( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&us,(void *)&Width,(void *)&Height };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(97, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D16)(CUdeviceptr,size_t,unsigned short,size_t,size_t);
	*(void **)(*REAL_cuMemsetD2D16) = dlsym(RTLD_NEXT, "cuMemsetD2D16");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D16 ,dstDevice,dstPitch,us,Width,Height);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D32( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&ui,(void *)&Width,(void *)&Height };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(98, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D32)(CUdeviceptr,size_t,unsigned int,size_t,size_t);
	*(void **)(*REAL_cuMemsetD2D32) = dlsym(RTLD_NEXT, "cuMemsetD2D32");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D32 ,dstDevice,dstPitch,ui,Width,Height);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD8Async( CUdeviceptr dstDevice, unsigned char uc, size_t N, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&uc,(void *)&N,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(99, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD8Async)(CUdeviceptr,unsigned char,size_t,CUstream);
	*(void **)(*REAL_cuMemsetD8Async) = dlsym(RTLD_NEXT, "cuMemsetD8Async");
	DriverAPICall call = std::bind(REAL_cuMemsetD8Async ,dstDevice,uc,N,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD16Async( CUdeviceptr dstDevice, unsigned short us, size_t N, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&us,(void *)&N,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(100, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD16Async)(CUdeviceptr,unsigned short,size_t,CUstream);
	*(void **)(*REAL_cuMemsetD16Async) = dlsym(RTLD_NEXT, "cuMemsetD16Async");
	DriverAPICall call = std::bind(REAL_cuMemsetD16Async ,dstDevice,us,N,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD32Async( CUdeviceptr dstDevice, unsigned int ui, size_t N, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&ui,(void *)&N,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(101, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD32Async)(CUdeviceptr,unsigned int,size_t,CUstream);
	*(void **)(*REAL_cuMemsetD32Async) = dlsym(RTLD_NEXT, "cuMemsetD32Async");
	DriverAPICall call = std::bind(REAL_cuMemsetD32Async ,dstDevice,ui,N,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D8Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&uc,(void *)&Width,(void *)&Height,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(102, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D8Async)(CUdeviceptr,size_t,unsigned char,size_t,size_t,CUstream);
	*(void **)(*REAL_cuMemsetD2D8Async) = dlsym(RTLD_NEXT, "cuMemsetD2D8Async");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D8Async ,dstDevice,dstPitch,uc,Width,Height,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D16Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&us,(void *)&Width,(void *)&Height,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(103, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D16Async)(CUdeviceptr,size_t,unsigned short,size_t,size_t,CUstream);
	*(void **)(*REAL_cuMemsetD2D16Async) = dlsym(RTLD_NEXT, "cuMemsetD2D16Async");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D16Async ,dstDevice,dstPitch,us,Width,Height,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D32Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&ui,(void *)&Width,(void *)&Height,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(104, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D32Async)(CUdeviceptr,size_t,unsigned int,size_t,size_t,CUstream);
	*(void **)(*REAL_cuMemsetD2D32Async) = dlsym(RTLD_NEXT, "cuMemsetD2D32Async");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D32Async ,dstDevice,dstPitch,ui,Width,Height,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuArrayCreate( CUarray * pHandle, const CUDA_ARRAY_DESCRIPTOR * pAllocateArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pHandle,(void *)&pAllocateArray };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(105, &params));
	CUresult CUDAAPI (*REAL_cuArrayCreate)(CUarray *,const CUDA_ARRAY_DESCRIPTOR *);
	*(void **)(*REAL_cuArrayCreate) = dlsym(RTLD_NEXT, "cuArrayCreate");
	DriverAPICall call = std::bind(REAL_cuArrayCreate ,pHandle,pAllocateArray);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuArrayGetDescriptor( CUDA_ARRAY_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pArrayDescriptor,(void *)&hArray };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(106, &params));
	CUresult CUDAAPI (*REAL_cuArrayGetDescriptor)(CUDA_ARRAY_DESCRIPTOR *,CUarray);
	*(void **)(*REAL_cuArrayGetDescriptor) = dlsym(RTLD_NEXT, "cuArrayGetDescriptor");
	DriverAPICall call = std::bind(REAL_cuArrayGetDescriptor ,pArrayDescriptor,hArray);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuArrayDestroy( CUarray hArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hArray };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(107, &params));
	CUresult CUDAAPI (*REAL_cuArrayDestroy)(CUarray);
	*(void **)(*REAL_cuArrayDestroy) = dlsym(RTLD_NEXT, "cuArrayDestroy");
	DriverAPICall call = std::bind(REAL_cuArrayDestroy ,hArray);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuArray3DCreate( CUarray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pAllocateArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pHandle,(void *)&pAllocateArray };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(108, &params));
	CUresult CUDAAPI (*REAL_cuArray3DCreate)(CUarray *,const CUDA_ARRAY3D_DESCRIPTOR *);
	*(void **)(*REAL_cuArray3DCreate) = dlsym(RTLD_NEXT, "cuArray3DCreate");
	DriverAPICall call = std::bind(REAL_cuArray3DCreate ,pHandle,pAllocateArray);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuArray3DGetDescriptor( CUDA_ARRAY3D_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pArrayDescriptor,(void *)&hArray };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(109, &params));
	CUresult CUDAAPI (*REAL_cuArray3DGetDescriptor)(CUDA_ARRAY3D_DESCRIPTOR *,CUarray);
	*(void **)(*REAL_cuArray3DGetDescriptor) = dlsym(RTLD_NEXT, "cuArray3DGetDescriptor");
	DriverAPICall call = std::bind(REAL_cuArray3DGetDescriptor ,pArrayDescriptor,hArray);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMipmappedArrayCreate( CUmipmappedArray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pMipmappedArrayDesc, unsigned int numMipmapLevels ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pHandle,(void *)&pMipmappedArrayDesc,(void *)&numMipmapLevels };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(110, &params));
	CUresult CUDAAPI (*REAL_cuMipmappedArrayCreate)(CUmipmappedArray *,const CUDA_ARRAY3D_DESCRIPTOR *,unsigned int);
	*(void **)(*REAL_cuMipmappedArrayCreate) = dlsym(RTLD_NEXT, "cuMipmappedArrayCreate");
	DriverAPICall call = std::bind(REAL_cuMipmappedArrayCreate ,pHandle,pMipmappedArrayDesc,numMipmapLevels);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMipmappedArrayGetLevel( CUarray * pLevelArray, CUmipmappedArray hMipmappedArray, unsigned int level ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pLevelArray,(void *)&hMipmappedArray,(void *)&level };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(111, &params));
	CUresult CUDAAPI (*REAL_cuMipmappedArrayGetLevel)(CUarray *,CUmipmappedArray,unsigned int);
	*(void **)(*REAL_cuMipmappedArrayGetLevel) = dlsym(RTLD_NEXT, "cuMipmappedArrayGetLevel");
	DriverAPICall call = std::bind(REAL_cuMipmappedArrayGetLevel ,pLevelArray,hMipmappedArray,level);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMipmappedArrayDestroy( CUmipmappedArray hMipmappedArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hMipmappedArray };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(112, &params));
	CUresult CUDAAPI (*REAL_cuMipmappedArrayDestroy)(CUmipmappedArray);
	*(void **)(*REAL_cuMipmappedArrayDestroy) = dlsym(RTLD_NEXT, "cuMipmappedArrayDestroy");
	DriverAPICall call = std::bind(REAL_cuMipmappedArrayDestroy ,hMipmappedArray);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuPointerGetAttribute( void * data, CUpointer_attribute attribute, CUdeviceptr ptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&data,(void *)&attribute,(void *)&ptr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(113, &params));
	CUresult CUDAAPI (*REAL_cuPointerGetAttribute)(void *,CUpointer_attribute,CUdeviceptr);
	*(void **)(*REAL_cuPointerGetAttribute) = dlsym(RTLD_NEXT, "cuPointerGetAttribute");
	DriverAPICall call = std::bind(REAL_cuPointerGetAttribute ,data,attribute,ptr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuPointerSetAttribute( const void * value, CUpointer_attribute attribute, CUdeviceptr ptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&value,(void *)&attribute,(void *)&ptr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(114, &params));
	CUresult CUDAAPI (*REAL_cuPointerSetAttribute)(const void *,CUpointer_attribute,CUdeviceptr);
	*(void **)(*REAL_cuPointerSetAttribute) = dlsym(RTLD_NEXT, "cuPointerSetAttribute");
	DriverAPICall call = std::bind(REAL_cuPointerSetAttribute ,value,attribute,ptr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuPointerGetAttributes( unsigned int numAttributes, CUpointer_attribute * attributes, void * * data, CUdeviceptr ptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&numAttributes,(void *)&attributes,(void *)&data,(void *)&ptr };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(115, &params));
	CUresult CUDAAPI (*REAL_cuPointerGetAttributes)(unsigned int,CUpointer_attribute *,void * *,CUdeviceptr);
	*(void **)(*REAL_cuPointerGetAttributes) = dlsym(RTLD_NEXT, "cuPointerGetAttributes");
	DriverAPICall call = std::bind(REAL_cuPointerGetAttributes ,numAttributes,attributes,data,ptr);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamCreate( CUstream * phStream, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&phStream,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(116, &params));
	CUresult CUDAAPI (*REAL_cuStreamCreate)(CUstream *,unsigned int);
	*(void **)(*REAL_cuStreamCreate) = dlsym(RTLD_NEXT, "cuStreamCreate");
	DriverAPICall call = std::bind(REAL_cuStreamCreate ,phStream,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamCreateWithPriority( CUstream * phStream, unsigned int flags, int priority ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&phStream,(void *)&flags,(void *)&priority };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(117, &params));
	CUresult CUDAAPI (*REAL_cuStreamCreateWithPriority)(CUstream *,unsigned int,int);
	*(void **)(*REAL_cuStreamCreateWithPriority) = dlsym(RTLD_NEXT, "cuStreamCreateWithPriority");
	DriverAPICall call = std::bind(REAL_cuStreamCreateWithPriority ,phStream,flags,priority);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamGetPriority( CUstream hStream, int * priority ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream,(void *)&priority };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(118, &params));
	CUresult CUDAAPI (*REAL_cuStreamGetPriority)(CUstream,int *);
	*(void **)(*REAL_cuStreamGetPriority) = dlsym(RTLD_NEXT, "cuStreamGetPriority");
	DriverAPICall call = std::bind(REAL_cuStreamGetPriority ,hStream,priority);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamGetFlags( CUstream hStream, unsigned int * flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(119, &params));
	CUresult CUDAAPI (*REAL_cuStreamGetFlags)(CUstream,unsigned int *);
	*(void **)(*REAL_cuStreamGetFlags) = dlsym(RTLD_NEXT, "cuStreamGetFlags");
	DriverAPICall call = std::bind(REAL_cuStreamGetFlags ,hStream,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamWaitEvent( CUstream hStream, CUevent hEvent, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream,(void *)&hEvent,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(120, &params));
	CUresult CUDAAPI (*REAL_cuStreamWaitEvent)(CUstream,CUevent,unsigned int);
	*(void **)(*REAL_cuStreamWaitEvent) = dlsym(RTLD_NEXT, "cuStreamWaitEvent");
	DriverAPICall call = std::bind(REAL_cuStreamWaitEvent ,hStream,hEvent,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamAddCallback( CUstream hStream, CUstreamCallback callback, void * userData, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream,(void *)&callback,(void *)&userData,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(121, &params));
	CUresult CUDAAPI (*REAL_cuStreamAddCallback)(CUstream,CUstreamCallback,void *,unsigned int);
	*(void **)(*REAL_cuStreamAddCallback) = dlsym(RTLD_NEXT, "cuStreamAddCallback");
	DriverAPICall call = std::bind(REAL_cuStreamAddCallback ,hStream,callback,userData,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamAttachMemAsync( CUstream hStream, CUdeviceptr dptr, size_t length, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream,(void *)&dptr,(void *)&length,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(122, &params));
	CUresult CUDAAPI (*REAL_cuStreamAttachMemAsync)(CUstream,CUdeviceptr,size_t,unsigned int);
	*(void **)(*REAL_cuStreamAttachMemAsync) = dlsym(RTLD_NEXT, "cuStreamAttachMemAsync");
	DriverAPICall call = std::bind(REAL_cuStreamAttachMemAsync ,hStream,dptr,length,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamQuery( CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(123, &params));
	CUresult CUDAAPI (*REAL_cuStreamQuery)(CUstream);
	*(void **)(*REAL_cuStreamQuery) = dlsym(RTLD_NEXT, "cuStreamQuery");
	DriverAPICall call = std::bind(REAL_cuStreamQuery ,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamSynchronize( CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(124, &params));
	CUresult CUDAAPI (*REAL_cuStreamSynchronize)(CUstream);
	*(void **)(*REAL_cuStreamSynchronize) = dlsym(RTLD_NEXT, "cuStreamSynchronize");
	DriverAPICall call = std::bind(REAL_cuStreamSynchronize ,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuStreamDestroy( CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(125, &params));
	CUresult CUDAAPI (*REAL_cuStreamDestroy)(CUstream);
	*(void **)(*REAL_cuStreamDestroy) = dlsym(RTLD_NEXT, "cuStreamDestroy");
	DriverAPICall call = std::bind(REAL_cuStreamDestroy ,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuEventCreate( CUevent * phEvent, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&phEvent,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(126, &params));
	CUresult CUDAAPI (*REAL_cuEventCreate)(CUevent *,unsigned int);
	*(void **)(*REAL_cuEventCreate) = dlsym(RTLD_NEXT, "cuEventCreate");
	DriverAPICall call = std::bind(REAL_cuEventCreate ,phEvent,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuEventRecord( CUevent hEvent, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hEvent,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(127, &params));
	CUresult CUDAAPI (*REAL_cuEventRecord)(CUevent,CUstream);
	*(void **)(*REAL_cuEventRecord) = dlsym(RTLD_NEXT, "cuEventRecord");
	DriverAPICall call = std::bind(REAL_cuEventRecord ,hEvent,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuEventQuery( CUevent hEvent ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hEvent };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(128, &params));
	CUresult CUDAAPI (*REAL_cuEventQuery)(CUevent);
	*(void **)(*REAL_cuEventQuery) = dlsym(RTLD_NEXT, "cuEventQuery");
	DriverAPICall call = std::bind(REAL_cuEventQuery ,hEvent);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuEventSynchronize( CUevent hEvent ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hEvent };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(129, &params));
	CUresult CUDAAPI (*REAL_cuEventSynchronize)(CUevent);
	*(void **)(*REAL_cuEventSynchronize) = dlsym(RTLD_NEXT, "cuEventSynchronize");
	DriverAPICall call = std::bind(REAL_cuEventSynchronize ,hEvent);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuEventDestroy( CUevent hEvent ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hEvent };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(130, &params));
	CUresult CUDAAPI (*REAL_cuEventDestroy)(CUevent);
	*(void **)(*REAL_cuEventDestroy) = dlsym(RTLD_NEXT, "cuEventDestroy");
	DriverAPICall call = std::bind(REAL_cuEventDestroy ,hEvent);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuEventElapsedTime( float * pMilliseconds, CUevent hStart, CUevent hEnd ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pMilliseconds,(void *)&hStart,(void *)&hEnd };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(131, &params));
	CUresult CUDAAPI (*REAL_cuEventElapsedTime)(float *,CUevent,CUevent);
	*(void **)(*REAL_cuEventElapsedTime) = dlsym(RTLD_NEXT, "cuEventElapsedTime");
	DriverAPICall call = std::bind(REAL_cuEventElapsedTime ,pMilliseconds,hStart,hEnd);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuFuncGetAttribute( int * pi, CUfunction_attribute attrib, CUfunction hfunc ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pi,(void *)&attrib,(void *)&hfunc };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(132, &params));
	CUresult CUDAAPI (*REAL_cuFuncGetAttribute)(int *,CUfunction_attribute,CUfunction);
	*(void **)(*REAL_cuFuncGetAttribute) = dlsym(RTLD_NEXT, "cuFuncGetAttribute");
	DriverAPICall call = std::bind(REAL_cuFuncGetAttribute ,pi,attrib,hfunc);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuFuncSetCacheConfig( CUfunction hfunc, CUfunc_cache config ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&config };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(133, &params));
	CUresult CUDAAPI (*REAL_cuFuncSetCacheConfig)(CUfunction,CUfunc_cache);
	*(void **)(*REAL_cuFuncSetCacheConfig) = dlsym(RTLD_NEXT, "cuFuncSetCacheConfig");
	DriverAPICall call = std::bind(REAL_cuFuncSetCacheConfig ,hfunc,config);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuFuncSetSharedMemConfig( CUfunction hfunc, CUsharedconfig config ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&config };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(134, &params));
	CUresult CUDAAPI (*REAL_cuFuncSetSharedMemConfig)(CUfunction,CUsharedconfig);
	*(void **)(*REAL_cuFuncSetSharedMemConfig) = dlsym(RTLD_NEXT, "cuFuncSetSharedMemConfig");
	DriverAPICall call = std::bind(REAL_cuFuncSetSharedMemConfig ,hfunc,config);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLaunchKernel( CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes, CUstream hStream, void * * kernelParams, void * * extra ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&f,(void *)&gridDimX,(void *)&gridDimY,(void *)&gridDimZ,(void *)&blockDimX,(void *)&blockDimY,(void *)&blockDimZ,(void *)&sharedMemBytes,(void *)&hStream,(void *)&kernelParams,(void *)&extra };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(135, &params));
	CUresult CUDAAPI (*REAL_cuLaunchKernel)(CUfunction,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,CUstream,void * *,void * *);
	*(void **)(*REAL_cuLaunchKernel) = dlsym(RTLD_NEXT, "cuLaunchKernel");
	DriverAPICall call = std::bind(REAL_cuLaunchKernel ,f,gridDimX,gridDimY,gridDimZ,blockDimX,blockDimY,blockDimZ,sharedMemBytes,hStream,kernelParams,extra);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuFuncSetBlockShape( CUfunction hfunc, int x, int y, int z ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&x,(void *)&y,(void *)&z };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(136, &params));
	CUresult CUDAAPI (*REAL_cuFuncSetBlockShape)(CUfunction,int,int,int);
	*(void **)(*REAL_cuFuncSetBlockShape) = dlsym(RTLD_NEXT, "cuFuncSetBlockShape");
	DriverAPICall call = std::bind(REAL_cuFuncSetBlockShape ,hfunc,x,y,z);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuFuncSetSharedSize( CUfunction hfunc, unsigned int bytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&bytes };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(137, &params));
	CUresult CUDAAPI (*REAL_cuFuncSetSharedSize)(CUfunction,unsigned int);
	*(void **)(*REAL_cuFuncSetSharedSize) = dlsym(RTLD_NEXT, "cuFuncSetSharedSize");
	DriverAPICall call = std::bind(REAL_cuFuncSetSharedSize ,hfunc,bytes);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuParamSetSize( CUfunction hfunc, unsigned int numbytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&numbytes };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(138, &params));
	CUresult CUDAAPI (*REAL_cuParamSetSize)(CUfunction,unsigned int);
	*(void **)(*REAL_cuParamSetSize) = dlsym(RTLD_NEXT, "cuParamSetSize");
	DriverAPICall call = std::bind(REAL_cuParamSetSize ,hfunc,numbytes);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuParamSeti( CUfunction hfunc, int offset, unsigned int value ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&offset,(void *)&value };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(139, &params));
	CUresult CUDAAPI (*REAL_cuParamSeti)(CUfunction,int,unsigned int);
	*(void **)(*REAL_cuParamSeti) = dlsym(RTLD_NEXT, "cuParamSeti");
	DriverAPICall call = std::bind(REAL_cuParamSeti ,hfunc,offset,value);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuParamSetf( CUfunction hfunc, int offset, float value ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&offset,(void *)&value };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(140, &params));
	CUresult CUDAAPI (*REAL_cuParamSetf)(CUfunction,int,float);
	*(void **)(*REAL_cuParamSetf) = dlsym(RTLD_NEXT, "cuParamSetf");
	DriverAPICall call = std::bind(REAL_cuParamSetf ,hfunc,offset,value);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuParamSetv( CUfunction hfunc, int offset, void * ptr, unsigned int numbytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&offset,(void *)&ptr,(void *)&numbytes };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(141, &params));
	CUresult CUDAAPI (*REAL_cuParamSetv)(CUfunction,int,void *,unsigned int);
	*(void **)(*REAL_cuParamSetv) = dlsym(RTLD_NEXT, "cuParamSetv");
	DriverAPICall call = std::bind(REAL_cuParamSetv ,hfunc,offset,ptr,numbytes);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLaunch( CUfunction f ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&f };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(142, &params));
	CUresult CUDAAPI (*REAL_cuLaunch)(CUfunction);
	*(void **)(*REAL_cuLaunch) = dlsym(RTLD_NEXT, "cuLaunch");
	DriverAPICall call = std::bind(REAL_cuLaunch ,f);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLaunchGrid( CUfunction f, int grid_width, int grid_height ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&f,(void *)&grid_width,(void *)&grid_height };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(143, &params));
	CUresult CUDAAPI (*REAL_cuLaunchGrid)(CUfunction,int,int);
	*(void **)(*REAL_cuLaunchGrid) = dlsym(RTLD_NEXT, "cuLaunchGrid");
	DriverAPICall call = std::bind(REAL_cuLaunchGrid ,f,grid_width,grid_height);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuLaunchGridAsync( CUfunction f, int grid_width, int grid_height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&f,(void *)&grid_width,(void *)&grid_height,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(144, &params));
	CUresult CUDAAPI (*REAL_cuLaunchGridAsync)(CUfunction,int,int,CUstream);
	*(void **)(*REAL_cuLaunchGridAsync) = dlsym(RTLD_NEXT, "cuLaunchGridAsync");
	DriverAPICall call = std::bind(REAL_cuLaunchGridAsync ,f,grid_width,grid_height,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuParamSetTexRef( CUfunction hfunc, int texunit, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hfunc,(void *)&texunit,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(145, &params));
	CUresult CUDAAPI (*REAL_cuParamSetTexRef)(CUfunction,int,CUtexref);
	*(void **)(*REAL_cuParamSetTexRef) = dlsym(RTLD_NEXT, "cuParamSetTexRef");
	DriverAPICall call = std::bind(REAL_cuParamSetTexRef ,hfunc,texunit,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuOccupancyMaxActiveBlocksPerMultiprocessor( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&numBlocks,(void *)&func,(void *)&blockSize,(void *)&dynamicSMemSize };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(146, &params));
	CUresult CUDAAPI (*REAL_cuOccupancyMaxActiveBlocksPerMultiprocessor)(int *,CUfunction,int,size_t);
	*(void **)(*REAL_cuOccupancyMaxActiveBlocksPerMultiprocessor) = dlsym(RTLD_NEXT, "cuOccupancyMaxActiveBlocksPerMultiprocessor");
	DriverAPICall call = std::bind(REAL_cuOccupancyMaxActiveBlocksPerMultiprocessor ,numBlocks,func,blockSize,dynamicSMemSize);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&numBlocks,(void *)&func,(void *)&blockSize,(void *)&dynamicSMemSize,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(147, &params));
	CUresult CUDAAPI (*REAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags)(int *,CUfunction,int,size_t,unsigned int);
	*(void **)(*REAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags) = dlsym(RTLD_NEXT, "cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags");
	DriverAPICall call = std::bind(REAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags ,numBlocks,func,blockSize,dynamicSMemSize,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuOccupancyMaxPotentialBlockSize( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&minGridSize,(void *)&blockSize,(void *)&func,(void *)&blockSizeToDynamicSMemSize,(void *)&dynamicSMemSize,(void *)&blockSizeLimit };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(148, &params));
	CUresult CUDAAPI (*REAL_cuOccupancyMaxPotentialBlockSize)(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int);
	*(void **)(*REAL_cuOccupancyMaxPotentialBlockSize) = dlsym(RTLD_NEXT, "cuOccupancyMaxPotentialBlockSize");
	DriverAPICall call = std::bind(REAL_cuOccupancyMaxPotentialBlockSize ,minGridSize,blockSize,func,blockSizeToDynamicSMemSize,dynamicSMemSize,blockSizeLimit);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuOccupancyMaxPotentialBlockSizeWithFlags( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&minGridSize,(void *)&blockSize,(void *)&func,(void *)&blockSizeToDynamicSMemSize,(void *)&dynamicSMemSize,(void *)&blockSizeLimit,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(149, &params));
	CUresult CUDAAPI (*REAL_cuOccupancyMaxPotentialBlockSizeWithFlags)(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int,unsigned int);
	*(void **)(*REAL_cuOccupancyMaxPotentialBlockSizeWithFlags) = dlsym(RTLD_NEXT, "cuOccupancyMaxPotentialBlockSizeWithFlags");
	DriverAPICall call = std::bind(REAL_cuOccupancyMaxPotentialBlockSizeWithFlags ,minGridSize,blockSize,func,blockSizeToDynamicSMemSize,dynamicSMemSize,blockSizeLimit,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetArray( CUtexref hTexRef, CUarray hArray, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&hArray,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(150, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetArray)(CUtexref,CUarray,unsigned int);
	*(void **)(*REAL_cuTexRefSetArray) = dlsym(RTLD_NEXT, "cuTexRefSetArray");
	DriverAPICall call = std::bind(REAL_cuTexRefSetArray ,hTexRef,hArray,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetMipmappedArray( CUtexref hTexRef, CUmipmappedArray hMipmappedArray, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&hMipmappedArray,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(151, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetMipmappedArray)(CUtexref,CUmipmappedArray,unsigned int);
	*(void **)(*REAL_cuTexRefSetMipmappedArray) = dlsym(RTLD_NEXT, "cuTexRefSetMipmappedArray");
	DriverAPICall call = std::bind(REAL_cuTexRefSetMipmappedArray ,hTexRef,hMipmappedArray,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetAddress( size_t * ByteOffset, CUtexref hTexRef, CUdeviceptr dptr, size_t bytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&ByteOffset,(void *)&hTexRef,(void *)&dptr,(void *)&bytes };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(152, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetAddress)(size_t *,CUtexref,CUdeviceptr,size_t);
	*(void **)(*REAL_cuTexRefSetAddress) = dlsym(RTLD_NEXT, "cuTexRefSetAddress");
	DriverAPICall call = std::bind(REAL_cuTexRefSetAddress ,ByteOffset,hTexRef,dptr,bytes);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetAddress2D( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&desc,(void *)&dptr,(void *)&Pitch };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(153, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetAddress2D)(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t);
	*(void **)(*REAL_cuTexRefSetAddress2D) = dlsym(RTLD_NEXT, "cuTexRefSetAddress2D");
	DriverAPICall call = std::bind(REAL_cuTexRefSetAddress2D ,hTexRef,desc,dptr,Pitch);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetFormat( CUtexref hTexRef, CUarray_format fmt, int NumPackedComponents ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&fmt,(void *)&NumPackedComponents };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(154, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetFormat)(CUtexref,CUarray_format,int);
	*(void **)(*REAL_cuTexRefSetFormat) = dlsym(RTLD_NEXT, "cuTexRefSetFormat");
	DriverAPICall call = std::bind(REAL_cuTexRefSetFormat ,hTexRef,fmt,NumPackedComponents);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetAddressMode( CUtexref hTexRef, int dim, CUaddress_mode am ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&dim,(void *)&am };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(155, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetAddressMode)(CUtexref,int,CUaddress_mode);
	*(void **)(*REAL_cuTexRefSetAddressMode) = dlsym(RTLD_NEXT, "cuTexRefSetAddressMode");
	DriverAPICall call = std::bind(REAL_cuTexRefSetAddressMode ,hTexRef,dim,am);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetFilterMode( CUtexref hTexRef, CUfilter_mode fm ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&fm };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(156, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetFilterMode)(CUtexref,CUfilter_mode);
	*(void **)(*REAL_cuTexRefSetFilterMode) = dlsym(RTLD_NEXT, "cuTexRefSetFilterMode");
	DriverAPICall call = std::bind(REAL_cuTexRefSetFilterMode ,hTexRef,fm);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetMipmapFilterMode( CUtexref hTexRef, CUfilter_mode fm ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&fm };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(157, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetMipmapFilterMode)(CUtexref,CUfilter_mode);
	*(void **)(*REAL_cuTexRefSetMipmapFilterMode) = dlsym(RTLD_NEXT, "cuTexRefSetMipmapFilterMode");
	DriverAPICall call = std::bind(REAL_cuTexRefSetMipmapFilterMode ,hTexRef,fm);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetMipmapLevelBias( CUtexref hTexRef, float bias ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&bias };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(158, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetMipmapLevelBias)(CUtexref,float);
	*(void **)(*REAL_cuTexRefSetMipmapLevelBias) = dlsym(RTLD_NEXT, "cuTexRefSetMipmapLevelBias");
	DriverAPICall call = std::bind(REAL_cuTexRefSetMipmapLevelBias ,hTexRef,bias);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetMipmapLevelClamp( CUtexref hTexRef, float minMipmapLevelClamp, float maxMipmapLevelClamp ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&minMipmapLevelClamp,(void *)&maxMipmapLevelClamp };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(159, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetMipmapLevelClamp)(CUtexref,float,float);
	*(void **)(*REAL_cuTexRefSetMipmapLevelClamp) = dlsym(RTLD_NEXT, "cuTexRefSetMipmapLevelClamp");
	DriverAPICall call = std::bind(REAL_cuTexRefSetMipmapLevelClamp ,hTexRef,minMipmapLevelClamp,maxMipmapLevelClamp);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetMaxAnisotropy( CUtexref hTexRef, unsigned int maxAniso ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&maxAniso };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(160, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetMaxAnisotropy)(CUtexref,unsigned int);
	*(void **)(*REAL_cuTexRefSetMaxAnisotropy) = dlsym(RTLD_NEXT, "cuTexRefSetMaxAnisotropy");
	DriverAPICall call = std::bind(REAL_cuTexRefSetMaxAnisotropy ,hTexRef,maxAniso);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetFlags( CUtexref hTexRef, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(161, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetFlags)(CUtexref,unsigned int);
	*(void **)(*REAL_cuTexRefSetFlags) = dlsym(RTLD_NEXT, "cuTexRefSetFlags");
	DriverAPICall call = std::bind(REAL_cuTexRefSetFlags ,hTexRef,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetAddress( CUdeviceptr * pdptr, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pdptr,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(162, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetAddress)(CUdeviceptr *,CUtexref);
	*(void **)(*REAL_cuTexRefGetAddress) = dlsym(RTLD_NEXT, "cuTexRefGetAddress");
	DriverAPICall call = std::bind(REAL_cuTexRefGetAddress ,pdptr,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetArray( CUarray * phArray, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&phArray,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(163, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetArray)(CUarray *,CUtexref);
	*(void **)(*REAL_cuTexRefGetArray) = dlsym(RTLD_NEXT, "cuTexRefGetArray");
	DriverAPICall call = std::bind(REAL_cuTexRefGetArray ,phArray,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetMipmappedArray( CUmipmappedArray * phMipmappedArray, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&phMipmappedArray,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(164, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetMipmappedArray)(CUmipmappedArray *,CUtexref);
	*(void **)(*REAL_cuTexRefGetMipmappedArray) = dlsym(RTLD_NEXT, "cuTexRefGetMipmappedArray");
	DriverAPICall call = std::bind(REAL_cuTexRefGetMipmappedArray ,phMipmappedArray,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetAddressMode( CUaddress_mode * pam, CUtexref hTexRef, int dim ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pam,(void *)&hTexRef,(void *)&dim };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(165, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetAddressMode)(CUaddress_mode *,CUtexref,int);
	*(void **)(*REAL_cuTexRefGetAddressMode) = dlsym(RTLD_NEXT, "cuTexRefGetAddressMode");
	DriverAPICall call = std::bind(REAL_cuTexRefGetAddressMode ,pam,hTexRef,dim);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pfm,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(166, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetFilterMode)(CUfilter_mode *,CUtexref);
	*(void **)(*REAL_cuTexRefGetFilterMode) = dlsym(RTLD_NEXT, "cuTexRefGetFilterMode");
	DriverAPICall call = std::bind(REAL_cuTexRefGetFilterMode ,pfm,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetFormat( CUarray_format * pFormat, int * pNumChannels, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pFormat,(void *)&pNumChannels,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(167, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetFormat)(CUarray_format *,int *,CUtexref);
	*(void **)(*REAL_cuTexRefGetFormat) = dlsym(RTLD_NEXT, "cuTexRefGetFormat");
	DriverAPICall call = std::bind(REAL_cuTexRefGetFormat ,pFormat,pNumChannels,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetMipmapFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pfm,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(168, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetMipmapFilterMode)(CUfilter_mode *,CUtexref);
	*(void **)(*REAL_cuTexRefGetMipmapFilterMode) = dlsym(RTLD_NEXT, "cuTexRefGetMipmapFilterMode");
	DriverAPICall call = std::bind(REAL_cuTexRefGetMipmapFilterMode ,pfm,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetMipmapLevelBias( float * pbias, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pbias,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(169, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetMipmapLevelBias)(float *,CUtexref);
	*(void **)(*REAL_cuTexRefGetMipmapLevelBias) = dlsym(RTLD_NEXT, "cuTexRefGetMipmapLevelBias");
	DriverAPICall call = std::bind(REAL_cuTexRefGetMipmapLevelBias ,pbias,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetMipmapLevelClamp( float * pminMipmapLevelClamp, float * pmaxMipmapLevelClamp, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pminMipmapLevelClamp,(void *)&pmaxMipmapLevelClamp,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(170, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetMipmapLevelClamp)(float *,float *,CUtexref);
	*(void **)(*REAL_cuTexRefGetMipmapLevelClamp) = dlsym(RTLD_NEXT, "cuTexRefGetMipmapLevelClamp");
	DriverAPICall call = std::bind(REAL_cuTexRefGetMipmapLevelClamp ,pminMipmapLevelClamp,pmaxMipmapLevelClamp,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetMaxAnisotropy( int * pmaxAniso, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pmaxAniso,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(171, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetMaxAnisotropy)(int *,CUtexref);
	*(void **)(*REAL_cuTexRefGetMaxAnisotropy) = dlsym(RTLD_NEXT, "cuTexRefGetMaxAnisotropy");
	DriverAPICall call = std::bind(REAL_cuTexRefGetMaxAnisotropy ,pmaxAniso,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefGetFlags( unsigned int * pFlags, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pFlags,(void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(172, &params));
	CUresult CUDAAPI (*REAL_cuTexRefGetFlags)(unsigned int *,CUtexref);
	*(void **)(*REAL_cuTexRefGetFlags) = dlsym(RTLD_NEXT, "cuTexRefGetFlags");
	DriverAPICall call = std::bind(REAL_cuTexRefGetFlags ,pFlags,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefCreate( CUtexref * pTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(173, &params));
	CUresult CUDAAPI (*REAL_cuTexRefCreate)(CUtexref *);
	*(void **)(*REAL_cuTexRefCreate) = dlsym(RTLD_NEXT, "cuTexRefCreate");
	DriverAPICall call = std::bind(REAL_cuTexRefCreate ,pTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefDestroy( CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(174, &params));
	CUresult CUDAAPI (*REAL_cuTexRefDestroy)(CUtexref);
	*(void **)(*REAL_cuTexRefDestroy) = dlsym(RTLD_NEXT, "cuTexRefDestroy");
	DriverAPICall call = std::bind(REAL_cuTexRefDestroy ,hTexRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuSurfRefSetArray( CUsurfref hSurfRef, CUarray hArray, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hSurfRef,(void *)&hArray,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(175, &params));
	CUresult CUDAAPI (*REAL_cuSurfRefSetArray)(CUsurfref,CUarray,unsigned int);
	*(void **)(*REAL_cuSurfRefSetArray) = dlsym(RTLD_NEXT, "cuSurfRefSetArray");
	DriverAPICall call = std::bind(REAL_cuSurfRefSetArray ,hSurfRef,hArray,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuSurfRefGetArray( CUarray * phArray, CUsurfref hSurfRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&phArray,(void *)&hSurfRef };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(176, &params));
	CUresult CUDAAPI (*REAL_cuSurfRefGetArray)(CUarray *,CUsurfref);
	*(void **)(*REAL_cuSurfRefGetArray) = dlsym(RTLD_NEXT, "cuSurfRefGetArray");
	DriverAPICall call = std::bind(REAL_cuSurfRefGetArray ,phArray,hSurfRef);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexObjectCreate( CUtexObject * pTexObject, const CUDA_RESOURCE_DESC * pResDesc, const CUDA_TEXTURE_DESC * pTexDesc, const CUDA_RESOURCE_VIEW_DESC * pResViewDesc ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pTexObject,(void *)&pResDesc,(void *)&pTexDesc,(void *)&pResViewDesc };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(177, &params));
	CUresult CUDAAPI (*REAL_cuTexObjectCreate)(CUtexObject *,const CUDA_RESOURCE_DESC *,const CUDA_TEXTURE_DESC *,const CUDA_RESOURCE_VIEW_DESC *);
	*(void **)(*REAL_cuTexObjectCreate) = dlsym(RTLD_NEXT, "cuTexObjectCreate");
	DriverAPICall call = std::bind(REAL_cuTexObjectCreate ,pTexObject,pResDesc,pTexDesc,pResViewDesc);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexObjectDestroy( CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&texObject };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(178, &params));
	CUresult CUDAAPI (*REAL_cuTexObjectDestroy)(CUtexObject);
	*(void **)(*REAL_cuTexObjectDestroy) = dlsym(RTLD_NEXT, "cuTexObjectDestroy");
	DriverAPICall call = std::bind(REAL_cuTexObjectDestroy ,texObject);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pResDesc,(void *)&texObject };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(179, &params));
	CUresult CUDAAPI (*REAL_cuTexObjectGetResourceDesc)(CUDA_RESOURCE_DESC *,CUtexObject);
	*(void **)(*REAL_cuTexObjectGetResourceDesc) = dlsym(RTLD_NEXT, "cuTexObjectGetResourceDesc");
	DriverAPICall call = std::bind(REAL_cuTexObjectGetResourceDesc ,pResDesc,texObject);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexObjectGetTextureDesc( CUDA_TEXTURE_DESC * pTexDesc, CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pTexDesc,(void *)&texObject };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(180, &params));
	CUresult CUDAAPI (*REAL_cuTexObjectGetTextureDesc)(CUDA_TEXTURE_DESC *,CUtexObject);
	*(void **)(*REAL_cuTexObjectGetTextureDesc) = dlsym(RTLD_NEXT, "cuTexObjectGetTextureDesc");
	DriverAPICall call = std::bind(REAL_cuTexObjectGetTextureDesc ,pTexDesc,texObject);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexObjectGetResourceViewDesc( CUDA_RESOURCE_VIEW_DESC * pResViewDesc, CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pResViewDesc,(void *)&texObject };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(181, &params));
	CUresult CUDAAPI (*REAL_cuTexObjectGetResourceViewDesc)(CUDA_RESOURCE_VIEW_DESC *,CUtexObject);
	*(void **)(*REAL_cuTexObjectGetResourceViewDesc) = dlsym(RTLD_NEXT, "cuTexObjectGetResourceViewDesc");
	DriverAPICall call = std::bind(REAL_cuTexObjectGetResourceViewDesc ,pResViewDesc,texObject);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuSurfObjectCreate( CUsurfObject * pSurfObject, const CUDA_RESOURCE_DESC * pResDesc ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pSurfObject,(void *)&pResDesc };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(182, &params));
	CUresult CUDAAPI (*REAL_cuSurfObjectCreate)(CUsurfObject *,const CUDA_RESOURCE_DESC *);
	*(void **)(*REAL_cuSurfObjectCreate) = dlsym(RTLD_NEXT, "cuSurfObjectCreate");
	DriverAPICall call = std::bind(REAL_cuSurfObjectCreate ,pSurfObject,pResDesc);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuSurfObjectDestroy( CUsurfObject surfObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&surfObject };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(183, &params));
	CUresult CUDAAPI (*REAL_cuSurfObjectDestroy)(CUsurfObject);
	*(void **)(*REAL_cuSurfObjectDestroy) = dlsym(RTLD_NEXT, "cuSurfObjectDestroy");
	DriverAPICall call = std::bind(REAL_cuSurfObjectDestroy ,surfObject);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuSurfObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUsurfObject surfObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pResDesc,(void *)&surfObject };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(184, &params));
	CUresult CUDAAPI (*REAL_cuSurfObjectGetResourceDesc)(CUDA_RESOURCE_DESC *,CUsurfObject);
	*(void **)(*REAL_cuSurfObjectGetResourceDesc) = dlsym(RTLD_NEXT, "cuSurfObjectGetResourceDesc");
	DriverAPICall call = std::bind(REAL_cuSurfObjectGetResourceDesc ,pResDesc,surfObject);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuDeviceCanAccessPeer( int * canAccessPeer, CUdevice dev, CUdevice peerDev ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&canAccessPeer,(void *)&dev,(void *)&peerDev };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(185, &params));
	CUresult CUDAAPI (*REAL_cuDeviceCanAccessPeer)(int *,CUdevice,CUdevice);
	*(void **)(*REAL_cuDeviceCanAccessPeer) = dlsym(RTLD_NEXT, "cuDeviceCanAccessPeer");
	DriverAPICall call = std::bind(REAL_cuDeviceCanAccessPeer ,canAccessPeer,dev,peerDev);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxEnablePeerAccess( CUcontext peerContext, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&peerContext,(void *)&Flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(186, &params));
	CUresult CUDAAPI (*REAL_cuCtxEnablePeerAccess)(CUcontext,unsigned int);
	*(void **)(*REAL_cuCtxEnablePeerAccess) = dlsym(RTLD_NEXT, "cuCtxEnablePeerAccess");
	DriverAPICall call = std::bind(REAL_cuCtxEnablePeerAccess ,peerContext,Flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuCtxDisablePeerAccess( CUcontext peerContext ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&peerContext };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(187, &params));
	CUresult CUDAAPI (*REAL_cuCtxDisablePeerAccess)(CUcontext);
	*(void **)(*REAL_cuCtxDisablePeerAccess) = dlsym(RTLD_NEXT, "cuCtxDisablePeerAccess");
	DriverAPICall call = std::bind(REAL_cuCtxDisablePeerAccess ,peerContext);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGraphicsUnregisterResource( CUgraphicsResource resource ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&resource };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(188, &params));
	CUresult CUDAAPI (*REAL_cuGraphicsUnregisterResource)(CUgraphicsResource);
	*(void **)(*REAL_cuGraphicsUnregisterResource) = dlsym(RTLD_NEXT, "cuGraphicsUnregisterResource");
	DriverAPICall call = std::bind(REAL_cuGraphicsUnregisterResource ,resource);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGraphicsSubResourceGetMappedArray( CUarray * pArray, CUgraphicsResource resource, unsigned int arrayIndex, unsigned int mipLevel ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pArray,(void *)&resource,(void *)&arrayIndex,(void *)&mipLevel };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(189, &params));
	CUresult CUDAAPI (*REAL_cuGraphicsSubResourceGetMappedArray)(CUarray *,CUgraphicsResource,unsigned int,unsigned int);
	*(void **)(*REAL_cuGraphicsSubResourceGetMappedArray) = dlsym(RTLD_NEXT, "cuGraphicsSubResourceGetMappedArray");
	DriverAPICall call = std::bind(REAL_cuGraphicsSubResourceGetMappedArray ,pArray,resource,arrayIndex,mipLevel);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGraphicsResourceGetMappedMipmappedArray( CUmipmappedArray * pMipmappedArray, CUgraphicsResource resource ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pMipmappedArray,(void *)&resource };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(190, &params));
	CUresult CUDAAPI (*REAL_cuGraphicsResourceGetMappedMipmappedArray)(CUmipmappedArray *,CUgraphicsResource);
	*(void **)(*REAL_cuGraphicsResourceGetMappedMipmappedArray) = dlsym(RTLD_NEXT, "cuGraphicsResourceGetMappedMipmappedArray");
	DriverAPICall call = std::bind(REAL_cuGraphicsResourceGetMappedMipmappedArray ,pMipmappedArray,resource);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGraphicsResourceGetMappedPointer( CUdeviceptr * pDevPtr, size_t * pSize, CUgraphicsResource resource ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pDevPtr,(void *)&pSize,(void *)&resource };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(191, &params));
	CUresult CUDAAPI (*REAL_cuGraphicsResourceGetMappedPointer)(CUdeviceptr *,size_t *,CUgraphicsResource);
	*(void **)(*REAL_cuGraphicsResourceGetMappedPointer) = dlsym(RTLD_NEXT, "cuGraphicsResourceGetMappedPointer");
	DriverAPICall call = std::bind(REAL_cuGraphicsResourceGetMappedPointer ,pDevPtr,pSize,resource);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGraphicsResourceSetMapFlags( CUgraphicsResource resource, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&resource,(void *)&flags };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(192, &params));
	CUresult CUDAAPI (*REAL_cuGraphicsResourceSetMapFlags)(CUgraphicsResource,unsigned int);
	*(void **)(*REAL_cuGraphicsResourceSetMapFlags) = dlsym(RTLD_NEXT, "cuGraphicsResourceSetMapFlags");
	DriverAPICall call = std::bind(REAL_cuGraphicsResourceSetMapFlags ,resource,flags);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGraphicsMapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&count,(void *)&resources,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(193, &params));
	CUresult CUDAAPI (*REAL_cuGraphicsMapResources)(unsigned int,CUgraphicsResource *,CUstream);
	*(void **)(*REAL_cuGraphicsMapResources) = dlsym(RTLD_NEXT, "cuGraphicsMapResources");
	DriverAPICall call = std::bind(REAL_cuGraphicsMapResources ,count,resources,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGraphicsUnmapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&count,(void *)&resources,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(194, &params));
	CUresult CUDAAPI (*REAL_cuGraphicsUnmapResources)(unsigned int,CUgraphicsResource *,CUstream);
	*(void **)(*REAL_cuGraphicsUnmapResources) = dlsym(RTLD_NEXT, "cuGraphicsUnmapResources");
	DriverAPICall call = std::bind(REAL_cuGraphicsUnmapResources ,count,resources,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuGetExportTable( const void * * ppExportTable, const CUuuid * pExportTableId ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&ppExportTable,(void *)&pExportTableId };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(195, &params));
	CUresult CUDAAPI (*REAL_cuGetExportTable)(const void * *,const CUuuid *);
	*(void **)(*REAL_cuGetExportTable) = dlsym(RTLD_NEXT, "cuGetExportTable");
	DriverAPICall call = std::bind(REAL_cuGetExportTable ,ppExportTable,pExportTableId);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuTexRefSetAddress2D_v2( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&hTexRef,(void *)&desc,(void *)&dptr,(void *)&Pitch };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(196, &params));
	CUresult CUDAAPI (*REAL_cuTexRefSetAddress2D_v2)(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t);
	*(void **)(*REAL_cuTexRefSetAddress2D_v2) = dlsym(RTLD_NEXT, "cuTexRefSetAddress2D_v2");
	DriverAPICall call = std::bind(REAL_cuTexRefSetAddress2D_v2 ,hTexRef,desc,dptr,Pitch);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoD_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcHost,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(197, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoD_v2)(CUdeviceptr,const void *,size_t);
	*(void **)(*REAL_cuMemcpyHtoD_v2) = dlsym(RTLD_NEXT, "cuMemcpyHtoD_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoD_v2 ,dstDevice,srcHost,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoH_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcDevice,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(198, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoH_v2)(void *,CUdeviceptr,size_t);
	*(void **)(*REAL_cuMemcpyDtoH_v2) = dlsym(RTLD_NEXT, "cuMemcpyDtoH_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoH_v2 ,dstHost,srcDevice,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoD_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcDevice,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(199, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoD_v2)(CUdeviceptr,CUdeviceptr,size_t);
	*(void **)(*REAL_cuMemcpyDtoD_v2) = dlsym(RTLD_NEXT, "cuMemcpyDtoD_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoD_v2 ,dstDevice,srcDevice,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoA_v2( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcDevice,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(200, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoA_v2)(CUarray,size_t,CUdeviceptr,size_t);
	*(void **)(*REAL_cuMemcpyDtoA_v2) = dlsym(RTLD_NEXT, "cuMemcpyDtoA_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoA_v2 ,dstArray,dstOffset,srcDevice,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoD_v2( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(201, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoD_v2)(CUdeviceptr,CUarray,size_t,size_t);
	*(void **)(*REAL_cuMemcpyAtoD_v2) = dlsym(RTLD_NEXT, "cuMemcpyAtoD_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoD_v2 ,dstDevice,srcArray,srcOffset,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoA_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcHost,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(202, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoA_v2)(CUarray,size_t,const void *,size_t);
	*(void **)(*REAL_cuMemcpyHtoA_v2) = dlsym(RTLD_NEXT, "cuMemcpyHtoA_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoA_v2 ,dstArray,dstOffset,srcHost,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoH_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(203, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoH_v2)(void *,CUarray,size_t,size_t);
	*(void **)(*REAL_cuMemcpyAtoH_v2) = dlsym(RTLD_NEXT, "cuMemcpyAtoH_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoH_v2 ,dstHost,srcArray,srcOffset,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoA_v2( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(204, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoA_v2)(CUarray,size_t,CUarray,size_t,size_t);
	*(void **)(*REAL_cuMemcpyAtoA_v2) = dlsym(RTLD_NEXT, "cuMemcpyAtoA_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoA_v2 ,dstArray,dstOffset,srcArray,srcOffset,ByteCount);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoAAsync_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstArray,(void *)&dstOffset,(void *)&srcHost,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(205, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoAAsync_v2)(CUarray,size_t,const void *,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyHtoAAsync_v2) = dlsym(RTLD_NEXT, "cuMemcpyHtoAAsync_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoAAsync_v2 ,dstArray,dstOffset,srcHost,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyAtoHAsync_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcArray,(void *)&srcOffset,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(206, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyAtoHAsync_v2)(void *,CUarray,size_t,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyAtoHAsync_v2) = dlsym(RTLD_NEXT, "cuMemcpyAtoHAsync_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyAtoHAsync_v2 ,dstHost,srcArray,srcOffset,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy2D_v2( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(207, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy2D_v2)(const CUDA_MEMCPY2D *);
	*(void **)(*REAL_cuMemcpy2D_v2) = dlsym(RTLD_NEXT, "cuMemcpy2D_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpy2D_v2 ,pCopy);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy2DUnaligned_v2( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(208, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy2DUnaligned_v2)(const CUDA_MEMCPY2D *);
	*(void **)(*REAL_cuMemcpy2DUnaligned_v2) = dlsym(RTLD_NEXT, "cuMemcpy2DUnaligned_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpy2DUnaligned_v2 ,pCopy);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy3D_v2( const CUDA_MEMCPY3D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(209, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy3D_v2)(const CUDA_MEMCPY3D *);
	*(void **)(*REAL_cuMemcpy3D_v2) = dlsym(RTLD_NEXT, "cuMemcpy3D_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpy3D_v2 ,pCopy);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyHtoDAsync_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcHost,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(210, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyHtoDAsync_v2)(CUdeviceptr,const void *,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyHtoDAsync_v2) = dlsym(RTLD_NEXT, "cuMemcpyHtoDAsync_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyHtoDAsync_v2 ,dstDevice,srcHost,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoHAsync_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstHost,(void *)&srcDevice,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(211, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoHAsync_v2)(void *,CUdeviceptr,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyDtoHAsync_v2) = dlsym(RTLD_NEXT, "cuMemcpyDtoHAsync_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoHAsync_v2 ,dstHost,srcDevice,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpyDtoDAsync_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&srcDevice,(void *)&ByteCount,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(212, &params));
	CUresult CUDAAPI (*REAL_cuMemcpyDtoDAsync_v2)(CUdeviceptr,CUdeviceptr,size_t,CUstream);
	*(void **)(*REAL_cuMemcpyDtoDAsync_v2) = dlsym(RTLD_NEXT, "cuMemcpyDtoDAsync_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpyDtoDAsync_v2 ,dstDevice,srcDevice,ByteCount,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy2DAsync_v2( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(213, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy2DAsync_v2)(const CUDA_MEMCPY2D *,CUstream);
	*(void **)(*REAL_cuMemcpy2DAsync_v2) = dlsym(RTLD_NEXT, "cuMemcpy2DAsync_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpy2DAsync_v2 ,pCopy,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemcpy3DAsync_v2( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&pCopy,(void *)&hStream };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(214, &params));
	CUresult CUDAAPI (*REAL_cuMemcpy3DAsync_v2)(const CUDA_MEMCPY3D *,CUstream);
	*(void **)(*REAL_cuMemcpy3DAsync_v2) = dlsym(RTLD_NEXT, "cuMemcpy3DAsync_v2");
	DriverAPICall call = std::bind(REAL_cuMemcpy3DAsync_v2 ,pCopy,hStream);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD8_v2( CUdeviceptr dstDevice, unsigned char uc, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&uc,(void *)&N };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(215, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD8_v2)(CUdeviceptr,unsigned char,size_t);
	*(void **)(*REAL_cuMemsetD8_v2) = dlsym(RTLD_NEXT, "cuMemsetD8_v2");
	DriverAPICall call = std::bind(REAL_cuMemsetD8_v2 ,dstDevice,uc,N);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD16_v2( CUdeviceptr dstDevice, unsigned short us, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&us,(void *)&N };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(216, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD16_v2)(CUdeviceptr,unsigned short,size_t);
	*(void **)(*REAL_cuMemsetD16_v2) = dlsym(RTLD_NEXT, "cuMemsetD16_v2");
	DriverAPICall call = std::bind(REAL_cuMemsetD16_v2 ,dstDevice,us,N);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD32_v2( CUdeviceptr dstDevice, unsigned int ui, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&ui,(void *)&N };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(217, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD32_v2)(CUdeviceptr,unsigned int,size_t);
	*(void **)(*REAL_cuMemsetD32_v2) = dlsym(RTLD_NEXT, "cuMemsetD32_v2");
	DriverAPICall call = std::bind(REAL_cuMemsetD32_v2 ,dstDevice,ui,N);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D8_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&uc,(void *)&Width,(void *)&Height };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(218, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D8_v2)(CUdeviceptr,size_t,unsigned char,size_t,size_t);
	*(void **)(*REAL_cuMemsetD2D8_v2) = dlsym(RTLD_NEXT, "cuMemsetD2D8_v2");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D8_v2 ,dstDevice,dstPitch,uc,Width,Height);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D16_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&us,(void *)&Width,(void *)&Height };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(219, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D16_v2)(CUdeviceptr,size_t,unsigned short,size_t,size_t);
	*(void **)(*REAL_cuMemsetD2D16_v2) = dlsym(RTLD_NEXT, "cuMemsetD2D16_v2");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D16_v2 ,dstDevice,dstPitch,us,Width,Height);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}
// This is the call that will take the place of the original
CUresult CUDAAPI cuMemsetD2D32_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	std::vector<void *> params = { (void *)&dstDevice,(void *)&dstPitch,(void *)&ui,(void *)&Width,(void *)&Height };
	std::shared_ptr<ParameterBase> paramsPtr(new ParameterImpl(220, &params));
	CUresult CUDAAPI (*REAL_cuMemsetD2D32_v2)(CUdeviceptr,size_t,unsigned int,size_t,size_t);
	*(void **)(*REAL_cuMemsetD2D32_v2) = dlsym(RTLD_NEXT, "cuMemsetD2D32_v2");
	DriverAPICall call = std::bind(REAL_cuMemsetD2D32_v2 ,dstDevice,dstPitch,ui,Width,Height);
	CUresult CUDAAPI ret = ( CUresult CUDAAPI ) FACTORY_PTR->PerformAction(call, paramsPtr);
	return ret;
}

};