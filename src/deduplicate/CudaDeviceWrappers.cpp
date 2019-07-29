#include "CudaWrapper.h"
#include "WrapperMacros.h"
#undef BOOST_PP_ARRAY_ENUM
#undef BOOST_PREPROCESSOR_ARRAY_ENUM_HPP
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/array/enum.hpp>
std::shared_ptr<InstrumentFactory> InstFactory;

// thread_local std::shared_ptr<CudaTimers> CudaTime;
// extern boost::recursive_mutex _log_mtx;

// Device Driver API
extern "C" {
// DataStruct HostToDeviceDedup(void * dst, void * src, size_t bytecount, bool blocking = false, CUstream cudaStream = 0) {
// 	DataStruct transfer;
// 	transfer.storePTR = src;
// 	transfer.destPtr = dst;
// 	transfer.size = bytecount;
// 	transfer.stream = (cudaStream_t)cudaStream;
// 	transfer.blocking = blocking;
// 	transfer = STORAGE_PTR->DeduplicateData(transfer);
// 	return transfer;
// }

// void DeviceToHostDedup(void * dst, void * src, size_t bytecount, bool blocking = false, CUstream cudaStream = 0) {
// 	// UNSUPPORTED
// 	assert("UNSUPPORTED FEATURE: Device To Host Deduplication" == 0);
// }

void CheckInit() {
	BUILD_STORAGE_CLASS
}

void HostToDeviceDetect(float time, void * dst, void * src, size_t bytecount) {
	
// #ifdef ENABLE_STACKTRACING
// 	STORAGE_PTR->TrackTransfer(0, bytecount, (char *)src, time, HOSTTODEVICE, GenStackTrace());
// #else 
// 	STORAGE_PTR->TrackTransfer(0, bytecount, (char *)src, time, HOSTTODEVICE);
// #endif
}

void DeviceToHostDetect(float time, void * dst, void * src, size_t bytecount) {
// 	cudaDeviceSynchronize();
// 	// Destination is the host and that is the only place we can do the check
// #ifdef ENABLE_STACKTRACING
// 	STORAGE_PTR->TrackTransfer(0, bytecount, (char*)dst, time, DEVICETOHOST, GenStackTrace());
// #else
// 	STORAGE_PTR->TrackTransfer(0, bytecount, (char*)dst, time, DEVICETOHOST);
// #endif

}


UNSUPPORTED_CALL(cuInit_detect, 1, int)

	return ret_value;
}

// Tracking Calls
UNSUPPORTED_CALL(cuMemcpy_detect, 3, int)
	//STORAGE_PTR->LogOutput("cuMemcpy - Unified Addressing Not Supported for detect");
	return ret_value;
}

UNSUPPORTED_CALL(cuMemcpyAsync_detect, 4, int)
	//STORAGE_PTR->LogOutput("cuMemcpyAsync - Unified Addressing Not Supported for detect");
	return ret_value;
}

UNSUPPORTED_CALL(cuMemcpyA2A_detect, 5, int)
	//STORAGE_PTR->LogOutput("cuMemcpyArrayToArray - Host to Host Copy Skipped");
	return ret_value;
}


UNSUPPORTED_CALL(cuMemcpyA2D_detect, 4, int) 
	
	//TIMER_MACRO(cuMemcpyA2D_detect, 4, (cudaStream_t) 0, int);

	// c, the third parameter of this function, is the offset into src (b) to copy from.
	//HostToDeviceDetect(TIMER_PTR->GetTime(), (void *)a, (void *)(((char *)b)+((size_t)c)) , (size_t)d);
	return ret_value;
}

UNSUPPORTED_CALL(cuMemcpyA2H_detect, 4, int) 
	// TIMER_MACRO(cuMemcpyA2H_detect, 4, (cudaStream_t) 0, int);

	// DeviceToHostDetect(TIMER_PTR->GetTime(), (void *)a,(void *)(((char *)b)+((size_t)c)), (size_t)d);
	return ret_value;
}

UNSUPPORTED_CALL(cuMemcpyA2HAsync_detect, 5, int)
	// TIMER_MACRO(cuMemcpyA2HAsync_detect, 5, (cudaStream_t) e, int);

	// DeviceToHostDetect(TIMER_PTR->GetTime(), (void *)a,(void *)(((char *)b)+((size_t)c)), (size_t)d);
	return ret_value;	
}

UNSUPPORTED_CALL(cuMemcpyD2A_detect, 4, int)
	// TIMER_MACRO(cuMemcpyD2A_detect, 4, (cudaStream_t) 0, int);
	// DeviceToHostDetect(TIMER_PTR->GetTime(), (void *)(((char *)a)+((size_t)b)), (void*)a, (size_t)d);
	return ret_value;
}

UNSUPPORTED_CALL(cuMemcpyD2D_detect, 3, int)
	//int ret_value = CALL_UNDERLYING(cuMemcpyD2D_detect, VARARRAY(3));
	return ret_value;
}

