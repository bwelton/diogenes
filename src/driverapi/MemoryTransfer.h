#pragma once
#define MEMORYTRANFER_HDR
#include "PluginCommon.h"
#include <algorithm>
#include <set>
#include <iostream>
#include "xxhash32.h"
#include "cuda.h"
//#define DEBUG 1
class MemoryTransfer;

#include "Parameters.h"

class MemoryTransfer {
private:
	int _supported;
	Parameters * _params;
	bool _transfer;
	bool _arrayTransfer;
	uint32_t _transferedData;
	uint32_t _origData;
	size_t _transferSize;
	bool _checkHash;
	CUmemorytype _srcType;
	CUmemorytype _dstType;
public:
	MemoryTransfer(Parameters * params);
	bool IsSupportedTransfer();
	bool IsTransfer();
	bool IsCPUDest();
	bool IsCPUSource();
	uint64_t GetCPUAddress();
	uint32_t GetOriginHash();
	uint32_t GetTransferHash();
	size_t GetSize();
	int GetStream();
	void SetHashGeneration(bool s);
	uint32_t GetHash(void * ptr, size_t size);
	uint32_t GetHashAtLocation( void * dstPtr, size_t tSize, CUmemorytype location);
	uint32_t GetSourceDataArray( void * dstPtr, size_t tSize, size_t offset);
	void PrecallHandleStandard();
	void PrecallHandleArray();
	void PostcallHandleArray();
	void PostcallHandleStandard();
	void PreTransfer();
	void PostTransfer();
	void Synchronize();
};

