#pragma once
#include <boost/preprocessor/array/enum.hpp>
#include "InstrumentFactory.h"
#include "HashData.h"

extern "C" {
	void CheckInit();
	// int cuMemcpyD2H_detect_CALL(void * a, void * b, void * c);
	// int cuMemcpyD2HAsync_detect_CALL(void * a, void * b, void * c, void * d);
	// int cuMemcpyH2D_detect_CALL(void * a, void * b, void * c);
	// int cuMemcpyH2DAsync_detect_CALL(void * a, void * b, void * c, void * d);
	// CREATE_HEADER(cuMemcpyD2H_detect_CALL, 3, int);
	// CREATE_HEADER(cuMemcpyD2HAsync_detect_CALL, 4, int);
	// CREATE_HEADER(cuMemcpyH2D_detect_CALL, 3, int);
	// CREATE_HEADER(cuMemcpyH2DAsync_detect_CALL, 4, int);
}
// #ifdef ENABLE_STACKTRACING
// #include "strace.h"
// #endif 


// struct CudaTimers {
// 	cudaEvent_t start, stop;
// 	CudaTimers() {
// 		cudaEventCreate(&start);
// 		cudaEventCreate(&stop);
// 	}
// 	void StartTimer(cudaStream_t s) {
// 		cudaEventRecord(start, s);
// 	}
// 	void EndTimer(cudaStream_t s) {
// 		cudaEventRecord(stop, s);
// 	}
// 	float GetTime() {
// 		float ret = 0.0;
// 		cudaEventSynchronize(stop);
// 		cudaEventElapsedTime(&ret, start, stop);
// 		return ret;
// 	}
// }; 

// extern thread_local std::shared_ptr<CudaTimers> CudaTime;
// #define BUILD_TIMER_CLASS \
// 	if (CudaTime.get() == NULL) { \
// 		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
// 		CudaTime.reset(new CudaTimers()); \
// 	} 

// #define TIMER_PTR CudaTime.get()

// #define TIMING_WRAPPER 