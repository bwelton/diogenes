#include <cassert>
#include <iostream>
#include <fstream> 
#include <memory>   
#include <string>
#include <stdio.h> 
#include <map>
#include <sstream> 
#include <vector>
// CUDA include
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <atomic> 
#include <cstring>
#include "gotcha/gotcha.h"
#include <mutex> 
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <pthread.h>
std::atomic<bool> DIOGENES_Atomic_Malloc(false);

volatile bool DIOGENES_MEMMANGE_TEAR_DOWN = false;
volatile bool IN_INSTRIMENTATION = false;
volatile void * DIOGENES_CURRENT_STREAM = NULL;

enum DIOGLockTypes{IN_INIT = 0, IN_OP, IN_NONE, IN_FREECALL, IN_TEARDOWN};

class DiogAtomicMutex {
private:
	DIOGLockTypes _m;
	std::atomic<DIOGLockTypes> _freeLock;
public:

	DiogAtomicMutex() : _m(IN_NONE), _freeLock(IN_NONE) {};
	~DiogAtomicMutex() {
		DIOGENES_MEMMANGE_TEAR_DOWN = true;
	};
	bool EnterOp() {
		if (_m == IN_NONE) {
			_m = IN_OP;
		} else if (_m == IN_OP || _m == IN_INIT || _m == IN_TEARDOWN) {
			return false;
		}
		return true;
	};

	void ExitOp() {
		_m = IN_NONE;
	};

	bool EnterFree() {
		DIOGLockTypes lt = IN_FREECALL;
		if(_freeLock.compare_exchange_weak(lt, IN_NONE)) {
			return true;
		}
		return false;
	};

	void ExitFree() {
		_freeLock.exchange(IN_NONE);
	};

	void EnterInit() {
		_m = IN_INIT;
	};

	void ExitInit() {
		_m = IN_NONE;
	};
};


//#define MEMMANAGE_DEBUG 1
class MemStats {
public:
	MemStats(std::string name);
	~MemStats();
	void AllocatedMemory(uint64_t size);
	void FreedMemory(uint64_t size);
	void UsedCache(size_t size);
	void TransferAddrKnown(uint64_t addr, bool known);
private:
	std::map<uint64_t, uint64_t> _sizes;
	std::map<uint64_t, uint64_t> _transAddresses;
	uint64_t _current;
	uint64_t _max;
	uint64_t _allocatedCount;
	uint64_t _freedCount;
	uint64_t _knownTransferAddrs;
	uint64_t _unknownTransferAddrs;
	std::string _type;
	uint64_t _cacheHits;
};


class MemManage {
public:
	MemManage();
	~MemManage();
	cudaError_t GPUAllocate(void ** mem, uint64_t size);
	cudaError_t GPUFree(void * mem, bool & sync);
	void * CPUAllocate(uint64_t size);
	void CheckDestTransMem(void * mem);
	void CPUFree(void * mem);

	void * MallocPinMim(uint64_t size, void * origLoc);
	void ReturnPinMem();
private:
	std::map<uint64_t, uint64_t> _gpuMem;
	std::map<uint64_t, std::vector<void *> > _gpuMemSize;
	std::map<uint64_t, uint64_t> _cpuMem;
	std::shared_ptr<MemStats> _cpuStats;
	std::shared_ptr<MemStats> _gpuStats;

	std::map<uint64_t, std::vector<void *> > _cpuPinnedPages;
	std::map<uint64_t, std::pair<uint64_t, void *> > _cpuPinMem;
	//
};

MemStats::MemStats(std::string name) :  _current(0), _max(0), _allocatedCount(0), _freedCount(0), 
										_type(name), _cacheHits(0), _knownTransferAddrs(0),
										_unknownTransferAddrs(0) {}

