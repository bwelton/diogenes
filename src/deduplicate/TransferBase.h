#pragma once
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <map>
#include <memory>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <iostream>
//#include "CudaWrapper.h"

extern "C" {
	int cuMemcpyD2H_detect_CALL(void * a, void * b, void * c);
	int cuMemcpyD2HAsync_detect_CALL(void * a, void * b, void * c, void * d);
	int cuMemcpyH2D_detect_CALL(void * a, void * b, void * c);
	int cuMemcpyH2DAsync_detect_CALL(void * a, void * b, void * c, void * d);
}

// The use of void * for all parameters is a cludge to get around 
// the wrappers in cudadevicewrapper.cpp generating functions with only void* parameters 
// This function wrapper should look like the following (and should be treated as such)
//     typedef boost::function<int(void * [gpu_ptr], void * [cpu_ptr], size_t, cudaMemcpyKind, cudaStream_t)> TransferCall;
typedef boost::function<int(void *, void *, void *, void *, void *)> TransferCall;

#define CheckGPUResult(ans) { gpuAssertion((ans), __FILE__, __LINE__); }
inline void gpuAssertion(cudaError_t ecode, const char *file, int line)
{
   if (ecode != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(ecode), file, line);
      exit(ecode);
   }
}

enum CALL_TYPE {
	SYNC = 0,
	ASYNC = 1
};

class TransferBase {
protected:
	uint64_t _globalId;
	void * _CPUPtr;
	void * _GPUPtr;
	size_t _size;
	cudaStream_t _stream;
	cudaMemcpyKind _kind;
	CALL_TYPE _sync;
	std::string _callName;
	TransferCall _call;
	int _ret; 
	bool _transferPerformed;
	bool _syncnd;
	std::map<cudaMemcpyKind, std::string> _kindMapping = {{cudaMemcpyHostToHost, std::string("HostToHost")}, 
														  {cudaMemcpyHostToDevice, std::string("HostToDevice")},
														  {cudaMemcpyDeviceToHost, std::string("DeviceToHost")},
														  {cudaMemcpyDeviceToDevice, std::string("DeviceToDevice")}};

    std::map<std::string, TransferCall> _transferCalls = {{std::string("HostToDevice"), boost::bind(cuMemcpyH2D_detect_CALL, _1, _2, _3)},
														  {std::string("HostToDeviceAsync"), boost::bind(cuMemcpyH2DAsync_detect_CALL, _1, _2, _3, _5)},
														  {std::string("DeviceToHost"), boost::bind(cuMemcpyD2H_detect_CALL, _2, _1, _3)},
														  {std::string("DeviceToHostAsync"), boost::bind(cuMemcpyD2HAsync_detect_CALL, _2, _1, _3, _5)}};
public:
	TransferBase(TransferCall call, void * CPUPtr, void * GPUPtr, size_t size, 
		CALL_TYPE sync, cudaMemcpyKind kind, cudaStream_t stream = 0);
	void PerformTransfer();
	std::string & GetStringTransferKind();
	size_t GetSize();
	uint32_t GetHash();
	int GetReturn();
	void * GetCPUPtr();
	uint64_t GetID();
	cudaStream_t GetStream();
	void SetID(uint64_t i);
	std::shared_ptr<void> GetDestinationData();
	virtual uint32_t Hash() = 0;
	virtual uint32_t HashPtr(std::shared_ptr<void> ptr, size_t size) = 0;
	virtual uint32_t HashPtr(void * ptr, size_t size) = 0;
	virtual ~TransferBase() = 0;
};

typedef std::shared_ptr<TransferBase> TransferPtr;