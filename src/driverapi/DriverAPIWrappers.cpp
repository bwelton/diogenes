#include "DriverAPIHeader.h"
#include <tuple>
#include "DriverWrapperBase.h"
#include "DriverWrapperFactory.h"
#include "cuda.h"
#include "gotcha/gotcha.h"
std::shared_ptr<DriverWrapperFactory> DriverFactory;
extern "C" {


// MANUALLY ADDED.....
//extern int ORIGINAL_InternalSynchronization( void * a, void * b, void * c);

int INTER_InternalSynchronization( void * a, void * b, void * c) {
	BUILD_FACTORY
	// std::vector<void **> params = { (void **)&a,(void **)&b, (void**)&c };	
	// std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_InternalSynchronization, (void*) &ORIGINAL_InternalSynchronization, params));
	// int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return 0;
}

void JustGenStackTrace() {
	BUILD_FACTORY
	FACTORY_PTR->PrintStack();
	return;
}


void CheckInit_DriverAPI() {
	
}
// This "function" will be rewritten to point to cuGetErrorString
//int ORIGINAL_cuGetErrorString( CUresult error, const char * * pStr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGetErrorString;
void * PTR_ORIGINAL_cuGetErrorString;
// This is the call that will take the place of the original
int INTER_cuGetErrorString( CUresult error, const char * * pStr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)error,(void *)pStr};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGetErrorString, (void*) PTR_ORIGINAL_cuGetErrorString, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGetErrorName
//int ORIGINAL_cuGetErrorName( CUresult error, const char * * pStr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGetErrorName;
void * PTR_ORIGINAL_cuGetErrorName;
// This is the call that will take the place of the original
int INTER_cuGetErrorName( CUresult error, const char * * pStr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)error,(void *)pStr};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGetErrorName, (void*) PTR_ORIGINAL_cuGetErrorName, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuInit
//int ORIGINAL_cuInit( unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuInit;
void * PTR_ORIGINAL_cuInit;
// This is the call that will take the place of the original
int INTER_cuInit( unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)Flags};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuInit, (void*) PTR_ORIGINAL_cuInit, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDriverGetVersion
//int ORIGINAL_cuDriverGetVersion( int * driverVersion ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDriverGetVersion;
void * PTR_ORIGINAL_cuDriverGetVersion;
// This is the call that will take the place of the original
int INTER_cuDriverGetVersion( int * driverVersion ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)driverVersion};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDriverGetVersion, (void*) PTR_ORIGINAL_cuDriverGetVersion, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGet
//int ORIGINAL_cuDeviceGet( CUdevice * device, int ordinal ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGet;
void * PTR_ORIGINAL_cuDeviceGet;
// This is the call that will take the place of the original
int INTER_cuDeviceGet( CUdevice * device, int ordinal ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)device,(void *)ordinal};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGet, (void*) PTR_ORIGINAL_cuDeviceGet, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGetCount
//int ORIGINAL_cuDeviceGetCount( int * count ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGetCount;
void * PTR_ORIGINAL_cuDeviceGetCount;
// This is the call that will take the place of the original
int INTER_cuDeviceGetCount( int * count ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)count};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGetCount, (void*) PTR_ORIGINAL_cuDeviceGetCount, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGetName
//int ORIGINAL_cuDeviceGetName( char * name, int len, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGetName;
void * PTR_ORIGINAL_cuDeviceGetName;
// This is the call that will take the place of the original
int INTER_cuDeviceGetName( char * name, int len, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)name,(void *)len,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGetName, (void*) PTR_ORIGINAL_cuDeviceGetName, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceTotalMem
//int ORIGINAL_cuDeviceTotalMem( size_t * bytes, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceTotalMem;
void * PTR_ORIGINAL_cuDeviceTotalMem;
// This is the call that will take the place of the original
int INTER_cuDeviceTotalMem( size_t * bytes, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)bytes,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceTotalMem, (void*) PTR_ORIGINAL_cuDeviceTotalMem, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGetAttribute
//int ORIGINAL_cuDeviceGetAttribute( int * pi, CUdevice_attribute attrib, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGetAttribute;
void * PTR_ORIGINAL_cuDeviceGetAttribute;
// This is the call that will take the place of the original
int INTER_cuDeviceGetAttribute( int * pi, CUdevice_attribute attrib, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pi,(void *)attrib,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGetAttribute, (void*) PTR_ORIGINAL_cuDeviceGetAttribute, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGetProperties
//int ORIGINAL_cuDeviceGetProperties( CUdevprop * prop, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGetProperties;
void * PTR_ORIGINAL_cuDeviceGetProperties;
// This is the call that will take the place of the original
int INTER_cuDeviceGetProperties( CUdevprop * prop, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)prop,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGetProperties, (void*) PTR_ORIGINAL_cuDeviceGetProperties, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceComputeCapability
//int ORIGINAL_cuDeviceComputeCapability( int * major, int * minor, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceComputeCapability;
void * PTR_ORIGINAL_cuDeviceComputeCapability;
// This is the call that will take the place of the original
int INTER_cuDeviceComputeCapability( int * major, int * minor, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)major,(void *)minor,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceComputeCapability, (void*) PTR_ORIGINAL_cuDeviceComputeCapability, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDevicePrimaryCtxRetain
//int ORIGINAL_cuDevicePrimaryCtxRetain( CUcontext * pctx, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDevicePrimaryCtxRetain;
void * PTR_ORIGINAL_cuDevicePrimaryCtxRetain;
// This is the call that will take the place of the original
int INTER_cuDevicePrimaryCtxRetain( CUcontext * pctx, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pctx,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDevicePrimaryCtxRetain, (void*) PTR_ORIGINAL_cuDevicePrimaryCtxRetain, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDevicePrimaryCtxRelease
//int ORIGINAL_cuDevicePrimaryCtxRelease( CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDevicePrimaryCtxRelease;
void * PTR_ORIGINAL_cuDevicePrimaryCtxRelease;
// This is the call that will take the place of the original
int INTER_cuDevicePrimaryCtxRelease( CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dev};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDevicePrimaryCtxRelease, (void*) PTR_ORIGINAL_cuDevicePrimaryCtxRelease, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDevicePrimaryCtxSetFlags
//int ORIGINAL_cuDevicePrimaryCtxSetFlags( CUdevice dev, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDevicePrimaryCtxSetFlags;
void * PTR_ORIGINAL_cuDevicePrimaryCtxSetFlags;
// This is the call that will take the place of the original
int INTER_cuDevicePrimaryCtxSetFlags( CUdevice dev, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dev,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDevicePrimaryCtxSetFlags, (void*) PTR_ORIGINAL_cuDevicePrimaryCtxSetFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDevicePrimaryCtxGetState
//int ORIGINAL_cuDevicePrimaryCtxGetState( CUdevice dev, unsigned int * flags, int * active ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDevicePrimaryCtxGetState;
void * PTR_ORIGINAL_cuDevicePrimaryCtxGetState;
// This is the call that will take the place of the original
int INTER_cuDevicePrimaryCtxGetState( CUdevice dev, unsigned int * flags, int * active ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dev,(void *)flags,(void *)active};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDevicePrimaryCtxGetState, (void*) PTR_ORIGINAL_cuDevicePrimaryCtxGetState, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDevicePrimaryCtxReset
//int ORIGINAL_cuDevicePrimaryCtxReset( CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDevicePrimaryCtxReset;
void * PTR_ORIGINAL_cuDevicePrimaryCtxReset;
// This is the call that will take the place of the original
int INTER_cuDevicePrimaryCtxReset( CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dev};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDevicePrimaryCtxReset, (void*) PTR_ORIGINAL_cuDevicePrimaryCtxReset, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxCreate
//int ORIGINAL_cuCtxCreate( CUcontext * pctx, unsigned int flags, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxCreate;
void * PTR_ORIGINAL_cuCtxCreate;
// This is the call that will take the place of the original
int INTER_cuCtxCreate( CUcontext * pctx, unsigned int flags, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pctx,(void *)flags,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxCreate, (void*) PTR_ORIGINAL_cuCtxCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxDestroy
//int ORIGINAL_cuCtxDestroy( CUcontext ctx ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxDestroy;
void * PTR_ORIGINAL_cuCtxDestroy;
// This is the call that will take the place of the original
int INTER_cuCtxDestroy( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)ctx};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxDestroy, (void*) PTR_ORIGINAL_cuCtxDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxPushCurrent
//int ORIGINAL_cuCtxPushCurrent( CUcontext ctx ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxPushCurrent;
void * PTR_ORIGINAL_cuCtxPushCurrent;
// This is the call that will take the place of the original
int INTER_cuCtxPushCurrent( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)ctx};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxPushCurrent, (void*) PTR_ORIGINAL_cuCtxPushCurrent, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxPopCurrent
//int ORIGINAL_cuCtxPopCurrent( CUcontext * pctx ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxPopCurrent;
void * PTR_ORIGINAL_cuCtxPopCurrent;
// This is the call that will take the place of the original
int INTER_cuCtxPopCurrent( CUcontext * pctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pctx};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxPopCurrent, (void*) PTR_ORIGINAL_cuCtxPopCurrent, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxSetCurrent
//int ORIGINAL_cuCtxSetCurrent( CUcontext ctx ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxSetCurrent;
void * PTR_ORIGINAL_cuCtxSetCurrent;
// This is the call that will take the place of the original
int INTER_cuCtxSetCurrent( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)ctx};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxSetCurrent, (void*) PTR_ORIGINAL_cuCtxSetCurrent, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetCurrent
//int ORIGINAL_cuCtxGetCurrent( CUcontext * pctx ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetCurrent;
void * PTR_ORIGINAL_cuCtxGetCurrent;
// This is the call that will take the place of the original
int INTER_cuCtxGetCurrent( CUcontext * pctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pctx};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetCurrent, (void*) PTR_ORIGINAL_cuCtxGetCurrent, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetDevice
//int ORIGINAL_cuCtxGetDevice( CUdevice * device ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetDevice;
void * PTR_ORIGINAL_cuCtxGetDevice;
// This is the call that will take the place of the original
int INTER_cuCtxGetDevice( CUdevice * device ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)device};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetDevice, (void*) PTR_ORIGINAL_cuCtxGetDevice, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetFlags
//int ORIGINAL_cuCtxGetFlags( unsigned int * flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetFlags;
void * PTR_ORIGINAL_cuCtxGetFlags;
// This is the call that will take the place of the original
int INTER_cuCtxGetFlags( unsigned int * flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)flags};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetFlags, (void*) PTR_ORIGINAL_cuCtxGetFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxSynchronize
//int ORIGINAL_cuCtxSynchronize( void  ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxSynchronize;
void * PTR_ORIGINAL_cuCtxSynchronize;
// This is the call that will take the place of the original
int INTER_cuCtxSynchronize( void  ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { };
	std::vector<void **> params = { };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxSynchronize, (void*) PTR_ORIGINAL_cuCtxSynchronize, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxSetLimit
//int ORIGINAL_cuCtxSetLimit( CUlimit limit, size_t value ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxSetLimit;
void * PTR_ORIGINAL_cuCtxSetLimit;
// This is the call that will take the place of the original
int INTER_cuCtxSetLimit( CUlimit limit, size_t value ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)limit,(void *)value};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxSetLimit, (void*) PTR_ORIGINAL_cuCtxSetLimit, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetLimit
//int ORIGINAL_cuCtxGetLimit( size_t * pvalue, CUlimit limit ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetLimit;
void * PTR_ORIGINAL_cuCtxGetLimit;
// This is the call that will take the place of the original
int INTER_cuCtxGetLimit( size_t * pvalue, CUlimit limit ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pvalue,(void *)limit};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetLimit, (void*) PTR_ORIGINAL_cuCtxGetLimit, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetCacheConfig
//int ORIGINAL_cuCtxGetCacheConfig( CUfunc_cache * pconfig ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetCacheConfig;
void * PTR_ORIGINAL_cuCtxGetCacheConfig;
// This is the call that will take the place of the original
int INTER_cuCtxGetCacheConfig( CUfunc_cache * pconfig ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pconfig};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetCacheConfig, (void*) PTR_ORIGINAL_cuCtxGetCacheConfig, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxSetCacheConfig
//int ORIGINAL_cuCtxSetCacheConfig( CUfunc_cache config ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxSetCacheConfig;
void * PTR_ORIGINAL_cuCtxSetCacheConfig;
// This is the call that will take the place of the original
int INTER_cuCtxSetCacheConfig( CUfunc_cache config ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)config};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxSetCacheConfig, (void*) PTR_ORIGINAL_cuCtxSetCacheConfig, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetSharedMemConfig
//int ORIGINAL_cuCtxGetSharedMemConfig( CUsharedconfig * pConfig ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetSharedMemConfig;
void * PTR_ORIGINAL_cuCtxGetSharedMemConfig;
// This is the call that will take the place of the original
int INTER_cuCtxGetSharedMemConfig( CUsharedconfig * pConfig ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pConfig};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetSharedMemConfig, (void*) PTR_ORIGINAL_cuCtxGetSharedMemConfig, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxSetSharedMemConfig
//int ORIGINAL_cuCtxSetSharedMemConfig( CUsharedconfig config ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxSetSharedMemConfig;
void * PTR_ORIGINAL_cuCtxSetSharedMemConfig;
// This is the call that will take the place of the original
int INTER_cuCtxSetSharedMemConfig( CUsharedconfig config ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)config};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxSetSharedMemConfig, (void*) PTR_ORIGINAL_cuCtxSetSharedMemConfig, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetApiVersion
//int ORIGINAL_cuCtxGetApiVersion( CUcontext ctx, unsigned int * version ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetApiVersion;
void * PTR_ORIGINAL_cuCtxGetApiVersion;
// This is the call that will take the place of the original
int INTER_cuCtxGetApiVersion( CUcontext ctx, unsigned int * version ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)ctx,(void *)version};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetApiVersion, (void*) PTR_ORIGINAL_cuCtxGetApiVersion, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxGetStreamPriorityRange
//int ORIGINAL_cuCtxGetStreamPriorityRange( int * leastPriority, int * greatestPriority ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxGetStreamPriorityRange;
void * PTR_ORIGINAL_cuCtxGetStreamPriorityRange;
// This is the call that will take the place of the original
int INTER_cuCtxGetStreamPriorityRange( int * leastPriority, int * greatestPriority ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)leastPriority,(void *)greatestPriority};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxGetStreamPriorityRange, (void*) PTR_ORIGINAL_cuCtxGetStreamPriorityRange, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxAttach
//int ORIGINAL_cuCtxAttach( CUcontext * pctx, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxAttach;
void * PTR_ORIGINAL_cuCtxAttach;
// This is the call that will take the place of the original
int INTER_cuCtxAttach( CUcontext * pctx, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pctx,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxAttach, (void*) PTR_ORIGINAL_cuCtxAttach, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxDetach
//int ORIGINAL_cuCtxDetach( CUcontext ctx ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxDetach;
void * PTR_ORIGINAL_cuCtxDetach;
// This is the call that will take the place of the original
int INTER_cuCtxDetach( CUcontext ctx ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)ctx};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxDetach, (void*) PTR_ORIGINAL_cuCtxDetach, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleLoad
//int ORIGINAL_cuModuleLoad( CUmodule * module, const char * fname ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleLoad;
void * PTR_ORIGINAL_cuModuleLoad;
// This is the call that will take the place of the original
int INTER_cuModuleLoad( CUmodule * module, const char * fname ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)module,(void *)fname};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleLoad, (void*) PTR_ORIGINAL_cuModuleLoad, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleLoadData
//int ORIGINAL_cuModuleLoadData( CUmodule * module, const void * image ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleLoadData;
void * PTR_ORIGINAL_cuModuleLoadData;
// This is the call that will take the place of the original
int INTER_cuModuleLoadData( CUmodule * module, const void * image ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)module,(void *)image};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleLoadData, (void*) PTR_ORIGINAL_cuModuleLoadData, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleLoadDataEx
//int ORIGINAL_cuModuleLoadDataEx( CUmodule * module, const void * image, unsigned int numOptions, CUjit_option * options, void * * optionValues ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleLoadDataEx;
void * PTR_ORIGINAL_cuModuleLoadDataEx;
// This is the call that will take the place of the original
int INTER_cuModuleLoadDataEx( CUmodule * module, const void * image, unsigned int numOptions, CUjit_option * options, void * * optionValues ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)module,(void *)image,(void *)numOptions,(void *)options,(void *)optionValues};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleLoadDataEx, (void*) PTR_ORIGINAL_cuModuleLoadDataEx, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleLoadFatBinary
//int ORIGINAL_cuModuleLoadFatBinary( CUmodule * module, const void * fatCubin ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleLoadFatBinary;
void * PTR_ORIGINAL_cuModuleLoadFatBinary;
// This is the call that will take the place of the original
int INTER_cuModuleLoadFatBinary( CUmodule * module, const void * fatCubin ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)module,(void *)fatCubin};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleLoadFatBinary, (void*) PTR_ORIGINAL_cuModuleLoadFatBinary, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleUnload
//int ORIGINAL_cuModuleUnload( CUmodule hmod ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleUnload;
void * PTR_ORIGINAL_cuModuleUnload;
// This is the call that will take the place of the original
int INTER_cuModuleUnload( CUmodule hmod ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hmod};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleUnload, (void*) PTR_ORIGINAL_cuModuleUnload, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleGetFunction
//int ORIGINAL_cuModuleGetFunction( CUfunction * hfunc, CUmodule hmod, const char * name ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleGetFunction;
void * PTR_ORIGINAL_cuModuleGetFunction;
// This is the call that will take the place of the original
int INTER_cuModuleGetFunction( CUfunction * hfunc, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)hmod,(void *)name};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleGetFunction, (void*) PTR_ORIGINAL_cuModuleGetFunction, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleGetGlobal
//int ORIGINAL_cuModuleGetGlobal( CUdeviceptr * dptr, size_t * bytes, CUmodule hmod, const char * name ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleGetGlobal;
void * PTR_ORIGINAL_cuModuleGetGlobal;
// This is the call that will take the place of the original
int INTER_cuModuleGetGlobal( CUdeviceptr * dptr, size_t * bytes, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dptr,(void *)bytes,(void *)hmod,(void *)name};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleGetGlobal, (void*) PTR_ORIGINAL_cuModuleGetGlobal, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleGetTexRef
//int ORIGINAL_cuModuleGetTexRef( CUtexref * pTexRef, CUmodule hmod, const char * name ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleGetTexRef;
void * PTR_ORIGINAL_cuModuleGetTexRef;
// This is the call that will take the place of the original
int INTER_cuModuleGetTexRef( CUtexref * pTexRef, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pTexRef,(void *)hmod,(void *)name};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleGetTexRef, (void*) PTR_ORIGINAL_cuModuleGetTexRef, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuModuleGetSurfRef
//int ORIGINAL_cuModuleGetSurfRef( CUsurfref * pSurfRef, CUmodule hmod, const char * name ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuModuleGetSurfRef;
void * PTR_ORIGINAL_cuModuleGetSurfRef;
// This is the call that will take the place of the original
int INTER_cuModuleGetSurfRef( CUsurfref * pSurfRef, CUmodule hmod, const char * name ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pSurfRef,(void *)hmod,(void *)name};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuModuleGetSurfRef, (void*) PTR_ORIGINAL_cuModuleGetSurfRef, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLinkCreate
//int ORIGINAL_cuLinkCreate( unsigned int numOptions, CUjit_option * options, void * * optionValues, CUlinkState * stateOut ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLinkCreate;
void * PTR_ORIGINAL_cuLinkCreate;
// This is the call that will take the place of the original
int INTER_cuLinkCreate( unsigned int numOptions, CUjit_option * options, void * * optionValues, CUlinkState * stateOut ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)numOptions,(void *)options,(void *)optionValues,(void *)stateOut};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLinkCreate, (void*) PTR_ORIGINAL_cuLinkCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLinkAddData
//int ORIGINAL_cuLinkAddData( CUlinkState state, CUjitInputType type, void * data, size_t size, const char * name, unsigned int numOptions, CUjit_option * options, void * * optionValues ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLinkAddData;
void * PTR_ORIGINAL_cuLinkAddData;
// This is the call that will take the place of the original
int INTER_cuLinkAddData( CUlinkState state, CUjitInputType type, void * data, size_t size, const char * name, unsigned int numOptions, CUjit_option * options, void * * optionValues ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)state,(void *)type,(void *)data,(void *)size,(void *)name,(void *)numOptions,(void *)options,(void *)optionValues};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5],&tmpParams[6],&tmpParams[7] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLinkAddData, (void*) PTR_ORIGINAL_cuLinkAddData, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLinkAddFile
//int ORIGINAL_cuLinkAddFile( CUlinkState state, CUjitInputType type, const char * path, unsigned int numOptions, CUjit_option * options, void * * optionValues ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLinkAddFile;
void * PTR_ORIGINAL_cuLinkAddFile;
// This is the call that will take the place of the original
int INTER_cuLinkAddFile( CUlinkState state, CUjitInputType type, const char * path, unsigned int numOptions, CUjit_option * options, void * * optionValues ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)state,(void *)type,(void *)path,(void *)numOptions,(void *)options,(void *)optionValues};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLinkAddFile, (void*) PTR_ORIGINAL_cuLinkAddFile, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLinkComplete
//int ORIGINAL_cuLinkComplete( CUlinkState state, void * * cubinOut, size_t * sizeOut ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLinkComplete;
void * PTR_ORIGINAL_cuLinkComplete;
// This is the call that will take the place of the original
int INTER_cuLinkComplete( CUlinkState state, void * * cubinOut, size_t * sizeOut ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)state,(void *)cubinOut,(void *)sizeOut};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLinkComplete, (void*) PTR_ORIGINAL_cuLinkComplete, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLinkDestroy
//int ORIGINAL_cuLinkDestroy( CUlinkState state ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLinkDestroy;
void * PTR_ORIGINAL_cuLinkDestroy;
// This is the call that will take the place of the original
int INTER_cuLinkDestroy( CUlinkState state ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)state};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLinkDestroy, (void*) PTR_ORIGINAL_cuLinkDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemGetInfo
//int ORIGINAL_cuMemGetInfo( size_t * free, size_t * total ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemGetInfo;
void * PTR_ORIGINAL_cuMemGetInfo;
// This is the call that will take the place of the original
int INTER_cuMemGetInfo( size_t * free, size_t * total ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)free,(void *)total};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemGetInfo, (void*) PTR_ORIGINAL_cuMemGetInfo, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemAlloc
//int ORIGINAL_cuMemAlloc( CUdeviceptr * dptr, size_t bytesize ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemAlloc;
void * PTR_ORIGINAL_cuMemAlloc;
// This is the call that will take the place of the original
int INTER_cuMemAlloc( CUdeviceptr * dptr, size_t bytesize ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dptr,(void *)bytesize};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemAlloc, (void*) PTR_ORIGINAL_cuMemAlloc, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemAllocPitch
//int ORIGINAL_cuMemAllocPitch( CUdeviceptr * dptr, size_t * pPitch, size_t WidthInBytes, size_t Height, unsigned int ElementSizeBytes ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemAllocPitch;
void * PTR_ORIGINAL_cuMemAllocPitch;
// This is the call that will take the place of the original
int INTER_cuMemAllocPitch( CUdeviceptr * dptr, size_t * pPitch, size_t WidthInBytes, size_t Height, unsigned int ElementSizeBytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dptr,(void *)pPitch,(void *)WidthInBytes,(void *)Height,(void *)ElementSizeBytes};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemAllocPitch, (void*) PTR_ORIGINAL_cuMemAllocPitch, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemFree
//int ORIGINAL_cuMemFree( CUdeviceptr dptr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemFree;
void * PTR_ORIGINAL_cuMemFree;
// This is the call that will take the place of the original
int INTER_cuMemFree( CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dptr};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemFree, (void*) PTR_ORIGINAL_cuMemFree, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemGetAddressRange
//int ORIGINAL_cuMemGetAddressRange( CUdeviceptr * pbase, size_t * psize, CUdeviceptr dptr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemGetAddressRange;
void * PTR_ORIGINAL_cuMemGetAddressRange;
// This is the call that will take the place of the original
int INTER_cuMemGetAddressRange( CUdeviceptr * pbase, size_t * psize, CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pbase,(void *)psize,(void *)dptr};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemGetAddressRange, (void*) PTR_ORIGINAL_cuMemGetAddressRange, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemAllocHost
//int ORIGINAL_cuMemAllocHost( void * * pp, size_t bytesize ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemAllocHost;
void * PTR_ORIGINAL_cuMemAllocHost;
// This is the call that will take the place of the original
int INTER_cuMemAllocHost( void * * pp, size_t bytesize ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pp,(void *)bytesize};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemAllocHost, (void*) PTR_ORIGINAL_cuMemAllocHost, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemFreeHost
//int ORIGINAL_cuMemFreeHost( void * p ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemFreeHost;
void * PTR_ORIGINAL_cuMemFreeHost;
// This is the call that will take the place of the original
int INTER_cuMemFreeHost( void * p ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)p};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemFreeHost, (void*) PTR_ORIGINAL_cuMemFreeHost, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemHostAlloc
//int ORIGINAL_cuMemHostAlloc( void * * pp, size_t bytesize, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemHostAlloc;
void * PTR_ORIGINAL_cuMemHostAlloc;
// This is the call that will take the place of the original
int INTER_cuMemHostAlloc( void * * pp, size_t bytesize, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pp,(void *)bytesize,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemHostAlloc, (void*) PTR_ORIGINAL_cuMemHostAlloc, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemHostGetDevicePointer
//int ORIGINAL_cuMemHostGetDevicePointer( CUdeviceptr * pdptr, void * p, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemHostGetDevicePointer;
void * PTR_ORIGINAL_cuMemHostGetDevicePointer;
// This is the call that will take the place of the original
int INTER_cuMemHostGetDevicePointer( CUdeviceptr * pdptr, void * p, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pdptr,(void *)p,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemHostGetDevicePointer, (void*) PTR_ORIGINAL_cuMemHostGetDevicePointer, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemHostGetFlags
//int ORIGINAL_cuMemHostGetFlags( unsigned int * pFlags, void * p ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemHostGetFlags;
void * PTR_ORIGINAL_cuMemHostGetFlags;
// This is the call that will take the place of the original
int INTER_cuMemHostGetFlags( unsigned int * pFlags, void * p ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pFlags,(void *)p};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemHostGetFlags, (void*) PTR_ORIGINAL_cuMemHostGetFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemAllocManaged
//int ORIGINAL_cuMemAllocManaged( CUdeviceptr * dptr, size_t bytesize, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemAllocManaged;
void * PTR_ORIGINAL_cuMemAllocManaged;
// This is the call that will take the place of the original
int INTER_cuMemAllocManaged( CUdeviceptr * dptr, size_t bytesize, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dptr,(void *)bytesize,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemAllocManaged, (void*) PTR_ORIGINAL_cuMemAllocManaged, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGetByPCIBusId
//int ORIGINAL_cuDeviceGetByPCIBusId( CUdevice * dev, const char * pciBusId ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGetByPCIBusId;
void * PTR_ORIGINAL_cuDeviceGetByPCIBusId;
// This is the call that will take the place of the original
int INTER_cuDeviceGetByPCIBusId( CUdevice * dev, const char * pciBusId ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dev,(void *)pciBusId};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGetByPCIBusId, (void*) PTR_ORIGINAL_cuDeviceGetByPCIBusId, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGetPCIBusId
//int ORIGINAL_cuDeviceGetPCIBusId( char * pciBusId, int len, CUdevice dev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGetPCIBusId;
void * PTR_ORIGINAL_cuDeviceGetPCIBusId;
// This is the call that will take the place of the original
int INTER_cuDeviceGetPCIBusId( char * pciBusId, int len, CUdevice dev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pciBusId,(void *)len,(void *)dev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGetPCIBusId, (void*) PTR_ORIGINAL_cuDeviceGetPCIBusId, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuIpcGetEventHandle
//int ORIGINAL_cuIpcGetEventHandle( CUipcEventHandle * pHandle, CUevent event ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuIpcGetEventHandle;
void * PTR_ORIGINAL_cuIpcGetEventHandle;
// This is the call that will take the place of the original
int INTER_cuIpcGetEventHandle( CUipcEventHandle * pHandle, CUevent event ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pHandle,(void *)event};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuIpcGetEventHandle, (void*) PTR_ORIGINAL_cuIpcGetEventHandle, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuIpcOpenEventHandle
//int ORIGINAL_cuIpcOpenEventHandle( CUevent * phEvent, CUipcEventHandle handle ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuIpcOpenEventHandle;
void * PTR_ORIGINAL_cuIpcOpenEventHandle;
// This is the call that will take the place of the original
int INTER_cuIpcOpenEventHandle( CUevent * phEvent, CUipcEventHandle handle ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)phEvent};
	std::vector<void **> params = { &tmpParams[0],(void**)&handle};
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuIpcOpenEventHandle, (void*) PTR_ORIGINAL_cuIpcOpenEventHandle, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuIpcGetMemHandle
//int ORIGINAL_cuIpcGetMemHandle( CUipcMemHandle * pHandle, CUdeviceptr dptr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuIpcGetMemHandle;
void * PTR_ORIGINAL_cuIpcGetMemHandle;
// This is the call that will take the place of the original
int INTER_cuIpcGetMemHandle( CUipcMemHandle * pHandle, CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pHandle,(void *)dptr};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuIpcGetMemHandle, (void*) PTR_ORIGINAL_cuIpcGetMemHandle, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuIpcOpenMemHandle
//int ORIGINAL_cuIpcOpenMemHandle( CUdeviceptr * pdptr, CUipcMemHandle handle, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuIpcOpenMemHandle;
void * PTR_ORIGINAL_cuIpcOpenMemHandle;
// This is the call that will take the place of the original
int INTER_cuIpcOpenMemHandle( CUdeviceptr * pdptr, CUipcMemHandle handle, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pdptr, (void *)Flags};
	std::vector<void **> params = { &tmpParams[0],(void **)&handle,&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuIpcOpenMemHandle, (void*) PTR_ORIGINAL_cuIpcOpenMemHandle, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuIpcCloseMemHandle
//int ORIGINAL_cuIpcCloseMemHandle( CUdeviceptr dptr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuIpcCloseMemHandle;
void * PTR_ORIGINAL_cuIpcCloseMemHandle;
// This is the call that will take the place of the original
int INTER_cuIpcCloseMemHandle( CUdeviceptr dptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dptr};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuIpcCloseMemHandle, (void*) PTR_ORIGINAL_cuIpcCloseMemHandle, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemHostRegister
//int ORIGINAL_cuMemHostRegister( void * p, size_t bytesize, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemHostRegister;
void * PTR_ORIGINAL_cuMemHostRegister;
// This is the call that will take the place of the original
int INTER_cuMemHostRegister( void * p, size_t bytesize, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)p,(void *)bytesize,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemHostRegister, (void*) PTR_ORIGINAL_cuMemHostRegister, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemHostUnregister
//int ORIGINAL_cuMemHostUnregister( void * p ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemHostUnregister;
void * PTR_ORIGINAL_cuMemHostUnregister;
// This is the call that will take the place of the original
int INTER_cuMemHostUnregister( void * p ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)p};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemHostUnregister, (void*) PTR_ORIGINAL_cuMemHostUnregister, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy
//int ORIGINAL_cuMemcpy( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy;
void * PTR_ORIGINAL_cuMemcpy;
// This is the call that will take the place of the original
int INTER_cuMemcpy( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dst,(void *)src,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy, (void*) PTR_ORIGINAL_cuMemcpy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyPeer
//int ORIGINAL_cuMemcpyPeer( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyPeer;
void * PTR_ORIGINAL_cuMemcpyPeer;
// This is the call that will take the place of the original
int INTER_cuMemcpyPeer( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstContext,(void *)srcDevice,(void *)srcContext,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyPeer, (void*) PTR_ORIGINAL_cuMemcpyPeer, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoD
//int ORIGINAL_cuMemcpyHtoD( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoD;
void * PTR_ORIGINAL_cuMemcpyHtoD;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoD( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcHost,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoD, (void*) PTR_ORIGINAL_cuMemcpyHtoD, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoH
//int ORIGINAL_cuMemcpyDtoH( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoH;
void * PTR_ORIGINAL_cuMemcpyDtoH;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoH( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcDevice,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoH, (void*) PTR_ORIGINAL_cuMemcpyDtoH, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoD
//int ORIGINAL_cuMemcpyDtoD( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoD;
void * PTR_ORIGINAL_cuMemcpyDtoD;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoD( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcDevice,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoD, (void*) PTR_ORIGINAL_cuMemcpyDtoD, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoA
//int ORIGINAL_cuMemcpyDtoA( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoA;
void * PTR_ORIGINAL_cuMemcpyDtoA;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoA( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcDevice,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoA, (void*) PTR_ORIGINAL_cuMemcpyDtoA, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoD
//int ORIGINAL_cuMemcpyAtoD( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoD;
void * PTR_ORIGINAL_cuMemcpyAtoD;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoD( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcArray,(void *)srcOffset,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoD, (void*) PTR_ORIGINAL_cuMemcpyAtoD, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoA
//int ORIGINAL_cuMemcpyHtoA( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoA;
void * PTR_ORIGINAL_cuMemcpyHtoA;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoA( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcHost,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoA, (void*) PTR_ORIGINAL_cuMemcpyHtoA, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoH
//int ORIGINAL_cuMemcpyAtoH( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoH;
void * PTR_ORIGINAL_cuMemcpyAtoH;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoH( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcArray,(void *)srcOffset,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoH, (void*) PTR_ORIGINAL_cuMemcpyAtoH, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoA
//int ORIGINAL_cuMemcpyAtoA( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoA;
void * PTR_ORIGINAL_cuMemcpyAtoA;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoA( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcArray,(void *)srcOffset,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoA, (void*) PTR_ORIGINAL_cuMemcpyAtoA, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy2D
//int ORIGINAL_cuMemcpy2D( const CUDA_MEMCPY2D * pCopy ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy2D;
void * PTR_ORIGINAL_cuMemcpy2D;
// This is the call that will take the place of the original
int INTER_cuMemcpy2D( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy2D, (void*) PTR_ORIGINAL_cuMemcpy2D, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy2DUnaligned
//int ORIGINAL_cuMemcpy2DUnaligned( const CUDA_MEMCPY2D * pCopy ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy2DUnaligned;
void * PTR_ORIGINAL_cuMemcpy2DUnaligned;
// This is the call that will take the place of the original
int INTER_cuMemcpy2DUnaligned( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy2DUnaligned, (void*) PTR_ORIGINAL_cuMemcpy2DUnaligned, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy3D
//int ORIGINAL_cuMemcpy3D( const CUDA_MEMCPY3D * pCopy ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy3D;
void * PTR_ORIGINAL_cuMemcpy3D;
// This is the call that will take the place of the original
int INTER_cuMemcpy3D( const CUDA_MEMCPY3D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy3D, (void*) PTR_ORIGINAL_cuMemcpy3D, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy3DPeer
//int ORIGINAL_cuMemcpy3DPeer( const CUDA_MEMCPY3D_PEER * pCopy ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy3DPeer;
void * PTR_ORIGINAL_cuMemcpy3DPeer;
// This is the call that will take the place of the original
int INTER_cuMemcpy3DPeer( const CUDA_MEMCPY3D_PEER * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy3DPeer, (void*) PTR_ORIGINAL_cuMemcpy3DPeer, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAsync
//int ORIGINAL_cuMemcpyAsync( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAsync;
void * PTR_ORIGINAL_cuMemcpyAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpyAsync( CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dst,(void *)src,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAsync, (void*) PTR_ORIGINAL_cuMemcpyAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyPeerAsync
//int ORIGINAL_cuMemcpyPeerAsync( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyPeerAsync;
void * PTR_ORIGINAL_cuMemcpyPeerAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpyPeerAsync( CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstContext,(void *)srcDevice,(void *)srcContext,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyPeerAsync, (void*) PTR_ORIGINAL_cuMemcpyPeerAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoDAsync
//int ORIGINAL_cuMemcpyHtoDAsync( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoDAsync;
void * PTR_ORIGINAL_cuMemcpyHtoDAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoDAsync( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcHost,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoDAsync, (void*) PTR_ORIGINAL_cuMemcpyHtoDAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoHAsync
//int ORIGINAL_cuMemcpyDtoHAsync( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoHAsync;
void * PTR_ORIGINAL_cuMemcpyDtoHAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoHAsync( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcDevice,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoHAsync, (void*) PTR_ORIGINAL_cuMemcpyDtoHAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoDAsync
//int ORIGINAL_cuMemcpyDtoDAsync( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoDAsync;
void * PTR_ORIGINAL_cuMemcpyDtoDAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoDAsync( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcDevice,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoDAsync, (void*) PTR_ORIGINAL_cuMemcpyDtoDAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoAAsync
//int ORIGINAL_cuMemcpyHtoAAsync( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoAAsync;
void * PTR_ORIGINAL_cuMemcpyHtoAAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoAAsync( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcHost,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoAAsync, (void*) PTR_ORIGINAL_cuMemcpyHtoAAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoHAsync
//int ORIGINAL_cuMemcpyAtoHAsync( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoHAsync;
void * PTR_ORIGINAL_cuMemcpyAtoHAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoHAsync( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcArray,(void *)srcOffset,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoHAsync, (void*) PTR_ORIGINAL_cuMemcpyAtoHAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy2DAsync
//int ORIGINAL_cuMemcpy2DAsync( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy2DAsync;
void * PTR_ORIGINAL_cuMemcpy2DAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpy2DAsync( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy2DAsync, (void*) PTR_ORIGINAL_cuMemcpy2DAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy3DAsync
//int ORIGINAL_cuMemcpy3DAsync( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy3DAsync;
void * PTR_ORIGINAL_cuMemcpy3DAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpy3DAsync( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy3DAsync, (void*) PTR_ORIGINAL_cuMemcpy3DAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy3DPeerAsync
//int ORIGINAL_cuMemcpy3DPeerAsync( const CUDA_MEMCPY3D_PEER * pCopy, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy3DPeerAsync;
void * PTR_ORIGINAL_cuMemcpy3DPeerAsync;
// This is the call that will take the place of the original
int INTER_cuMemcpy3DPeerAsync( const CUDA_MEMCPY3D_PEER * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy3DPeerAsync, (void*) PTR_ORIGINAL_cuMemcpy3DPeerAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD8
//int ORIGINAL_cuMemsetD8( CUdeviceptr dstDevice, unsigned char uc, size_t N ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD8;
void * PTR_ORIGINAL_cuMemsetD8;
// This is the call that will take the place of the original
int INTER_cuMemsetD8( CUdeviceptr dstDevice, unsigned char uc, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)uc,(void *)N};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD8, (void*) PTR_ORIGINAL_cuMemsetD8, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD16
//int ORIGINAL_cuMemsetD16( CUdeviceptr dstDevice, unsigned short us, size_t N ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD16;
void * PTR_ORIGINAL_cuMemsetD16;
// This is the call that will take the place of the original
int INTER_cuMemsetD16( CUdeviceptr dstDevice, unsigned short us, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)us,(void *)N};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD16, (void*) PTR_ORIGINAL_cuMemsetD16, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD32
//int ORIGINAL_cuMemsetD32( CUdeviceptr dstDevice, unsigned int ui, size_t N ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD32;
void * PTR_ORIGINAL_cuMemsetD32;
// This is the call that will take the place of the original
int INTER_cuMemsetD32( CUdeviceptr dstDevice, unsigned int ui, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)ui,(void *)N};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD32, (void*) PTR_ORIGINAL_cuMemsetD32, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D8
//int ORIGINAL_cuMemsetD2D8( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D8;
void * PTR_ORIGINAL_cuMemsetD2D8;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D8( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)uc,(void *)Width,(void *)Height};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D8, (void*) PTR_ORIGINAL_cuMemsetD2D8, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D16
//int ORIGINAL_cuMemsetD2D16( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D16;
void * PTR_ORIGINAL_cuMemsetD2D16;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D16( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)us,(void *)Width,(void *)Height};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D16, (void*) PTR_ORIGINAL_cuMemsetD2D16, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D32
//int ORIGINAL_cuMemsetD2D32( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D32;
void * PTR_ORIGINAL_cuMemsetD2D32;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D32( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)ui,(void *)Width,(void *)Height};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D32, (void*) PTR_ORIGINAL_cuMemsetD2D32, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD8Async
//int ORIGINAL_cuMemsetD8Async( CUdeviceptr dstDevice, unsigned char uc, size_t N, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD8Async;
void * PTR_ORIGINAL_cuMemsetD8Async;
// This is the call that will take the place of the original
int INTER_cuMemsetD8Async( CUdeviceptr dstDevice, unsigned char uc, size_t N, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)uc,(void *)N,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD8Async, (void*) PTR_ORIGINAL_cuMemsetD8Async, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD16Async
//int ORIGINAL_cuMemsetD16Async( CUdeviceptr dstDevice, unsigned short us, size_t N, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD16Async;
void * PTR_ORIGINAL_cuMemsetD16Async;
// This is the call that will take the place of the original
int INTER_cuMemsetD16Async( CUdeviceptr dstDevice, unsigned short us, size_t N, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)us,(void *)N,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD16Async, (void*) PTR_ORIGINAL_cuMemsetD16Async, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD32Async
//int ORIGINAL_cuMemsetD32Async( CUdeviceptr dstDevice, unsigned int ui, size_t N, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD32Async;
void * PTR_ORIGINAL_cuMemsetD32Async;
// This is the call that will take the place of the original
int INTER_cuMemsetD32Async( CUdeviceptr dstDevice, unsigned int ui, size_t N, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)ui,(void *)N,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD32Async, (void*) PTR_ORIGINAL_cuMemsetD32Async, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D8Async
//int ORIGINAL_cuMemsetD2D8Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D8Async;
void * PTR_ORIGINAL_cuMemsetD2D8Async;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D8Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)uc,(void *)Width,(void *)Height,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D8Async, (void*) PTR_ORIGINAL_cuMemsetD2D8Async, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D16Async
//int ORIGINAL_cuMemsetD2D16Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D16Async;
void * PTR_ORIGINAL_cuMemsetD2D16Async;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D16Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)us,(void *)Width,(void *)Height,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D16Async, (void*) PTR_ORIGINAL_cuMemsetD2D16Async, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D32Async
//int ORIGINAL_cuMemsetD2D32Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D32Async;
void * PTR_ORIGINAL_cuMemsetD2D32Async;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D32Async( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)ui,(void *)Width,(void *)Height,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D32Async, (void*) PTR_ORIGINAL_cuMemsetD2D32Async, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuArrayCreate
//int ORIGINAL_cuArrayCreate( CUarray * pHandle, const CUDA_ARRAY_DESCRIPTOR * pAllocateArray ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuArrayCreate;
void * PTR_ORIGINAL_cuArrayCreate;
// This is the call that will take the place of the original
int INTER_cuArrayCreate( CUarray * pHandle, const CUDA_ARRAY_DESCRIPTOR * pAllocateArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pHandle,(void *)pAllocateArray};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuArrayCreate, (void*) PTR_ORIGINAL_cuArrayCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuArrayGetDescriptor
//int ORIGINAL_cuArrayGetDescriptor( CUDA_ARRAY_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuArrayGetDescriptor;
void * PTR_ORIGINAL_cuArrayGetDescriptor;
// This is the call that will take the place of the original
int INTER_cuArrayGetDescriptor( CUDA_ARRAY_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pArrayDescriptor,(void *)hArray};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuArrayGetDescriptor, (void*) PTR_ORIGINAL_cuArrayGetDescriptor, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuArrayDestroy
//int ORIGINAL_cuArrayDestroy( CUarray hArray ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuArrayDestroy;
void * PTR_ORIGINAL_cuArrayDestroy;
// This is the call that will take the place of the original
int INTER_cuArrayDestroy( CUarray hArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hArray};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuArrayDestroy, (void*) PTR_ORIGINAL_cuArrayDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuArray3DCreate
//int ORIGINAL_cuArray3DCreate( CUarray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pAllocateArray ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuArray3DCreate;
void * PTR_ORIGINAL_cuArray3DCreate;
// This is the call that will take the place of the original
int INTER_cuArray3DCreate( CUarray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pAllocateArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pHandle,(void *)pAllocateArray};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuArray3DCreate, (void*) PTR_ORIGINAL_cuArray3DCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuArray3DGetDescriptor
//int ORIGINAL_cuArray3DGetDescriptor( CUDA_ARRAY3D_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuArray3DGetDescriptor;
void * PTR_ORIGINAL_cuArray3DGetDescriptor;
// This is the call that will take the place of the original
int INTER_cuArray3DGetDescriptor( CUDA_ARRAY3D_DESCRIPTOR * pArrayDescriptor, CUarray hArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pArrayDescriptor,(void *)hArray};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuArray3DGetDescriptor, (void*) PTR_ORIGINAL_cuArray3DGetDescriptor, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMipmappedArrayCreate
//int ORIGINAL_cuMipmappedArrayCreate( CUmipmappedArray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pMipmappedArrayDesc, unsigned int numMipmapLevels ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMipmappedArrayCreate;
void * PTR_ORIGINAL_cuMipmappedArrayCreate;
// This is the call that will take the place of the original
int INTER_cuMipmappedArrayCreate( CUmipmappedArray * pHandle, const CUDA_ARRAY3D_DESCRIPTOR * pMipmappedArrayDesc, unsigned int numMipmapLevels ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pHandle,(void *)pMipmappedArrayDesc,(void *)numMipmapLevels};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMipmappedArrayCreate, (void*) PTR_ORIGINAL_cuMipmappedArrayCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMipmappedArrayGetLevel
//int ORIGINAL_cuMipmappedArrayGetLevel( CUarray * pLevelArray, CUmipmappedArray hMipmappedArray, unsigned int level ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMipmappedArrayGetLevel;
void * PTR_ORIGINAL_cuMipmappedArrayGetLevel;
// This is the call that will take the place of the original
int INTER_cuMipmappedArrayGetLevel( CUarray * pLevelArray, CUmipmappedArray hMipmappedArray, unsigned int level ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pLevelArray,(void *)hMipmappedArray,(void *)level};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMipmappedArrayGetLevel, (void*) PTR_ORIGINAL_cuMipmappedArrayGetLevel, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMipmappedArrayDestroy
//int ORIGINAL_cuMipmappedArrayDestroy( CUmipmappedArray hMipmappedArray ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMipmappedArrayDestroy;
void * PTR_ORIGINAL_cuMipmappedArrayDestroy;
// This is the call that will take the place of the original
int INTER_cuMipmappedArrayDestroy( CUmipmappedArray hMipmappedArray ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hMipmappedArray};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMipmappedArrayDestroy, (void*) PTR_ORIGINAL_cuMipmappedArrayDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuPointerGetAttribute
//int ORIGINAL_cuPointerGetAttribute( void * data, CUpointer_attribute attribute, CUdeviceptr ptr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuPointerGetAttribute;
void * PTR_ORIGINAL_cuPointerGetAttribute;
// This is the call that will take the place of the original
int INTER_cuPointerGetAttribute( void * data, CUpointer_attribute attribute, CUdeviceptr ptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)data,(void *)attribute,(void *)ptr};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuPointerGetAttribute, (void*) PTR_ORIGINAL_cuPointerGetAttribute, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemPrefetchAsync
//int ORIGINAL_cuMemPrefetchAsync( CUdeviceptr devPtr, size_t count, CUdevice dstDevice, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemPrefetchAsync;
void * PTR_ORIGINAL_cuMemPrefetchAsync;
// This is the call that will take the place of the original
int INTER_cuMemPrefetchAsync( CUdeviceptr devPtr, size_t count, CUdevice dstDevice, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)devPtr,(void *)count,(void *)dstDevice,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemPrefetchAsync, (void*) PTR_ORIGINAL_cuMemPrefetchAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemAdvise
//int ORIGINAL_cuMemAdvise( CUdeviceptr devPtr, size_t count, CUmem_advise advice, CUdevice device ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemAdvise;
void * PTR_ORIGINAL_cuMemAdvise;
// This is the call that will take the place of the original
int INTER_cuMemAdvise( CUdeviceptr devPtr, size_t count, CUmem_advise advice, CUdevice device ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)devPtr,(void *)count,(void *)advice,(void *)device};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemAdvise, (void*) PTR_ORIGINAL_cuMemAdvise, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemRangeGetAttribute
//int ORIGINAL_cuMemRangeGetAttribute( void * data, size_t dataSize, CUmem_range_attribute attribute, CUdeviceptr devPtr, size_t count ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemRangeGetAttribute;
void * PTR_ORIGINAL_cuMemRangeGetAttribute;
// This is the call that will take the place of the original
int INTER_cuMemRangeGetAttribute( void * data, size_t dataSize, CUmem_range_attribute attribute, CUdeviceptr devPtr, size_t count ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)data,(void *)dataSize,(void *)attribute,(void *)devPtr,(void *)count};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemRangeGetAttribute, (void*) PTR_ORIGINAL_cuMemRangeGetAttribute, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemRangeGetAttributes
//int ORIGINAL_cuMemRangeGetAttributes( void * * data, size_t * dataSizes, CUmem_range_attribute * attributes, size_t numAttributes, CUdeviceptr devPtr, size_t count ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemRangeGetAttributes;
void * PTR_ORIGINAL_cuMemRangeGetAttributes;
// This is the call that will take the place of the original
int INTER_cuMemRangeGetAttributes( void * * data, size_t * dataSizes, CUmem_range_attribute * attributes, size_t numAttributes, CUdeviceptr devPtr, size_t count ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)data,(void *)dataSizes,(void *)attributes,(void *)numAttributes,(void *)devPtr,(void *)count};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemRangeGetAttributes, (void*) PTR_ORIGINAL_cuMemRangeGetAttributes, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuPointerSetAttribute
//int ORIGINAL_cuPointerSetAttribute( const void * value, CUpointer_attribute attribute, CUdeviceptr ptr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuPointerSetAttribute;
void * PTR_ORIGINAL_cuPointerSetAttribute;
// This is the call that will take the place of the original
int INTER_cuPointerSetAttribute( const void * value, CUpointer_attribute attribute, CUdeviceptr ptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)value,(void *)attribute,(void *)ptr};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuPointerSetAttribute, (void*) PTR_ORIGINAL_cuPointerSetAttribute, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuPointerGetAttributes
//int ORIGINAL_cuPointerGetAttributes( unsigned int numAttributes, CUpointer_attribute * attributes, void * * data, CUdeviceptr ptr ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuPointerGetAttributes;
void * PTR_ORIGINAL_cuPointerGetAttributes;
// This is the call that will take the place of the original
int INTER_cuPointerGetAttributes( unsigned int numAttributes, CUpointer_attribute * attributes, void * * data, CUdeviceptr ptr ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)numAttributes,(void *)attributes,(void *)data,(void *)ptr};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuPointerGetAttributes, (void*) PTR_ORIGINAL_cuPointerGetAttributes, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamCreate
//int ORIGINAL_cuStreamCreate( CUstream * phStream, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamCreate;
void * PTR_ORIGINAL_cuStreamCreate;
// This is the call that will take the place of the original
int INTER_cuStreamCreate( CUstream * phStream, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)phStream,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamCreate, (void*) PTR_ORIGINAL_cuStreamCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamCreateWithPriority
//int ORIGINAL_cuStreamCreateWithPriority( CUstream * phStream, unsigned int flags, int priority ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamCreateWithPriority;
void * PTR_ORIGINAL_cuStreamCreateWithPriority;
// This is the call that will take the place of the original
int INTER_cuStreamCreateWithPriority( CUstream * phStream, unsigned int flags, int priority ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)phStream,(void *)flags,(void *)priority};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamCreateWithPriority, (void*) PTR_ORIGINAL_cuStreamCreateWithPriority, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamGetPriority
//int ORIGINAL_cuStreamGetPriority( CUstream hStream, int * priority ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamGetPriority;
void * PTR_ORIGINAL_cuStreamGetPriority;
// This is the call that will take the place of the original
int INTER_cuStreamGetPriority( CUstream hStream, int * priority ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream,(void *)priority};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamGetPriority, (void*) PTR_ORIGINAL_cuStreamGetPriority, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamGetFlags
//int ORIGINAL_cuStreamGetFlags( CUstream hStream, unsigned int * flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamGetFlags;
void * PTR_ORIGINAL_cuStreamGetFlags;
// This is the call that will take the place of the original
int INTER_cuStreamGetFlags( CUstream hStream, unsigned int * flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamGetFlags, (void*) PTR_ORIGINAL_cuStreamGetFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamWaitEvent
//int ORIGINAL_cuStreamWaitEvent( CUstream hStream, CUevent hEvent, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamWaitEvent;
void * PTR_ORIGINAL_cuStreamWaitEvent;
// This is the call that will take the place of the original
int INTER_cuStreamWaitEvent( CUstream hStream, CUevent hEvent, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream,(void *)hEvent,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamWaitEvent, (void*) PTR_ORIGINAL_cuStreamWaitEvent, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamAddCallback
//int ORIGINAL_cuStreamAddCallback( CUstream hStream, CUstreamCallback callback, void * userData, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamAddCallback;
void * PTR_ORIGINAL_cuStreamAddCallback;
// This is the call that will take the place of the original
int INTER_cuStreamAddCallback( CUstream hStream, CUstreamCallback callback, void * userData, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream,(void *)callback,(void *)userData,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamAddCallback, (void*) PTR_ORIGINAL_cuStreamAddCallback, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamAttachMemAsync
//int ORIGINAL_cuStreamAttachMemAsync( CUstream hStream, CUdeviceptr dptr, size_t length, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamAttachMemAsync;
void * PTR_ORIGINAL_cuStreamAttachMemAsync;
// This is the call that will take the place of the original
int INTER_cuStreamAttachMemAsync( CUstream hStream, CUdeviceptr dptr, size_t length, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream,(void *)dptr,(void *)length,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamAttachMemAsync, (void*) PTR_ORIGINAL_cuStreamAttachMemAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamQuery
//int ORIGINAL_cuStreamQuery( CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamQuery;
void * PTR_ORIGINAL_cuStreamQuery;
// This is the call that will take the place of the original
int INTER_cuStreamQuery( CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamQuery, (void*) PTR_ORIGINAL_cuStreamQuery, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamSynchronize
//int ORIGINAL_cuStreamSynchronize( CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamSynchronize;
void * PTR_ORIGINAL_cuStreamSynchronize;
// This is the call that will take the place of the original
int INTER_cuStreamSynchronize( CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamSynchronize, (void*) PTR_ORIGINAL_cuStreamSynchronize, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamDestroy
//int ORIGINAL_cuStreamDestroy( CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamDestroy;
void * PTR_ORIGINAL_cuStreamDestroy;
// This is the call that will take the place of the original
int INTER_cuStreamDestroy( CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hStream};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamDestroy, (void*) PTR_ORIGINAL_cuStreamDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuEventCreate
//int ORIGINAL_cuEventCreate( CUevent * phEvent, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuEventCreate;
void * PTR_ORIGINAL_cuEventCreate;
// This is the call that will take the place of the original
int INTER_cuEventCreate( CUevent * phEvent, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)phEvent,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuEventCreate, (void*) PTR_ORIGINAL_cuEventCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuEventRecord
//int ORIGINAL_cuEventRecord( CUevent hEvent, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuEventRecord;
void * PTR_ORIGINAL_cuEventRecord;
// This is the call that will take the place of the original
int INTER_cuEventRecord( CUevent hEvent, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hEvent,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuEventRecord, (void*) PTR_ORIGINAL_cuEventRecord, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuEventQuery
//int ORIGINAL_cuEventQuery( CUevent hEvent ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuEventQuery;
void * PTR_ORIGINAL_cuEventQuery;
// This is the call that will take the place of the original
int INTER_cuEventQuery( CUevent hEvent ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hEvent};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuEventQuery, (void*) PTR_ORIGINAL_cuEventQuery, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuEventSynchronize
//int ORIGINAL_cuEventSynchronize( CUevent hEvent ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuEventSynchronize;
void * PTR_ORIGINAL_cuEventSynchronize;
// This is the call that will take the place of the original
int INTER_cuEventSynchronize( CUevent hEvent ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hEvent};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuEventSynchronize, (void*) PTR_ORIGINAL_cuEventSynchronize, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuEventDestroy
//int ORIGINAL_cuEventDestroy( CUevent hEvent ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuEventDestroy;
void * PTR_ORIGINAL_cuEventDestroy;
// This is the call that will take the place of the original
int INTER_cuEventDestroy( CUevent hEvent ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hEvent};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuEventDestroy, (void*) PTR_ORIGINAL_cuEventDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuEventElapsedTime
//int ORIGINAL_cuEventElapsedTime( float * pMilliseconds, CUevent hStart, CUevent hEnd ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuEventElapsedTime;
void * PTR_ORIGINAL_cuEventElapsedTime;
// This is the call that will take the place of the original
int INTER_cuEventElapsedTime( float * pMilliseconds, CUevent hStart, CUevent hEnd ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pMilliseconds,(void *)hStart,(void *)hEnd};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuEventElapsedTime, (void*) PTR_ORIGINAL_cuEventElapsedTime, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamWaitValue32
//int ORIGINAL_cuStreamWaitValue32( CUstream stream, CUdeviceptr addr, cuuint32_t value, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamWaitValue32;
void * PTR_ORIGINAL_cuStreamWaitValue32;
// This is the call that will take the place of the original
int INTER_cuStreamWaitValue32( CUstream stream, CUdeviceptr addr, cuuint32_t value, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)stream,(void *)addr,(void *)value,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamWaitValue32, (void*) PTR_ORIGINAL_cuStreamWaitValue32, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamWriteValue32
//int ORIGINAL_cuStreamWriteValue32( CUstream stream, CUdeviceptr addr, cuuint32_t value, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamWriteValue32;
void * PTR_ORIGINAL_cuStreamWriteValue32;
// This is the call that will take the place of the original
int INTER_cuStreamWriteValue32( CUstream stream, CUdeviceptr addr, cuuint32_t value, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)stream,(void *)addr,(void *)value,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamWriteValue32, (void*) PTR_ORIGINAL_cuStreamWriteValue32, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuStreamBatchMemOp
//int ORIGINAL_cuStreamBatchMemOp( CUstream stream, unsigned int count, CUstreamBatchMemOpParams * paramArray, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuStreamBatchMemOp;
void * PTR_ORIGINAL_cuStreamBatchMemOp;
// This is the call that will take the place of the original
int INTER_cuStreamBatchMemOp( CUstream stream, unsigned int count, CUstreamBatchMemOpParams * paramArray, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)stream,(void *)count,(void *)paramArray,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuStreamBatchMemOp, (void*) PTR_ORIGINAL_cuStreamBatchMemOp, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuFuncGetAttribute
//int ORIGINAL_cuFuncGetAttribute( int * pi, CUfunction_attribute attrib, CUfunction hfunc ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuFuncGetAttribute;
void * PTR_ORIGINAL_cuFuncGetAttribute;
// This is the call that will take the place of the original
int INTER_cuFuncGetAttribute( int * pi, CUfunction_attribute attrib, CUfunction hfunc ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pi,(void *)attrib,(void *)hfunc};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuFuncGetAttribute, (void*) PTR_ORIGINAL_cuFuncGetAttribute, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuFuncSetCacheConfig
//int ORIGINAL_cuFuncSetCacheConfig( CUfunction hfunc, CUfunc_cache config ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuFuncSetCacheConfig;
void * PTR_ORIGINAL_cuFuncSetCacheConfig;
// This is the call that will take the place of the original
int INTER_cuFuncSetCacheConfig( CUfunction hfunc, CUfunc_cache config ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)config};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuFuncSetCacheConfig, (void*) PTR_ORIGINAL_cuFuncSetCacheConfig, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuFuncSetSharedMemConfig
//int ORIGINAL_cuFuncSetSharedMemConfig( CUfunction hfunc, CUsharedconfig config ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuFuncSetSharedMemConfig;
void * PTR_ORIGINAL_cuFuncSetSharedMemConfig;
// This is the call that will take the place of the original
int INTER_cuFuncSetSharedMemConfig( CUfunction hfunc, CUsharedconfig config ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)config};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuFuncSetSharedMemConfig, (void*) PTR_ORIGINAL_cuFuncSetSharedMemConfig, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLaunchKernel
//int ORIGINAL_cuLaunchKernel( CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes, CUstream hStream, void * * kernelParams, void * * extra ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLaunchKernel;
void * PTR_ORIGINAL_cuLaunchKernel;
// This is the call that will take the place of the original
int INTER_cuLaunchKernel( CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes, CUstream hStream, void * * kernelParams, void * * extra ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)f,(void *)gridDimX,(void *)gridDimY,(void *)gridDimZ,(void *)blockDimX,(void *)blockDimY,(void *)blockDimZ,(void *)sharedMemBytes,(void *)hStream,(void *)kernelParams,(void *)extra};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5],&tmpParams[6],&tmpParams[7],&tmpParams[8],&tmpParams[9],&tmpParams[10] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLaunchKernel, (void*) PTR_ORIGINAL_cuLaunchKernel, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuFuncSetBlockShape
//int ORIGINAL_cuFuncSetBlockShape( CUfunction hfunc, int x, int y, int z ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuFuncSetBlockShape;
void * PTR_ORIGINAL_cuFuncSetBlockShape;
// This is the call that will take the place of the original
int INTER_cuFuncSetBlockShape( CUfunction hfunc, int x, int y, int z ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)x,(void *)y,(void *)z};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuFuncSetBlockShape, (void*) PTR_ORIGINAL_cuFuncSetBlockShape, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuFuncSetSharedSize
//int ORIGINAL_cuFuncSetSharedSize( CUfunction hfunc, unsigned int bytes ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuFuncSetSharedSize;
void * PTR_ORIGINAL_cuFuncSetSharedSize;
// This is the call that will take the place of the original
int INTER_cuFuncSetSharedSize( CUfunction hfunc, unsigned int bytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)bytes};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuFuncSetSharedSize, (void*) PTR_ORIGINAL_cuFuncSetSharedSize, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuParamSetSize
//int ORIGINAL_cuParamSetSize( CUfunction hfunc, unsigned int numbytes ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuParamSetSize;
void * PTR_ORIGINAL_cuParamSetSize;
// This is the call that will take the place of the original
int INTER_cuParamSetSize( CUfunction hfunc, unsigned int numbytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)numbytes};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuParamSetSize, (void*) PTR_ORIGINAL_cuParamSetSize, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuParamSeti
//int ORIGINAL_cuParamSeti( CUfunction hfunc, int offset, unsigned int value ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuParamSeti;
void * PTR_ORIGINAL_cuParamSeti;
// This is the call that will take the place of the original
int INTER_cuParamSeti( CUfunction hfunc, int offset, unsigned int value ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)offset,(void *)value};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuParamSeti, (void*) PTR_ORIGINAL_cuParamSeti, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuParamSetf
//int ORIGINAL_cuParamSetf( CUfunction hfunc, int offset, float value ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuParamSetf;
void * PTR_ORIGINAL_cuParamSetf;
// This is the call that will take the place of the original
int INTER_cuParamSetf( CUfunction hfunc, int offset, float value ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	// Really schetchy fix, lets assert if this is ever called to check it
	assert(1==0);
	// One off fix for float.... but likely doesn't work (FML). 
	char * tmpFloat = (char *) malloc(1 * sizeof(float*));
	memcpy(tmpFloat, &value, sizeof(float));

	std::vector<void *> tmpParams = { (void *)hfunc,(void *)offset,(void *)tmpFloat};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuParamSetf, (void*) PTR_ORIGINAL_cuParamSetf, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	free(tmpFloat);
	return ret;
}
// This "function" will be rewritten to point to cuParamSetv
//int ORIGINAL_cuParamSetv( CUfunction hfunc, int offset, void * ptr, unsigned int numbytes ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuParamSetv;
void * PTR_ORIGINAL_cuParamSetv;
// This is the call that will take the place of the original
int INTER_cuParamSetv( CUfunction hfunc, int offset, void * ptr, unsigned int numbytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)offset,(void *)ptr,(void *)numbytes};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuParamSetv, (void*) PTR_ORIGINAL_cuParamSetv, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLaunch
//int ORIGINAL_cuLaunch( CUfunction f ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLaunch;
void * PTR_ORIGINAL_cuLaunch;
// This is the call that will take the place of the original
int INTER_cuLaunch( CUfunction f ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)f};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLaunch, (void*) PTR_ORIGINAL_cuLaunch, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLaunchGrid
//int ORIGINAL_cuLaunchGrid( CUfunction f, int grid_width, int grid_height ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLaunchGrid;
void * PTR_ORIGINAL_cuLaunchGrid;
// This is the call that will take the place of the original
int INTER_cuLaunchGrid( CUfunction f, int grid_width, int grid_height ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)f,(void *)grid_width,(void *)grid_height};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLaunchGrid, (void*) PTR_ORIGINAL_cuLaunchGrid, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuLaunchGridAsync
//int ORIGINAL_cuLaunchGridAsync( CUfunction f, int grid_width, int grid_height, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuLaunchGridAsync;
void * PTR_ORIGINAL_cuLaunchGridAsync;
// This is the call that will take the place of the original
int INTER_cuLaunchGridAsync( CUfunction f, int grid_width, int grid_height, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)f,(void *)grid_width,(void *)grid_height,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuLaunchGridAsync, (void*) PTR_ORIGINAL_cuLaunchGridAsync, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuParamSetTexRef
//int ORIGINAL_cuParamSetTexRef( CUfunction hfunc, int texunit, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuParamSetTexRef;
void * PTR_ORIGINAL_cuParamSetTexRef;
// This is the call that will take the place of the original
int INTER_cuParamSetTexRef( CUfunction hfunc, int texunit, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hfunc,(void *)texunit,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuParamSetTexRef, (void*) PTR_ORIGINAL_cuParamSetTexRef, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuOccupancyMaxActiveBlocksPerMultiprocessor
//int ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessor( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessor;
void * PTR_ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessor;
// This is the call that will take the place of the original
int INTER_cuOccupancyMaxActiveBlocksPerMultiprocessor( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)numBlocks,(void *)func,(void *)blockSize,(void *)dynamicSMemSize};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuOccupancyMaxActiveBlocksPerMultiprocessor, (void*) PTR_ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessor, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags
//int ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags;
void * PTR_ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags;
// This is the call that will take the place of the original
int INTER_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags( int * numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)numBlocks,(void *)func,(void *)blockSize,(void *)dynamicSMemSize,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags, (void*) PTR_ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuOccupancyMaxPotentialBlockSize
//int ORIGINAL_cuOccupancyMaxPotentialBlockSize( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuOccupancyMaxPotentialBlockSize;
void * PTR_ORIGINAL_cuOccupancyMaxPotentialBlockSize;
// This is the call that will take the place of the original
int INTER_cuOccupancyMaxPotentialBlockSize( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)minGridSize,(void *)blockSize,(void *)func,(void *)blockSizeToDynamicSMemSize,(void *)dynamicSMemSize,(void *)blockSizeLimit};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuOccupancyMaxPotentialBlockSize, (void*) PTR_ORIGINAL_cuOccupancyMaxPotentialBlockSize, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuOccupancyMaxPotentialBlockSizeWithFlags
//int ORIGINAL_cuOccupancyMaxPotentialBlockSizeWithFlags( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuOccupancyMaxPotentialBlockSizeWithFlags;
void * PTR_ORIGINAL_cuOccupancyMaxPotentialBlockSizeWithFlags;
// This is the call that will take the place of the original
int INTER_cuOccupancyMaxPotentialBlockSizeWithFlags( int * minGridSize, int * blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)minGridSize,(void *)blockSize,(void *)func,(void *)blockSizeToDynamicSMemSize,(void *)dynamicSMemSize,(void *)blockSizeLimit,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4],&tmpParams[5],&tmpParams[6] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuOccupancyMaxPotentialBlockSizeWithFlags, (void*) PTR_ORIGINAL_cuOccupancyMaxPotentialBlockSizeWithFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetArray
//int ORIGINAL_cuTexRefSetArray( CUtexref hTexRef, CUarray hArray, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetArray;
void * PTR_ORIGINAL_cuTexRefSetArray;
// This is the call that will take the place of the original
int INTER_cuTexRefSetArray( CUtexref hTexRef, CUarray hArray, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)hArray,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetArray, (void*) PTR_ORIGINAL_cuTexRefSetArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetMipmappedArray
//int ORIGINAL_cuTexRefSetMipmappedArray( CUtexref hTexRef, CUmipmappedArray hMipmappedArray, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetMipmappedArray;
void * PTR_ORIGINAL_cuTexRefSetMipmappedArray;
// This is the call that will take the place of the original
int INTER_cuTexRefSetMipmappedArray( CUtexref hTexRef, CUmipmappedArray hMipmappedArray, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)hMipmappedArray,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetMipmappedArray, (void*) PTR_ORIGINAL_cuTexRefSetMipmappedArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetAddress
//int ORIGINAL_cuTexRefSetAddress( size_t * ByteOffset, CUtexref hTexRef, CUdeviceptr dptr, size_t bytes ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetAddress;
void * PTR_ORIGINAL_cuTexRefSetAddress;
// This is the call that will take the place of the original
int INTER_cuTexRefSetAddress( size_t * ByteOffset, CUtexref hTexRef, CUdeviceptr dptr, size_t bytes ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)ByteOffset,(void *)hTexRef,(void *)dptr,(void *)bytes};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetAddress, (void*) PTR_ORIGINAL_cuTexRefSetAddress, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetAddress2D
//int ORIGINAL_cuTexRefSetAddress2D( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetAddress2D;
void * PTR_ORIGINAL_cuTexRefSetAddress2D;
// This is the call that will take the place of the original
int INTER_cuTexRefSetAddress2D( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)desc,(void *)dptr,(void *)Pitch};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetAddress2D, (void*) PTR_ORIGINAL_cuTexRefSetAddress2D, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetFormat
//int ORIGINAL_cuTexRefSetFormat( CUtexref hTexRef, CUarray_format fmt, int NumPackedComponents ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetFormat;
void * PTR_ORIGINAL_cuTexRefSetFormat;
// This is the call that will take the place of the original
int INTER_cuTexRefSetFormat( CUtexref hTexRef, CUarray_format fmt, int NumPackedComponents ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)fmt,(void *)NumPackedComponents};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetFormat, (void*) PTR_ORIGINAL_cuTexRefSetFormat, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetAddressMode
//int ORIGINAL_cuTexRefSetAddressMode( CUtexref hTexRef, int dim, CUaddress_mode am ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetAddressMode;
void * PTR_ORIGINAL_cuTexRefSetAddressMode;
// This is the call that will take the place of the original
int INTER_cuTexRefSetAddressMode( CUtexref hTexRef, int dim, CUaddress_mode am ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)dim,(void *)am};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetAddressMode, (void*) PTR_ORIGINAL_cuTexRefSetAddressMode, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetFilterMode
//int ORIGINAL_cuTexRefSetFilterMode( CUtexref hTexRef, CUfilter_mode fm ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetFilterMode;
void * PTR_ORIGINAL_cuTexRefSetFilterMode;
// This is the call that will take the place of the original
int INTER_cuTexRefSetFilterMode( CUtexref hTexRef, CUfilter_mode fm ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)fm};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetFilterMode, (void*) PTR_ORIGINAL_cuTexRefSetFilterMode, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetMipmapFilterMode
//int ORIGINAL_cuTexRefSetMipmapFilterMode( CUtexref hTexRef, CUfilter_mode fm ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetMipmapFilterMode;
void * PTR_ORIGINAL_cuTexRefSetMipmapFilterMode;
// This is the call that will take the place of the original
int INTER_cuTexRefSetMipmapFilterMode( CUtexref hTexRef, CUfilter_mode fm ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)fm};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetMipmapFilterMode, (void*) PTR_ORIGINAL_cuTexRefSetMipmapFilterMode, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetMipmapLevelBias
//int ORIGINAL_cuTexRefSetMipmapLevelBias( CUtexref hTexRef, float bias ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetMipmapLevelBias;
void * PTR_ORIGINAL_cuTexRefSetMipmapLevelBias;
// This is the call that will take the place of the original
int INTER_cuTexRefSetMipmapLevelBias( CUtexref hTexRef, float bias ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Really schetchy fix, lets assert if this is ever called to check it
	//assert(1==0);
	// Gets around an issue on POWER
	char * tmpFloat = (char *) malloc(1 * sizeof(float*));
	memcpy(tmpFloat, &bias, sizeof(float));
	
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)tmpFloat};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetMipmapLevelBias, (void*) PTR_ORIGINAL_cuTexRefSetMipmapLevelBias, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	free(tmpFloat);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetMipmapLevelClamp
//int ORIGINAL_cuTexRefSetMipmapLevelClamp( CUtexref hTexRef, float minMipmapLevelClamp, float maxMipmapLevelClamp ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetMipmapLevelClamp;
void * PTR_ORIGINAL_cuTexRefSetMipmapLevelClamp;
// This is the call that will take the place of the original
int INTER_cuTexRefSetMipmapLevelClamp( CUtexref hTexRef, float minMipmapLevelClamp, float maxMipmapLevelClamp ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	// Really schetchy fix, lets assert if this is ever called to check it
	//assert(1==0);
	char * tmpFloat = (char *) malloc(2 * sizeof(float*));
	memcpy(tmpFloat, &minMipmapLevelClamp, sizeof(float));
	memcpy((void *)&(tmpFloat[sizeof(float*)]), &maxMipmapLevelClamp, sizeof(float));
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)&(tmpFloat[0]),(void *)&(tmpFloat[8])};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetMipmapLevelClamp, (void*) PTR_ORIGINAL_cuTexRefSetMipmapLevelClamp, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	free(tmpFloat);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetMaxAnisotropy
//int ORIGINAL_cuTexRefSetMaxAnisotropy( CUtexref hTexRef, unsigned int maxAniso ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetMaxAnisotropy;
void * PTR_ORIGINAL_cuTexRefSetMaxAnisotropy;
// This is the call that will take the place of the original
int INTER_cuTexRefSetMaxAnisotropy( CUtexref hTexRef, unsigned int maxAniso ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)maxAniso};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetMaxAnisotropy, (void*) PTR_ORIGINAL_cuTexRefSetMaxAnisotropy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetBorderColor
//int ORIGINAL_cuTexRefSetBorderColor( CUtexref hTexRef, float * pBorderColor ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetBorderColor;
void * PTR_ORIGINAL_cuTexRefSetBorderColor;
// This is the call that will take the place of the original
int INTER_cuTexRefSetBorderColor( CUtexref hTexRef, float * pBorderColor ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)pBorderColor};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetBorderColor, (void*) PTR_ORIGINAL_cuTexRefSetBorderColor, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetFlags
//int ORIGINAL_cuTexRefSetFlags( CUtexref hTexRef, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetFlags;
void * PTR_ORIGINAL_cuTexRefSetFlags;
// This is the call that will take the place of the original
int INTER_cuTexRefSetFlags( CUtexref hTexRef, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetFlags, (void*) PTR_ORIGINAL_cuTexRefSetFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetAddress
//int ORIGINAL_cuTexRefGetAddress( CUdeviceptr * pdptr, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetAddress;
void * PTR_ORIGINAL_cuTexRefGetAddress;
// This is the call that will take the place of the original
int INTER_cuTexRefGetAddress( CUdeviceptr * pdptr, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pdptr,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetAddress, (void*) PTR_ORIGINAL_cuTexRefGetAddress, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetArray
//int ORIGINAL_cuTexRefGetArray( CUarray * phArray, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetArray;
void * PTR_ORIGINAL_cuTexRefGetArray;
// This is the call that will take the place of the original
int INTER_cuTexRefGetArray( CUarray * phArray, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)phArray,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetArray, (void*) PTR_ORIGINAL_cuTexRefGetArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetMipmappedArray
//int ORIGINAL_cuTexRefGetMipmappedArray( CUmipmappedArray * phMipmappedArray, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetMipmappedArray;
void * PTR_ORIGINAL_cuTexRefGetMipmappedArray;
// This is the call that will take the place of the original
int INTER_cuTexRefGetMipmappedArray( CUmipmappedArray * phMipmappedArray, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)phMipmappedArray,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetMipmappedArray, (void*) PTR_ORIGINAL_cuTexRefGetMipmappedArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetAddressMode
//int ORIGINAL_cuTexRefGetAddressMode( CUaddress_mode * pam, CUtexref hTexRef, int dim ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetAddressMode;
void * PTR_ORIGINAL_cuTexRefGetAddressMode;
// This is the call that will take the place of the original
int INTER_cuTexRefGetAddressMode( CUaddress_mode * pam, CUtexref hTexRef, int dim ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pam,(void *)hTexRef,(void *)dim};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetAddressMode, (void*) PTR_ORIGINAL_cuTexRefGetAddressMode, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetFilterMode
//int ORIGINAL_cuTexRefGetFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetFilterMode;
void * PTR_ORIGINAL_cuTexRefGetFilterMode;
// This is the call that will take the place of the original
int INTER_cuTexRefGetFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pfm,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetFilterMode, (void*) PTR_ORIGINAL_cuTexRefGetFilterMode, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetFormat
//int ORIGINAL_cuTexRefGetFormat( CUarray_format * pFormat, int * pNumChannels, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetFormat;
void * PTR_ORIGINAL_cuTexRefGetFormat;
// This is the call that will take the place of the original
int INTER_cuTexRefGetFormat( CUarray_format * pFormat, int * pNumChannels, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pFormat,(void *)pNumChannels,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetFormat, (void*) PTR_ORIGINAL_cuTexRefGetFormat, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetMipmapFilterMode
//int ORIGINAL_cuTexRefGetMipmapFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetMipmapFilterMode;
void * PTR_ORIGINAL_cuTexRefGetMipmapFilterMode;
// This is the call that will take the place of the original
int INTER_cuTexRefGetMipmapFilterMode( CUfilter_mode * pfm, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pfm,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetMipmapFilterMode, (void*) PTR_ORIGINAL_cuTexRefGetMipmapFilterMode, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetMipmapLevelBias
//int ORIGINAL_cuTexRefGetMipmapLevelBias( float * pbias, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetMipmapLevelBias;
void * PTR_ORIGINAL_cuTexRefGetMipmapLevelBias;
// This is the call that will take the place of the original
int INTER_cuTexRefGetMipmapLevelBias( float * pbias, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pbias,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetMipmapLevelBias, (void*) PTR_ORIGINAL_cuTexRefGetMipmapLevelBias, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetMipmapLevelClamp
//int ORIGINAL_cuTexRefGetMipmapLevelClamp( float * pminMipmapLevelClamp, float * pmaxMipmapLevelClamp, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetMipmapLevelClamp;
void * PTR_ORIGINAL_cuTexRefGetMipmapLevelClamp;
// This is the call that will take the place of the original
int INTER_cuTexRefGetMipmapLevelClamp( float * pminMipmapLevelClamp, float * pmaxMipmapLevelClamp, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pminMipmapLevelClamp,(void *)pmaxMipmapLevelClamp,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetMipmapLevelClamp, (void*) PTR_ORIGINAL_cuTexRefGetMipmapLevelClamp, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetMaxAnisotropy
//int ORIGINAL_cuTexRefGetMaxAnisotropy( int * pmaxAniso, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetMaxAnisotropy;
void * PTR_ORIGINAL_cuTexRefGetMaxAnisotropy;
// This is the call that will take the place of the original
int INTER_cuTexRefGetMaxAnisotropy( int * pmaxAniso, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pmaxAniso,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetMaxAnisotropy, (void*) PTR_ORIGINAL_cuTexRefGetMaxAnisotropy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetBorderColor
//int ORIGINAL_cuTexRefGetBorderColor( float * pBorderColor, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetBorderColor;
void * PTR_ORIGINAL_cuTexRefGetBorderColor;
// This is the call that will take the place of the original
int INTER_cuTexRefGetBorderColor( float * pBorderColor, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pBorderColor,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetBorderColor, (void*) PTR_ORIGINAL_cuTexRefGetBorderColor, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefGetFlags
//int ORIGINAL_cuTexRefGetFlags( unsigned int * pFlags, CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefGetFlags;
void * PTR_ORIGINAL_cuTexRefGetFlags;
// This is the call that will take the place of the original
int INTER_cuTexRefGetFlags( unsigned int * pFlags, CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pFlags,(void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefGetFlags, (void*) PTR_ORIGINAL_cuTexRefGetFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefCreate
//int ORIGINAL_cuTexRefCreate( CUtexref * pTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefCreate;
void * PTR_ORIGINAL_cuTexRefCreate;
// This is the call that will take the place of the original
int INTER_cuTexRefCreate( CUtexref * pTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pTexRef};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefCreate, (void*) PTR_ORIGINAL_cuTexRefCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefDestroy
//int ORIGINAL_cuTexRefDestroy( CUtexref hTexRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefDestroy;
void * PTR_ORIGINAL_cuTexRefDestroy;
// This is the call that will take the place of the original
int INTER_cuTexRefDestroy( CUtexref hTexRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefDestroy, (void*) PTR_ORIGINAL_cuTexRefDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuSurfRefSetArray
//int ORIGINAL_cuSurfRefSetArray( CUsurfref hSurfRef, CUarray hArray, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuSurfRefSetArray;
void * PTR_ORIGINAL_cuSurfRefSetArray;
// This is the call that will take the place of the original
int INTER_cuSurfRefSetArray( CUsurfref hSurfRef, CUarray hArray, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hSurfRef,(void *)hArray,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuSurfRefSetArray, (void*) PTR_ORIGINAL_cuSurfRefSetArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuSurfRefGetArray
//int ORIGINAL_cuSurfRefGetArray( CUarray * phArray, CUsurfref hSurfRef ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuSurfRefGetArray;
void * PTR_ORIGINAL_cuSurfRefGetArray;
// This is the call that will take the place of the original
int INTER_cuSurfRefGetArray( CUarray * phArray, CUsurfref hSurfRef ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)phArray,(void *)hSurfRef};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuSurfRefGetArray, (void*) PTR_ORIGINAL_cuSurfRefGetArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexObjectCreate
//int ORIGINAL_cuTexObjectCreate( CUtexObject * pTexObject, const CUDA_RESOURCE_DESC * pResDesc, const CUDA_TEXTURE_DESC * pTexDesc, const CUDA_RESOURCE_VIEW_DESC * pResViewDesc ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexObjectCreate;
void * PTR_ORIGINAL_cuTexObjectCreate;
// This is the call that will take the place of the original
int INTER_cuTexObjectCreate( CUtexObject * pTexObject, const CUDA_RESOURCE_DESC * pResDesc, const CUDA_TEXTURE_DESC * pTexDesc, const CUDA_RESOURCE_VIEW_DESC * pResViewDesc ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pTexObject,(void *)pResDesc,(void *)pTexDesc,(void *)pResViewDesc};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexObjectCreate, (void*) PTR_ORIGINAL_cuTexObjectCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexObjectDestroy
//int ORIGINAL_cuTexObjectDestroy( CUtexObject texObject ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexObjectDestroy;
void * PTR_ORIGINAL_cuTexObjectDestroy;
// This is the call that will take the place of the original
int INTER_cuTexObjectDestroy( CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)texObject};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexObjectDestroy, (void*) PTR_ORIGINAL_cuTexObjectDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexObjectGetResourceDesc
//int ORIGINAL_cuTexObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUtexObject texObject ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexObjectGetResourceDesc;
void * PTR_ORIGINAL_cuTexObjectGetResourceDesc;
// This is the call that will take the place of the original
int INTER_cuTexObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pResDesc,(void *)texObject};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexObjectGetResourceDesc, (void*) PTR_ORIGINAL_cuTexObjectGetResourceDesc, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexObjectGetTextureDesc
//int ORIGINAL_cuTexObjectGetTextureDesc( CUDA_TEXTURE_DESC * pTexDesc, CUtexObject texObject ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexObjectGetTextureDesc;
void * PTR_ORIGINAL_cuTexObjectGetTextureDesc;
// This is the call that will take the place of the original
int INTER_cuTexObjectGetTextureDesc( CUDA_TEXTURE_DESC * pTexDesc, CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pTexDesc,(void *)texObject};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexObjectGetTextureDesc, (void*) PTR_ORIGINAL_cuTexObjectGetTextureDesc, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexObjectGetResourceViewDesc
//int ORIGINAL_cuTexObjectGetResourceViewDesc( CUDA_RESOURCE_VIEW_DESC * pResViewDesc, CUtexObject texObject ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexObjectGetResourceViewDesc;
void * PTR_ORIGINAL_cuTexObjectGetResourceViewDesc;
// This is the call that will take the place of the original
int INTER_cuTexObjectGetResourceViewDesc( CUDA_RESOURCE_VIEW_DESC * pResViewDesc, CUtexObject texObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pResViewDesc,(void *)texObject};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexObjectGetResourceViewDesc, (void*) PTR_ORIGINAL_cuTexObjectGetResourceViewDesc, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuSurfObjectCreate
//int ORIGINAL_cuSurfObjectCreate( CUsurfObject * pSurfObject, const CUDA_RESOURCE_DESC * pResDesc ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuSurfObjectCreate;
void * PTR_ORIGINAL_cuSurfObjectCreate;
// This is the call that will take the place of the original
int INTER_cuSurfObjectCreate( CUsurfObject * pSurfObject, const CUDA_RESOURCE_DESC * pResDesc ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pSurfObject,(void *)pResDesc};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuSurfObjectCreate, (void*) PTR_ORIGINAL_cuSurfObjectCreate, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuSurfObjectDestroy
//int ORIGINAL_cuSurfObjectDestroy( CUsurfObject surfObject ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuSurfObjectDestroy;
void * PTR_ORIGINAL_cuSurfObjectDestroy;
// This is the call that will take the place of the original
int INTER_cuSurfObjectDestroy( CUsurfObject surfObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)surfObject};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuSurfObjectDestroy, (void*) PTR_ORIGINAL_cuSurfObjectDestroy, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuSurfObjectGetResourceDesc
//int ORIGINAL_cuSurfObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUsurfObject surfObject ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuSurfObjectGetResourceDesc;
void * PTR_ORIGINAL_cuSurfObjectGetResourceDesc;
// This is the call that will take the place of the original
int INTER_cuSurfObjectGetResourceDesc( CUDA_RESOURCE_DESC * pResDesc, CUsurfObject surfObject ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pResDesc,(void *)surfObject};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuSurfObjectGetResourceDesc, (void*) PTR_ORIGINAL_cuSurfObjectGetResourceDesc, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceCanAccessPeer
//int ORIGINAL_cuDeviceCanAccessPeer( int * canAccessPeer, CUdevice dev, CUdevice peerDev ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceCanAccessPeer;
void * PTR_ORIGINAL_cuDeviceCanAccessPeer;
// This is the call that will take the place of the original
int INTER_cuDeviceCanAccessPeer( int * canAccessPeer, CUdevice dev, CUdevice peerDev ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)canAccessPeer,(void *)dev,(void *)peerDev};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceCanAccessPeer, (void*) PTR_ORIGINAL_cuDeviceCanAccessPeer, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuDeviceGetP2PAttribute
//int ORIGINAL_cuDeviceGetP2PAttribute( int * value, CUdevice_P2PAttribute attrib, CUdevice srcDevice, CUdevice dstDevice ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuDeviceGetP2PAttribute;
void * PTR_ORIGINAL_cuDeviceGetP2PAttribute;
// This is the call that will take the place of the original
int INTER_cuDeviceGetP2PAttribute( int * value, CUdevice_P2PAttribute attrib, CUdevice srcDevice, CUdevice dstDevice ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)value,(void *)attrib,(void *)srcDevice,(void *)dstDevice};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuDeviceGetP2PAttribute, (void*) PTR_ORIGINAL_cuDeviceGetP2PAttribute, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxEnablePeerAccess
//int ORIGINAL_cuCtxEnablePeerAccess( CUcontext peerContext, unsigned int Flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxEnablePeerAccess;
void * PTR_ORIGINAL_cuCtxEnablePeerAccess;
// This is the call that will take the place of the original
int INTER_cuCtxEnablePeerAccess( CUcontext peerContext, unsigned int Flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)peerContext,(void *)Flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxEnablePeerAccess, (void*) PTR_ORIGINAL_cuCtxEnablePeerAccess, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuCtxDisablePeerAccess
//int ORIGINAL_cuCtxDisablePeerAccess( CUcontext peerContext ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuCtxDisablePeerAccess;
void * PTR_ORIGINAL_cuCtxDisablePeerAccess;
// This is the call that will take the place of the original
int INTER_cuCtxDisablePeerAccess( CUcontext peerContext ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)peerContext};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuCtxDisablePeerAccess, (void*) PTR_ORIGINAL_cuCtxDisablePeerAccess, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGraphicsUnregisterResource
//int ORIGINAL_cuGraphicsUnregisterResource( CUgraphicsResource resource ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGraphicsUnregisterResource;
void * PTR_ORIGINAL_cuGraphicsUnregisterResource;
// This is the call that will take the place of the original
int INTER_cuGraphicsUnregisterResource( CUgraphicsResource resource ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)resource};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGraphicsUnregisterResource, (void*) PTR_ORIGINAL_cuGraphicsUnregisterResource, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGraphicsSubResourceGetMappedArray
//int ORIGINAL_cuGraphicsSubResourceGetMappedArray( CUarray * pArray, CUgraphicsResource resource, unsigned int arrayIndex, unsigned int mipLevel ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGraphicsSubResourceGetMappedArray;
void * PTR_ORIGINAL_cuGraphicsSubResourceGetMappedArray;
// This is the call that will take the place of the original
int INTER_cuGraphicsSubResourceGetMappedArray( CUarray * pArray, CUgraphicsResource resource, unsigned int arrayIndex, unsigned int mipLevel ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pArray,(void *)resource,(void *)arrayIndex,(void *)mipLevel};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGraphicsSubResourceGetMappedArray, (void*) PTR_ORIGINAL_cuGraphicsSubResourceGetMappedArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGraphicsResourceGetMappedMipmappedArray
//int ORIGINAL_cuGraphicsResourceGetMappedMipmappedArray( CUmipmappedArray * pMipmappedArray, CUgraphicsResource resource ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGraphicsResourceGetMappedMipmappedArray;
void * PTR_ORIGINAL_cuGraphicsResourceGetMappedMipmappedArray;
// This is the call that will take the place of the original
int INTER_cuGraphicsResourceGetMappedMipmappedArray( CUmipmappedArray * pMipmappedArray, CUgraphicsResource resource ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pMipmappedArray,(void *)resource};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGraphicsResourceGetMappedMipmappedArray, (void*) PTR_ORIGINAL_cuGraphicsResourceGetMappedMipmappedArray, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGraphicsResourceGetMappedPointer
//int ORIGINAL_cuGraphicsResourceGetMappedPointer( CUdeviceptr * pDevPtr, size_t * pSize, CUgraphicsResource resource ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGraphicsResourceGetMappedPointer;
void * PTR_ORIGINAL_cuGraphicsResourceGetMappedPointer;
// This is the call that will take the place of the original
int INTER_cuGraphicsResourceGetMappedPointer( CUdeviceptr * pDevPtr, size_t * pSize, CUgraphicsResource resource ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pDevPtr,(void *)pSize,(void *)resource};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGraphicsResourceGetMappedPointer, (void*) PTR_ORIGINAL_cuGraphicsResourceGetMappedPointer, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGraphicsResourceSetMapFlags
//int ORIGINAL_cuGraphicsResourceSetMapFlags( CUgraphicsResource resource, unsigned int flags ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGraphicsResourceSetMapFlags;
void * PTR_ORIGINAL_cuGraphicsResourceSetMapFlags;
// This is the call that will take the place of the original
int INTER_cuGraphicsResourceSetMapFlags( CUgraphicsResource resource, unsigned int flags ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)resource,(void *)flags};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGraphicsResourceSetMapFlags, (void*) PTR_ORIGINAL_cuGraphicsResourceSetMapFlags, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGraphicsMapResources
//int ORIGINAL_cuGraphicsMapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGraphicsMapResources;
void * PTR_ORIGINAL_cuGraphicsMapResources;
// This is the call that will take the place of the original
int INTER_cuGraphicsMapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)count,(void *)resources,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGraphicsMapResources, (void*) PTR_ORIGINAL_cuGraphicsMapResources, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGraphicsUnmapResources
//int ORIGINAL_cuGraphicsUnmapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGraphicsUnmapResources;
void * PTR_ORIGINAL_cuGraphicsUnmapResources;
// This is the call that will take the place of the original
int INTER_cuGraphicsUnmapResources( unsigned int count, CUgraphicsResource * resources, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)count,(void *)resources,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGraphicsUnmapResources, (void*) PTR_ORIGINAL_cuGraphicsUnmapResources, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuGetExportTable
//int ORIGINAL_cuGetExportTable( const void * * ppExportTable, const CUuuid * pExportTableId ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuGetExportTable;
void * PTR_ORIGINAL_cuGetExportTable;
// This is the call that will take the place of the original
int INTER_cuGetExportTable( const void * * ppExportTable, const CUuuid * pExportTableId ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)ppExportTable,(void *)pExportTableId};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuGetExportTable, (void*) PTR_ORIGINAL_cuGetExportTable, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuTexRefSetAddress2D_v2
//int ORIGINAL_cuTexRefSetAddress2D_v2( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuTexRefSetAddress2D_v2;
void * PTR_ORIGINAL_cuTexRefSetAddress2D_v2;
// This is the call that will take the place of the original
int INTER_cuTexRefSetAddress2D_v2( CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR * desc, CUdeviceptr dptr, size_t Pitch ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)hTexRef,(void *)desc,(void *)dptr,(void *)Pitch};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuTexRefSetAddress2D_v2, (void*) PTR_ORIGINAL_cuTexRefSetAddress2D_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoD_v2
//int ORIGINAL_cuMemcpyHtoD_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoD_v2;
void * PTR_ORIGINAL_cuMemcpyHtoD_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoD_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcHost,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoD_v2, (void*) PTR_ORIGINAL_cuMemcpyHtoD_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoH_v2
//int ORIGINAL_cuMemcpyDtoH_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoH_v2;
void * PTR_ORIGINAL_cuMemcpyDtoH_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoH_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcDevice,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoH_v2, (void*) PTR_ORIGINAL_cuMemcpyDtoH_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoD_v2
//int ORIGINAL_cuMemcpyDtoD_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoD_v2;
void * PTR_ORIGINAL_cuMemcpyDtoD_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoD_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcDevice,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoD_v2, (void*) PTR_ORIGINAL_cuMemcpyDtoD_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoA_v2
//int ORIGINAL_cuMemcpyDtoA_v2( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoA_v2;
void * PTR_ORIGINAL_cuMemcpyDtoA_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoA_v2( CUarray dstArray, size_t dstOffset, CUdeviceptr srcDevice, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcDevice,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoA_v2, (void*) PTR_ORIGINAL_cuMemcpyDtoA_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoD_v2
//int ORIGINAL_cuMemcpyAtoD_v2( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoD_v2;
void * PTR_ORIGINAL_cuMemcpyAtoD_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoD_v2( CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcArray,(void *)srcOffset,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoD_v2, (void*) PTR_ORIGINAL_cuMemcpyAtoD_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoA_v2
//int ORIGINAL_cuMemcpyHtoA_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoA_v2;
void * PTR_ORIGINAL_cuMemcpyHtoA_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoA_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcHost,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoA_v2, (void*) PTR_ORIGINAL_cuMemcpyHtoA_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoH_v2
//int ORIGINAL_cuMemcpyAtoH_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoH_v2;
void * PTR_ORIGINAL_cuMemcpyAtoH_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoH_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcArray,(void *)srcOffset,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoH_v2, (void*) PTR_ORIGINAL_cuMemcpyAtoH_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoA_v2
//int ORIGINAL_cuMemcpyAtoA_v2( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoA_v2;
void * PTR_ORIGINAL_cuMemcpyAtoA_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoA_v2( CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcArray,(void *)srcOffset,(void *)ByteCount};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoA_v2, (void*) PTR_ORIGINAL_cuMemcpyAtoA_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoAAsync_v2
//int ORIGINAL_cuMemcpyHtoAAsync_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoAAsync_v2;
void * PTR_ORIGINAL_cuMemcpyHtoAAsync_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoAAsync_v2( CUarray dstArray, size_t dstOffset, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstArray,(void *)dstOffset,(void *)srcHost,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoAAsync_v2, (void*) PTR_ORIGINAL_cuMemcpyHtoAAsync_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyAtoHAsync_v2
//int ORIGINAL_cuMemcpyAtoHAsync_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyAtoHAsync_v2;
void * PTR_ORIGINAL_cuMemcpyAtoHAsync_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyAtoHAsync_v2( void * dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcArray,(void *)srcOffset,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyAtoHAsync_v2, (void*) PTR_ORIGINAL_cuMemcpyAtoHAsync_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy2D_v2
//int ORIGINAL_cuMemcpy2D_v2( const CUDA_MEMCPY2D * pCopy ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy2D_v2;
void * PTR_ORIGINAL_cuMemcpy2D_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpy2D_v2( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy2D_v2, (void*) PTR_ORIGINAL_cuMemcpy2D_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy2DUnaligned_v2
//int ORIGINAL_cuMemcpy2DUnaligned_v2( const CUDA_MEMCPY2D * pCopy ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy2DUnaligned_v2;
void * PTR_ORIGINAL_cuMemcpy2DUnaligned_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpy2DUnaligned_v2( const CUDA_MEMCPY2D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy2DUnaligned_v2, (void*) PTR_ORIGINAL_cuMemcpy2DUnaligned_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy3D_v2
//int ORIGINAL_cuMemcpy3D_v2( const CUDA_MEMCPY3D * pCopy ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy3D_v2;
void * PTR_ORIGINAL_cuMemcpy3D_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpy3D_v2( const CUDA_MEMCPY3D * pCopy ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy};
	std::vector<void **> params = { &tmpParams[0] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy3D_v2, (void*) PTR_ORIGINAL_cuMemcpy3D_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyHtoDAsync_v2
//int ORIGINAL_cuMemcpyHtoDAsync_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyHtoDAsync_v2;
void * PTR_ORIGINAL_cuMemcpyHtoDAsync_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyHtoDAsync_v2( CUdeviceptr dstDevice, const void * srcHost, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcHost,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyHtoDAsync_v2, (void*) PTR_ORIGINAL_cuMemcpyHtoDAsync_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoHAsync_v2
//int ORIGINAL_cuMemcpyDtoHAsync_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoHAsync_v2;
void * PTR_ORIGINAL_cuMemcpyDtoHAsync_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoHAsync_v2( void * dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstHost,(void *)srcDevice,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoHAsync_v2, (void*) PTR_ORIGINAL_cuMemcpyDtoHAsync_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpyDtoDAsync_v2
//int ORIGINAL_cuMemcpyDtoDAsync_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpyDtoDAsync_v2;
void * PTR_ORIGINAL_cuMemcpyDtoDAsync_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpyDtoDAsync_v2( CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)srcDevice,(void *)ByteCount,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpyDtoDAsync_v2, (void*) PTR_ORIGINAL_cuMemcpyDtoDAsync_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy2DAsync_v2
//int ORIGINAL_cuMemcpy2DAsync_v2( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy2DAsync_v2;
void * PTR_ORIGINAL_cuMemcpy2DAsync_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpy2DAsync_v2( const CUDA_MEMCPY2D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy2DAsync_v2, (void*) PTR_ORIGINAL_cuMemcpy2DAsync_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemcpy3DAsync_v2
//int ORIGINAL_cuMemcpy3DAsync_v2( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemcpy3DAsync_v2;
void * PTR_ORIGINAL_cuMemcpy3DAsync_v2;
// This is the call that will take the place of the original
int INTER_cuMemcpy3DAsync_v2( const CUDA_MEMCPY3D * pCopy, CUstream hStream ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)pCopy,(void *)hStream};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemcpy3DAsync_v2, (void*) PTR_ORIGINAL_cuMemcpy3DAsync_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD8_v2
//int ORIGINAL_cuMemsetD8_v2( CUdeviceptr dstDevice, unsigned char uc, size_t N ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD8_v2;
void * PTR_ORIGINAL_cuMemsetD8_v2;
// This is the call that will take the place of the original
int INTER_cuMemsetD8_v2( CUdeviceptr dstDevice, unsigned char uc, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)uc,(void *)N};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD8_v2, (void*) PTR_ORIGINAL_cuMemsetD8_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD16_v2
//int ORIGINAL_cuMemsetD16_v2( CUdeviceptr dstDevice, unsigned short us, size_t N ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD16_v2;
void * PTR_ORIGINAL_cuMemsetD16_v2;
// This is the call that will take the place of the original
int INTER_cuMemsetD16_v2( CUdeviceptr dstDevice, unsigned short us, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)us,(void *)N};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD16_v2, (void*) PTR_ORIGINAL_cuMemsetD16_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD32_v2
//int ORIGINAL_cuMemsetD32_v2( CUdeviceptr dstDevice, unsigned int ui, size_t N ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD32_v2;
void * PTR_ORIGINAL_cuMemsetD32_v2;
// This is the call that will take the place of the original
int INTER_cuMemsetD32_v2( CUdeviceptr dstDevice, unsigned int ui, size_t N ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)ui,(void *)N};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD32_v2, (void*) PTR_ORIGINAL_cuMemsetD32_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D8_v2
//int ORIGINAL_cuMemsetD2D8_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D8_v2;
void * PTR_ORIGINAL_cuMemsetD2D8_v2;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D8_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)uc,(void *)Width,(void *)Height};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D8_v2, (void*) PTR_ORIGINAL_cuMemsetD2D8_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D16_v2
//int ORIGINAL_cuMemsetD2D16_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D16_v2;
void * PTR_ORIGINAL_cuMemsetD2D16_v2;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D16_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)us,(void *)Width,(void *)Height};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D16_v2, (void*) PTR_ORIGINAL_cuMemsetD2D16_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}
// This "function" will be rewritten to point to cuMemsetD2D32_v2
//int ORIGINAL_cuMemsetD2D32_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) { }

static gotcha_wrappee_handle_t GOTTCHA_cuMemsetD2D32_v2;
void * PTR_ORIGINAL_cuMemsetD2D32_v2;
// This is the call that will take the place of the original
int INTER_cuMemsetD2D32_v2( CUdeviceptr dstDevice, size_t dstPitch, unsigned int ui, size_t Width, size_t Height ) {
	// Build the instriment factory
	BUILD_FACTORY
	// Gets around an issue on POWER
	std::vector<void *> tmpParams = { (void *)dstDevice,(void *)dstPitch,(void *)ui,(void *)Width,(void *)Height};
	std::vector<void **> params = { &tmpParams[0],&tmpParams[1],&tmpParams[2],&tmpParams[3],&tmpParams[4] };
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_cuMemsetD2D32_v2, (void*) PTR_ORIGINAL_cuMemsetD2D32_v2, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}

}
//static const std::vector<const char *> CallVector = {"cuGetErrorString","cuGetErrorName","cuInit","cuDriverGetVersion","cuDeviceGet","cuDeviceGetCount","cuDeviceGetName","cuDeviceTotalMem","cuDeviceGetAttribute","cuDeviceGetProperties","cuDeviceComputeCapability","cuDevicePrimaryCtxRetain","cuDevicePrimaryCtxRelease","cuDevicePrimaryCtxSetFlags","cuDevicePrimaryCtxGetState","cuDevicePrimaryCtxReset","cuCtxCreate","cuCtxDestroy","cuCtxPushCurrent","cuCtxPopCurrent","cuCtxSetCurrent","cuCtxGetCurrent","cuCtxGetDevice","cuCtxGetFlags","cuCtxSynchronize","cuCtxSetLimit","cuCtxGetLimit","cuCtxGetCacheConfig","cuCtxSetCacheConfig","cuCtxGetSharedMemConfig","cuCtxSetSharedMemConfig","cuCtxGetApiVersion","cuCtxGetStreamPriorityRange","cuCtxAttach","cuCtxDetach","cuModuleLoad","cuModuleLoadData","cuModuleLoadDataEx","cuModuleLoadFatBinary","cuModuleUnload","cuModuleGetFunction","cuModuleGetGlobal","cuModuleGetTexRef","cuModuleGetSurfRef","cuLinkCreate","cuLinkAddData","cuLinkAddFile","cuLinkComplete","cuLinkDestroy","cuMemGetInfo","cuMemAlloc","cuMemAllocPitch","cuMemFree","cuMemGetAddressRange","cuMemAllocHost","cuMemFreeHost","cuMemHostAlloc","cuMemHostGetDevicePointer","cuMemHostGetFlags","cuMemAllocManaged","cuDeviceGetByPCIBusId","cuDeviceGetPCIBusId","cuIpcGetEventHandle","cuIpcOpenEventHandle","cuIpcGetMemHandle","cuIpcOpenMemHandle","cuIpcCloseMemHandle","cuMemHostRegister","cuMemHostUnregister","cuMemcpy","cuMemcpyPeer","cuMemcpyHtoD","cuMemcpyDtoH","cuMemcpyDtoD","cuMemcpyDtoA","cuMemcpyAtoD","cuMemcpyHtoA","cuMemcpyAtoH","cuMemcpyAtoA","cuMemcpy2D","cuMemcpy2DUnaligned","cuMemcpy3D","cuMemcpy3DPeer","cuMemcpyAsync","cuMemcpyPeerAsync","cuMemcpyHtoDAsync","cuMemcpyDtoHAsync","cuMemcpyDtoDAsync","cuMemcpyHtoAAsync","cuMemcpyAtoHAsync","cuMemcpy2DAsync","cuMemcpy3DAsync","cuMemcpy3DPeerAsync","cuMemsetD8","cuMemsetD16","cuMemsetD32","cuMemsetD2D8","cuMemsetD2D16","cuMemsetD2D32","cuMemsetD8Async","cuMemsetD16Async","cuMemsetD32Async","cuMemsetD2D8Async","cuMemsetD2D16Async","cuMemsetD2D32Async","cuArrayCreate","cuArrayGetDescriptor","cuArrayDestroy","cuArray3DCreate","cuArray3DGetDescriptor","cuMipmappedArrayCreate","cuMipmappedArrayGetLevel","cuMipmappedArrayDestroy","cuPointerGetAttribute","cuMemPrefetchAsync","cuMemAdvise","cuMemRangeGetAttribute","cuMemRangeGetAttributes","cuPointerSetAttribute","cuPointerGetAttributes","cuStreamCreate","cuStreamCreateWithPriority","cuStreamGetPriority","cuStreamGetFlags","cuStreamWaitEvent","cuStreamAddCallback","cuStreamAttachMemAsync","cuStreamQuery","cuStreamSynchronize","cuStreamDestroy","cuEventCreate","cuEventRecord","cuEventQuery","cuEventSynchronize","cuEventDestroy","cuEventElapsedTime","cuStreamWaitValue32","cuStreamWriteValue32","cuStreamBatchMemOp","cuFuncGetAttribute","cuFuncSetCacheConfig","cuFuncSetSharedMemConfig","cuLaunchKernel","cuFuncSetBlockShape","cuFuncSetSharedSize","cuParamSetSize","cuParamSeti","cuParamSetf","cuParamSetv","cuLaunch","cuLaunchGrid","cuLaunchGridAsync","cuParamSetTexRef","cuOccupancyMaxActiveBlocksPerMultiprocessor","cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags","cuOccupancyMaxPotentialBlockSize","cuOccupancyMaxPotentialBlockSizeWithFlags","cuTexRefSetArray","cuTexRefSetMipmappedArray","cuTexRefSetAddress","cuTexRefSetAddress2D","cuTexRefSetFormat","cuTexRefSetAddressMode","cuTexRefSetFilterMode","cuTexRefSetMipmapFilterMode","cuTexRefSetMipmapLevelBias","cuTexRefSetMipmapLevelClamp","cuTexRefSetMaxAnisotropy","cuTexRefSetBorderColor","cuTexRefSetFlags","cuTexRefGetAddress","cuTexRefGetArray","cuTexRefGetMipmappedArray","cuTexRefGetAddressMode","cuTexRefGetFilterMode","cuTexRefGetFormat","cuTexRefGetMipmapFilterMode","cuTexRefGetMipmapLevelBias","cuTexRefGetMipmapLevelClamp","cuTexRefGetMaxAnisotropy","cuTexRefGetBorderColor","cuTexRefGetFlags","cuTexRefCreate","cuTexRefDestroy","cuSurfRefSetArray","cuSurfRefGetArray","cuTexObjectCreate","cuTexObjectDestroy","cuTexObjectGetResourceDesc","cuTexObjectGetTextureDesc","cuTexObjectGetResourceViewDesc","cuSurfObjectCreate","cuSurfObjectDestroy","cuSurfObjectGetResourceDesc","cuDeviceCanAccessPeer","cuDeviceGetP2PAttribute","cuCtxEnablePeerAccess","cuCtxDisablePeerAccess","cuGraphicsUnregisterResource","cuGraphicsSubResourceGetMappedArray","cuGraphicsResourceGetMappedMipmappedArray","cuGraphicsResourceGetMappedPointer","cuGraphicsResourceSetMapFlags","cuGraphicsMapResources","cuGraphicsUnmapResources","cuGetExportTable","cuTexRefSetAddress2D_v2","cuMemcpyHtoD_v2","cuMemcpyDtoH_v2","cuMemcpyDtoD_v2","cuMemcpyDtoA_v2","cuMemcpyAtoD_v2","cuMemcpyHtoA_v2","cuMemcpyAtoH_v2","cuMemcpyAtoA_v2","cuMemcpyHtoAAsync_v2","cuMemcpyAtoHAsync_v2","cuMemcpy2D_v2","cuMemcpy2DUnaligned_v2","cuMemcpy3D_v2","cuMemcpyHtoDAsync_v2","cuMemcpyDtoHAsync_v2","cuMemcpyDtoDAsync_v2","cuMemcpy2DAsync_v2","cuMemcpy3DAsync_v2","cuMemsetD8_v2","cuMemsetD16_v2","cuMemsetD32_v2","cuMemsetD2D8_v2","cuMemsetD2D16_v2","cuMemsetD2D32_v2"};
//	{"cuTexRefSetMipmapLevelBias",(void *)INTER_cuTexRefSetMipmapLevelBias,&GOTTCHA_cuTexRefSetMipmapLevelBias},
//	{"cuTexRefSetMipmapLevelClamp",(void *)INTER_cuTexRefSetMipmapLevelClamp,&GOTTCHA_cuTexRefSetMipmapLevelClamp},
#define NUM_GOTFUNCS 229
struct gotcha_binding_t gotfuncs[] = {
	{"cuGetErrorString",(void *)INTER_cuGetErrorString,&GOTTCHA_cuGetErrorString},
	{"cuGetErrorName",(void *)INTER_cuGetErrorName,&GOTTCHA_cuGetErrorName},
	{"cuInit",(void *)INTER_cuInit,&GOTTCHA_cuInit},
	{"cuDriverGetVersion",(void *)INTER_cuDriverGetVersion,&GOTTCHA_cuDriverGetVersion},
	{"cuDeviceGet",(void *)INTER_cuDeviceGet,&GOTTCHA_cuDeviceGet},
	{"cuDeviceGetCount",(void *)INTER_cuDeviceGetCount,&GOTTCHA_cuDeviceGetCount},
	{"cuDeviceGetName",(void *)INTER_cuDeviceGetName,&GOTTCHA_cuDeviceGetName},
	{"cuDeviceTotalMem",(void *)INTER_cuDeviceTotalMem,&GOTTCHA_cuDeviceTotalMem},
	{"cuDeviceGetAttribute",(void *)INTER_cuDeviceGetAttribute,&GOTTCHA_cuDeviceGetAttribute},
	{"cuDeviceGetProperties",(void *)INTER_cuDeviceGetProperties,&GOTTCHA_cuDeviceGetProperties},
	{"cuDeviceComputeCapability",(void *)INTER_cuDeviceComputeCapability,&GOTTCHA_cuDeviceComputeCapability},
	{"cuDevicePrimaryCtxRetain",(void *)INTER_cuDevicePrimaryCtxRetain,&GOTTCHA_cuDevicePrimaryCtxRetain},
	{"cuDevicePrimaryCtxRelease",(void *)INTER_cuDevicePrimaryCtxRelease,&GOTTCHA_cuDevicePrimaryCtxRelease},
	{"cuDevicePrimaryCtxSetFlags",(void *)INTER_cuDevicePrimaryCtxSetFlags,&GOTTCHA_cuDevicePrimaryCtxSetFlags},
	{"cuDevicePrimaryCtxGetState",(void *)INTER_cuDevicePrimaryCtxGetState,&GOTTCHA_cuDevicePrimaryCtxGetState},
	{"cuDevicePrimaryCtxReset",(void *)INTER_cuDevicePrimaryCtxReset,&GOTTCHA_cuDevicePrimaryCtxReset},
	{"cuCtxCreate",(void *)INTER_cuCtxCreate,&GOTTCHA_cuCtxCreate},
	{"cuCtxDestroy",(void *)INTER_cuCtxDestroy,&GOTTCHA_cuCtxDestroy},
	{"cuCtxPushCurrent",(void *)INTER_cuCtxPushCurrent,&GOTTCHA_cuCtxPushCurrent},
	{"cuCtxPopCurrent",(void *)INTER_cuCtxPopCurrent,&GOTTCHA_cuCtxPopCurrent},
	{"cuCtxSetCurrent",(void *)INTER_cuCtxSetCurrent,&GOTTCHA_cuCtxSetCurrent},
	{"cuCtxGetCurrent",(void *)INTER_cuCtxGetCurrent,&GOTTCHA_cuCtxGetCurrent},
	{"cuCtxGetDevice",(void *)INTER_cuCtxGetDevice,&GOTTCHA_cuCtxGetDevice},
	{"cuCtxGetFlags",(void *)INTER_cuCtxGetFlags,&GOTTCHA_cuCtxGetFlags},
	{"cuCtxSynchronize",(void *)INTER_cuCtxSynchronize,&GOTTCHA_cuCtxSynchronize},
	{"cuCtxSetLimit",(void *)INTER_cuCtxSetLimit,&GOTTCHA_cuCtxSetLimit},
	{"cuCtxGetLimit",(void *)INTER_cuCtxGetLimit,&GOTTCHA_cuCtxGetLimit},
	{"cuCtxGetCacheConfig",(void *)INTER_cuCtxGetCacheConfig,&GOTTCHA_cuCtxGetCacheConfig},
	{"cuCtxSetCacheConfig",(void *)INTER_cuCtxSetCacheConfig,&GOTTCHA_cuCtxSetCacheConfig},
	{"cuCtxGetSharedMemConfig",(void *)INTER_cuCtxGetSharedMemConfig,&GOTTCHA_cuCtxGetSharedMemConfig},
	{"cuCtxSetSharedMemConfig",(void *)INTER_cuCtxSetSharedMemConfig,&GOTTCHA_cuCtxSetSharedMemConfig},
	{"cuCtxGetApiVersion",(void *)INTER_cuCtxGetApiVersion,&GOTTCHA_cuCtxGetApiVersion},
	{"cuCtxGetStreamPriorityRange",(void *)INTER_cuCtxGetStreamPriorityRange,&GOTTCHA_cuCtxGetStreamPriorityRange},
	{"cuCtxAttach",(void *)INTER_cuCtxAttach,&GOTTCHA_cuCtxAttach},
	{"cuCtxDetach",(void *)INTER_cuCtxDetach,&GOTTCHA_cuCtxDetach},
	{"cuModuleLoad",(void *)INTER_cuModuleLoad,&GOTTCHA_cuModuleLoad},
	{"cuModuleLoadData",(void *)INTER_cuModuleLoadData,&GOTTCHA_cuModuleLoadData},
	{"cuModuleLoadDataEx",(void *)INTER_cuModuleLoadDataEx,&GOTTCHA_cuModuleLoadDataEx},
	{"cuModuleLoadFatBinary",(void *)INTER_cuModuleLoadFatBinary,&GOTTCHA_cuModuleLoadFatBinary},
	{"cuModuleUnload",(void *)INTER_cuModuleUnload,&GOTTCHA_cuModuleUnload},
	{"cuModuleGetFunction",(void *)INTER_cuModuleGetFunction,&GOTTCHA_cuModuleGetFunction},
	{"cuModuleGetGlobal",(void *)INTER_cuModuleGetGlobal,&GOTTCHA_cuModuleGetGlobal},
	{"cuModuleGetTexRef",(void *)INTER_cuModuleGetTexRef,&GOTTCHA_cuModuleGetTexRef},
	{"cuModuleGetSurfRef",(void *)INTER_cuModuleGetSurfRef,&GOTTCHA_cuModuleGetSurfRef},
	{"cuLinkCreate",(void *)INTER_cuLinkCreate,&GOTTCHA_cuLinkCreate},
	{"cuLinkAddData",(void *)INTER_cuLinkAddData,&GOTTCHA_cuLinkAddData},
	{"cuLinkAddFile",(void *)INTER_cuLinkAddFile,&GOTTCHA_cuLinkAddFile},
	{"cuLinkComplete",(void *)INTER_cuLinkComplete,&GOTTCHA_cuLinkComplete},
	{"cuLinkDestroy",(void *)INTER_cuLinkDestroy,&GOTTCHA_cuLinkDestroy},
	{"cuMemGetInfo",(void *)INTER_cuMemGetInfo,&GOTTCHA_cuMemGetInfo},
	{"cuMemAlloc",(void *)INTER_cuMemAlloc,&GOTTCHA_cuMemAlloc},
	{"cuMemAllocPitch",(void *)INTER_cuMemAllocPitch,&GOTTCHA_cuMemAllocPitch},
	{"cuMemFree_v2",(void *)INTER_cuMemFree,&GOTTCHA_cuMemFree},
	{"cuMemGetAddressRange",(void *)INTER_cuMemGetAddressRange,&GOTTCHA_cuMemGetAddressRange},
	{"cuMemAllocHost",(void *)INTER_cuMemAllocHost,&GOTTCHA_cuMemAllocHost},
	{"cuMemFreeHost",(void *)INTER_cuMemFreeHost,&GOTTCHA_cuMemFreeHost},
	{"cuMemHostAlloc",(void *)INTER_cuMemHostAlloc,&GOTTCHA_cuMemHostAlloc},
	{"cuMemHostGetDevicePointer",(void *)INTER_cuMemHostGetDevicePointer,&GOTTCHA_cuMemHostGetDevicePointer},
	{"cuMemHostGetFlags",(void *)INTER_cuMemHostGetFlags,&GOTTCHA_cuMemHostGetFlags},
	{"cuMemAllocManaged",(void *)INTER_cuMemAllocManaged,&GOTTCHA_cuMemAllocManaged},
	{"cuDeviceGetByPCIBusId",(void *)INTER_cuDeviceGetByPCIBusId,&GOTTCHA_cuDeviceGetByPCIBusId},
	{"cuDeviceGetPCIBusId",(void *)INTER_cuDeviceGetPCIBusId,&GOTTCHA_cuDeviceGetPCIBusId},
	{"cuIpcGetEventHandle",(void *)INTER_cuIpcGetEventHandle,&GOTTCHA_cuIpcGetEventHandle},
	{"cuIpcOpenEventHandle",(void *)INTER_cuIpcOpenEventHandle,&GOTTCHA_cuIpcOpenEventHandle},
	{"cuIpcGetMemHandle",(void *)INTER_cuIpcGetMemHandle,&GOTTCHA_cuIpcGetMemHandle},
	{"cuIpcOpenMemHandle",(void *)INTER_cuIpcOpenMemHandle,&GOTTCHA_cuIpcOpenMemHandle},
	{"cuIpcCloseMemHandle",(void *)INTER_cuIpcCloseMemHandle,&GOTTCHA_cuIpcCloseMemHandle},
	{"cuMemHostRegister",(void *)INTER_cuMemHostRegister,&GOTTCHA_cuMemHostRegister},
	{"cuMemHostUnregister",(void *)INTER_cuMemHostUnregister,&GOTTCHA_cuMemHostUnregister},
	{"cuMemcpy",(void *)INTER_cuMemcpy,&GOTTCHA_cuMemcpy},
	{"cuMemcpyPeer",(void *)INTER_cuMemcpyPeer,&GOTTCHA_cuMemcpyPeer},
	{"cuMemcpyHtoD",(void *)INTER_cuMemcpyHtoD,&GOTTCHA_cuMemcpyHtoD},
	{"cuMemcpyDtoH",(void *)INTER_cuMemcpyDtoH,&GOTTCHA_cuMemcpyDtoH},
	{"cuMemcpyDtoD",(void *)INTER_cuMemcpyDtoD,&GOTTCHA_cuMemcpyDtoD},
	{"cuMemcpyDtoA",(void *)INTER_cuMemcpyDtoA,&GOTTCHA_cuMemcpyDtoA},
	{"cuMemcpyAtoD",(void *)INTER_cuMemcpyAtoD,&GOTTCHA_cuMemcpyAtoD},
	{"cuMemcpyHtoA",(void *)INTER_cuMemcpyHtoA,&GOTTCHA_cuMemcpyHtoA},
	{"cuMemcpyAtoH",(void *)INTER_cuMemcpyAtoH,&GOTTCHA_cuMemcpyAtoH},
	{"cuMemcpyAtoA",(void *)INTER_cuMemcpyAtoA,&GOTTCHA_cuMemcpyAtoA},
	{"cuMemcpy2D",(void *)INTER_cuMemcpy2D,&GOTTCHA_cuMemcpy2D},
	{"cuMemcpy2DUnaligned",(void *)INTER_cuMemcpy2DUnaligned,&GOTTCHA_cuMemcpy2DUnaligned},
	{"cuMemcpy3D",(void *)INTER_cuMemcpy3D,&GOTTCHA_cuMemcpy3D},
	{"cuMemcpy3DPeer",(void *)INTER_cuMemcpy3DPeer,&GOTTCHA_cuMemcpy3DPeer},
	{"cuMemcpyAsync",(void *)INTER_cuMemcpyAsync,&GOTTCHA_cuMemcpyAsync},
	{"cuMemcpyPeerAsync",(void *)INTER_cuMemcpyPeerAsync,&GOTTCHA_cuMemcpyPeerAsync},
	{"cuMemcpyHtoDAsync",(void *)INTER_cuMemcpyHtoDAsync,&GOTTCHA_cuMemcpyHtoDAsync},
	{"cuMemcpyDtoHAsync",(void *)INTER_cuMemcpyDtoHAsync,&GOTTCHA_cuMemcpyDtoHAsync},
	{"cuMemcpyDtoDAsync",(void *)INTER_cuMemcpyDtoDAsync,&GOTTCHA_cuMemcpyDtoDAsync},
	{"cuMemcpyHtoAAsync",(void *)INTER_cuMemcpyHtoAAsync,&GOTTCHA_cuMemcpyHtoAAsync},
	{"cuMemcpyAtoHAsync",(void *)INTER_cuMemcpyAtoHAsync,&GOTTCHA_cuMemcpyAtoHAsync},
	{"cuMemcpy2DAsync",(void *)INTER_cuMemcpy2DAsync,&GOTTCHA_cuMemcpy2DAsync},
	{"cuMemcpy3DAsync",(void *)INTER_cuMemcpy3DAsync,&GOTTCHA_cuMemcpy3DAsync},
	{"cuMemcpy3DPeerAsync",(void *)INTER_cuMemcpy3DPeerAsync,&GOTTCHA_cuMemcpy3DPeerAsync},
	{"cuMemsetD8",(void *)INTER_cuMemsetD8,&GOTTCHA_cuMemsetD8},
	{"cuMemsetD16",(void *)INTER_cuMemsetD16,&GOTTCHA_cuMemsetD16},
	{"cuMemsetD32",(void *)INTER_cuMemsetD32,&GOTTCHA_cuMemsetD32},
	{"cuMemsetD2D8",(void *)INTER_cuMemsetD2D8,&GOTTCHA_cuMemsetD2D8},
	{"cuMemsetD2D16",(void *)INTER_cuMemsetD2D16,&GOTTCHA_cuMemsetD2D16},
	{"cuMemsetD2D32",(void *)INTER_cuMemsetD2D32,&GOTTCHA_cuMemsetD2D32},
	{"cuMemsetD8Async",(void *)INTER_cuMemsetD8Async,&GOTTCHA_cuMemsetD8Async},
	{"cuMemsetD16Async",(void *)INTER_cuMemsetD16Async,&GOTTCHA_cuMemsetD16Async},
	{"cuMemsetD32Async",(void *)INTER_cuMemsetD32Async,&GOTTCHA_cuMemsetD32Async},
	{"cuMemsetD2D8Async",(void *)INTER_cuMemsetD2D8Async,&GOTTCHA_cuMemsetD2D8Async},
	{"cuMemsetD2D16Async",(void *)INTER_cuMemsetD2D16Async,&GOTTCHA_cuMemsetD2D16Async},
	{"cuMemsetD2D32Async",(void *)INTER_cuMemsetD2D32Async,&GOTTCHA_cuMemsetD2D32Async},
	{"cuArrayCreate",(void *)INTER_cuArrayCreate,&GOTTCHA_cuArrayCreate},
	{"cuArrayGetDescriptor",(void *)INTER_cuArrayGetDescriptor,&GOTTCHA_cuArrayGetDescriptor},
	{"cuArrayDestroy",(void *)INTER_cuArrayDestroy,&GOTTCHA_cuArrayDestroy},
	{"cuArray3DCreate",(void *)INTER_cuArray3DCreate,&GOTTCHA_cuArray3DCreate},
	{"cuArray3DGetDescriptor",(void *)INTER_cuArray3DGetDescriptor,&GOTTCHA_cuArray3DGetDescriptor},
	{"cuMipmappedArrayCreate",(void *)INTER_cuMipmappedArrayCreate,&GOTTCHA_cuMipmappedArrayCreate},
	{"cuMipmappedArrayGetLevel",(void *)INTER_cuMipmappedArrayGetLevel,&GOTTCHA_cuMipmappedArrayGetLevel},
	{"cuMipmappedArrayDestroy",(void *)INTER_cuMipmappedArrayDestroy,&GOTTCHA_cuMipmappedArrayDestroy},
	{"cuPointerGetAttribute",(void *)INTER_cuPointerGetAttribute,&GOTTCHA_cuPointerGetAttribute},
	{"cuMemPrefetchAsync",(void *)INTER_cuMemPrefetchAsync,&GOTTCHA_cuMemPrefetchAsync},
	{"cuMemAdvise",(void *)INTER_cuMemAdvise,&GOTTCHA_cuMemAdvise},
	{"cuMemRangeGetAttribute",(void *)INTER_cuMemRangeGetAttribute,&GOTTCHA_cuMemRangeGetAttribute},
	{"cuMemRangeGetAttributes",(void *)INTER_cuMemRangeGetAttributes,&GOTTCHA_cuMemRangeGetAttributes},
	{"cuPointerSetAttribute",(void *)INTER_cuPointerSetAttribute,&GOTTCHA_cuPointerSetAttribute},
	{"cuPointerGetAttributes",(void *)INTER_cuPointerGetAttributes,&GOTTCHA_cuPointerGetAttributes},
	{"cuStreamCreate",(void *)INTER_cuStreamCreate,&GOTTCHA_cuStreamCreate},
	{"cuStreamCreateWithPriority",(void *)INTER_cuStreamCreateWithPriority,&GOTTCHA_cuStreamCreateWithPriority},
	{"cuStreamGetPriority",(void *)INTER_cuStreamGetPriority,&GOTTCHA_cuStreamGetPriority},
	{"cuStreamGetFlags",(void *)INTER_cuStreamGetFlags,&GOTTCHA_cuStreamGetFlags},
	{"cuStreamWaitEvent",(void *)INTER_cuStreamWaitEvent,&GOTTCHA_cuStreamWaitEvent},
	{"cuStreamAddCallback",(void *)INTER_cuStreamAddCallback,&GOTTCHA_cuStreamAddCallback},
	{"cuStreamAttachMemAsync",(void *)INTER_cuStreamAttachMemAsync,&GOTTCHA_cuStreamAttachMemAsync},
	{"cuStreamQuery",(void *)INTER_cuStreamQuery,&GOTTCHA_cuStreamQuery},
	{"cuStreamSynchronize",(void *)INTER_cuStreamSynchronize,&GOTTCHA_cuStreamSynchronize},
	{"cuStreamDestroy",(void *)INTER_cuStreamDestroy,&GOTTCHA_cuStreamDestroy},
	{"cuEventCreate",(void *)INTER_cuEventCreate,&GOTTCHA_cuEventCreate},
	{"cuEventRecord",(void *)INTER_cuEventRecord,&GOTTCHA_cuEventRecord},
	{"cuEventQuery",(void *)INTER_cuEventQuery,&GOTTCHA_cuEventQuery},
	{"cuEventSynchronize",(void *)INTER_cuEventSynchronize,&GOTTCHA_cuEventSynchronize},
	{"cuEventDestroy",(void *)INTER_cuEventDestroy,&GOTTCHA_cuEventDestroy},
	{"cuEventElapsedTime",(void *)INTER_cuEventElapsedTime,&GOTTCHA_cuEventElapsedTime},
	{"cuStreamWaitValue32",(void *)INTER_cuStreamWaitValue32,&GOTTCHA_cuStreamWaitValue32},
	{"cuStreamWriteValue32",(void *)INTER_cuStreamWriteValue32,&GOTTCHA_cuStreamWriteValue32},
	{"cuStreamBatchMemOp",(void *)INTER_cuStreamBatchMemOp,&GOTTCHA_cuStreamBatchMemOp},
	{"cuFuncGetAttribute",(void *)INTER_cuFuncGetAttribute,&GOTTCHA_cuFuncGetAttribute},
	{"cuFuncSetCacheConfig",(void *)INTER_cuFuncSetCacheConfig,&GOTTCHA_cuFuncSetCacheConfig},
	{"cuFuncSetSharedMemConfig",(void *)INTER_cuFuncSetSharedMemConfig,&GOTTCHA_cuFuncSetSharedMemConfig},
	{"cuLaunchKernel",(void *)INTER_cuLaunchKernel,&GOTTCHA_cuLaunchKernel},
	{"cuFuncSetBlockShape",(void *)INTER_cuFuncSetBlockShape,&GOTTCHA_cuFuncSetBlockShape},
	{"cuFuncSetSharedSize",(void *)INTER_cuFuncSetSharedSize,&GOTTCHA_cuFuncSetSharedSize},
	{"cuParamSetSize",(void *)INTER_cuParamSetSize,&GOTTCHA_cuParamSetSize},
	{"cuParamSeti",(void *)INTER_cuParamSeti,&GOTTCHA_cuParamSeti},
	{"cuParamSetf",(void *)INTER_cuParamSetf,&GOTTCHA_cuParamSetf},
	{"cuParamSetv",(void *)INTER_cuParamSetv,&GOTTCHA_cuParamSetv},
	{"cuLaunch",(void *)INTER_cuLaunch,&GOTTCHA_cuLaunch},
	{"cuLaunchGrid",(void *)INTER_cuLaunchGrid,&GOTTCHA_cuLaunchGrid},
	{"cuLaunchGridAsync",(void *)INTER_cuLaunchGridAsync,&GOTTCHA_cuLaunchGridAsync},
	{"cuParamSetTexRef",(void *)INTER_cuParamSetTexRef,&GOTTCHA_cuParamSetTexRef},
	{"cuOccupancyMaxActiveBlocksPerMultiprocessor",(void *)INTER_cuOccupancyMaxActiveBlocksPerMultiprocessor,&GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessor},
	{"cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags",(void *)INTER_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags,&GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags},
	{"cuOccupancyMaxPotentialBlockSize",(void *)INTER_cuOccupancyMaxPotentialBlockSize,&GOTTCHA_cuOccupancyMaxPotentialBlockSize},
	{"cuOccupancyMaxPotentialBlockSizeWithFlags",(void *)INTER_cuOccupancyMaxPotentialBlockSizeWithFlags,&GOTTCHA_cuOccupancyMaxPotentialBlockSizeWithFlags},
	{"cuTexRefSetArray",(void *)INTER_cuTexRefSetArray,&GOTTCHA_cuTexRefSetArray},
	{"cuTexRefSetMipmappedArray",(void *)INTER_cuTexRefSetMipmappedArray,&GOTTCHA_cuTexRefSetMipmappedArray},
	{"cuTexRefSetAddress",(void *)INTER_cuTexRefSetAddress,&GOTTCHA_cuTexRefSetAddress},
	{"cuTexRefSetAddress2D",(void *)INTER_cuTexRefSetAddress2D,&GOTTCHA_cuTexRefSetAddress2D},
	{"cuTexRefSetFormat",(void *)INTER_cuTexRefSetFormat,&GOTTCHA_cuTexRefSetFormat},
	{"cuTexRefSetAddressMode",(void *)INTER_cuTexRefSetAddressMode,&GOTTCHA_cuTexRefSetAddressMode},
	{"cuTexRefSetFilterMode",(void *)INTER_cuTexRefSetFilterMode,&GOTTCHA_cuTexRefSetFilterMode},
	{"cuTexRefSetMipmapFilterMode",(void *)INTER_cuTexRefSetMipmapFilterMode,&GOTTCHA_cuTexRefSetMipmapFilterMode},
	{"cuTexRefSetMaxAnisotropy",(void *)INTER_cuTexRefSetMaxAnisotropy,&GOTTCHA_cuTexRefSetMaxAnisotropy},
	{"cuTexRefSetBorderColor",(void *)INTER_cuTexRefSetBorderColor,&GOTTCHA_cuTexRefSetBorderColor},
	{"cuTexRefSetFlags",(void *)INTER_cuTexRefSetFlags,&GOTTCHA_cuTexRefSetFlags},
	{"cuTexRefGetAddress",(void *)INTER_cuTexRefGetAddress,&GOTTCHA_cuTexRefGetAddress},
	{"cuTexRefGetArray",(void *)INTER_cuTexRefGetArray,&GOTTCHA_cuTexRefGetArray},
	{"cuTexRefGetMipmappedArray",(void *)INTER_cuTexRefGetMipmappedArray,&GOTTCHA_cuTexRefGetMipmappedArray},
	{"cuTexRefGetAddressMode",(void *)INTER_cuTexRefGetAddressMode,&GOTTCHA_cuTexRefGetAddressMode},
	{"cuTexRefGetFilterMode",(void *)INTER_cuTexRefGetFilterMode,&GOTTCHA_cuTexRefGetFilterMode},
	{"cuTexRefGetFormat",(void *)INTER_cuTexRefGetFormat,&GOTTCHA_cuTexRefGetFormat},
	{"cuTexRefGetMipmapFilterMode",(void *)INTER_cuTexRefGetMipmapFilterMode,&GOTTCHA_cuTexRefGetMipmapFilterMode},
	{"cuTexRefGetMipmapLevelBias",(void *)INTER_cuTexRefGetMipmapLevelBias,&GOTTCHA_cuTexRefGetMipmapLevelBias},
	{"cuTexRefGetMipmapLevelClamp",(void *)INTER_cuTexRefGetMipmapLevelClamp,&GOTTCHA_cuTexRefGetMipmapLevelClamp},
	{"cuTexRefGetMaxAnisotropy",(void *)INTER_cuTexRefGetMaxAnisotropy,&GOTTCHA_cuTexRefGetMaxAnisotropy},
	{"cuTexRefGetBorderColor",(void *)INTER_cuTexRefGetBorderColor,&GOTTCHA_cuTexRefGetBorderColor},
	{"cuTexRefGetFlags",(void *)INTER_cuTexRefGetFlags,&GOTTCHA_cuTexRefGetFlags},
	{"cuTexRefCreate",(void *)INTER_cuTexRefCreate,&GOTTCHA_cuTexRefCreate},
	{"cuTexRefDestroy",(void *)INTER_cuTexRefDestroy,&GOTTCHA_cuTexRefDestroy},
	{"cuSurfRefSetArray",(void *)INTER_cuSurfRefSetArray,&GOTTCHA_cuSurfRefSetArray},
	{"cuSurfRefGetArray",(void *)INTER_cuSurfRefGetArray,&GOTTCHA_cuSurfRefGetArray},
	{"cuTexObjectCreate",(void *)INTER_cuTexObjectCreate,&GOTTCHA_cuTexObjectCreate},
	{"cuTexObjectDestroy",(void *)INTER_cuTexObjectDestroy,&GOTTCHA_cuTexObjectDestroy},
	{"cuTexObjectGetResourceDesc",(void *)INTER_cuTexObjectGetResourceDesc,&GOTTCHA_cuTexObjectGetResourceDesc},
	{"cuTexObjectGetTextureDesc",(void *)INTER_cuTexObjectGetTextureDesc,&GOTTCHA_cuTexObjectGetTextureDesc},
	{"cuTexObjectGetResourceViewDesc",(void *)INTER_cuTexObjectGetResourceViewDesc,&GOTTCHA_cuTexObjectGetResourceViewDesc},
	{"cuSurfObjectCreate",(void *)INTER_cuSurfObjectCreate,&GOTTCHA_cuSurfObjectCreate},
	{"cuSurfObjectDestroy",(void *)INTER_cuSurfObjectDestroy,&GOTTCHA_cuSurfObjectDestroy},
	{"cuSurfObjectGetResourceDesc",(void *)INTER_cuSurfObjectGetResourceDesc,&GOTTCHA_cuSurfObjectGetResourceDesc},
	{"cuDeviceCanAccessPeer",(void *)INTER_cuDeviceCanAccessPeer,&GOTTCHA_cuDeviceCanAccessPeer},
	{"cuDeviceGetP2PAttribute",(void *)INTER_cuDeviceGetP2PAttribute,&GOTTCHA_cuDeviceGetP2PAttribute},
	{"cuCtxEnablePeerAccess",(void *)INTER_cuCtxEnablePeerAccess,&GOTTCHA_cuCtxEnablePeerAccess},
	{"cuCtxDisablePeerAccess",(void *)INTER_cuCtxDisablePeerAccess,&GOTTCHA_cuCtxDisablePeerAccess},
	{"cuGraphicsUnregisterResource",(void *)INTER_cuGraphicsUnregisterResource,&GOTTCHA_cuGraphicsUnregisterResource},
	{"cuGraphicsSubResourceGetMappedArray",(void *)INTER_cuGraphicsSubResourceGetMappedArray,&GOTTCHA_cuGraphicsSubResourceGetMappedArray},
	{"cuGraphicsResourceGetMappedMipmappedArray",(void *)INTER_cuGraphicsResourceGetMappedMipmappedArray,&GOTTCHA_cuGraphicsResourceGetMappedMipmappedArray},
	{"cuGraphicsResourceGetMappedPointer",(void *)INTER_cuGraphicsResourceGetMappedPointer,&GOTTCHA_cuGraphicsResourceGetMappedPointer},
	{"cuGraphicsResourceSetMapFlags",(void *)INTER_cuGraphicsResourceSetMapFlags,&GOTTCHA_cuGraphicsResourceSetMapFlags},
	{"cuGraphicsMapResources",(void *)INTER_cuGraphicsMapResources,&GOTTCHA_cuGraphicsMapResources},
	{"cuGraphicsUnmapResources",(void *)INTER_cuGraphicsUnmapResources,&GOTTCHA_cuGraphicsUnmapResources},
	{"cuGetExportTable",(void *)INTER_cuGetExportTable,&GOTTCHA_cuGetExportTable},
	{"cuTexRefSetAddress2D_v2",(void *)INTER_cuTexRefSetAddress2D_v2,&GOTTCHA_cuTexRefSetAddress2D_v2},
	{"cuMemcpyHtoD_v2",(void *)INTER_cuMemcpyHtoD_v2,&GOTTCHA_cuMemcpyHtoD_v2},
	{"cuMemcpyDtoH_v2",(void *)INTER_cuMemcpyDtoH_v2,&GOTTCHA_cuMemcpyDtoH_v2},
	{"cuMemcpyDtoD_v2",(void *)INTER_cuMemcpyDtoD_v2,&GOTTCHA_cuMemcpyDtoD_v2},
	{"cuMemcpyDtoA_v2",(void *)INTER_cuMemcpyDtoA_v2,&GOTTCHA_cuMemcpyDtoA_v2},
	{"cuMemcpyAtoD_v2",(void *)INTER_cuMemcpyAtoD_v2,&GOTTCHA_cuMemcpyAtoD_v2},
	{"cuMemcpyHtoA_v2",(void *)INTER_cuMemcpyHtoA_v2,&GOTTCHA_cuMemcpyHtoA_v2},
	{"cuMemcpyAtoH_v2",(void *)INTER_cuMemcpyAtoH_v2,&GOTTCHA_cuMemcpyAtoH_v2},
	{"cuMemcpyAtoA_v2",(void *)INTER_cuMemcpyAtoA_v2,&GOTTCHA_cuMemcpyAtoA_v2},
	{"cuMemcpyHtoAAsync_v2",(void *)INTER_cuMemcpyHtoAAsync_v2,&GOTTCHA_cuMemcpyHtoAAsync_v2},
	{"cuMemcpyAtoHAsync_v2",(void *)INTER_cuMemcpyAtoHAsync_v2,&GOTTCHA_cuMemcpyAtoHAsync_v2},
	{"cuMemcpy2D_v2",(void *)INTER_cuMemcpy2D_v2,&GOTTCHA_cuMemcpy2D_v2},
	{"cuMemcpy2DUnaligned_v2",(void *)INTER_cuMemcpy2DUnaligned_v2,&GOTTCHA_cuMemcpy2DUnaligned_v2},
	{"cuMemcpy3D_v2",(void *)INTER_cuMemcpy3D_v2,&GOTTCHA_cuMemcpy3D_v2},
	{"cuMemcpyHtoDAsync_v2",(void *)INTER_cuMemcpyHtoDAsync_v2,&GOTTCHA_cuMemcpyHtoDAsync_v2},
	{"cuMemcpyDtoHAsync_v2",(void *)INTER_cuMemcpyDtoHAsync_v2,&GOTTCHA_cuMemcpyDtoHAsync_v2},
	{"cuMemcpyDtoDAsync_v2",(void *)INTER_cuMemcpyDtoDAsync_v2,&GOTTCHA_cuMemcpyDtoDAsync_v2},
	{"cuMemcpy2DAsync_v2",(void *)INTER_cuMemcpy2DAsync_v2,&GOTTCHA_cuMemcpy2DAsync_v2},
	{"cuMemcpy3DAsync_v2",(void *)INTER_cuMemcpy3DAsync_v2,&GOTTCHA_cuMemcpy3DAsync_v2},
	{"cuMemsetD8_v2",(void *)INTER_cuMemsetD8_v2,&GOTTCHA_cuMemsetD8_v2},
	{"cuMemsetD16_v2",(void *)INTER_cuMemsetD16_v2,&GOTTCHA_cuMemsetD16_v2},
	{"cuMemsetD32_v2",(void *)INTER_cuMemsetD32_v2,&GOTTCHA_cuMemsetD32_v2},
	{"cuMemsetD2D8_v2",(void *)INTER_cuMemsetD2D8_v2,&GOTTCHA_cuMemsetD2D8_v2},
	{"cuMemsetD2D16_v2",(void *)INTER_cuMemsetD2D16_v2,&GOTTCHA_cuMemsetD2D16_v2},
	{"cuMemsetD2D32_v2",(void *)INTER_cuMemsetD2D32_v2,&GOTTCHA_cuMemsetD2D32_v2}
};
#define DEFINED_TEMPLATES 1
#ifndef DEFINED_TEMPLATES
#define EXTERN_FLAG extern
#else
#define EXTERN_FLAG 
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