MemStats::~MemStats() {
	std::stringstream ss;
	ss << "==============================================================" << std::endl;
	ss << "Allocator Type: " << _type << std::endl;
	ss << "Alloc Count: " << _allocatedCount << " Free Count: " << _freedCount << std::endl;
	ss << "Max Allocated: " << _max << std::endl;
	ss << "Known Transfer Addr: " << _knownTransferAddrs << " Unknown Trans Addr: " << _unknownTransferAddrs << std::endl;
	ss << "Cache Hits: " << _cacheHits << std::endl;
	ss << "Sizes Allocated... " << std::endl;

	for (auto i : _sizes) {
		ss << i.first << " " << i.second << std::endl;
	}

	ss << "Transfer Addresses.... " << std::endl;
	for (auto i : _transAddresses)
		ss << std::hex << i.first << " " << std::dec << i.second << std::endl;
	std::cerr << ss.str();
}

void MemStats::UsedCache(size_t size) {
#ifdef MEMMANAGE_DEBUG
	std::cout << "[MemStats::UsedCache] Cached memory range used for alloc of size - " << size << std::endl;
#endif
	_cacheHits++;
}


void MemStats::TransferAddrKnown(uint64_t addr, bool known) {
	if (known)
		_knownTransferAddrs++;
	else
		_unknownTransferAddrs++;
	if (_transAddresses.find(addr) == _transAddresses.end()){
		_transAddresses[addr] = 1;
	} else {
		_transAddresses[addr] += 1;
	}
}
void MemStats::AllocatedMemory(uint64_t size) {
#ifdef MEMMANAGE_DEBUG
	std::cout << "[MemStats::AllocatedMemory] Allocating Memory of size - " << size << std::endl;
#endif
	if (_sizes.find(size) == _sizes.end())
		_sizes[size] = 1;
	else
		_sizes[size] += 1;
	_current += size;
	if (_current > _max)
		_max = _current;
	_allocatedCount++;
}

void MemStats::FreedMemory(uint64_t size) {
#ifdef MEMMANAGE_DEBUG
	std::cout << "[MemStats::FreedMemory] Freeing Memory of size - " << size << std::endl;
#endif
	_freedCount += 1;
	if (_current > size) 
		_current -= size;
	else
		std::cerr << "[MemStats::FreedMemory] We have a free from a memory location we don't know" << std::endl;
}


MemManage::MemManage() :  _cpuStats(new MemStats(std::string("CPU"))), _gpuStats(new MemStats(std::string("GPU"))) {

}

bool FindFreeMemory(std::map<uint64_t, std::vector<void *> >  & memRegions, void ** mem, uint64_t size) {
	if(memRegions.find(size) != memRegions.end())
		if (memRegions[size].size() > 0){
			*mem = (memRegions[size].back());
			memRegions[size].pop_back();
			return true;
		}

	return false;
}

void MemManage::ReturnPinMem() {
	for (auto i : _cpuPinMem) {
		if(_cpuPinnedPages.find(i.second.first) == _cpuPinnedPages.end())
			_cpuPinnedPages[i.second.first] = std::vector<void *>();
		std::memcpy(i.second.second, (void*)i.first, i.second.first);
		_cpuPinnedPages[i.second.first].push_back((void*)i.first);
	}
	_cpuPinMem.clear();
}

void * MemManage::MallocPinMim(uint64_t size, void * orig) {
	void * tmp = NULL;

	if (FindFreeMemory(_cpuPinnedPages, &tmp, size)) {
		_cpuPinMem[(uint64_t)tmp] = std::make_pair(size, orig);
		return tmp;
	} else {
		if (cudaMallocHost(&tmp, size_t(size)) == cudaSuccess){
			_cpuPinMem[(uint64_t)tmp] = std::make_pair(size, orig);
			return tmp;
		}
	}

	std::cerr << "We cannot allocate via cudaMallocHost, using original address" << std::endl;
	return orig;
}

cudaError_t MemManage::GPUAllocate (void** mem, uint64_t size) {
	#ifdef MEMMANAGE_DEBUG
		std::cout << "[MemManage::GPUAllocate] Entering GPU allocator to get size of " << size << std::endl;
	#endif
	if (FindFreeMemory(_gpuMemSize, mem, size)) {
		#ifdef MEMMANAGE_DEBUG
			std::cout << "[MemManage::GPUAllocate] Using a cached address " << std::endl;
		#endif
		_gpuStats->UsedCache(size);
		return cudaSuccess;
	}
	#ifdef MEMMANAGE_DEBUG
		std::cout << "[MemManage::GPUAllocate] Allocating a new GPU memory address " << std::endl;
	#endif	
	// Allocate if we must...
	cudaError_t tmp = cudaMalloc(mem, size);
	if (tmp == cudaSuccess){
		_gpuStats->AllocatedMemory(size);
		if (_gpuMemSize.find(size) == _gpuMemSize.end())
			_gpuMemSize[size] = std::vector<void *>();
		_gpuMem[*((uint64_t*)mem)] = size;
	}
	return tmp;
}