UNSUPPORTED_CALL(cuMemcpyD2DAsync_detect, 4, int)
	//int ret_value = CALL_UNDERLYING(cuMemcpyD2DAsync_detect, VARARRAY(4));
	return ret_value;
}

// typedef boost::function<int(void * [gpu], void *[cpu], size_t, cudaMemcpyKind, cudaStream_t)> TransferCall;
// _ret = _call(_GPUPtr, _CPUPtr, _size, _kind, _stream);
// (TransferCall call, void * CPUPtr, void * GPUPtr, size_t size, 
// 		CALL_TYPE sync, cudaMemcpyKind kind, cudaStream_t stream = 0
WRAP_SUPPORTED_CALL_TIME(cuMemcpyD2H_detect, 3, int)

	TransferCall t = boost::bind(cuMemcpyD2H_detect_CALL, _2, _1, _3);
//	fprintf(stderr, "%s\n", "We are here");
	std::shared_ptr<TransferBase> transfer(new HashData(t, (void * )a, (void *) b, (size_t) c, SYNC, cudaMemcpyDeviceToHost));
	int ret = STORAGE_PTR->PerformAction(transfer);
	return transfer.get()->GetReturn();
}


WRAP_SUPPORTED_CALL_TIME(cuMemcpyD2HAsync_detect, 4, int) 
	//uint32_t beforeTransfer = XXHash32::hash((void *) a, (size_t)c, 0);
	TransferCall t = boost::bind(cuMemcpyD2HAsync_detect_CALL, _2, _1, _3, _5);
//	fprintf(stderr, "%s\n", "We are here");

	std::shared_ptr<TransferBase> transfer(new HashData(t, (void * )a, (void *) b, (size_t) c, ASYNC, cudaMemcpyDeviceToHost, (cudaStream_t) d));
	int ret = STORAGE_PTR->PerformAction(transfer);
	return transfer.get()->GetReturn();
}

UNSUPPORTED_CALL(cuMemcpyH2A_detect, 4, int) 
	//int ret_value = CALL_UNDERLYING(cuMemcpyH2A_detect, VARARRAY(4));
	return ret_value;
}

UNSUPPORTED_CALL(cuMemcpyH2AAsync_detect, 5, int) 
	//int ret_value = CALL_UNDERLYING(cuMemcpyH2AAsync_detect, VARARRAY(5));
	return ret_value;
}

WRAP_SUPPORTED_CALL_TIME(cuMemcpyH2D_detect, 3, int) 
	TransferCall t = boost::bind(cuMemcpyH2D_detect_CALL, _1, _2, _3);
//	fprintf(stderr, "%s\n", "We are here");
	std::shared_ptr<TransferBase> transfer(new HashData(t, (void * )b, (void *) a, (size_t) c, SYNC, cudaMemcpyHostToDevice));
	int ret = STORAGE_PTR->PerformAction(transfer);
	return transfer.get()->GetReturn();
}

WRAP_SUPPORTED_CALL_TIME(cuMemcpyH2DAsync_detect, 4, int)
	TransferCall t = boost::bind(cuMemcpyH2DAsync_detect_CALL, _1, _2, _3, _5);
//	fprintf(stderr, "%s\n", "We are here");
	std::shared_ptr<TransferBase> transfer(new HashData(t, (void * )b, (void *) a, (size_t) c, ASYNC, cudaMemcpyHostToDevice, (cudaStream_t) d));
	int ret = STORAGE_PTR->PerformAction(transfer);
	return transfer.get()->GetReturn();
}

// Deduplication Wrapper Functions.
// Currently only support Host -> Device Deduplication

// WRAP_SUPPORTED_CALL_TIME(cuMemcpyHtoDAsync_dedup, 4, int) 
// 	int error = (int)CUDA_SUCCESS;
// 	DataStruct ret = HostToDeviceDedup(a, b, (size_t)c, false, (CUstream) d);
// 	if (ret.duplicate == false) {
// 		STORAGE_PTR->LogOutput("No existing duplicate found, perform transfer H2DAsync");
// 		int v = CALL_UNDERLYING_ARGS(cuMemcpyHtoDAsync_dedup, 4);
// 		if (v != error){
// 			STORAGE_PTR->LogOutput("Could not perform underlying transfer - cuMemcpyH2DAsync");
// 			return v;
// 		}
// 		ret.storePTR = (void *) a;
// 		STORAGE_PTR->AddNew(ret);
// 	} else {
// 		STORAGE_PTR->LogOutput("Duplicate found- H2D Async");
// 	}
// 	return error;
// }

