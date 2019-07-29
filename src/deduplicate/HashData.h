#pragma once
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "TransferBase.h"
#include "xxhash32.h"

class HashData : public TransferBase {
protected:
	uint32_t _hash;
public: 
	uint32_t Hash();
	HashData(TransferCall call, void * CPUPtr, void * GPUPtr, size_t size, 
		CALL_TYPE sync, cudaMemcpyKind kind, cudaStream_t stream = 0);
	uint32_t HashPtr(std::shared_ptr<void> ptr, size_t size);
	uint32_t HashPtr(void * ptr, size_t size);
	~HashData();
};