cudaError_t MemManage::GPUFree(void * mem, bool & sync){
	if (_gpuMem.find((uint64_t)mem) != _gpuMem.end()) {
		_gpuMemSize[_gpuMem[(uint64_t)mem]].push_back(mem);
		sync = false;
		return cudaSuccess;
	} 
	sync = true;
	// Free the memory, we have no idea what this is.
	//std::cerr << "Freeing Memory we don't know where it was allocated" << std::endl;
	return cudaFree(mem);
}

void * MemManage::CPUAllocate(uint64_t size) {
	void * tmp = malloc(size);
	_cpuMem[(uint64_t) tmp] = size;
	_cpuStats->AllocatedMemory(size);
	return tmp;
}

void MemManage::CPUFree(void * mem) {
	if (_cpuMem.find((uint64_t) mem) != _cpuMem.end()) {
		_cpuStats->FreedMemory(_cpuMem[(uint64_t)mem]);
		_cpuMem.erase((uint64_t)mem);
	} else {
		std::cerr << "[MemManage::CPUFree] We have no idea what this memory address is... " << std::hex << mem << std::endl;
	}
	free(mem);
}

uint64_t Diogenes_FindMemKey(std::map<uint64_t,uint64_t> & m, uint64_t addr) {
	std::map<uint64_t, uint64_t>::iterator lb = m.lower_bound(addr);
	if (lb == m.end())
		return 0;
	if (lb->first + lb->second >= addr)
		return lb->first;
	return 0;
}

void MemManage::CheckDestTransMem(void * mem) {
	uint64_t key = Diogenes_FindMemKey(_cpuMem, (uint64_t) mem);
	if (key == 0)
		_cpuStats->TransferAddrKnown((uint64_t) mem, false);
	else 
		_cpuStats->TransferAddrKnown((uint64_t) mem, true);
		//std::cerr << "[MemManage::CheckDestTransMem] We do not know about this CPU address in memory transfer - " << std::hex << (uint64_t) mem << std::endl;
}


MemManage::~MemManage() {
	DIOGENES_Atomic_Malloc.exchange(true);
}
std::shared_ptr<MemManage> DIOGENES_MEMORY_MANAGER;
std::shared_ptr<DiogAtomicMutex> DIOGENES_MUTEX_MANAGER = NULL;
pthread_t DIOG_PROC_TID = 0;
// class DiogenesMemLockExchangeReset {
// 	bool _original;
// 	std::atomic<bool> * _lock;
// public:
// 	DiogenesMemLockExchangeReset(std::atomic<bool> * lock) : _lock(lock), _original(lock->exchange(true)) { };
// 	~DiogenesMemLockExchangeReset() {_lock->exchange(_original)};
// };

thread_local pthread_t L_PID = 0;
pthread_t gettid() {
	if (L_PID == 0)
		L_PID = pthread_self();
	return L_PID;
}

#define PLUG_BUILD_FACTORY(param) \
	if (DIOGENES_MUTEX_MANAGER == NULL && DIOGENES_MEMMANGE_TEAR_DOWN == false) {\
		DIOGENES_MEMMANGE_TEAR_DOWN = true; \
		DIOG_PROC_TID = gettid();\
		DIOGENES_MUTEX_MANAGER.reset(new DiogAtomicMutex()); \
		DIOGENES_MEMMANGE_TEAR_DOWN = false; \
	} \
	if (DIOGENES_TRANSFER_MEMMANGE.get() == NULL && DIOGENES_MEMMANGE_TEAR_DOWN == false) { \
		DIOGENES_MEMMANGE_TEAR_DOWN = true; \
		DIOGENES_MUTEX_MANAGER->EnterInit(); \
		DIOGENES_TRANSFER_MEMMANGE.reset(new TransferMemoryManager()); \
		DIOGENES_MEMORY_MANAGER.reset(new MemManage(param)); \
		DIOGENES_MUTEX_MANAGER->ExitInit(); \
		DIOGENES_MEMMANGE_TEAR_DOWN = false; \
	} 