// WRAP_SUPPORTED_CALL_TIME(cuMemcpyHtoD_dedup, 3, int) 
// 	int error = (int)CUDA_SUCCESS;
// 	DataStruct ret = HostToDeviceDedup(a, b, (size_t)c, true, 0);	
// 	if (ret.duplicate == false) {
// 		#ifdef DEBUG_OUTPUT
// 		STORAGE_PTR->LogOutput("No existing duplicate found, perform transfer H2D");
// 		#endif
// 		int v = CALL_UNDERLYING_ARGS(cuMemcpyHtoD_dedup, 3);
// 		if (v != error){
// 			STORAGE_PTR->LogOutput("Could not perform underlying transfer - cuMemcpyH2D");
// 			return v;
// 		}
// 		ret.storePTR = (void *) a;
// 		STORAGE_PTR->AddNew(ret);
// 	} else {
// 		// #ifdef DEBUG_OUTPUT
// 		// STORAGE_PTR->LogOutput("Duplicate found- H2D");
// 		// #endif
// 		// error = cudaDeviceSynchronize();		
// 	}
// 	return error;
// }


// WRAP_SUPPORTED_CALL_TIME(cuMemcpyAtoD_dedup, 4, int) 
// 	int error = (int)CUDA_SUCCESS;
// 	DataStruct ret = HostToDeviceDedup(a, (void *)(((char*)b)+(size_t)c), (size_t)d, true, 0);	
// 	if (ret.duplicate == false) {
// 		#ifdef DEBUG_OUTPUT
// 		STORAGE_PTR->LogOutput("No existing duplicate found, perform transfer A2D");
// 		#endif
// 		int v = CALL_UNDERLYING_ARGS(cuMemcpyAtoD_dedup, 4);
// 		if (v != error){
// 			STORAGE_PTR->LogOutput("Could not perform underlying transfer - cuMemcpyA2D");
// 			return v;
// 		}
// 		ret.storePTR = (void *) a;
// 		STORAGE_PTR->AddNew(ret);
// 	} else {
// 		#ifdef DEBUG_OUTPUT
// 		STORAGE_PTR->LogOutput("Duplicate found- A2D");
// 		#endif
// 		error = cudaDeviceSynchronize();		
// 	}
// 	return error;
// }

// Experimental wrappers for hoomd
// WRAP_SUPPORTED_CALL_TIME(cuMemcpyDTH_dedup, 3, int) 
// 	int error = (int)CUDA_SUCCESS;
// 	if (STORAGE_PTR->tcount1 < 15) {
// 		STORAGE_PTR->tcount1++;
// 		int v = CALL_UNDERLYING_ARGS(cuMemcpyDTH_dedup, 3);
// 		return v;		
// 	}
// 	uint32_t beforeTransfer = XXHash32::hash((void *) a, (size_t)c, 0);
// 	if (beforeTransfer == 148298089) {
// 		if (STORAGE_PTR->transferedOnce == true) {
// 			//fprintf(stderr, "%s: %u\n", "Caught Duplicate:", beforeTransfer);
// 			memcpy((void *) a, STORAGE_PTR->cache, (size_t) c);
// 			return error;
// 		} else {
// 			int v = CALL_UNDERLYING_ARGS(cuMemcpyDTH_dedup, 3);
// 			STORAGE_PTR->cache = (char*) malloc((size_t)c);
// 			memcpy(STORAGE_PTR->cache,(void *) a, (size_t) c);
// 			STORAGE_PTR->transferedOnce = true;
// 			return v;
// 		}
// 	} else if (beforeTransfer == 4022131285) {
// 		//fprintf(stderr, "%s: %u\n", "Caught Duplicate:", beforeTransfer);
// 		return error;
// 	} else {
// 		int v = CALL_UNDERLYING_ARGS(cuMemcpyDTH_dedup, 3);
// 		return v;
// 	}
// 	return error;
// }

// WRAP_SUPPORTED_CALL_TIME(cuMemcpyDTHAsync_dedup, 4, int) 
// 	int error = (int)CUDA_SUCCESS;
// 	if (STORAGE_PTR->tcount1 < 15) {
// 		STORAGE_PTR->tcount1++;
// 		int v = CALL_UNDERLYING_ARGS(cuMemcpyDTHAsync_dedup, 4);
// 		return v;		
// 	}
// 	uint32_t beforeTransfer = XXHash32::hash((void *) a, (size_t)c, 0);
// 	if (beforeTransfer == 148298089) {
// 		if (STORAGE_PTR->transferedOnce == true) {
// 			//fprintf(stderr, "%s: %u\n", "Caught Duplicate:", beforeTransfer);
// 			memcpy((void *) a, STORAGE_PTR->cache, (size_t) c);
// 			return error;
// 		} else {
// 			int v = CALL_UNDERLYING_ARGS(cuMemcpyDTHAsync_dedup, 4);
// 			STORAGE_PTR->cache = (char*) malloc((size_t)c);
// 			memcpy(STORAGE_PTR->cache,(void *) a, (size_t) c);
// 			STORAGE_PTR->transferedOnce = true;
// 			return v;
// 		}
// 	} else if (beforeTransfer == 4022131285) {
// 		//fprintf(stderr, "%s: %u\n", "Caught Duplicate:", beforeTransfer);
// 		return error;
// 	} else {
// 		int v = CALL_UNDERLYING_ARGS(cuMemcpyDTH_dedup, 3);
// 		return v;
// 	}
// 	return error;
// }

}