bool _setBindings = false;
extern "C" void DefineBinders() {
	if (_setBindings == true)
		return;
	gotcha_set_priority("cuda/driverapibinders", 1);
	int result = gotcha_wrap(gotfuncs, NUM_GOTFUNCS, "cuda/driverapibinders");
	assert(result == GOTCHA_SUCCESS);
	_setBindings = true;
	std::cerr << "[DefineBinders] In define binders...." << std::endl;
	int (*TMP_PTR_cuGetErrorString)(CUresult,const char * *) = (int(*)(CUresult,const char * *)) gotcha_get_wrappee(GOTTCHA_cuGetErrorString);
	PTR_ORIGINAL_cuGetErrorString = (void *) gotcha_get_wrappee(GOTTCHA_cuGetErrorString);
	Bound_cuGetErrorString = std::bind(TMP_PTR_cuGetErrorString,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuGetErrorName)(CUresult,const char * *) = (int(*)(CUresult,const char * *)) gotcha_get_wrappee(GOTTCHA_cuGetErrorName);
	PTR_ORIGINAL_cuGetErrorName = (void *) gotcha_get_wrappee(GOTTCHA_cuGetErrorName);
	Bound_cuGetErrorName = std::bind(TMP_PTR_cuGetErrorName,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuInit)(unsigned int) = (int(*)(unsigned int)) gotcha_get_wrappee(GOTTCHA_cuInit);
	PTR_ORIGINAL_cuInit = (void *) gotcha_get_wrappee(GOTTCHA_cuInit);
	Bound_cuInit = std::bind(TMP_PTR_cuInit,std::placeholders::_1);
	int (*TMP_PTR_cuDriverGetVersion)(int *) = (int(*)(int *)) gotcha_get_wrappee(GOTTCHA_cuDriverGetVersion);
	PTR_ORIGINAL_cuDriverGetVersion = (void *) gotcha_get_wrappee(GOTTCHA_cuDriverGetVersion);
	Bound_cuDriverGetVersion = std::bind(TMP_PTR_cuDriverGetVersion,std::placeholders::_1);
	int (*TMP_PTR_cuDeviceGet)(CUdevice *,int) = (int(*)(CUdevice *,int)) gotcha_get_wrappee(GOTTCHA_cuDeviceGet);
	PTR_ORIGINAL_cuDeviceGet = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGet);
	Bound_cuDeviceGet = std::bind(TMP_PTR_cuDeviceGet,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuDeviceGetCount)(int *) = (int(*)(int *)) gotcha_get_wrappee(GOTTCHA_cuDeviceGetCount);
	PTR_ORIGINAL_cuDeviceGetCount = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGetCount);
	Bound_cuDeviceGetCount = std::bind(TMP_PTR_cuDeviceGetCount,std::placeholders::_1);
	int (*TMP_PTR_cuDeviceGetName)(char *,int,CUdevice) = (int(*)(char *,int,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceGetName);
	PTR_ORIGINAL_cuDeviceGetName = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGetName);
	Bound_cuDeviceGetName = std::bind(TMP_PTR_cuDeviceGetName,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuDeviceTotalMem)(size_t *,CUdevice) = (int(*)(size_t *,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceTotalMem);
	PTR_ORIGINAL_cuDeviceTotalMem = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceTotalMem);
	Bound_cuDeviceTotalMem = std::bind(TMP_PTR_cuDeviceTotalMem,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuDeviceGetAttribute)(int *,CUdevice_attribute,CUdevice) = (int(*)(int *,CUdevice_attribute,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceGetAttribute);
	PTR_ORIGINAL_cuDeviceGetAttribute = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGetAttribute);
	Bound_cuDeviceGetAttribute = std::bind(TMP_PTR_cuDeviceGetAttribute,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuDeviceGetProperties)(CUdevprop *,CUdevice) = (int(*)(CUdevprop *,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceGetProperties);
	PTR_ORIGINAL_cuDeviceGetProperties = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGetProperties);
	Bound_cuDeviceGetProperties = std::bind(TMP_PTR_cuDeviceGetProperties,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuDeviceComputeCapability)(int *,int *,CUdevice) = (int(*)(int *,int *,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceComputeCapability);
	PTR_ORIGINAL_cuDeviceComputeCapability = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceComputeCapability);
	Bound_cuDeviceComputeCapability = std::bind(TMP_PTR_cuDeviceComputeCapability,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuDevicePrimaryCtxRetain)(CUcontext *,CUdevice) = (int(*)(CUcontext *,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxRetain);
	PTR_ORIGINAL_cuDevicePrimaryCtxRetain = (void *) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxRetain);
	Bound_cuDevicePrimaryCtxRetain = std::bind(TMP_PTR_cuDevicePrimaryCtxRetain,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuDevicePrimaryCtxRelease)(CUdevice) = (int(*)(CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxRelease);
	PTR_ORIGINAL_cuDevicePrimaryCtxRelease = (void *) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxRelease);
	Bound_cuDevicePrimaryCtxRelease = std::bind(TMP_PTR_cuDevicePrimaryCtxRelease,std::placeholders::_1);
	int (*TMP_PTR_cuDevicePrimaryCtxSetFlags)(CUdevice,unsigned int) = (int(*)(CUdevice,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxSetFlags);
	PTR_ORIGINAL_cuDevicePrimaryCtxSetFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxSetFlags);
	Bound_cuDevicePrimaryCtxSetFlags = std::bind(TMP_PTR_cuDevicePrimaryCtxSetFlags,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuDevicePrimaryCtxGetState)(CUdevice,unsigned int *,int *) = (int(*)(CUdevice,unsigned int *,int *)) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxGetState);
	PTR_ORIGINAL_cuDevicePrimaryCtxGetState = (void *) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxGetState);
	Bound_cuDevicePrimaryCtxGetState = std::bind(TMP_PTR_cuDevicePrimaryCtxGetState,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuDevicePrimaryCtxReset)(CUdevice) = (int(*)(CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxReset);
	PTR_ORIGINAL_cuDevicePrimaryCtxReset = (void *) gotcha_get_wrappee(GOTTCHA_cuDevicePrimaryCtxReset);
	Bound_cuDevicePrimaryCtxReset = std::bind(TMP_PTR_cuDevicePrimaryCtxReset,std::placeholders::_1);
	int (*TMP_PTR_cuCtxCreate)(CUcontext *,unsigned int,CUdevice) = (int(*)(CUcontext *,unsigned int,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuCtxCreate);
	PTR_ORIGINAL_cuCtxCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxCreate);
	Bound_cuCtxCreate = std::bind(TMP_PTR_cuCtxCreate,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuCtxDestroy)(CUcontext) = (int(*)(CUcontext)) gotcha_get_wrappee(GOTTCHA_cuCtxDestroy);
	PTR_ORIGINAL_cuCtxDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxDestroy);
	Bound_cuCtxDestroy = std::bind(TMP_PTR_cuCtxDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuCtxPushCurrent)(CUcontext) = (int(*)(CUcontext)) gotcha_get_wrappee(GOTTCHA_cuCtxPushCurrent);
	PTR_ORIGINAL_cuCtxPushCurrent = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxPushCurrent);
	Bound_cuCtxPushCurrent = std::bind(TMP_PTR_cuCtxPushCurrent,std::placeholders::_1);
	int (*TMP_PTR_cuCtxPopCurrent)(CUcontext *) = (int(*)(CUcontext *)) gotcha_get_wrappee(GOTTCHA_cuCtxPopCurrent);
	PTR_ORIGINAL_cuCtxPopCurrent = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxPopCurrent);
	Bound_cuCtxPopCurrent = std::bind(TMP_PTR_cuCtxPopCurrent,std::placeholders::_1);
	int (*TMP_PTR_cuCtxSetCurrent)(CUcontext) = (int(*)(CUcontext)) gotcha_get_wrappee(GOTTCHA_cuCtxSetCurrent);
	PTR_ORIGINAL_cuCtxSetCurrent = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxSetCurrent);
	Bound_cuCtxSetCurrent = std::bind(TMP_PTR_cuCtxSetCurrent,std::placeholders::_1);
	int (*TMP_PTR_cuCtxGetCurrent)(CUcontext *) = (int(*)(CUcontext *)) gotcha_get_wrappee(GOTTCHA_cuCtxGetCurrent);
	PTR_ORIGINAL_cuCtxGetCurrent = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetCurrent);
	Bound_cuCtxGetCurrent = std::bind(TMP_PTR_cuCtxGetCurrent,std::placeholders::_1);
	int (*TMP_PTR_cuCtxGetDevice)(CUdevice *) = (int(*)(CUdevice *)) gotcha_get_wrappee(GOTTCHA_cuCtxGetDevice);
	PTR_ORIGINAL_cuCtxGetDevice = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetDevice);
	Bound_cuCtxGetDevice = std::bind(TMP_PTR_cuCtxGetDevice,std::placeholders::_1);
	int (*TMP_PTR_cuCtxGetFlags)(unsigned int *) = (int(*)(unsigned int *)) gotcha_get_wrappee(GOTTCHA_cuCtxGetFlags);
	PTR_ORIGINAL_cuCtxGetFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetFlags);
	Bound_cuCtxGetFlags = std::bind(TMP_PTR_cuCtxGetFlags,std::placeholders::_1);
	int (*TMP_PTR_cuCtxSynchronize)(void) = (int(*)(void)) gotcha_get_wrappee(GOTTCHA_cuCtxSynchronize);
	PTR_ORIGINAL_cuCtxSynchronize = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxSynchronize);
	Bound_cuCtxSynchronize = std::bind(TMP_PTR_cuCtxSynchronize);
	int (*TMP_PTR_cuCtxSetLimit)(CUlimit,size_t) = (int(*)(CUlimit,size_t)) gotcha_get_wrappee(GOTTCHA_cuCtxSetLimit);
	PTR_ORIGINAL_cuCtxSetLimit = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxSetLimit);
	Bound_cuCtxSetLimit = std::bind(TMP_PTR_cuCtxSetLimit,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuCtxGetLimit)(size_t *,CUlimit) = (int(*)(size_t *,CUlimit)) gotcha_get_wrappee(GOTTCHA_cuCtxGetLimit);
	PTR_ORIGINAL_cuCtxGetLimit = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetLimit);
	Bound_cuCtxGetLimit = std::bind(TMP_PTR_cuCtxGetLimit,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuCtxGetCacheConfig)(CUfunc_cache *) = (int(*)(CUfunc_cache *)) gotcha_get_wrappee(GOTTCHA_cuCtxGetCacheConfig);
	PTR_ORIGINAL_cuCtxGetCacheConfig = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetCacheConfig);
	Bound_cuCtxGetCacheConfig = std::bind(TMP_PTR_cuCtxGetCacheConfig,std::placeholders::_1);
	int (*TMP_PTR_cuCtxSetCacheConfig)(CUfunc_cache) = (int(*)(CUfunc_cache)) gotcha_get_wrappee(GOTTCHA_cuCtxSetCacheConfig);
	PTR_ORIGINAL_cuCtxSetCacheConfig = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxSetCacheConfig);
	Bound_cuCtxSetCacheConfig = std::bind(TMP_PTR_cuCtxSetCacheConfig,std::placeholders::_1);
	int (*TMP_PTR_cuCtxGetSharedMemConfig)(CUsharedconfig *) = (int(*)(CUsharedconfig *)) gotcha_get_wrappee(GOTTCHA_cuCtxGetSharedMemConfig);
	PTR_ORIGINAL_cuCtxGetSharedMemConfig = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetSharedMemConfig);
	Bound_cuCtxGetSharedMemConfig = std::bind(TMP_PTR_cuCtxGetSharedMemConfig,std::placeholders::_1);
	int (*TMP_PTR_cuCtxSetSharedMemConfig)(CUsharedconfig) = (int(*)(CUsharedconfig)) gotcha_get_wrappee(GOTTCHA_cuCtxSetSharedMemConfig);
	PTR_ORIGINAL_cuCtxSetSharedMemConfig = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxSetSharedMemConfig);
	Bound_cuCtxSetSharedMemConfig = std::bind(TMP_PTR_cuCtxSetSharedMemConfig,std::placeholders::_1);
	int (*TMP_PTR_cuCtxGetApiVersion)(CUcontext,unsigned int *) = (int(*)(CUcontext,unsigned int *)) gotcha_get_wrappee(GOTTCHA_cuCtxGetApiVersion);
	PTR_ORIGINAL_cuCtxGetApiVersion = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetApiVersion);
	Bound_cuCtxGetApiVersion = std::bind(TMP_PTR_cuCtxGetApiVersion,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuCtxGetStreamPriorityRange)(int *,int *) = (int(*)(int *,int *)) gotcha_get_wrappee(GOTTCHA_cuCtxGetStreamPriorityRange);
	PTR_ORIGINAL_cuCtxGetStreamPriorityRange = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxGetStreamPriorityRange);
	Bound_cuCtxGetStreamPriorityRange = std::bind(TMP_PTR_cuCtxGetStreamPriorityRange,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuCtxAttach)(CUcontext *,unsigned int) = (int(*)(CUcontext *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuCtxAttach);
	PTR_ORIGINAL_cuCtxAttach = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxAttach);
	Bound_cuCtxAttach = std::bind(TMP_PTR_cuCtxAttach,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuCtxDetach)(CUcontext) = (int(*)(CUcontext)) gotcha_get_wrappee(GOTTCHA_cuCtxDetach);
	PTR_ORIGINAL_cuCtxDetach = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxDetach);
	Bound_cuCtxDetach = std::bind(TMP_PTR_cuCtxDetach,std::placeholders::_1);
	int (*TMP_PTR_cuModuleLoad)(CUmodule *,const char *) = (int(*)(CUmodule *,const char *)) gotcha_get_wrappee(GOTTCHA_cuModuleLoad);
	PTR_ORIGINAL_cuModuleLoad = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleLoad);
	Bound_cuModuleLoad = std::bind(TMP_PTR_cuModuleLoad,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuModuleLoadData)(CUmodule *,const void *) = (int(*)(CUmodule *,const void *)) gotcha_get_wrappee(GOTTCHA_cuModuleLoadData);
	PTR_ORIGINAL_cuModuleLoadData = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleLoadData);
	Bound_cuModuleLoadData = std::bind(TMP_PTR_cuModuleLoadData,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuModuleLoadDataEx)(CUmodule *,const void *,unsigned int,CUjit_option *,void * *) = (int(*)(CUmodule *,const void *,unsigned int,CUjit_option *,void * *)) gotcha_get_wrappee(GOTTCHA_cuModuleLoadDataEx);
	PTR_ORIGINAL_cuModuleLoadDataEx = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleLoadDataEx);
	Bound_cuModuleLoadDataEx = std::bind(TMP_PTR_cuModuleLoadDataEx,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuModuleLoadFatBinary)(CUmodule *,const void *) = (int(*)(CUmodule *,const void *)) gotcha_get_wrappee(GOTTCHA_cuModuleLoadFatBinary);
	PTR_ORIGINAL_cuModuleLoadFatBinary = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleLoadFatBinary);
	Bound_cuModuleLoadFatBinary = std::bind(TMP_PTR_cuModuleLoadFatBinary,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuModuleUnload)(CUmodule) = (int(*)(CUmodule)) gotcha_get_wrappee(GOTTCHA_cuModuleUnload);
	PTR_ORIGINAL_cuModuleUnload = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleUnload);
	Bound_cuModuleUnload = std::bind(TMP_PTR_cuModuleUnload,std::placeholders::_1);
	int (*TMP_PTR_cuModuleGetFunction)(CUfunction *,CUmodule,const char *) = (int(*)(CUfunction *,CUmodule,const char *)) gotcha_get_wrappee(GOTTCHA_cuModuleGetFunction);
	PTR_ORIGINAL_cuModuleGetFunction = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleGetFunction);
	Bound_cuModuleGetFunction = std::bind(TMP_PTR_cuModuleGetFunction,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuModuleGetGlobal)(CUdeviceptr *,size_t *,CUmodule,const char *) = (int(*)(CUdeviceptr *,size_t *,CUmodule,const char *)) gotcha_get_wrappee(GOTTCHA_cuModuleGetGlobal);
	PTR_ORIGINAL_cuModuleGetGlobal = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleGetGlobal);
	Bound_cuModuleGetGlobal = std::bind(TMP_PTR_cuModuleGetGlobal,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuModuleGetTexRef)(CUtexref *,CUmodule,const char *) = (int(*)(CUtexref *,CUmodule,const char *)) gotcha_get_wrappee(GOTTCHA_cuModuleGetTexRef);
	PTR_ORIGINAL_cuModuleGetTexRef = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleGetTexRef);
	Bound_cuModuleGetTexRef = std::bind(TMP_PTR_cuModuleGetTexRef,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuModuleGetSurfRef)(CUsurfref *,CUmodule,const char *) = (int(*)(CUsurfref *,CUmodule,const char *)) gotcha_get_wrappee(GOTTCHA_cuModuleGetSurfRef);
	PTR_ORIGINAL_cuModuleGetSurfRef = (void *) gotcha_get_wrappee(GOTTCHA_cuModuleGetSurfRef);
	Bound_cuModuleGetSurfRef = std::bind(TMP_PTR_cuModuleGetSurfRef,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuLinkCreate)(unsigned int,CUjit_option *,void * *,CUlinkState *) = (int(*)(unsigned int,CUjit_option *,void * *,CUlinkState *)) gotcha_get_wrappee(GOTTCHA_cuLinkCreate);
	PTR_ORIGINAL_cuLinkCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuLinkCreate);
	Bound_cuLinkCreate = std::bind(TMP_PTR_cuLinkCreate,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuLinkAddData)(CUlinkState,CUjitInputType,void *,size_t,const char *,unsigned int,CUjit_option *,void * *) = (int(*)(CUlinkState,CUjitInputType,void *,size_t,const char *,unsigned int,CUjit_option *,void * *)) gotcha_get_wrappee(GOTTCHA_cuLinkAddData);
	PTR_ORIGINAL_cuLinkAddData = (void *) gotcha_get_wrappee(GOTTCHA_cuLinkAddData);
	Bound_cuLinkAddData = std::bind(TMP_PTR_cuLinkAddData,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6,std::placeholders::_7,std::placeholders::_8);
	int (*TMP_PTR_cuLinkAddFile)(CUlinkState,CUjitInputType,const char *,unsigned int,CUjit_option *,void * *) = (int(*)(CUlinkState,CUjitInputType,const char *,unsigned int,CUjit_option *,void * *)) gotcha_get_wrappee(GOTTCHA_cuLinkAddFile);
	PTR_ORIGINAL_cuLinkAddFile = (void *) gotcha_get_wrappee(GOTTCHA_cuLinkAddFile);
	Bound_cuLinkAddFile = std::bind(TMP_PTR_cuLinkAddFile,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
	int (*TMP_PTR_cuLinkComplete)(CUlinkState,void * *,size_t *) = (int(*)(CUlinkState,void * *,size_t *)) gotcha_get_wrappee(GOTTCHA_cuLinkComplete);
	PTR_ORIGINAL_cuLinkComplete = (void *) gotcha_get_wrappee(GOTTCHA_cuLinkComplete);
	Bound_cuLinkComplete = std::bind(TMP_PTR_cuLinkComplete,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuLinkDestroy)(CUlinkState) = (int(*)(CUlinkState)) gotcha_get_wrappee(GOTTCHA_cuLinkDestroy);
	PTR_ORIGINAL_cuLinkDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuLinkDestroy);
	Bound_cuLinkDestroy = std::bind(TMP_PTR_cuLinkDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuMemGetInfo)(size_t *,size_t *) = (int(*)(size_t *,size_t *)) gotcha_get_wrappee(GOTTCHA_cuMemGetInfo);
	PTR_ORIGINAL_cuMemGetInfo = (void *) gotcha_get_wrappee(GOTTCHA_cuMemGetInfo);
	Bound_cuMemGetInfo = std::bind(TMP_PTR_cuMemGetInfo,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemAlloc)(CUdeviceptr *,size_t) = (int(*)(CUdeviceptr *,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemAlloc);
	PTR_ORIGINAL_cuMemAlloc = (void *) gotcha_get_wrappee(GOTTCHA_cuMemAlloc);
	Bound_cuMemAlloc = std::bind(TMP_PTR_cuMemAlloc,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemAllocPitch)(CUdeviceptr *,size_t *,size_t,size_t,unsigned int) = (int(*)(CUdeviceptr *,size_t *,size_t,size_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuMemAllocPitch);
	PTR_ORIGINAL_cuMemAllocPitch = (void *) gotcha_get_wrappee(GOTTCHA_cuMemAllocPitch);
	Bound_cuMemAllocPitch = std::bind(TMP_PTR_cuMemAllocPitch,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemFree)(CUdeviceptr) = (int(*)(CUdeviceptr)) gotcha_get_wrappee(GOTTCHA_cuMemFree);
	PTR_ORIGINAL_cuMemFree = (void *) gotcha_get_wrappee(GOTTCHA_cuMemFree);
	Bound_cuMemFree = std::bind(TMP_PTR_cuMemFree,std::placeholders::_1);
	int (*TMP_PTR_cuMemGetAddressRange)(CUdeviceptr *,size_t *,CUdeviceptr) = (int(*)(CUdeviceptr *,size_t *,CUdeviceptr)) gotcha_get_wrappee(GOTTCHA_cuMemGetAddressRange);
	PTR_ORIGINAL_cuMemGetAddressRange = (void *) gotcha_get_wrappee(GOTTCHA_cuMemGetAddressRange);
	Bound_cuMemGetAddressRange = std::bind(TMP_PTR_cuMemGetAddressRange,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemAllocHost)(void * *,size_t) = (int(*)(void * *,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemAllocHost);
	PTR_ORIGINAL_cuMemAllocHost = (void *) gotcha_get_wrappee(GOTTCHA_cuMemAllocHost);
	Bound_cuMemAllocHost = std::bind(TMP_PTR_cuMemAllocHost,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemFreeHost)(void *) = (int(*)(void *)) gotcha_get_wrappee(GOTTCHA_cuMemFreeHost);
	PTR_ORIGINAL_cuMemFreeHost = (void *) gotcha_get_wrappee(GOTTCHA_cuMemFreeHost);
	Bound_cuMemFreeHost = std::bind(TMP_PTR_cuMemFreeHost,std::placeholders::_1);
	int (*TMP_PTR_cuMemHostAlloc)(void * *,size_t,unsigned int) = (int(*)(void * *,size_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuMemHostAlloc);
	PTR_ORIGINAL_cuMemHostAlloc = (void *) gotcha_get_wrappee(GOTTCHA_cuMemHostAlloc);
	Bound_cuMemHostAlloc = std::bind(TMP_PTR_cuMemHostAlloc,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemHostGetDevicePointer)(CUdeviceptr *,void *,unsigned int) = (int(*)(CUdeviceptr *,void *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuMemHostGetDevicePointer);
	PTR_ORIGINAL_cuMemHostGetDevicePointer = (void *) gotcha_get_wrappee(GOTTCHA_cuMemHostGetDevicePointer);
	Bound_cuMemHostGetDevicePointer = std::bind(TMP_PTR_cuMemHostGetDevicePointer,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemHostGetFlags)(unsigned int *,void *) = (int(*)(unsigned int *,void *)) gotcha_get_wrappee(GOTTCHA_cuMemHostGetFlags);
	PTR_ORIGINAL_cuMemHostGetFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuMemHostGetFlags);
	Bound_cuMemHostGetFlags = std::bind(TMP_PTR_cuMemHostGetFlags,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemAllocManaged)(CUdeviceptr *,size_t,unsigned int) = (int(*)(CUdeviceptr *,size_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuMemAllocManaged);
	PTR_ORIGINAL_cuMemAllocManaged = (void *) gotcha_get_wrappee(GOTTCHA_cuMemAllocManaged);
	Bound_cuMemAllocManaged = std::bind(TMP_PTR_cuMemAllocManaged,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuDeviceGetByPCIBusId)(CUdevice *,const char *) = (int(*)(CUdevice *,const char *)) gotcha_get_wrappee(GOTTCHA_cuDeviceGetByPCIBusId);
	PTR_ORIGINAL_cuDeviceGetByPCIBusId = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGetByPCIBusId);
	Bound_cuDeviceGetByPCIBusId = std::bind(TMP_PTR_cuDeviceGetByPCIBusId,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuDeviceGetPCIBusId)(char *,int,CUdevice) = (int(*)(char *,int,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceGetPCIBusId);
	PTR_ORIGINAL_cuDeviceGetPCIBusId = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGetPCIBusId);
	Bound_cuDeviceGetPCIBusId = std::bind(TMP_PTR_cuDeviceGetPCIBusId,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuIpcGetEventHandle)(CUipcEventHandle *,CUevent) = (int(*)(CUipcEventHandle *,CUevent)) gotcha_get_wrappee(GOTTCHA_cuIpcGetEventHandle);
	PTR_ORIGINAL_cuIpcGetEventHandle = (void *) gotcha_get_wrappee(GOTTCHA_cuIpcGetEventHandle);
	Bound_cuIpcGetEventHandle = std::bind(TMP_PTR_cuIpcGetEventHandle,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuIpcOpenEventHandle)(CUevent *,CUipcEventHandle) = (int(*)(CUevent *,CUipcEventHandle)) gotcha_get_wrappee(GOTTCHA_cuIpcOpenEventHandle);
	PTR_ORIGINAL_cuIpcOpenEventHandle = (void *) gotcha_get_wrappee(GOTTCHA_cuIpcOpenEventHandle);
	Bound_cuIpcOpenEventHandle = std::bind(TMP_PTR_cuIpcOpenEventHandle,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuIpcGetMemHandle)(CUipcMemHandle *,CUdeviceptr) = (int(*)(CUipcMemHandle *,CUdeviceptr)) gotcha_get_wrappee(GOTTCHA_cuIpcGetMemHandle);
	PTR_ORIGINAL_cuIpcGetMemHandle = (void *) gotcha_get_wrappee(GOTTCHA_cuIpcGetMemHandle);
	Bound_cuIpcGetMemHandle = std::bind(TMP_PTR_cuIpcGetMemHandle,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuIpcOpenMemHandle)(CUdeviceptr *,CUipcMemHandle,unsigned int) = (int(*)(CUdeviceptr *,CUipcMemHandle,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuIpcOpenMemHandle);
	PTR_ORIGINAL_cuIpcOpenMemHandle = (void *) gotcha_get_wrappee(GOTTCHA_cuIpcOpenMemHandle);
	Bound_cuIpcOpenMemHandle = std::bind(TMP_PTR_cuIpcOpenMemHandle,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuIpcCloseMemHandle)(CUdeviceptr) = (int(*)(CUdeviceptr)) gotcha_get_wrappee(GOTTCHA_cuIpcCloseMemHandle);
	PTR_ORIGINAL_cuIpcCloseMemHandle = (void *) gotcha_get_wrappee(GOTTCHA_cuIpcCloseMemHandle);
	Bound_cuIpcCloseMemHandle = std::bind(TMP_PTR_cuIpcCloseMemHandle,std::placeholders::_1);
	int (*TMP_PTR_cuMemHostRegister)(void *,size_t,unsigned int) = (int(*)(void *,size_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuMemHostRegister);
	PTR_ORIGINAL_cuMemHostRegister = (void *) gotcha_get_wrappee(GOTTCHA_cuMemHostRegister);
	Bound_cuMemHostRegister = std::bind(TMP_PTR_cuMemHostRegister,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemHostUnregister)(void *) = (int(*)(void *)) gotcha_get_wrappee(GOTTCHA_cuMemHostUnregister);
	PTR_ORIGINAL_cuMemHostUnregister = (void *) gotcha_get_wrappee(GOTTCHA_cuMemHostUnregister);
	Bound_cuMemHostUnregister = std::bind(TMP_PTR_cuMemHostUnregister,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpy)(CUdeviceptr,CUdeviceptr,size_t) = (int(*)(CUdeviceptr,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpy);
	PTR_ORIGINAL_cuMemcpy = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy);
	Bound_cuMemcpy = std::bind(TMP_PTR_cuMemcpy,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemcpyPeer)(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t) = (int(*)(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyPeer);
	PTR_ORIGINAL_cuMemcpyPeer = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyPeer);
	Bound_cuMemcpyPeer = std::bind(TMP_PTR_cuMemcpyPeer,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemcpyHtoD)(CUdeviceptr,const void *,size_t) = (int(*)(CUdeviceptr,const void *,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoD);
	PTR_ORIGINAL_cuMemcpyHtoD = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoD);
	Bound_cuMemcpyHtoD = std::bind(TMP_PTR_cuMemcpyHtoD,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemcpyDtoH)(void *,CUdeviceptr,size_t) = (int(*)(void *,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoH);
	PTR_ORIGINAL_cuMemcpyDtoH = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoH);
	Bound_cuMemcpyDtoH = std::bind(TMP_PTR_cuMemcpyDtoH,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemcpyDtoD)(CUdeviceptr,CUdeviceptr,size_t) = (int(*)(CUdeviceptr,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoD);
	PTR_ORIGINAL_cuMemcpyDtoD = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoD);
	Bound_cuMemcpyDtoD = std::bind(TMP_PTR_cuMemcpyDtoD,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemcpyDtoA)(CUarray,size_t,CUdeviceptr,size_t) = (int(*)(CUarray,size_t,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoA);
	PTR_ORIGINAL_cuMemcpyDtoA = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoA);
	Bound_cuMemcpyDtoA = std::bind(TMP_PTR_cuMemcpyDtoA,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyAtoD)(CUdeviceptr,CUarray,size_t,size_t) = (int(*)(CUdeviceptr,CUarray,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoD);
	PTR_ORIGINAL_cuMemcpyAtoD = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoD);
	Bound_cuMemcpyAtoD = std::bind(TMP_PTR_cuMemcpyAtoD,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyHtoA)(CUarray,size_t,const void *,size_t) = (int(*)(CUarray,size_t,const void *,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoA);
	PTR_ORIGINAL_cuMemcpyHtoA = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoA);
	Bound_cuMemcpyHtoA = std::bind(TMP_PTR_cuMemcpyHtoA,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyAtoH)(void *,CUarray,size_t,size_t) = (int(*)(void *,CUarray,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoH);
	PTR_ORIGINAL_cuMemcpyAtoH = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoH);
	Bound_cuMemcpyAtoH = std::bind(TMP_PTR_cuMemcpyAtoH,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyAtoA)(CUarray,size_t,CUarray,size_t,size_t) = (int(*)(CUarray,size_t,CUarray,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoA);
	PTR_ORIGINAL_cuMemcpyAtoA = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoA);
	Bound_cuMemcpyAtoA = std::bind(TMP_PTR_cuMemcpyAtoA,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemcpy2D)(const CUDA_MEMCPY2D *) = (int(*)(const CUDA_MEMCPY2D *)) gotcha_get_wrappee(GOTTCHA_cuMemcpy2D);
	PTR_ORIGINAL_cuMemcpy2D = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy2D);
	Bound_cuMemcpy2D = std::bind(TMP_PTR_cuMemcpy2D,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpy2DUnaligned)(const CUDA_MEMCPY2D *) = (int(*)(const CUDA_MEMCPY2D *)) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DUnaligned);
	PTR_ORIGINAL_cuMemcpy2DUnaligned = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DUnaligned);
	Bound_cuMemcpy2DUnaligned = std::bind(TMP_PTR_cuMemcpy2DUnaligned,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpy3D)(const CUDA_MEMCPY3D *) = (int(*)(const CUDA_MEMCPY3D *)) gotcha_get_wrappee(GOTTCHA_cuMemcpy3D);
	PTR_ORIGINAL_cuMemcpy3D = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy3D);
	Bound_cuMemcpy3D = std::bind(TMP_PTR_cuMemcpy3D,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpy3DPeer)(const CUDA_MEMCPY3D_PEER *) = (int(*)(const CUDA_MEMCPY3D_PEER *)) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DPeer);
	PTR_ORIGINAL_cuMemcpy3DPeer = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DPeer);
	Bound_cuMemcpy3DPeer = std::bind(TMP_PTR_cuMemcpy3DPeer,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpyAsync)(CUdeviceptr,CUdeviceptr,size_t,CUstream) = (int(*)(CUdeviceptr,CUdeviceptr,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAsync);
	PTR_ORIGINAL_cuMemcpyAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAsync);
	Bound_cuMemcpyAsync = std::bind(TMP_PTR_cuMemcpyAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyPeerAsync)(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t,CUstream) = (int(*)(CUdeviceptr,CUcontext,CUdeviceptr,CUcontext,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyPeerAsync);
	PTR_ORIGINAL_cuMemcpyPeerAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyPeerAsync);
	Bound_cuMemcpyPeerAsync = std::bind(TMP_PTR_cuMemcpyPeerAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
	int (*TMP_PTR_cuMemcpyHtoDAsync)(CUdeviceptr,const void *,size_t,CUstream) = (int(*)(CUdeviceptr,const void *,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoDAsync);
	PTR_ORIGINAL_cuMemcpyHtoDAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoDAsync);
	Bound_cuMemcpyHtoDAsync = std::bind(TMP_PTR_cuMemcpyHtoDAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyDtoHAsync)(void *,CUdeviceptr,size_t,CUstream) = (int(*)(void *,CUdeviceptr,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoHAsync);
	PTR_ORIGINAL_cuMemcpyDtoHAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoHAsync);
	Bound_cuMemcpyDtoHAsync = std::bind(TMP_PTR_cuMemcpyDtoHAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyDtoDAsync)(CUdeviceptr,CUdeviceptr,size_t,CUstream) = (int(*)(CUdeviceptr,CUdeviceptr,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoDAsync);
	PTR_ORIGINAL_cuMemcpyDtoDAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoDAsync);
	Bound_cuMemcpyDtoDAsync = std::bind(TMP_PTR_cuMemcpyDtoDAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyHtoAAsync)(CUarray,size_t,const void *,size_t,CUstream) = (int(*)(CUarray,size_t,const void *,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoAAsync);
	PTR_ORIGINAL_cuMemcpyHtoAAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoAAsync);
	Bound_cuMemcpyHtoAAsync = std::bind(TMP_PTR_cuMemcpyHtoAAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemcpyAtoHAsync)(void *,CUarray,size_t,size_t,CUstream) = (int(*)(void *,CUarray,size_t,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoHAsync);
	PTR_ORIGINAL_cuMemcpyAtoHAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoHAsync);
	Bound_cuMemcpyAtoHAsync = std::bind(TMP_PTR_cuMemcpyAtoHAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemcpy2DAsync)(const CUDA_MEMCPY2D *,CUstream) = (int(*)(const CUDA_MEMCPY2D *,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DAsync);
	PTR_ORIGINAL_cuMemcpy2DAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DAsync);
	Bound_cuMemcpy2DAsync = std::bind(TMP_PTR_cuMemcpy2DAsync,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemcpy3DAsync)(const CUDA_MEMCPY3D *,CUstream) = (int(*)(const CUDA_MEMCPY3D *,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DAsync);
	PTR_ORIGINAL_cuMemcpy3DAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DAsync);
	Bound_cuMemcpy3DAsync = std::bind(TMP_PTR_cuMemcpy3DAsync,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemcpy3DPeerAsync)(const CUDA_MEMCPY3D_PEER *,CUstream) = (int(*)(const CUDA_MEMCPY3D_PEER *,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DPeerAsync);
	PTR_ORIGINAL_cuMemcpy3DPeerAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DPeerAsync);
	Bound_cuMemcpy3DPeerAsync = std::bind(TMP_PTR_cuMemcpy3DPeerAsync,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemsetD8)(CUdeviceptr,unsigned char,size_t) = (int(*)(CUdeviceptr,unsigned char,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD8);
	PTR_ORIGINAL_cuMemsetD8 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD8);
	Bound_cuMemsetD8 = std::bind(TMP_PTR_cuMemsetD8,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemsetD16)(CUdeviceptr,unsigned short,size_t) = (int(*)(CUdeviceptr,unsigned short,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD16);
	PTR_ORIGINAL_cuMemsetD16 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD16);
	Bound_cuMemsetD16 = std::bind(TMP_PTR_cuMemsetD16,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemsetD32)(CUdeviceptr,unsigned int,size_t) = (int(*)(CUdeviceptr,unsigned int,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD32);
	PTR_ORIGINAL_cuMemsetD32 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD32);
	Bound_cuMemsetD32 = std::bind(TMP_PTR_cuMemsetD32,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemsetD2D8)(CUdeviceptr,size_t,unsigned char,size_t,size_t) = (int(*)(CUdeviceptr,size_t,unsigned char,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D8);
	PTR_ORIGINAL_cuMemsetD2D8 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D8);
	Bound_cuMemsetD2D8 = std::bind(TMP_PTR_cuMemsetD2D8,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemsetD2D16)(CUdeviceptr,size_t,unsigned short,size_t,size_t) = (int(*)(CUdeviceptr,size_t,unsigned short,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D16);
	PTR_ORIGINAL_cuMemsetD2D16 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D16);
	Bound_cuMemsetD2D16 = std::bind(TMP_PTR_cuMemsetD2D16,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemsetD2D32)(CUdeviceptr,size_t,unsigned int,size_t,size_t) = (int(*)(CUdeviceptr,size_t,unsigned int,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D32);
	PTR_ORIGINAL_cuMemsetD2D32 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D32);
	Bound_cuMemsetD2D32 = std::bind(TMP_PTR_cuMemsetD2D32,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemsetD8Async)(CUdeviceptr,unsigned char,size_t,CUstream) = (int(*)(CUdeviceptr,unsigned char,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemsetD8Async);
	PTR_ORIGINAL_cuMemsetD8Async = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD8Async);
	Bound_cuMemsetD8Async = std::bind(TMP_PTR_cuMemsetD8Async,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemsetD16Async)(CUdeviceptr,unsigned short,size_t,CUstream) = (int(*)(CUdeviceptr,unsigned short,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemsetD16Async);
	PTR_ORIGINAL_cuMemsetD16Async = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD16Async);
	Bound_cuMemsetD16Async = std::bind(TMP_PTR_cuMemsetD16Async,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemsetD32Async)(CUdeviceptr,unsigned int,size_t,CUstream) = (int(*)(CUdeviceptr,unsigned int,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemsetD32Async);
	PTR_ORIGINAL_cuMemsetD32Async = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD32Async);
	Bound_cuMemsetD32Async = std::bind(TMP_PTR_cuMemsetD32Async,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemsetD2D8Async)(CUdeviceptr,size_t,unsigned char,size_t,size_t,CUstream) = (int(*)(CUdeviceptr,size_t,unsigned char,size_t,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D8Async);
	PTR_ORIGINAL_cuMemsetD2D8Async = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D8Async);
	Bound_cuMemsetD2D8Async = std::bind(TMP_PTR_cuMemsetD2D8Async,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
	int (*TMP_PTR_cuMemsetD2D16Async)(CUdeviceptr,size_t,unsigned short,size_t,size_t,CUstream) = (int(*)(CUdeviceptr,size_t,unsigned short,size_t,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D16Async);
	PTR_ORIGINAL_cuMemsetD2D16Async = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D16Async);
	Bound_cuMemsetD2D16Async = std::bind(TMP_PTR_cuMemsetD2D16Async,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
	int (*TMP_PTR_cuMemsetD2D32Async)(CUdeviceptr,size_t,unsigned int,size_t,size_t,CUstream) = (int(*)(CUdeviceptr,size_t,unsigned int,size_t,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D32Async);
	PTR_ORIGINAL_cuMemsetD2D32Async = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D32Async);
	Bound_cuMemsetD2D32Async = std::bind(TMP_PTR_cuMemsetD2D32Async,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
	int (*TMP_PTR_cuArrayCreate)(CUarray *,const CUDA_ARRAY_DESCRIPTOR *) = (int(*)(CUarray *,const CUDA_ARRAY_DESCRIPTOR *)) gotcha_get_wrappee(GOTTCHA_cuArrayCreate);
	PTR_ORIGINAL_cuArrayCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuArrayCreate);
	Bound_cuArrayCreate = std::bind(TMP_PTR_cuArrayCreate,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuArrayGetDescriptor)(CUDA_ARRAY_DESCRIPTOR *,CUarray) = (int(*)(CUDA_ARRAY_DESCRIPTOR *,CUarray)) gotcha_get_wrappee(GOTTCHA_cuArrayGetDescriptor);
	PTR_ORIGINAL_cuArrayGetDescriptor = (void *) gotcha_get_wrappee(GOTTCHA_cuArrayGetDescriptor);
	Bound_cuArrayGetDescriptor = std::bind(TMP_PTR_cuArrayGetDescriptor,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuArrayDestroy)(CUarray) = (int(*)(CUarray)) gotcha_get_wrappee(GOTTCHA_cuArrayDestroy);
	PTR_ORIGINAL_cuArrayDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuArrayDestroy);
	Bound_cuArrayDestroy = std::bind(TMP_PTR_cuArrayDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuArray3DCreate)(CUarray *,const CUDA_ARRAY3D_DESCRIPTOR *) = (int(*)(CUarray *,const CUDA_ARRAY3D_DESCRIPTOR *)) gotcha_get_wrappee(GOTTCHA_cuArray3DCreate);
	PTR_ORIGINAL_cuArray3DCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuArray3DCreate);
	Bound_cuArray3DCreate = std::bind(TMP_PTR_cuArray3DCreate,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuArray3DGetDescriptor)(CUDA_ARRAY3D_DESCRIPTOR *,CUarray) = (int(*)(CUDA_ARRAY3D_DESCRIPTOR *,CUarray)) gotcha_get_wrappee(GOTTCHA_cuArray3DGetDescriptor);
	PTR_ORIGINAL_cuArray3DGetDescriptor = (void *) gotcha_get_wrappee(GOTTCHA_cuArray3DGetDescriptor);
	Bound_cuArray3DGetDescriptor = std::bind(TMP_PTR_cuArray3DGetDescriptor,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMipmappedArrayCreate)(CUmipmappedArray *,const CUDA_ARRAY3D_DESCRIPTOR *,unsigned int) = (int(*)(CUmipmappedArray *,const CUDA_ARRAY3D_DESCRIPTOR *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuMipmappedArrayCreate);
	PTR_ORIGINAL_cuMipmappedArrayCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuMipmappedArrayCreate);
	Bound_cuMipmappedArrayCreate = std::bind(TMP_PTR_cuMipmappedArrayCreate,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMipmappedArrayGetLevel)(CUarray *,CUmipmappedArray,unsigned int) = (int(*)(CUarray *,CUmipmappedArray,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuMipmappedArrayGetLevel);
	PTR_ORIGINAL_cuMipmappedArrayGetLevel = (void *) gotcha_get_wrappee(GOTTCHA_cuMipmappedArrayGetLevel);
	Bound_cuMipmappedArrayGetLevel = std::bind(TMP_PTR_cuMipmappedArrayGetLevel,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMipmappedArrayDestroy)(CUmipmappedArray) = (int(*)(CUmipmappedArray)) gotcha_get_wrappee(GOTTCHA_cuMipmappedArrayDestroy);
	PTR_ORIGINAL_cuMipmappedArrayDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuMipmappedArrayDestroy);
	Bound_cuMipmappedArrayDestroy = std::bind(TMP_PTR_cuMipmappedArrayDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuPointerGetAttribute)(void *,CUpointer_attribute,CUdeviceptr) = (int(*)(void *,CUpointer_attribute,CUdeviceptr)) gotcha_get_wrappee(GOTTCHA_cuPointerGetAttribute);
	PTR_ORIGINAL_cuPointerGetAttribute = (void *) gotcha_get_wrappee(GOTTCHA_cuPointerGetAttribute);
	Bound_cuPointerGetAttribute = std::bind(TMP_PTR_cuPointerGetAttribute,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemPrefetchAsync)(CUdeviceptr,size_t,CUdevice,CUstream) = (int(*)(CUdeviceptr,size_t,CUdevice,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemPrefetchAsync);
	PTR_ORIGINAL_cuMemPrefetchAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuMemPrefetchAsync);
	Bound_cuMemPrefetchAsync = std::bind(TMP_PTR_cuMemPrefetchAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemAdvise)(CUdeviceptr,size_t,CUmem_advise,CUdevice) = (int(*)(CUdeviceptr,size_t,CUmem_advise,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuMemAdvise);
	PTR_ORIGINAL_cuMemAdvise = (void *) gotcha_get_wrappee(GOTTCHA_cuMemAdvise);
	Bound_cuMemAdvise = std::bind(TMP_PTR_cuMemAdvise,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemRangeGetAttribute)(void *,size_t,CUmem_range_attribute,CUdeviceptr,size_t) = (int(*)(void *,size_t,CUmem_range_attribute,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemRangeGetAttribute);
	PTR_ORIGINAL_cuMemRangeGetAttribute = (void *) gotcha_get_wrappee(GOTTCHA_cuMemRangeGetAttribute);
	Bound_cuMemRangeGetAttribute = std::bind(TMP_PTR_cuMemRangeGetAttribute,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemRangeGetAttributes)(void * *,size_t *,CUmem_range_attribute *,size_t,CUdeviceptr,size_t) = (int(*)(void * *,size_t *,CUmem_range_attribute *,size_t,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemRangeGetAttributes);
	PTR_ORIGINAL_cuMemRangeGetAttributes = (void *) gotcha_get_wrappee(GOTTCHA_cuMemRangeGetAttributes);
	Bound_cuMemRangeGetAttributes = std::bind(TMP_PTR_cuMemRangeGetAttributes,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
	int (*TMP_PTR_cuPointerSetAttribute)(const void *,CUpointer_attribute,CUdeviceptr) = (int(*)(const void *,CUpointer_attribute,CUdeviceptr)) gotcha_get_wrappee(GOTTCHA_cuPointerSetAttribute);
	PTR_ORIGINAL_cuPointerSetAttribute = (void *) gotcha_get_wrappee(GOTTCHA_cuPointerSetAttribute);
	Bound_cuPointerSetAttribute = std::bind(TMP_PTR_cuPointerSetAttribute,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuPointerGetAttributes)(unsigned int,CUpointer_attribute *,void * *,CUdeviceptr) = (int(*)(unsigned int,CUpointer_attribute *,void * *,CUdeviceptr)) gotcha_get_wrappee(GOTTCHA_cuPointerGetAttributes);
	PTR_ORIGINAL_cuPointerGetAttributes = (void *) gotcha_get_wrappee(GOTTCHA_cuPointerGetAttributes);
	Bound_cuPointerGetAttributes = std::bind(TMP_PTR_cuPointerGetAttributes,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuStreamCreate)(CUstream *,unsigned int) = (int(*)(CUstream *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuStreamCreate);
	PTR_ORIGINAL_cuStreamCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamCreate);
	Bound_cuStreamCreate = std::bind(TMP_PTR_cuStreamCreate,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuStreamCreateWithPriority)(CUstream *,unsigned int,int) = (int(*)(CUstream *,unsigned int,int)) gotcha_get_wrappee(GOTTCHA_cuStreamCreateWithPriority);
	PTR_ORIGINAL_cuStreamCreateWithPriority = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamCreateWithPriority);
	Bound_cuStreamCreateWithPriority = std::bind(TMP_PTR_cuStreamCreateWithPriority,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuStreamGetPriority)(CUstream,int *) = (int(*)(CUstream,int *)) gotcha_get_wrappee(GOTTCHA_cuStreamGetPriority);
	PTR_ORIGINAL_cuStreamGetPriority = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamGetPriority);
	Bound_cuStreamGetPriority = std::bind(TMP_PTR_cuStreamGetPriority,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuStreamGetFlags)(CUstream,unsigned int *) = (int(*)(CUstream,unsigned int *)) gotcha_get_wrappee(GOTTCHA_cuStreamGetFlags);
	PTR_ORIGINAL_cuStreamGetFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamGetFlags);
	Bound_cuStreamGetFlags = std::bind(TMP_PTR_cuStreamGetFlags,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuStreamWaitEvent)(CUstream,CUevent,unsigned int) = (int(*)(CUstream,CUevent,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuStreamWaitEvent);
	PTR_ORIGINAL_cuStreamWaitEvent = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamWaitEvent);
	Bound_cuStreamWaitEvent = std::bind(TMP_PTR_cuStreamWaitEvent,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuStreamAddCallback)(CUstream,CUstreamCallback,void *,unsigned int) = (int(*)(CUstream,CUstreamCallback,void *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuStreamAddCallback);
	PTR_ORIGINAL_cuStreamAddCallback = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamAddCallback);
	Bound_cuStreamAddCallback = std::bind(TMP_PTR_cuStreamAddCallback,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuStreamAttachMemAsync)(CUstream,CUdeviceptr,size_t,unsigned int) = (int(*)(CUstream,CUdeviceptr,size_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuStreamAttachMemAsync);
	PTR_ORIGINAL_cuStreamAttachMemAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamAttachMemAsync);
	Bound_cuStreamAttachMemAsync = std::bind(TMP_PTR_cuStreamAttachMemAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuStreamQuery)(CUstream) = (int(*)(CUstream)) gotcha_get_wrappee(GOTTCHA_cuStreamQuery);
	PTR_ORIGINAL_cuStreamQuery = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamQuery);
	Bound_cuStreamQuery = std::bind(TMP_PTR_cuStreamQuery,std::placeholders::_1);
	int (*TMP_PTR_cuStreamSynchronize)(CUstream) = (int(*)(CUstream)) gotcha_get_wrappee(GOTTCHA_cuStreamSynchronize);
	PTR_ORIGINAL_cuStreamSynchronize = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamSynchronize);
	Bound_cuStreamSynchronize = std::bind(TMP_PTR_cuStreamSynchronize,std::placeholders::_1);
	int (*TMP_PTR_cuStreamDestroy)(CUstream) = (int(*)(CUstream)) gotcha_get_wrappee(GOTTCHA_cuStreamDestroy);
	PTR_ORIGINAL_cuStreamDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamDestroy);
	Bound_cuStreamDestroy = std::bind(TMP_PTR_cuStreamDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuEventCreate)(CUevent *,unsigned int) = (int(*)(CUevent *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuEventCreate);
	PTR_ORIGINAL_cuEventCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuEventCreate);
	Bound_cuEventCreate = std::bind(TMP_PTR_cuEventCreate,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuEventRecord)(CUevent,CUstream) = (int(*)(CUevent,CUstream)) gotcha_get_wrappee(GOTTCHA_cuEventRecord);
	PTR_ORIGINAL_cuEventRecord = (void *) gotcha_get_wrappee(GOTTCHA_cuEventRecord);
	Bound_cuEventRecord = std::bind(TMP_PTR_cuEventRecord,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuEventQuery)(CUevent) = (int(*)(CUevent)) gotcha_get_wrappee(GOTTCHA_cuEventQuery);
	PTR_ORIGINAL_cuEventQuery = (void *) gotcha_get_wrappee(GOTTCHA_cuEventQuery);
	Bound_cuEventQuery = std::bind(TMP_PTR_cuEventQuery,std::placeholders::_1);
	int (*TMP_PTR_cuEventSynchronize)(CUevent) = (int(*)(CUevent)) gotcha_get_wrappee(GOTTCHA_cuEventSynchronize);
	PTR_ORIGINAL_cuEventSynchronize = (void *) gotcha_get_wrappee(GOTTCHA_cuEventSynchronize);
	Bound_cuEventSynchronize = std::bind(TMP_PTR_cuEventSynchronize,std::placeholders::_1);
	int (*TMP_PTR_cuEventDestroy)(CUevent) = (int(*)(CUevent)) gotcha_get_wrappee(GOTTCHA_cuEventDestroy);
	PTR_ORIGINAL_cuEventDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuEventDestroy);
	Bound_cuEventDestroy = std::bind(TMP_PTR_cuEventDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuEventElapsedTime)(float *,CUevent,CUevent) = (int(*)(float *,CUevent,CUevent)) gotcha_get_wrappee(GOTTCHA_cuEventElapsedTime);
	PTR_ORIGINAL_cuEventElapsedTime = (void *) gotcha_get_wrappee(GOTTCHA_cuEventElapsedTime);
	Bound_cuEventElapsedTime = std::bind(TMP_PTR_cuEventElapsedTime,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuStreamWaitValue32)(CUstream,CUdeviceptr,cuuint32_t,unsigned int) = (int(*)(CUstream,CUdeviceptr,cuuint32_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuStreamWaitValue32);
	PTR_ORIGINAL_cuStreamWaitValue32 = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamWaitValue32);
	Bound_cuStreamWaitValue32 = std::bind(TMP_PTR_cuStreamWaitValue32,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuStreamWriteValue32)(CUstream,CUdeviceptr,cuuint32_t,unsigned int) = (int(*)(CUstream,CUdeviceptr,cuuint32_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuStreamWriteValue32);
	PTR_ORIGINAL_cuStreamWriteValue32 = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamWriteValue32);
	Bound_cuStreamWriteValue32 = std::bind(TMP_PTR_cuStreamWriteValue32,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuStreamBatchMemOp)(CUstream,unsigned int,CUstreamBatchMemOpParams *,unsigned int) = (int(*)(CUstream,unsigned int,CUstreamBatchMemOpParams *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuStreamBatchMemOp);
	PTR_ORIGINAL_cuStreamBatchMemOp = (void *) gotcha_get_wrappee(GOTTCHA_cuStreamBatchMemOp);
	Bound_cuStreamBatchMemOp = std::bind(TMP_PTR_cuStreamBatchMemOp,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuFuncGetAttribute)(int *,CUfunction_attribute,CUfunction) = (int(*)(int *,CUfunction_attribute,CUfunction)) gotcha_get_wrappee(GOTTCHA_cuFuncGetAttribute);
	PTR_ORIGINAL_cuFuncGetAttribute = (void *) gotcha_get_wrappee(GOTTCHA_cuFuncGetAttribute);
	Bound_cuFuncGetAttribute = std::bind(TMP_PTR_cuFuncGetAttribute,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuFuncSetCacheConfig)(CUfunction,CUfunc_cache) = (int(*)(CUfunction,CUfunc_cache)) gotcha_get_wrappee(GOTTCHA_cuFuncSetCacheConfig);
	PTR_ORIGINAL_cuFuncSetCacheConfig = (void *) gotcha_get_wrappee(GOTTCHA_cuFuncSetCacheConfig);
	Bound_cuFuncSetCacheConfig = std::bind(TMP_PTR_cuFuncSetCacheConfig,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuFuncSetSharedMemConfig)(CUfunction,CUsharedconfig) = (int(*)(CUfunction,CUsharedconfig)) gotcha_get_wrappee(GOTTCHA_cuFuncSetSharedMemConfig);
	PTR_ORIGINAL_cuFuncSetSharedMemConfig = (void *) gotcha_get_wrappee(GOTTCHA_cuFuncSetSharedMemConfig);
	Bound_cuFuncSetSharedMemConfig = std::bind(TMP_PTR_cuFuncSetSharedMemConfig,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuLaunchKernel)(CUfunction,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,CUstream,void * *,void * *) = (int(*)(CUfunction,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,CUstream,void * *,void * *)) gotcha_get_wrappee(GOTTCHA_cuLaunchKernel);
	PTR_ORIGINAL_cuLaunchKernel = (void *) gotcha_get_wrappee(GOTTCHA_cuLaunchKernel);
	Bound_cuLaunchKernel = std::bind(TMP_PTR_cuLaunchKernel,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6,std::placeholders::_7,std::placeholders::_8,std::placeholders::_9,std::placeholders::_10,std::placeholders::_11);
	int (*TMP_PTR_cuFuncSetBlockShape)(CUfunction,int,int,int) = (int(*)(CUfunction,int,int,int)) gotcha_get_wrappee(GOTTCHA_cuFuncSetBlockShape);
	PTR_ORIGINAL_cuFuncSetBlockShape = (void *) gotcha_get_wrappee(GOTTCHA_cuFuncSetBlockShape);
	Bound_cuFuncSetBlockShape = std::bind(TMP_PTR_cuFuncSetBlockShape,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuFuncSetSharedSize)(CUfunction,unsigned int) = (int(*)(CUfunction,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuFuncSetSharedSize);
	PTR_ORIGINAL_cuFuncSetSharedSize = (void *) gotcha_get_wrappee(GOTTCHA_cuFuncSetSharedSize);
	Bound_cuFuncSetSharedSize = std::bind(TMP_PTR_cuFuncSetSharedSize,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuParamSetSize)(CUfunction,unsigned int) = (int(*)(CUfunction,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuParamSetSize);
	PTR_ORIGINAL_cuParamSetSize = (void *) gotcha_get_wrappee(GOTTCHA_cuParamSetSize);
	Bound_cuParamSetSize = std::bind(TMP_PTR_cuParamSetSize,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuParamSeti)(CUfunction,int,unsigned int) = (int(*)(CUfunction,int,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuParamSeti);
	PTR_ORIGINAL_cuParamSeti = (void *) gotcha_get_wrappee(GOTTCHA_cuParamSeti);
	Bound_cuParamSeti = std::bind(TMP_PTR_cuParamSeti,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuParamSetf)(CUfunction,int,float) = (int(*)(CUfunction,int,float)) gotcha_get_wrappee(GOTTCHA_cuParamSetf);
	PTR_ORIGINAL_cuParamSetf = (void *) gotcha_get_wrappee(GOTTCHA_cuParamSetf);
	Bound_cuParamSetf = std::bind(TMP_PTR_cuParamSetf,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuParamSetv)(CUfunction,int,void *,unsigned int) = (int(*)(CUfunction,int,void *,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuParamSetv);
	PTR_ORIGINAL_cuParamSetv = (void *) gotcha_get_wrappee(GOTTCHA_cuParamSetv);
	Bound_cuParamSetv = std::bind(TMP_PTR_cuParamSetv,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuLaunch)(CUfunction) = (int(*)(CUfunction)) gotcha_get_wrappee(GOTTCHA_cuLaunch);
	PTR_ORIGINAL_cuLaunch = (void *) gotcha_get_wrappee(GOTTCHA_cuLaunch);
	Bound_cuLaunch = std::bind(TMP_PTR_cuLaunch,std::placeholders::_1);
	int (*TMP_PTR_cuLaunchGrid)(CUfunction,int,int) = (int(*)(CUfunction,int,int)) gotcha_get_wrappee(GOTTCHA_cuLaunchGrid);
	PTR_ORIGINAL_cuLaunchGrid = (void *) gotcha_get_wrappee(GOTTCHA_cuLaunchGrid);
	Bound_cuLaunchGrid = std::bind(TMP_PTR_cuLaunchGrid,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuLaunchGridAsync)(CUfunction,int,int,CUstream) = (int(*)(CUfunction,int,int,CUstream)) gotcha_get_wrappee(GOTTCHA_cuLaunchGridAsync);
	PTR_ORIGINAL_cuLaunchGridAsync = (void *) gotcha_get_wrappee(GOTTCHA_cuLaunchGridAsync);
	Bound_cuLaunchGridAsync = std::bind(TMP_PTR_cuLaunchGridAsync,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuParamSetTexRef)(CUfunction,int,CUtexref) = (int(*)(CUfunction,int,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuParamSetTexRef);
	PTR_ORIGINAL_cuParamSetTexRef = (void *) gotcha_get_wrappee(GOTTCHA_cuParamSetTexRef);
	Bound_cuParamSetTexRef = std::bind(TMP_PTR_cuParamSetTexRef,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuOccupancyMaxActiveBlocksPerMultiprocessor)(int *,CUfunction,int,size_t) = (int(*)(int *,CUfunction,int,size_t)) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessor);
	PTR_ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessor = (void *) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessor);
	Bound_cuOccupancyMaxActiveBlocksPerMultiprocessor = std::bind(TMP_PTR_cuOccupancyMaxActiveBlocksPerMultiprocessor,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags)(int *,CUfunction,int,size_t,unsigned int) = (int(*)(int *,CUfunction,int,size_t,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags);
	PTR_ORIGINAL_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags);
	Bound_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags = std::bind(TMP_PTR_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuOccupancyMaxPotentialBlockSize)(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int) = (int(*)(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int)) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxPotentialBlockSize);
	PTR_ORIGINAL_cuOccupancyMaxPotentialBlockSize = (void *) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxPotentialBlockSize);
	Bound_cuOccupancyMaxPotentialBlockSize = std::bind(TMP_PTR_cuOccupancyMaxPotentialBlockSize,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
	int (*TMP_PTR_cuOccupancyMaxPotentialBlockSizeWithFlags)(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int,unsigned int) = (int(*)(int *,int *,CUfunction,CUoccupancyB2DSize,size_t,int,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxPotentialBlockSizeWithFlags);
	PTR_ORIGINAL_cuOccupancyMaxPotentialBlockSizeWithFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuOccupancyMaxPotentialBlockSizeWithFlags);
	Bound_cuOccupancyMaxPotentialBlockSizeWithFlags = std::bind(TMP_PTR_cuOccupancyMaxPotentialBlockSizeWithFlags,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6,std::placeholders::_7);
	int (*TMP_PTR_cuTexRefSetArray)(CUtexref,CUarray,unsigned int) = (int(*)(CUtexref,CUarray,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetArray);
	PTR_ORIGINAL_cuTexRefSetArray = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetArray);
	Bound_cuTexRefSetArray = std::bind(TMP_PTR_cuTexRefSetArray,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefSetMipmappedArray)(CUtexref,CUmipmappedArray,unsigned int) = (int(*)(CUtexref,CUmipmappedArray,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmappedArray);
	PTR_ORIGINAL_cuTexRefSetMipmappedArray = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmappedArray);
	Bound_cuTexRefSetMipmappedArray = std::bind(TMP_PTR_cuTexRefSetMipmappedArray,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefSetAddress)(size_t *,CUtexref,CUdeviceptr,size_t) = (int(*)(size_t *,CUtexref,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddress);
	PTR_ORIGINAL_cuTexRefSetAddress = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddress);
	Bound_cuTexRefSetAddress = std::bind(TMP_PTR_cuTexRefSetAddress,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuTexRefSetAddress2D)(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t) = (int(*)(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddress2D);
	PTR_ORIGINAL_cuTexRefSetAddress2D = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddress2D);
	Bound_cuTexRefSetAddress2D = std::bind(TMP_PTR_cuTexRefSetAddress2D,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuTexRefSetFormat)(CUtexref,CUarray_format,int) = (int(*)(CUtexref,CUarray_format,int)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetFormat);
	PTR_ORIGINAL_cuTexRefSetFormat = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetFormat);
	Bound_cuTexRefSetFormat = std::bind(TMP_PTR_cuTexRefSetFormat,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefSetAddressMode)(CUtexref,int,CUaddress_mode) = (int(*)(CUtexref,int,CUaddress_mode)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddressMode);
	PTR_ORIGINAL_cuTexRefSetAddressMode = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddressMode);
	Bound_cuTexRefSetAddressMode = std::bind(TMP_PTR_cuTexRefSetAddressMode,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefSetFilterMode)(CUtexref,CUfilter_mode) = (int(*)(CUtexref,CUfilter_mode)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetFilterMode);
	PTR_ORIGINAL_cuTexRefSetFilterMode = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetFilterMode);
	Bound_cuTexRefSetFilterMode = std::bind(TMP_PTR_cuTexRefSetFilterMode,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefSetMipmapFilterMode)(CUtexref,CUfilter_mode) = (int(*)(CUtexref,CUfilter_mode)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmapFilterMode);
	PTR_ORIGINAL_cuTexRefSetMipmapFilterMode = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmapFilterMode);
	Bound_cuTexRefSetMipmapFilterMode = std::bind(TMP_PTR_cuTexRefSetMipmapFilterMode,std::placeholders::_1,std::placeholders::_2);
	// int (*TMP_PTR_cuTexRefSetMipmapLevelBias)(CUtexref,float) = (int(*)(CUtexref,float)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmapLevelBias);
	// PTR_ORIGINAL_cuTexRefSetMipmapLevelBias = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmapLevelBias);
	// Bound_cuTexRefSetMipmapLevelBias = std::bind(TMP_PTR_cuTexRefSetMipmapLevelBias,std::placeholders::_1,std::placeholders::_2);
	// int (*TMP_PTR_cuTexRefSetMipmapLevelClamp)(CUtexref,float,float) = (int(*)(CUtexref,float,float)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmapLevelClamp);
	// PTR_ORIGINAL_cuTexRefSetMipmapLevelClamp = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMipmapLevelClamp);
	// Bound_cuTexRefSetMipmapLevelClamp = std::bind(TMP_PTR_cuTexRefSetMipmapLevelClamp,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefSetMaxAnisotropy)(CUtexref,unsigned int) = (int(*)(CUtexref,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMaxAnisotropy);
	PTR_ORIGINAL_cuTexRefSetMaxAnisotropy = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetMaxAnisotropy);
	Bound_cuTexRefSetMaxAnisotropy = std::bind(TMP_PTR_cuTexRefSetMaxAnisotropy,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefSetBorderColor)(CUtexref,float *) = (int(*)(CUtexref,float *)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetBorderColor);
	PTR_ORIGINAL_cuTexRefSetBorderColor = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetBorderColor);
	Bound_cuTexRefSetBorderColor = std::bind(TMP_PTR_cuTexRefSetBorderColor,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefSetFlags)(CUtexref,unsigned int) = (int(*)(CUtexref,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetFlags);
	PTR_ORIGINAL_cuTexRefSetFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetFlags);
	Bound_cuTexRefSetFlags = std::bind(TMP_PTR_cuTexRefSetFlags,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetAddress)(CUdeviceptr *,CUtexref) = (int(*)(CUdeviceptr *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetAddress);
	PTR_ORIGINAL_cuTexRefGetAddress = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetAddress);
	Bound_cuTexRefGetAddress = std::bind(TMP_PTR_cuTexRefGetAddress,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetArray)(CUarray *,CUtexref) = (int(*)(CUarray *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetArray);
	PTR_ORIGINAL_cuTexRefGetArray = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetArray);
	Bound_cuTexRefGetArray = std::bind(TMP_PTR_cuTexRefGetArray,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetMipmappedArray)(CUmipmappedArray *,CUtexref) = (int(*)(CUmipmappedArray *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmappedArray);
	PTR_ORIGINAL_cuTexRefGetMipmappedArray = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmappedArray);
	Bound_cuTexRefGetMipmappedArray = std::bind(TMP_PTR_cuTexRefGetMipmappedArray,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetAddressMode)(CUaddress_mode *,CUtexref,int) = (int(*)(CUaddress_mode *,CUtexref,int)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetAddressMode);
	PTR_ORIGINAL_cuTexRefGetAddressMode = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetAddressMode);
	Bound_cuTexRefGetAddressMode = std::bind(TMP_PTR_cuTexRefGetAddressMode,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefGetFilterMode)(CUfilter_mode *,CUtexref) = (int(*)(CUfilter_mode *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetFilterMode);
	PTR_ORIGINAL_cuTexRefGetFilterMode = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetFilterMode);
	Bound_cuTexRefGetFilterMode = std::bind(TMP_PTR_cuTexRefGetFilterMode,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetFormat)(CUarray_format *,int *,CUtexref) = (int(*)(CUarray_format *,int *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetFormat);
	PTR_ORIGINAL_cuTexRefGetFormat = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetFormat);
	Bound_cuTexRefGetFormat = std::bind(TMP_PTR_cuTexRefGetFormat,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefGetMipmapFilterMode)(CUfilter_mode *,CUtexref) = (int(*)(CUfilter_mode *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmapFilterMode);
	PTR_ORIGINAL_cuTexRefGetMipmapFilterMode = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmapFilterMode);
	Bound_cuTexRefGetMipmapFilterMode = std::bind(TMP_PTR_cuTexRefGetMipmapFilterMode,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetMipmapLevelBias)(float *,CUtexref) = (int(*)(float *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmapLevelBias);
	PTR_ORIGINAL_cuTexRefGetMipmapLevelBias = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmapLevelBias);
	Bound_cuTexRefGetMipmapLevelBias = std::bind(TMP_PTR_cuTexRefGetMipmapLevelBias,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetMipmapLevelClamp)(float *,float *,CUtexref) = (int(*)(float *,float *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmapLevelClamp);
	PTR_ORIGINAL_cuTexRefGetMipmapLevelClamp = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMipmapLevelClamp);
	Bound_cuTexRefGetMipmapLevelClamp = std::bind(TMP_PTR_cuTexRefGetMipmapLevelClamp,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuTexRefGetMaxAnisotropy)(int *,CUtexref) = (int(*)(int *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMaxAnisotropy);
	PTR_ORIGINAL_cuTexRefGetMaxAnisotropy = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetMaxAnisotropy);
	Bound_cuTexRefGetMaxAnisotropy = std::bind(TMP_PTR_cuTexRefGetMaxAnisotropy,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetBorderColor)(float *,CUtexref) = (int(*)(float *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetBorderColor);
	PTR_ORIGINAL_cuTexRefGetBorderColor = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetBorderColor);
	Bound_cuTexRefGetBorderColor = std::bind(TMP_PTR_cuTexRefGetBorderColor,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefGetFlags)(unsigned int *,CUtexref) = (int(*)(unsigned int *,CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefGetFlags);
	PTR_ORIGINAL_cuTexRefGetFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefGetFlags);
	Bound_cuTexRefGetFlags = std::bind(TMP_PTR_cuTexRefGetFlags,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefCreate)(CUtexref *) = (int(*)(CUtexref *)) gotcha_get_wrappee(GOTTCHA_cuTexRefCreate);
	PTR_ORIGINAL_cuTexRefCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefCreate);
	Bound_cuTexRefCreate = std::bind(TMP_PTR_cuTexRefCreate,std::placeholders::_1);
	int (*TMP_PTR_cuTexRefDestroy)(CUtexref) = (int(*)(CUtexref)) gotcha_get_wrappee(GOTTCHA_cuTexRefDestroy);
	PTR_ORIGINAL_cuTexRefDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefDestroy);
	Bound_cuTexRefDestroy = std::bind(TMP_PTR_cuTexRefDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuSurfRefSetArray)(CUsurfref,CUarray,unsigned int) = (int(*)(CUsurfref,CUarray,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuSurfRefSetArray);
	PTR_ORIGINAL_cuSurfRefSetArray = (void *) gotcha_get_wrappee(GOTTCHA_cuSurfRefSetArray);
	Bound_cuSurfRefSetArray = std::bind(TMP_PTR_cuSurfRefSetArray,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuSurfRefGetArray)(CUarray *,CUsurfref) = (int(*)(CUarray *,CUsurfref)) gotcha_get_wrappee(GOTTCHA_cuSurfRefGetArray);
	PTR_ORIGINAL_cuSurfRefGetArray = (void *) gotcha_get_wrappee(GOTTCHA_cuSurfRefGetArray);
	Bound_cuSurfRefGetArray = std::bind(TMP_PTR_cuSurfRefGetArray,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexObjectCreate)(CUtexObject *,const CUDA_RESOURCE_DESC *,const CUDA_TEXTURE_DESC *,const CUDA_RESOURCE_VIEW_DESC *) = (int(*)(CUtexObject *,const CUDA_RESOURCE_DESC *,const CUDA_TEXTURE_DESC *,const CUDA_RESOURCE_VIEW_DESC *)) gotcha_get_wrappee(GOTTCHA_cuTexObjectCreate);
	PTR_ORIGINAL_cuTexObjectCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuTexObjectCreate);
	Bound_cuTexObjectCreate = std::bind(TMP_PTR_cuTexObjectCreate,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuTexObjectDestroy)(CUtexObject) = (int(*)(CUtexObject)) gotcha_get_wrappee(GOTTCHA_cuTexObjectDestroy);
	PTR_ORIGINAL_cuTexObjectDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuTexObjectDestroy);
	Bound_cuTexObjectDestroy = std::bind(TMP_PTR_cuTexObjectDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuTexObjectGetResourceDesc)(CUDA_RESOURCE_DESC *,CUtexObject) = (int(*)(CUDA_RESOURCE_DESC *,CUtexObject)) gotcha_get_wrappee(GOTTCHA_cuTexObjectGetResourceDesc);
	PTR_ORIGINAL_cuTexObjectGetResourceDesc = (void *) gotcha_get_wrappee(GOTTCHA_cuTexObjectGetResourceDesc);
	Bound_cuTexObjectGetResourceDesc = std::bind(TMP_PTR_cuTexObjectGetResourceDesc,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexObjectGetTextureDesc)(CUDA_TEXTURE_DESC *,CUtexObject) = (int(*)(CUDA_TEXTURE_DESC *,CUtexObject)) gotcha_get_wrappee(GOTTCHA_cuTexObjectGetTextureDesc);
	PTR_ORIGINAL_cuTexObjectGetTextureDesc = (void *) gotcha_get_wrappee(GOTTCHA_cuTexObjectGetTextureDesc);
	Bound_cuTexObjectGetTextureDesc = std::bind(TMP_PTR_cuTexObjectGetTextureDesc,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexObjectGetResourceViewDesc)(CUDA_RESOURCE_VIEW_DESC *,CUtexObject) = (int(*)(CUDA_RESOURCE_VIEW_DESC *,CUtexObject)) gotcha_get_wrappee(GOTTCHA_cuTexObjectGetResourceViewDesc);
	PTR_ORIGINAL_cuTexObjectGetResourceViewDesc = (void *) gotcha_get_wrappee(GOTTCHA_cuTexObjectGetResourceViewDesc);
	Bound_cuTexObjectGetResourceViewDesc = std::bind(TMP_PTR_cuTexObjectGetResourceViewDesc,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuSurfObjectCreate)(CUsurfObject *,const CUDA_RESOURCE_DESC *) = (int(*)(CUsurfObject *,const CUDA_RESOURCE_DESC *)) gotcha_get_wrappee(GOTTCHA_cuSurfObjectCreate);
	PTR_ORIGINAL_cuSurfObjectCreate = (void *) gotcha_get_wrappee(GOTTCHA_cuSurfObjectCreate);
	Bound_cuSurfObjectCreate = std::bind(TMP_PTR_cuSurfObjectCreate,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuSurfObjectDestroy)(CUsurfObject) = (int(*)(CUsurfObject)) gotcha_get_wrappee(GOTTCHA_cuSurfObjectDestroy);
	PTR_ORIGINAL_cuSurfObjectDestroy = (void *) gotcha_get_wrappee(GOTTCHA_cuSurfObjectDestroy);
	Bound_cuSurfObjectDestroy = std::bind(TMP_PTR_cuSurfObjectDestroy,std::placeholders::_1);
	int (*TMP_PTR_cuSurfObjectGetResourceDesc)(CUDA_RESOURCE_DESC *,CUsurfObject) = (int(*)(CUDA_RESOURCE_DESC *,CUsurfObject)) gotcha_get_wrappee(GOTTCHA_cuSurfObjectGetResourceDesc);
	PTR_ORIGINAL_cuSurfObjectGetResourceDesc = (void *) gotcha_get_wrappee(GOTTCHA_cuSurfObjectGetResourceDesc);
	Bound_cuSurfObjectGetResourceDesc = std::bind(TMP_PTR_cuSurfObjectGetResourceDesc,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuDeviceCanAccessPeer)(int *,CUdevice,CUdevice) = (int(*)(int *,CUdevice,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceCanAccessPeer);
	PTR_ORIGINAL_cuDeviceCanAccessPeer = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceCanAccessPeer);
	Bound_cuDeviceCanAccessPeer = std::bind(TMP_PTR_cuDeviceCanAccessPeer,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuDeviceGetP2PAttribute)(int *,CUdevice_P2PAttribute,CUdevice,CUdevice) = (int(*)(int *,CUdevice_P2PAttribute,CUdevice,CUdevice)) gotcha_get_wrappee(GOTTCHA_cuDeviceGetP2PAttribute);
	PTR_ORIGINAL_cuDeviceGetP2PAttribute = (void *) gotcha_get_wrappee(GOTTCHA_cuDeviceGetP2PAttribute);
	Bound_cuDeviceGetP2PAttribute = std::bind(TMP_PTR_cuDeviceGetP2PAttribute,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuCtxEnablePeerAccess)(CUcontext,unsigned int) = (int(*)(CUcontext,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuCtxEnablePeerAccess);
	PTR_ORIGINAL_cuCtxEnablePeerAccess = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxEnablePeerAccess);
	Bound_cuCtxEnablePeerAccess = std::bind(TMP_PTR_cuCtxEnablePeerAccess,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuCtxDisablePeerAccess)(CUcontext) = (int(*)(CUcontext)) gotcha_get_wrappee(GOTTCHA_cuCtxDisablePeerAccess);
	PTR_ORIGINAL_cuCtxDisablePeerAccess = (void *) gotcha_get_wrappee(GOTTCHA_cuCtxDisablePeerAccess);
	Bound_cuCtxDisablePeerAccess = std::bind(TMP_PTR_cuCtxDisablePeerAccess,std::placeholders::_1);
	int (*TMP_PTR_cuGraphicsUnregisterResource)(CUgraphicsResource) = (int(*)(CUgraphicsResource)) gotcha_get_wrappee(GOTTCHA_cuGraphicsUnregisterResource);
	PTR_ORIGINAL_cuGraphicsUnregisterResource = (void *) gotcha_get_wrappee(GOTTCHA_cuGraphicsUnregisterResource);
	Bound_cuGraphicsUnregisterResource = std::bind(TMP_PTR_cuGraphicsUnregisterResource,std::placeholders::_1);
	int (*TMP_PTR_cuGraphicsSubResourceGetMappedArray)(CUarray *,CUgraphicsResource,unsigned int,unsigned int) = (int(*)(CUarray *,CUgraphicsResource,unsigned int,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuGraphicsSubResourceGetMappedArray);
	PTR_ORIGINAL_cuGraphicsSubResourceGetMappedArray = (void *) gotcha_get_wrappee(GOTTCHA_cuGraphicsSubResourceGetMappedArray);
	Bound_cuGraphicsSubResourceGetMappedArray = std::bind(TMP_PTR_cuGraphicsSubResourceGetMappedArray,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuGraphicsResourceGetMappedMipmappedArray)(CUmipmappedArray *,CUgraphicsResource) = (int(*)(CUmipmappedArray *,CUgraphicsResource)) gotcha_get_wrappee(GOTTCHA_cuGraphicsResourceGetMappedMipmappedArray);
	PTR_ORIGINAL_cuGraphicsResourceGetMappedMipmappedArray = (void *) gotcha_get_wrappee(GOTTCHA_cuGraphicsResourceGetMappedMipmappedArray);
	Bound_cuGraphicsResourceGetMappedMipmappedArray = std::bind(TMP_PTR_cuGraphicsResourceGetMappedMipmappedArray,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuGraphicsResourceGetMappedPointer)(CUdeviceptr *,size_t *,CUgraphicsResource) = (int(*)(CUdeviceptr *,size_t *,CUgraphicsResource)) gotcha_get_wrappee(GOTTCHA_cuGraphicsResourceGetMappedPointer);
	PTR_ORIGINAL_cuGraphicsResourceGetMappedPointer = (void *) gotcha_get_wrappee(GOTTCHA_cuGraphicsResourceGetMappedPointer);
	Bound_cuGraphicsResourceGetMappedPointer = std::bind(TMP_PTR_cuGraphicsResourceGetMappedPointer,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuGraphicsResourceSetMapFlags)(CUgraphicsResource,unsigned int) = (int(*)(CUgraphicsResource,unsigned int)) gotcha_get_wrappee(GOTTCHA_cuGraphicsResourceSetMapFlags);
	PTR_ORIGINAL_cuGraphicsResourceSetMapFlags = (void *) gotcha_get_wrappee(GOTTCHA_cuGraphicsResourceSetMapFlags);
	Bound_cuGraphicsResourceSetMapFlags = std::bind(TMP_PTR_cuGraphicsResourceSetMapFlags,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuGraphicsMapResources)(unsigned int,CUgraphicsResource *,CUstream) = (int(*)(unsigned int,CUgraphicsResource *,CUstream)) gotcha_get_wrappee(GOTTCHA_cuGraphicsMapResources);
	PTR_ORIGINAL_cuGraphicsMapResources = (void *) gotcha_get_wrappee(GOTTCHA_cuGraphicsMapResources);
	Bound_cuGraphicsMapResources = std::bind(TMP_PTR_cuGraphicsMapResources,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuGraphicsUnmapResources)(unsigned int,CUgraphicsResource *,CUstream) = (int(*)(unsigned int,CUgraphicsResource *,CUstream)) gotcha_get_wrappee(GOTTCHA_cuGraphicsUnmapResources);
	PTR_ORIGINAL_cuGraphicsUnmapResources = (void *) gotcha_get_wrappee(GOTTCHA_cuGraphicsUnmapResources);
	Bound_cuGraphicsUnmapResources = std::bind(TMP_PTR_cuGraphicsUnmapResources,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuGetExportTable)(const void * *,const CUuuid *) = (int(*)(const void * *,const CUuuid *)) gotcha_get_wrappee(GOTTCHA_cuGetExportTable);
	PTR_ORIGINAL_cuGetExportTable = (void *) gotcha_get_wrappee(GOTTCHA_cuGetExportTable);
	Bound_cuGetExportTable = std::bind(TMP_PTR_cuGetExportTable,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuTexRefSetAddress2D_v2)(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t) = (int(*)(CUtexref,const CUDA_ARRAY_DESCRIPTOR *,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddress2D_v2);
	PTR_ORIGINAL_cuTexRefSetAddress2D_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuTexRefSetAddress2D_v2);
	Bound_cuTexRefSetAddress2D_v2 = std::bind(TMP_PTR_cuTexRefSetAddress2D_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyHtoD_v2)(CUdeviceptr,const void *,size_t) = (int(*)(CUdeviceptr,const void *,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoD_v2);
	PTR_ORIGINAL_cuMemcpyHtoD_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoD_v2);
	Bound_cuMemcpyHtoD_v2 = std::bind(TMP_PTR_cuMemcpyHtoD_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemcpyDtoH_v2)(void *,CUdeviceptr,size_t) = (int(*)(void *,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoH_v2);
	PTR_ORIGINAL_cuMemcpyDtoH_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoH_v2);
	Bound_cuMemcpyDtoH_v2 = std::bind(TMP_PTR_cuMemcpyDtoH_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemcpyDtoD_v2)(CUdeviceptr,CUdeviceptr,size_t) = (int(*)(CUdeviceptr,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoD_v2);
	PTR_ORIGINAL_cuMemcpyDtoD_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoD_v2);
	Bound_cuMemcpyDtoD_v2 = std::bind(TMP_PTR_cuMemcpyDtoD_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemcpyDtoA_v2)(CUarray,size_t,CUdeviceptr,size_t) = (int(*)(CUarray,size_t,CUdeviceptr,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoA_v2);
	PTR_ORIGINAL_cuMemcpyDtoA_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoA_v2);
	Bound_cuMemcpyDtoA_v2 = std::bind(TMP_PTR_cuMemcpyDtoA_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyAtoD_v2)(CUdeviceptr,CUarray,size_t,size_t) = (int(*)(CUdeviceptr,CUarray,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoD_v2);
	PTR_ORIGINAL_cuMemcpyAtoD_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoD_v2);
	Bound_cuMemcpyAtoD_v2 = std::bind(TMP_PTR_cuMemcpyAtoD_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyHtoA_v2)(CUarray,size_t,const void *,size_t) = (int(*)(CUarray,size_t,const void *,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoA_v2);
	PTR_ORIGINAL_cuMemcpyHtoA_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoA_v2);
	Bound_cuMemcpyHtoA_v2 = std::bind(TMP_PTR_cuMemcpyHtoA_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyAtoH_v2)(void *,CUarray,size_t,size_t) = (int(*)(void *,CUarray,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoH_v2);
	PTR_ORIGINAL_cuMemcpyAtoH_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoH_v2);
	Bound_cuMemcpyAtoH_v2 = std::bind(TMP_PTR_cuMemcpyAtoH_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyAtoA_v2)(CUarray,size_t,CUarray,size_t,size_t) = (int(*)(CUarray,size_t,CUarray,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoA_v2);
	PTR_ORIGINAL_cuMemcpyAtoA_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoA_v2);
	Bound_cuMemcpyAtoA_v2 = std::bind(TMP_PTR_cuMemcpyAtoA_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemcpyHtoAAsync_v2)(CUarray,size_t,const void *,size_t,CUstream) = (int(*)(CUarray,size_t,const void *,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoAAsync_v2);
	PTR_ORIGINAL_cuMemcpyHtoAAsync_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoAAsync_v2);
	Bound_cuMemcpyHtoAAsync_v2 = std::bind(TMP_PTR_cuMemcpyHtoAAsync_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemcpyAtoHAsync_v2)(void *,CUarray,size_t,size_t,CUstream) = (int(*)(void *,CUarray,size_t,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoHAsync_v2);
	PTR_ORIGINAL_cuMemcpyAtoHAsync_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyAtoHAsync_v2);
	Bound_cuMemcpyAtoHAsync_v2 = std::bind(TMP_PTR_cuMemcpyAtoHAsync_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemcpy2D_v2)(const CUDA_MEMCPY2D *) = (int(*)(const CUDA_MEMCPY2D *)) gotcha_get_wrappee(GOTTCHA_cuMemcpy2D_v2);
	PTR_ORIGINAL_cuMemcpy2D_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy2D_v2);
	Bound_cuMemcpy2D_v2 = std::bind(TMP_PTR_cuMemcpy2D_v2,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpy2DUnaligned_v2)(const CUDA_MEMCPY2D *) = (int(*)(const CUDA_MEMCPY2D *)) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DUnaligned_v2);
	PTR_ORIGINAL_cuMemcpy2DUnaligned_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DUnaligned_v2);
	Bound_cuMemcpy2DUnaligned_v2 = std::bind(TMP_PTR_cuMemcpy2DUnaligned_v2,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpy3D_v2)(const CUDA_MEMCPY3D *) = (int(*)(const CUDA_MEMCPY3D *)) gotcha_get_wrappee(GOTTCHA_cuMemcpy3D_v2);
	PTR_ORIGINAL_cuMemcpy3D_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy3D_v2);
	Bound_cuMemcpy3D_v2 = std::bind(TMP_PTR_cuMemcpy3D_v2,std::placeholders::_1);
	int (*TMP_PTR_cuMemcpyHtoDAsync_v2)(CUdeviceptr,const void *,size_t,CUstream) = (int(*)(CUdeviceptr,const void *,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoDAsync_v2);
	PTR_ORIGINAL_cuMemcpyHtoDAsync_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyHtoDAsync_v2);
	Bound_cuMemcpyHtoDAsync_v2 = std::bind(TMP_PTR_cuMemcpyHtoDAsync_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyDtoHAsync_v2)(void *,CUdeviceptr,size_t,CUstream) = (int(*)(void *,CUdeviceptr,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoHAsync_v2);
	PTR_ORIGINAL_cuMemcpyDtoHAsync_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoHAsync_v2);
	Bound_cuMemcpyDtoHAsync_v2 = std::bind(TMP_PTR_cuMemcpyDtoHAsync_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpyDtoDAsync_v2)(CUdeviceptr,CUdeviceptr,size_t,CUstream) = (int(*)(CUdeviceptr,CUdeviceptr,size_t,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoDAsync_v2);
	PTR_ORIGINAL_cuMemcpyDtoDAsync_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpyDtoDAsync_v2);
	Bound_cuMemcpyDtoDAsync_v2 = std::bind(TMP_PTR_cuMemcpyDtoDAsync_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
	int (*TMP_PTR_cuMemcpy2DAsync_v2)(const CUDA_MEMCPY2D *,CUstream) = (int(*)(const CUDA_MEMCPY2D *,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DAsync_v2);
	PTR_ORIGINAL_cuMemcpy2DAsync_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy2DAsync_v2);
	Bound_cuMemcpy2DAsync_v2 = std::bind(TMP_PTR_cuMemcpy2DAsync_v2,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemcpy3DAsync_v2)(const CUDA_MEMCPY3D *,CUstream) = (int(*)(const CUDA_MEMCPY3D *,CUstream)) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DAsync_v2);
	PTR_ORIGINAL_cuMemcpy3DAsync_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemcpy3DAsync_v2);
	Bound_cuMemcpy3DAsync_v2 = std::bind(TMP_PTR_cuMemcpy3DAsync_v2,std::placeholders::_1,std::placeholders::_2);
	int (*TMP_PTR_cuMemsetD8_v2)(CUdeviceptr,unsigned char,size_t) = (int(*)(CUdeviceptr,unsigned char,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD8_v2);
	PTR_ORIGINAL_cuMemsetD8_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD8_v2);
	Bound_cuMemsetD8_v2 = std::bind(TMP_PTR_cuMemsetD8_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemsetD16_v2)(CUdeviceptr,unsigned short,size_t) = (int(*)(CUdeviceptr,unsigned short,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD16_v2);
	PTR_ORIGINAL_cuMemsetD16_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD16_v2);
	Bound_cuMemsetD16_v2 = std::bind(TMP_PTR_cuMemsetD16_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemsetD32_v2)(CUdeviceptr,unsigned int,size_t) = (int(*)(CUdeviceptr,unsigned int,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD32_v2);
	PTR_ORIGINAL_cuMemsetD32_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD32_v2);
	Bound_cuMemsetD32_v2 = std::bind(TMP_PTR_cuMemsetD32_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	int (*TMP_PTR_cuMemsetD2D8_v2)(CUdeviceptr,size_t,unsigned char,size_t,size_t) = (int(*)(CUdeviceptr,size_t,unsigned char,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D8_v2);
	PTR_ORIGINAL_cuMemsetD2D8_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D8_v2);
	Bound_cuMemsetD2D8_v2 = std::bind(TMP_PTR_cuMemsetD2D8_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemsetD2D16_v2)(CUdeviceptr,size_t,unsigned short,size_t,size_t) = (int(*)(CUdeviceptr,size_t,unsigned short,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D16_v2);
	PTR_ORIGINAL_cuMemsetD2D16_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D16_v2);
	Bound_cuMemsetD2D16_v2 = std::bind(TMP_PTR_cuMemsetD2D16_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
	int (*TMP_PTR_cuMemsetD2D32_v2)(CUdeviceptr,size_t,unsigned int,size_t,size_t) = (int(*)(CUdeviceptr,size_t,unsigned int,size_t,size_t)) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D32_v2);
	PTR_ORIGINAL_cuMemsetD2D32_v2 = (void *) gotcha_get_wrappee(GOTTCHA_cuMemsetD2D32_v2);
	Bound_cuMemsetD2D32_v2 = std::bind(TMP_PTR_cuMemsetD2D32_v2,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);

}