#define PLUG_FACTORY_PTR DIOGENES_MEMORY_MANAGER.get()


#define NUM_GOTFUNCS 2

extern "C" void DIOGENES_FREEWrapper(void * mem);
extern "C" cudaError_t DIOGENES_CudaFreeHostWrapper(void * mem);
extern "C" cudaError_t DIOGENES_CudaMallocHostWrapper(void ** mem, size_t size);

typeof(&DIOGENES_FREEWrapper) DIOGENES_LIBCFREE;

typeof(&DIOGENES_CudaFreeHostWrapper) DIOGENES_CUDAFREEHOST;
typeof(&DIOGENES_CudaMallocHostWrapper) DIOGENES_CUDAMALLOCHOST;


gotcha_wrappee_handle_t DIOGENES_wrappee_free_handle;
gotcha_wrappee_handle_t DIOGENES_wrappee_cudaFreeHost_handle;
gotcha_wrappee_handle_t DIOGENES_wrappee_cudaMallocHost_handle;
struct gotcha_binding_t DIOGNESE_gotfuncs[] = {
	{"cudaFreeHost",(void*)DIOGENES_CudaFreeHostWrapper,&DIOGENES_wrappee_cudaFreeHost_handle},
	{"cudaMallocHost",(void*)DIOGENES_CudaMallocHostWrapper,&DIOGENES_wrappee_cudaMallocHost_handle}};


class NoFreeVector {
private:
	uint64_t * _storage;
	int _size;

public:
	NoFreeVector() : _storage((uint64_t*)malloc(10000*sizeof(uint64_t))), _size(0) { };

	~NoFreeVector() {};
	int size() {
		return _size;
	};
	void * back() {
		if (_size > 0)
			return (void*)(_storage[_size - 1]);
		return NULL;
	};

	void pop_back() {
		if (_size > 0)
			_size--;
	};

	void push_back(void * m){ 
		if (_size < 10000){
			_storage[_size] = (uint64_t)m;
			_size++;
		} else {
			assert("WE SHOULD NOT BE HERE!" == 0);
		}
	};
};

class MemAllocatorManager {
private:
	std::map<size_t, NoFreeVector *> _memRanges;
	std::map<uint64_t, size_t> _MemAddrToSize;
	std::map<uint64_t, size_t> _appInitCudaManaged;
	void * InternalAllocate(size_t size) {
		void * mem;
		if(cudaMallocHost(&mem,size) != cudaSuccess)
			assert("Could not malloc anything with cudaMallocHost!" != 0);
		_MemAddrToSize[(uint64_t)mem] = size;
		auto it = _memRanges.find((uint64_t)size);
		if(it == _memRanges.end())
			_memRanges[(uint64_t)size] = new NoFreeVector();
		return mem;
	};

public:
	MemAllocatorManager() {};
	void * AllocateMemory(size_t size) {
		auto it = _memRanges.find((uint64_t)size);
		if (it == _memRanges.end())
			return InternalAllocate(size);
		if (it->second->size() > 0) {
			void * ret = it->second->back();
			it->second->pop_back();
			return ret;
		}
		return InternalAllocate(size);
	};

	void AddManagedMemory(void * mem, size_t size) {
		_appInitCudaManaged[(uint64_t)mem] = size;
	}

	void FreeManagedMemory(void * mem) {
		auto it = _appInitCudaManaged.find((uint64_t)mem);
		if (it != _appInitCudaManaged.end())
			_appInitCudaManaged.erase(it);
	}


	bool IsManagedMemory(void * mem) {
		auto it = _appInitCudaManaged.find((uint64_t)mem);
		if (it != _appInitCudaManaged.end() && it->first <= (uint64_t)mem && it->first + it->second >= (uint64_t) mem)
			return true;
		return false;
	}

	bool IsOurAllocation(void * mem) {
		auto it = _MemAddrToSize.lower_bound((uint64_t)mem);
		if (it == _MemAddrToSize.end())
			return false;
		if (it->first <= (uint64_t)mem && it->first + it->second >= (uint64_t)mem)
			return true;
		return false;
	}
	bool ReleaseMemory(void * mem) {
		auto it = _MemAddrToSize.find((uint64_t)mem);
		if (it == _MemAddrToSize.end())
			return false;
		_memRanges[it->second]->push_back(mem);
		return true;
	};
};

inline cudaStream_t ConvertInternalCUStream(volatile void * inStream) {
	return (cudaStream_t)(((char*)inStream) - 136);
};

inline cudaStream_t ConvertUserToInternalCUStream(cudaStream_t inStream)  {
	if (inStream == NULL)
		return NULL;
	return *((cudaStream_t*)inStream);
};

struct  DelayedTransferCopy {
	uint64_t size;
	void * processAddress;
	void * diogenesTemp;
	cudaStream_t stream;
	DelayedTransferCopy(void * _processAddress, void * _diogenesTemp, uint64_t _size, cudaStream_t _stream) : processAddress(_processAddress), 
						diogenesTemp(_diogenesTemp), size(_size), stream(_stream) {};

	inline void CopyTempToPro() {
		memcpy(processAddress, diogenesTemp, size);
	};

	void * GetTempAddress() {
		return diogenesTemp;
	};
};

class TransferMemoryStreamTracker {
private:
	std::map<cudaStream_t, uint64_t> _streamsSeen;
public:
	TransferMemoryStreamTracker() {};

	void SeenSynchronization(cudaStream_t stream) {
		auto it = _streamsSeen.find(stream);
		if (it == _streamsSeen.end()) {
			_streamsSeen[stream] = 0;
			it = _streamsSeen.find(stream);
		}
		it->second++;
	};

	std::string Print(cudaStream_t ctxStream, cudaStream_t defaultStream){
		std::stringstream ss;
		ss << "[TransferMemoryStreamTracker::StreamsSeen] Streams seen during execution " << std::endl;
		ss << "[TransferMemoryStreamTracker::StreamsSeen] CTX stream = " << std::hex << (uint64_t)ctxStream << std::endl;
		ss << "[TransferMemoryStreamTracker::StreamsSeen] Default stream = " << std::hex << (uint64_t)defaultStream << std::endl;
		for (auto i : _streamsSeen) {
			ss << "[TransferMemoryStreamTracker::StreamsSeen] " << std::hex << (uint64_t)i.first << " Sync Calls Seen = " << std::dec << i.second << std::endl;
		}
		return ss.str();
	};
};


class TransferMemoryManager {
private:
	std::mutex mtx;
	std::shared_ptr<MemAllocatorManager> _MemAlloc;
	std::map<cudaStream_t, std::vector<std::shared_ptr<DelayedTransferCopy>>> _delayedCopies;
	TransferMemoryStreamTracker _streamsSeen;
	bool _initStreams;
	// hack for ctx synchronization
	cudaStream_t _ctxSynchronize;
 	cudaStream_t _defaultStream;

 	uint64_t _initTransferCallCount;
 	void FindDefaultStream(){
 		_defaultStream = NULL;
 		_ctxSynchronize = (cudaStream_t)(0x999);

		// void * host = malloc(1024);
		// void * dst;
		// int attempts = 0;
		// DIOGENES_CURRENT_STREAM = NULL;
		// assert(cudaMalloc(&dst, 1024) == cudaSuccess);
		// while (DIOGENES_CURRENT_STREAM == NULL && attempts < 10) {
		// 	cudaMemcpyAsync(dst, host, 1024, cudaMemcpyHostToDevice, 0);
		// 	DIOGENES_CURRENT_STREAM = NULL;
		// 	cudaStreamSynchronize(0);
		// 	attempts++;
		// }	
		// assert(DIOGENES_CURRENT_STREAM != NULL);
		// _defaultStream = ConvertInternalCUStream(DIOGENES_CURRENT_STREAM);
		// _initStreams = true;
		// DIOGENES_LIBCFREE(host);
		// cudaFree(dst);
 	};

	void FindCudaDeviceSynchronization() {
		// DIOGENES_CURRENT_STREAM = NULL;
		// cudaDeviceSynchronize();
		// if (DIOGENES_CURRENT_STREAM != NULL) {
		// 	_ctxSynchronize = ConvertInternalCUStream(DIOGENES_CURRENT_STREAM);
		// 	_initStreams = true;
		// 	DIOGENES_CURRENT_STREAM = NULL;
		// 	return;
		// }
		// // Perform a small transfer to get this address...
		// void * host = malloc(1024);
		// void * dst;
		// assert(cudaMalloc(&dst, 1024) == cudaSuccess);
		// assert(cudaMemcpyAsync(dst, host, 1024, cudaMemcpyHostToDevice, 0) == cudaSuccess);
		// DIOGENES_CURRENT_STREAM = NULL;
		// cudaDeviceSynchronize();
		// assert(DIOGENES_CURRENT_STREAM != NULL);
		// _ctxSynchronize = ConvertInternalCUStream(DIOGENES_CURRENT_STREAM);
		// _initStreams = true;
		// DIOGENES_LIBCFREE(host);
		// cudaFree(dst);
		// DIOGENES_CURRENT_STREAM = NULL;
	};
public:

	TransferMemoryManager() : _MemAlloc(new MemAllocatorManager()), _initStreams(false), _defaultStream(NULL),_ctxSynchronize(NULL), _initTransferCallCount(0) {
		void * handle = dlopen("libcudart.so", RTLD_LAZY);
		assert(handle != NULL);
		DIOGENES_CUDAMALLOCHOST = (typeof(&DIOGENES_CudaMallocHostWrapper)) dlsym(handle, "cudaMallocHost");
		DIOGENES_CUDAFREEHOST = (typeof(&DIOGENES_CudaFreeHostWrapper)) dlsym(handle, "cudaFreeHost");

		assert(DIOGENES_CUDAMALLOCHOST != NULL);
		assert(DIOGENES_CUDAFREEHOST != NULL);

		gotcha_wrap(DIOGNESE_gotfuncs, sizeof(DIOGNESE_gotfuncs)/sizeof(struct gotcha_binding_t), "diogenes");

		//DIOGENES_LIBCFREE = (typeof(&DIOGENES_FREEWrapper)) gotcha_get_wrappee(DIOGENES_wrappee_free_handle);
		//assert(DIOGENES_LIBCFREE != NULL);
	};
	~TransferMemoryManager() { 
		DIOGENES_MEMMANGE_TEAR_DOWN = true;
		std::cerr << "Init transfer call count = " << _initTransferCallCount << std::endl;
		std::cerr << _streamsSeen.Print(_ctxSynchronize, _defaultStream) << std::endl;
	};

	void MallocManaged(void * mem, size_t size) {
		_MemAlloc->AddManagedMemory(mem, size);
	}

	void FreeManaged(void * mem) {
		_MemAlloc->FreeManagedMemory(mem);
	}

	void * MallocMemory(size_t size) {
		//mtx.lock();
		void * ret = _MemAlloc->AllocateMemory(size);
		//mtx.unlock();
		return ret;
	};

	void ReleaseMemory(void * mem) {
		//mtx.lock();
		if (_MemAlloc->ReleaseMemory(mem) == false) 
			DIOGENES_LIBCFREE(mem);
		//mtx.unlock();
	};

	template <typename T, typename D> 
	void AddToMapVector(T key, D & value, std::map<T,std::vector<D>> & mmap) {
		auto it = mmap.find(key);
		if (it == mmap.end()) {
			mmap[key] = std::vector<D>();
			it = mmap.find(key);
		}
		it->second.push_back(value);
	};


	void * InitiateTransfer(void * dst, size_t size, cudaStream_t stream) {
		if (_initStreams == false){
			FindCudaDeviceSynchronization();
			FindDefaultStream();
			_initStreams = true;
		}
		if (stream == NULL || ((uint64_t)stream) == 1)
			stream = _defaultStream;
		else 
			stream = ConvertUserToInternalCUStream(stream);
		//_streamsSeen.SeenSynchronization(stream);
		//return dst;
		// if (stream == 0)
		// 	stream = _defaultStream;
		// else {
		// 	stream = ConvertUserToInternalCUStream(stream);
		// }
		_initTransferCallCount++;
		if (_MemAlloc->IsManagedMemory(dst) == false && _MemAlloc->IsOurAllocation(dst) == false) {
			std::shared_ptr<DelayedTransferCopy> tmp(new DelayedTransferCopy(dst, _MemAlloc->AllocateMemory(size), size, stream));
			AddToMapVector<cudaStream_t, std::shared_ptr<DelayedTransferCopy>>(stream, tmp, _delayedCopies);
			return tmp->GetTempAddress();
		}
	    return dst;
	};


	void PerformSynchronizationAction(cudaStream_t stream) {
		//volatile void * local = DIOGENES_CURRENT_STREAM;
		cudaStream_t myStream = stream;
		if (myStream == NULL || ((uint64_t) myStream) == 1) {
			myStream = _defaultStream;
		} else if (((uint64_t)myStream) == 0x999) {
			myStream = _ctxSynchronize;
		} else {
			myStream = ConvertUserToInternalCUStream(myStream);
		}
		//cudaStream_t myStream = ConvertInternalCUStream(local);
		_streamsSeen.SeenSynchronization(myStream);
		if (myStream == _ctxSynchronize) {
			// Synchronize everything
			for (auto & i : _delayedCopies) {
				for(auto n : i.second) {
					n->CopyTempToPro();
					ReleaseMemory(n->GetTempAddress());
				}
				i.second.clear();
			}
		} else {
			auto it = _delayedCopies.find(myStream);
			if (it != _delayedCopies.end()) {
				for (auto n : it->second) {
					n->CopyTempToPro();
					ReleaseMemory(n->GetTempAddress());
				}
				it->second.clear();
			}
		}
	
	};
};

std::shared_ptr<TransferMemoryManager> DIOGENES_TRANSFER_MEMMANGE;


extern "C" {

cudaError_t DIOGENES_CudaMallocHostWrapper(void ** mem, size_t size) {
	cudaError_t ret;
	if (DIOGENES_CUDAMALLOCHOST != NULL)
		ret = DIOGENES_CUDAMALLOCHOST(mem, size);
	else 
		assert("WE SHOULD NOT BE HERE! " == 0);

	if (DIOGENES_MEMMANGE_TEAR_DOWN == false && ret == cudaSuccess) {
		PLUG_BUILD_FACTORY()
		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
			DIOGENES_TRANSFER_MEMMANGE->MallocManaged(*mem, size);
		}
		DIOGENES_MUTEX_MANAGER->ExitOp();	
	}
	return ret;
}


cudaError_t DIOGENES_CudaFreeHostWrapper(void * mem) {
	cudaError_t ret;
	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
			DIOGENES_TRANSFER_MEMMANGE->FreeManaged(mem);
		}
		DIOGENES_MUTEX_MANAGER->ExitOp();
	}
	if (DIOGENES_CUDAFREEHOST != NULL)
		return DIOGENES_CUDAFREEHOST(mem);
	else 
		assert("WE SHOULD NOT BE HERE" == 0);
	return cudaSuccess;

}
cudaError_t  DIOGENES_cudaFreeWrapper(void * mem) {
	cudaError_t ret;
	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
			bool sync;
			ret = PLUG_FACTORY_PTR->GPUFree(mem, sync);
			DIOGENES_MUTEX_MANAGER->ExitOp();
		} else {
			assert( "WE SHOULD NOT BE HERE!" == 0);
		}
		return ret;
	} 
	return cudaSuccess;
	// //fprintf(stderr,"I am freeing an address of %p \n", mem);
	// return cudaFree(mem);
}

cudaError_t  DIOGENES_synchronousCudaFree(void * mem) {
	// bool original = DIOGENES_Atomic_Malloc.exchange(true); 
	// if (IN_INSTRIMENTATION == false) {
	// 	IN_INSTRIMENTATION = true;
	cudaError_t ret;
	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
			bool sync;
			ret = PLUG_FACTORY_PTR->GPUFree(mem, sync);
			if (sync == false)
				cudaDeviceSynchronize();
			DIOGENES_MUTEX_MANAGER->ExitOp();
		} else {
			assert("WE SHOULD NOT BE HERE!" == 0);
		}
		return ret;
	}
	return cudaSuccess;
	// //fprintf(stderr,"I am freeing an address of %p \n", mem);
	// return cudaFree(mem);
}


cudaError_t DIOGENES_cudaMallocWrapper(void ** mem, size_t size) {
	cudaError_t  ret;
	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
		    ret = PLUG_FACTORY_PTR->GPUAllocate(mem, uint64_t(size));
			DIOGENES_MUTEX_MANAGER->ExitOp();
		} else {
			assert("WE SHOULD NOT BE HERE!" == 0);
		}
	} else {
		assert("WE SHOULD NOT BE HERE!" == 0);
	}
	return ret;
	// cudaError_t tmp = cudaMalloc(mem, size);
	// if (tmp == cudaSuccess)
	// 	std::cerr << "I alloced an address at " << std::hex << *((uint64_t**)mem)  <<  " of size " << size << std::endl;
	// return tmp;
}



// void DIOGENES_SUB_333898PRECALL(void * param2) {
// 	PLUG_BUILD_FACTORY()
// 	DIOGENES_CURRENT_STREAM = param2;
// };


void DIOGENES_SIGNALSYNC(cudaStream_t stream) {
	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		DIOGENES_TRANSFER_MEMMANGE->PerformSynchronizationAction(stream);
	}
};

cudaError_t DIOGENES_cudaMemcpyAsyncWrapper(void * dst, const void * src, size_t size, cudaMemcpyKind kind, cudaStream_t stream) {
	bool original = DIOGENES_Atomic_Malloc.exchange(true); 
	if (kind == cudaMemcpyDeviceToHost && DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		dst = DIOGENES_TRANSFER_MEMMANGE->InitiateTransfer(dst, size, stream);
		//void * mem = PLUG_FACTORY_PTR->MallocPinMim(size, dst);
		//st = mem;
		//PLUG_FACTORY_PTR->CheckDestTransMem(dst);
	}
	//std::cerr << "Initiating a transfer between  " << std::hex << dst <<  " and " << std::hex << src << " of size " << size << std::endl;
	//DIOGENES_Atomic_Malloc.exchange(original);
	return cudaMemcpyAsync(dst, src, size, kind, stream);
}

void * DIOGENES_MALLOCWrapper(size_t size) {
	void * tmp = NULL;
	bool setVal = false;

	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		if (DIOG_PROC_TID != gettid()) {
			tmp = malloc(size);
		} else if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
			tmp = DIOGENES_TRANSFER_MEMMANGE->MallocMemory(size);
			DIOGENES_MUTEX_MANAGER->ExitOp();
		} else {
			tmp = malloc(size);
		}
	} else {
		tmp = malloc(size);
	}
	return tmp;
	// if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
	// 	if(DIOGENES_Atomic_Malloc.compare_exchange_weak(setVal, true)) {
	// 		PLUG_BUILD_FACTORY()	
	// 		tmp = DIOGENES_TRANSFER_MEMMANGE->MallocMemory(size);
	// 		//tmp = PLUG_FACTORY_PTR->CPUAllocate(uint64_t(size));
	// 		DIOGENES_Atomic_Malloc.exchange(false);
	// 	} else {
	// 		tmp = malloc(size);
	// 	}
	// } else {
	// 	tmp = malloc(size);
	// } 
	// //std::cerr << "Malloced data at  " << std::hex << tmp << " of size " << size << std::endl;
	// return tmp;
}

// void DIOGENES_EXITWrapper() {
// 	DIOGENES_Atomic_Malloc.exchange(true);
// }

void DIOGENES_SyncExit() {
	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		//DIOGENES_TRANSFER_MEMMANGE->PerformSynchronizationAction();
	}
}

void DIOGENES_FREEWrapper(void * mem) {
	bool setVal = false;
	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
		PLUG_BUILD_FACTORY()
		if (DIOG_PROC_TID != gettid()) {
			//std::cerr << "Different TID Detected! " << gettid() << std::endl;
			DIOGENES_LIBCFREE(mem);
			return;
		}
		DIOGENES_TRANSFER_MEMMANGE->ReleaseMemory(mem);
	} 
}
}

