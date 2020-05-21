#include <iostream>
#include <fstream> 
#include <memory>   
#include <string>
#include <stdio.h> 
#include <map>
#include <sstream> 
#include <vector>
#include <atomic> 
#include <cstring>
#include <list>
#include <deque>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <unistd.h>
#include <dlfcn.h>
#include "gotcha/gotcha.h"
#include "MemRecDataFile.h"
#include "StackPoint.h"
#include "StackwalkingCommon.h"

extern "C" {
	volatile int64_t DIOGENES_CTX_ID = -1;
	volatile int64_t DIOGENES_UNKNOWN_CTX_ID = 100000000;
}

struct MemAddress {
	uint64_t addr;
	int64_t loc;
	uint64_t size;
};

class MemKeeper {
public:
	MemKeeper() {
		allocCount = 10000;
		for (int i = 0; i < 10000; i++)
			_allocedAddresses.push_back(new MemAddress());
	};

	~MemKeeper() {
		for(auto i : _allocedAddresses)
			delete i;
		for(auto i : _addressMap)
			delete i.second;
		_addressMap.clear();
		_allocedAddresses.clear();
	}

	MemAddress * ReturnEmpty() {
		MemAddress * ret = NULL;
		if (_allocedAddresses.size() > 0) {
			ret = _allocedAddresses.front();
			_allocedAddresses.pop_front();
		} else {
			allocCount++;
			if (allocCount % 10000 == 0)
				std::cerr << "[MemAddress::ReturnEmpty] Alloced - " << allocCount <<  " MemAddress structs" << std::endl;
			ret = new MemAddress();
		}
		return ret;
	}

	MemAddress * Get(uint64_t addr) {
		if (_addressMap.find(addr) == _addressMap.end())
			return NULL;
		return _addressMap[addr];
	};

	MemAddress * GetLowerBound(uint64_t addr)  {
		auto lb = _addressMap.lower_bound(addr);
		if (lb == _addressMap.end())
			return NULL;
		if (lb->second->addr + lb->second->size >= addr)
			return lb->second;
		return NULL;
	}
	void AddOneSeen(int64_t id, std::map<int64_t, int64_t> & seenMap) {
		auto it = seenMap.find(id);
		if (it == seenMap.end()) {
			seenMap[id] = 1;
		} else {
			(it->second)++;
		}
	};


	void SubNumSeen(int64_t id, std::map<int64_t, int64_t> & seenMap, int64_t count) {
		auto it = seenMap.find(id);
		if (it != seenMap.end()) {
			it->second -= count;
			if (it->second <= 0)
				seenMap.erase(it);
		}
	};
	MemAddress * Set(uint64_t addr, uint64_t size, int64_t loc) {
		MemAddress * n;
		if (_addressMap.find(addr) != _addressMap.end()) {
		   //std::cerr << "We have already seen this address! Potential Memory Leak/Weirdness! " << std::dec << loc << std::endl;
			n = _addressMap[addr];
		} else {
			n = ReturnEmpty();
		}
		n->addr = addr;
		n->loc = loc;
		n->size = size;
		_addressMap[addr] = n;
		AddOneSeen(loc, _allocSeen);
		return n;
	};

	std::map<uint64_t, MemAddress*> * GetAddressMapPtr() {
		return &_addressMap;
	}
	void Delete(int64_t loc, uint64_t addr) {
		if (_addressMap.find(addr) != _addressMap.end()) {
			_allocedAddresses.push_back(_addressMap[addr]);
			_addressMap.erase(addr);
		}
		AddOneSeen(loc, _freeSeen);
	};

	uint64_t allocCount;
	std::map<int64_t, int64_t> _allocSeen;
	std::map<int64_t, int64_t> _freeSeen;
	std::map<uint64_t, MemAddress*> _addressMap;
	std::deque<MemAddress *> _allocedAddresses;
};


class OutputWriter{
public:
	OutputWriter(std::shared_ptr<MemKeeper> cmem, std::shared_ptr<MemKeeper> gmem) : _cpuLocalMem(cmem), _gpuLocalMem(gmem) {};
	void RecordGPUMallocPair(MemAddress * addr, int64_t freeLoc) {
		if (_GPUMallocRecords.find(addr->loc) == _GPUMallocRecords.end())
			_GPUMallocRecords[addr->loc] = std::map<int64_t,CUMallocTracker*>();
		if (_GPUMallocRecords[addr->loc].find(freeLoc) != _GPUMallocRecords[addr->loc].end())
			_GPUMallocRecords[addr->loc][freeLoc]->count++;
		else {
			CUMallocTracker * tmp = new CUMallocTracker();
			tmp->allocSite = addr->loc;
			tmp->freeSite = freeLoc;
			tmp->count = 1;
			_GPUMallocRecords[addr->loc][freeLoc] = tmp;
		}
	};

	void RecordCPUMallocPair(MemAddress * addr, int64_t freeLoc) {
		if (_CPUTransRecords.find(addr->loc) == _CPUTransRecords.end())
			_CPUTransRecords[addr->loc] = std::map<int64_t,GLIBMallocTracker*>();
		if (_CPUTransRecords[addr->loc].find(freeLoc) != _CPUTransRecords[addr->loc].end())
			_CPUTransRecords[addr->loc][freeLoc]->count++;
		else {
			GLIBMallocTracker * tmp = new GLIBMallocTracker();
			tmp->allocSite = addr->loc;
			tmp->freeSite = freeLoc;
			tmp->count = 1;
			_CPUTransRecords[addr->loc][freeLoc] = tmp;
		}
	};

	void RecordCopyRecord(MemAddress * addr, int64_t copyLoc) {
		if (_CopyRecords.find(addr->loc) == _CopyRecords.end())
			_CopyRecords[addr->loc] = std::map<int64_t,CUMemTransferTracker*>();
		if (_CopyRecords[addr->loc].find(copyLoc) != _CopyRecords[addr->loc].end())
			_CopyRecords[addr->loc][copyLoc]->count++;
		else {
			CUMemTransferTracker * tmp = new CUMemTransferTracker();
			tmp->allocSite = addr->loc;
			tmp->copyID = copyLoc;
			tmp->count = 1;
			_CopyRecords[addr->loc][copyLoc] = tmp;
		}		
	};
	// template <typename T> 
	// std::set<int64_t> CheckForUnwrittenEntries(std::shared_ptr<MemKeeper> mem, std::map<int64_t, std::map<int64_t,T>> & m) {
	// 	std::set<int64_t> ret;
	// 	std::map<uint64_t, MemAddress*> * a = mem->GetAddressMapPtr();
	// 	for(auto i : *a) {
	// 		if (m.find(i.second.loc) == m.end()) {
	// 			ret.insert(i.second.loc);
	// 		}
	// 	}
	// 	return ret;
	// };

	// template<typename T>
	// void WriteUnknownMallocs(std::set<int64_t>)


	// void WriteUnknownCPUMallocs(std::shared_ptr<MemKeeper> cpuMem) {
	// 	std::set<int64_t> tmp = CheckForUnwrittenEntries<GLIBMallocTracker*>(cpuMem, _CPUTransRecords);

	template<typename T> 
	void WriteMemData(MemRecDataFile & wfile, std::map<int64_t, std::map<int64_t,T*>> & mmap, std::shared_ptr<MemKeeper> mkeep) {
		std::vector<T*> outVec;
		for (auto i : mmap){
			for (auto x : i.second) {
				outVec.push_back(x.second);
				mkeep->SubNumSeen(i.first,mkeep->_allocSeen,x.second->count);
				mkeep->SubNumSeen(x.first,mkeep->_freeSeen,x.second->count);
			}
		}
		for (auto i : mkeep->_allocSeen) {
			T * tmp = new T();
			tmp->allocSite = i.first;
			tmp->count = i.second;
			tmp->freeSite = -1;
			outVec.push_back(tmp);
		}
		for (auto i : mkeep->_freeSeen) {
			T * tmp = new T();
			tmp->freeSite = i.first;
			tmp->count = i.second;
			tmp->allocSite = -1;
			outVec.push_back(tmp);
		}
		wfile.Write<T *>(outVec);
	};

	// };
	~OutputWriter() {
		MemRecDataFile wfile(fopen("DIOGENES_MemRecords.bin","wb"));
		WriteMemData<CUMallocTracker>(wfile, _GPUMallocRecords, _gpuLocalMem);
		WriteMemData<GLIBMallocTracker>(wfile, _CPUTransRecords, _cpuLocalMem);	
		// GPUMallocVec gvec;
		// for (auto i : _GPUMallocRecords)
		// 	for (auto x : i.second)
		// 		gvec.push_back(x.second);
		// wfile.Write<CUMallocTracker*>(gvec);

		// CPUMallocVec cvec;
		// for (auto i : _CPUTransRecords)
		// 	for (auto x : i.second)
		// 		cvec.push_back(x.second);
		// wfile.Write<GLIBMallocTracker*>(cvec);

		MemTransVec mvec;
		for (auto i : _CopyRecords)
			for (auto x : i.second)
				mvec.push_back(x.second);

		wfile.Write<CUMemTransferTracker*>(mvec);
	};
	std::shared_ptr<MemKeeper> _cpuLocalMem;
	std::shared_ptr<MemKeeper> _gpuLocalMem;
	std::map<int64_t, std::map<int64_t,CUMemTransferTracker*>> _CopyRecords;
	std::map<int64_t, std::map<int64_t,GLIBMallocTracker*>> _CPUTransRecords;
	std::map<int64_t, std::map<int64_t,CUMallocTracker*>> _GPUMallocRecords;
};

volatile bool DIOGENES_TEAR_DOWN = false;
class MemTracker {
public:

	MemTracker() {
		_managedCount = 999999;
		_NullAddr = new MemAddress();
		_NullAddr->loc = -1;
		_NullAddr->size = 0;
		_cpuLocalMem.reset(new MemKeeper());
		_gpuLocalMem.reset(new MemKeeper());
		_pinnedMemory.reset(new MemKeeper());
		_outWriter.reset(new OutputWriter(_cpuLocalMem, _gpuLocalMem));
	};

	~MemTracker() {
		DIOGENES_TEAR_DOWN = true;
	}
	void CPUMallocData(uint64_t addr, size_t size, int64_t loc) {
		_cpuLocalMem->Set(addr,size, loc);
	};

	void CPUFreeData(uint64_t addr, int64_t loc) {
		MemAddress * cpuAddr = _cpuLocalMem->Get(addr);
		if (cpuAddr != NULL) {
			_outWriter->RecordCPUMallocPair(cpuAddr, loc);
		}
		_cpuLocalMem->Delete(loc,addr);
	};

	void GPUMallocData(uint64_t addr, size_t size, int64_t loc)  {
		_gpuLocalMem->Set(addr,size, loc);
	};
	void GPUFreeData(uint64_t addr, int64_t loc) {
		MemAddress * cudaManaged = _pinnedMemory->GetLowerBound(addr);
		if (cudaManaged != NULL){
			FreePinnedMemory(addr);
			return;
		}

		MemAddress * gpuAddr = _gpuLocalMem->Get(addr);
		if (gpuAddr != NULL) {
			_outWriter->RecordGPUMallocPair(gpuAddr, loc);
		}		
		_gpuLocalMem->Delete(loc,addr);	
	};

	void AllocatePinnedMemory(uint64_t addr, size_t size) {
		_pinnedMemory->Set(addr, size, _managedCount);
		//_managedCount++;
	};

	void FreePinnedMemory(uint64_t addr) {
		_pinnedMemory->Delete(_managedCount,addr);	
	}
 	
	void RecordMemTransfer(uint64_t addr, int64_t loc) {
		MemAddress * cudaManaged = _pinnedMemory->GetLowerBound(addr);
		if (cudaManaged != NULL)
			return;

		MemAddress * cpuAddr = _cpuLocalMem->GetLowerBound(addr);
		if (cpuAddr == NULL) {
			cpuAddr = _NullAddr;
		}
		_outWriter->RecordCopyRecord(cpuAddr, loc);
	}
	uint64_t _managedCount;
	MemAddress * _NullAddr;
	std::shared_ptr<OutputWriter> _outWriter;
	std::shared_ptr<MemKeeper> _cpuLocalMem;
	std::shared_ptr<MemKeeper> _gpuLocalMem;
	std::shared_ptr<MemKeeper> _pinnedMemory;
};


struct WriteTotals {
	FILE * _fp;
	uint64_t freeCount;
	uint64_t mallocCount;
	WriteTotals(FILE * f) : _fp(f), freeCount(0), mallocCount(0) {

		std::cerr << "[DIOGENES::WriteTotals] Opening new file" << std::endl;
	};
	~WriteTotals() { 
		fwrite(&mallocCount, 1, sizeof(uint64_t), _fp);
		fwrite(&freeCount, 1, sizeof(uint64_t), _fp);
		fclose(_fp);
		std::cout << "[DIOGENES::WriteTotals] Malloc Call Count = " << std::dec << mallocCount <<  " Free Call Count = " << freeCount << std::endl;

	};

};

// Lock Atomic
std::atomic<bool> DIOGENES_MEM_Atomic_Malloc(false);
std::atomic<bool> DIOGENES_MEM_IN_WRAPPER(false);
inline bool DIOGENES_GetGlobalLock() {
	bool setVal = false;
	return DIOGENES_MEM_Atomic_Malloc.compare_exchange_weak(setVal, true);
}

inline bool DIOGENES_ReleaseGlobalLock() {
	DIOGENES_MEM_Atomic_Malloc.exchange(false);
	return true;
}

inline void DIOGENES_SetInWrapper() {
	DIOGENES_MEM_IN_WRAPPER.exchange(true);
}

inline void DIOGENES_UnsetInWrapper() {
	DIOGENES_MEM_IN_WRAPPER.exchange(false);
}
inline bool DIOGENES_GetWrapperStatus() {
	return DIOGENES_MEM_IN_WRAPPER.load();
};

enum DIOG_IDNUMBER
{
E_cudaFree = 10,
E_cudaMalloc = 11,
E_cudaMemcpyAsync = 12,
E_cudaMemcpy = 13,
E_cudaMallocHost = 14,
E_cuMemcpyHtoD_v2 = 15,
E_cuMemcpyDtoH_v2 = 16,
E_cuMemFree_v2 = 17,
E_cuMemAlloc_v2 = 18,
E_cuMemAllocHost_v2 = 19,
E_cuMemFreeHost = 20,
E_glibMalloc = 21,
E_glibFree = 22,
};

struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

std::shared_ptr<MemTracker> DIOGENES_MEMORY_RECORDER;
std::shared_ptr<StackKeyWriter> DIOGENES_MEM_KEYFILE;
std::shared_ptr<WriteTotals> DIOG_WriteTotals;
std::shared_ptr<std::unordered_map<DIOG_IDNUMBER,StackPoint,EnumClassHash>> DIOG_GLOBAL_SPS;
extern "C" void * DIOGENES_REC_GLIBMALLOC(size_t size);
extern "C" void DIOGENES_REC_GLIBFREE(void * addr);
extern "C" cudaError_t DIOGENES_cudaFree(void * mem);
extern "C" cudaError_t DIOGENES_cudaMemcpy(void * dst, void * src, size_t count, cudaMemcpyKind kind);
extern "C" cudaError_t DIOGENES_cudaMemcpyAsync(void * dst, void * src, size_t count, cudaMemcpyKind kind, cudaStream_t stream);

gotcha_wrappee_handle_t DIOGENES_cudaFree_handle;
gotcha_wrappee_handle_t DIOGENES_cudaMemcpy_handle;
gotcha_wrappee_handle_t DIOGENES_libcmalloc_handle;
gotcha_wrappee_handle_t DIOGENES_libcfree_handle;
gotcha_wrappee_handle_t DIOGENES_cudaMemcpyAsync_handle;

typeof(&DIOGENES_cudaMemcpy) DIOGENES_cudaMemcpy_wrapper;
typeof(&DIOGENES_cudaFree) DIOGENES_cudaFree_wrapper;
typeof(&DIOGENES_REC_GLIBMALLOC) DIOGENES_libcmalloc_wrapper;
typeof(&DIOGENES_REC_GLIBFREE) DIOGENES_libcfree_wrapper;
typeof(&DIOGENES_cudaMemcpyAsync) DIOGENES_cudaMemcpyAsync_wrapper;

struct gotcha_binding_t DIOGNESE_gotfuncs[] = {{"cudaFree", (void*)DIOGENES_cudaFree,&DIOGENES_cudaFree_handle},
											   {"cudaMemcpyAsync", (void*)DIOGENES_cudaMemcpyAsync,&DIOGENES_cudaMemcpyAsync_handle},
											   {"cudaMemcpy", (void*)DIOGENES_cudaMemcpy,&DIOGENES_cudaMemcpy_handle},
											   {"malloc", (void*)DIOGENES_REC_GLIBMALLOC,&DIOGENES_libcmalloc_handle},
											   {"free", (void *)DIOGENES_REC_GLIBFREE, &DIOGENES_libcfree_handle}};


void SetupDiogGlobalSPS() {
	DIOG_GLOBAL_SPS.reset(new std::unordered_map<DIOG_IDNUMBER,StackPoint,EnumClassHash>());
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cudaFree, StackPoint("cudaFree", "cudaFree",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cudaMalloc, StackPoint("cudaMalloc", "cudaMalloc",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cudaMemcpyAsync, StackPoint("cudaMemcpyAsync", "cudaMemcpyAsync",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cudaMemcpy, StackPoint("cudaMemcpy", "cudaMemcpy",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cudaMallocHost, StackPoint("cudaMallocHost", "cudaMallocHost",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cuMemcpyHtoD_v2, StackPoint("cuMemcpyHtoD_v2", "cuMemcpyHtoD_v2",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cuMemcpyDtoH_v2, StackPoint("cuMemcpyDtoH_v2", "cuMemcpyDtoH_v2",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cuMemFree_v2, StackPoint("cuMemFree_v2", "cuMemFree_v2",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cuMemAlloc_v2, StackPoint("cuMemAlloc_v2", "cuMemAlloc_v2",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cuMemAllocHost_v2, StackPoint("cuMemAllocHost_v2","cuMemAllocHost_v2",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_cuMemFreeHost, StackPoint("cuMemFreeHost","cuMemFreeHost",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_glibMalloc, StackPoint("__GI___libc_malloc","__GI___libc_malloc",0,0)));
	DIOG_GLOBAL_SPS->insert(std::make_pair(E_glibFree, StackPoint("__libc_free","__libc_free",0,0)));
	DIOG_GLOBAL_SPS->find(E_cudaFree)->second.raFramePos =(uint64_t)E_cudaFree; 
	DIOG_GLOBAL_SPS->find(E_cudaMalloc)->second.raFramePos =(uint64_t)E_cudaMalloc; 
	DIOG_GLOBAL_SPS->find(E_cudaMemcpyAsync)->second.raFramePos =(uint64_t)E_cudaMemcpyAsync; 
	DIOG_GLOBAL_SPS->find(E_cudaMemcpy)->second.raFramePos =(uint64_t)E_cudaMemcpy; 
	DIOG_GLOBAL_SPS->find(E_cudaMallocHost)->second.raFramePos =(uint64_t)E_cudaMallocHost; 
	DIOG_GLOBAL_SPS->find(E_cuMemcpyHtoD_v2)->second.raFramePos =(uint64_t)E_cuMemcpyHtoD_v2; 
	DIOG_GLOBAL_SPS->find(E_cuMemcpyDtoH_v2)->second.raFramePos =(uint64_t)E_cuMemcpyDtoH_v2; 
	DIOG_GLOBAL_SPS->find(E_cuMemFree_v2)->second.raFramePos =(uint64_t)E_cuMemFree_v2; 
	DIOG_GLOBAL_SPS->find(E_cuMemAlloc_v2)->second.raFramePos =(uint64_t)E_cuMemAlloc_v2; 
	DIOG_GLOBAL_SPS->find(E_cuMemFreeHost)->second.raFramePos = (uint64_t)E_cuMemFreeHost;
	DIOG_GLOBAL_SPS->find(E_glibMalloc)->second.raFramePos = (uint64_t)E_glibMalloc;
	DIOG_GLOBAL_SPS->find(E_glibFree)->second.raFramePos = (uint64_t)E_glibFree;
};




#define PLUG_BUILD_FACTORY() \
	if (DIOGENES_MEMORY_RECORDER.get() == NULL && DIOGENES_TEAR_DOWN == false) { \
		atexit(DIOGENES_exitDestroyer); \
		DIOGENES_MEMORY_RECORDER.reset(new MemTracker()); \
		SetupDiogGlobalSPS();\
		DIOGENES_SETUP_GOTCHA();\
		DIOGENES_MEM_KEYFILE.reset(new StackKeyWriter(fopen("DIOENES_MemRecUnknowns.bin","w"), static_cast<uint64_t>(DIOGENES_UNKNOWN_CTX_ID))); \
	} 
		//
#define PLUG_FACTORY_PTR DIOGENES_MEMORY_RECORDER.get()

int debugPrintCount = 0;

thread_local void ** DIOGENES_MALLOC_MEMLOCATION = NULL;
thread_local size_t DIOGENES_MALLOC_MEMSIZE = 0;
thread_local bool DIOGENES_SEEN_RUNTIMEMALLOCAPI = false;
thread_local bool DIOGENES_SEEN_RUNTIMECPY = false;
thread_local bool DIOGENES_SEEN_RUNTIMEFREE = false;
thread_local std::vector<StackPoint> DIOGENES_CACHED_POINTS;
thread_local volatile size_t DIOGENSE_GLIB_MALLOC_SIZE = 0;
thread_local volatile bool DIOGENES_IN_MEMORYFREE = false;
thread_local volatile bool DIOGENSE_IN_MEMORYALLOC = false;
volatile bool SEEN_FIRST_CUDACALL = false;
extern "C" {

	void DIOGENES_exitDestroyer() {
		DIOGENES_TEAR_DOWN = true;
		DIOGENES_GetGlobalLock();
	}

	void DIOGENES_SETUP_BINDINGS() {
		// PLUG_BUILD_FACTORY();
		// void * glibc = dlopen("libc.so.6", RTLD_LAZY); 
		std::cerr << "[DIOGENES_SETUP_BINDINGS] In Original Wrapping for libc_malloc and libc_free" << std::endl;
		// DIOGENES_libcmalloc_wrapper = (typeof(&DIOGENES_REC_GLIBMALLOC))dlsym(glibc,"malloc"); 
		// DIOGENES_libcfree_wrapper = (typeof(&DIOGENES_REC_GLIBFREE))dlsym(glibc,"free"); 
		// gotcha_wrap(DIOGNESE_gotfuncs, sizeof(DIOGNESE_gotfuncs)/sizeof(struct gotcha_binding_t), "diogenes"); 
	}

	void DIOGENES_SETUP_GOTCHA() {
		void * glibc = dlopen("libc.so.6", RTLD_LAZY);
		void * cudarthandle = dlopen("libcudart.so", RTLD_LAZY); 
		std::cerr << "[DIOGENES_SETUP_BINDINGS] Wrapping libc_malloc and libc_free" << std::endl;
		DIOGENES_libcmalloc_wrapper = (typeof(&DIOGENES_REC_GLIBMALLOC))dlsym(glibc,"malloc"); 
		DIOGENES_libcfree_wrapper = (typeof(&DIOGENES_REC_GLIBFREE))dlsym(glibc,"free"); 
		DIOGENES_cudaFree_wrapper = (typeof(&DIOGENES_cudaFree))dlsym(cudarthandle,"cudaFree");
		DIOGENES_cudaMemcpy_wrapper = (typeof(&DIOGENES_cudaMemcpy))dlsym(cudarthandle,"cudaMemcpy");
		DIOGENES_cudaMemcpyAsync_wrapper = (typeof(&DIOGENES_cudaMemcpyAsync))dlsym(cudarthandle,"cudaMemcpyAsync");
		gotcha_wrap(DIOGNESE_gotfuncs, sizeof(DIOGNESE_gotfuncs)/sizeof(struct gotcha_binding_t), "diogenes"); 		
	}

	void DIOGENES_REC_CudaMalloc(void ** mem, size_t size) {
		DIOGENES_MALLOC_MEMLOCATION = mem;
		DIOGENES_MALLOC_MEMSIZE = size;
		DIOGENES_SEEN_RUNTIMEMALLOCAPI = true;
	}

	void DIOGENES_REC_HostCudaMalloc(void ** mem, size_t size) {
		DIOGENES_MALLOC_MEMLOCATION = mem;
		DIOGENES_MALLOC_MEMSIZE = size;
		DIOGENES_SEEN_RUNTIMEMALLOCAPI = true;
	}


	void DIOGENES_REC_cuMemAllocHost(void ** mem, size_t size) {
		if(!DIOGENES_SEEN_RUNTIMEMALLOCAPI) {
			DIOGENES_MALLOC_MEMLOCATION = mem;
			DIOGENES_MALLOC_MEMSIZE = size;
		}
	}

	void DIOGENES_REC_cuMemAlloc(void ** mem, size_t size) {
		if(!DIOGENES_SEEN_RUNTIMEMALLOCAPI) {
			DIOGENES_MALLOC_MEMLOCATION = mem;
			DIOGENES_MALLOC_MEMSIZE = size;
		}		
	}
	

	
	void POSTPROCESS_MALLOC(uint64_t addr, size_t size, DIOG_IDNUMBER idType) {
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			SEEN_FIRST_CUDACALL = true;
			PLUG_BUILD_FACTORY();
			std::shared_ptr<std::unordered_map<DIOG_IDNUMBER,StackPoint,EnumClassHash>> local = DIOG_GLOBAL_SPS;
			DIOGENES_CACHED_POINTS.clear();
			bool ret = GET_FP_STACKWALK(DIOGENES_CACHED_POINTS);
			auto n = local->find(idType);
			if (n == local->end())
				assert(n != local->end());
			DIOGENES_CACHED_POINTS.push_back(n->second);
			int64_t myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(DIOGENES_CACHED_POINTS));
			if (idType != E_cuMemAllocHost_v2 && idType != E_cudaMallocHost)
				PLUG_FACTORY_PTR->GPUMallocData((uint64_t)addr, size, myID);
			else
				PLUG_FACTORY_PTR->AllocatePinnedMemory(addr, size);
			DIOGENES_ReleaseGlobalLock();
		}
	}


	void POSTPROCESS_GNUMALLOC(uint64_t addr, size_t size) {
		if (DIOGENSE_IN_MEMORYALLOC == false && DIOGENES_IN_MEMORYFREE == false && DIOGENES_TEAR_DOWN == false) {
			DIOGENSE_IN_MEMORYALLOC = true;
			if(DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
				PLUG_BUILD_FACTORY();
				//std::cout << "Processing Malloc at addr: " << std::hex << addr << " size of " << std::dec << size << std::endl;
				std::shared_ptr<std::unordered_map<DIOG_IDNUMBER,StackPoint,EnumClassHash>> local = DIOG_GLOBAL_SPS;
				std::vector<StackPoint> freeSiteStacks;

				int64_t myID = GET_FP_STACKWALK_FAST_NONSW(freeSiteStacks);
				if (myID <= 1) {
					auto n = local->find(E_glibMalloc);
					if (n == local->end())
						assert(n != local->end());
					freeSiteStacks.push_back(n->second);
					myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(freeSiteStacks));
					freeSiteStacks.pop_back();
					GET_FP_REGISTER_ID(myID, freeSiteStacks);
					//freeSiteStacks.push_back(n->second);
				}
				//bool ret = GET_FP_STACKWALK_NONSW(freeSiteStacks);

				// bool ret = GET_FP_STACKWALK_NONSW(freeSiteStacks);
				// auto n = local->find(E_glibMalloc);
				// if (n == local->end())
				// 	assert(n != local->end());
				// freeSiteStacks.push_back(n->second);
				// int64_t myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(freeSiteStacks));
				PLUG_FACTORY_PTR->CPUMallocData(addr, size, myID);
				DIOGENES_ReleaseGlobalLock();
			}
			DIOGENSE_IN_MEMORYALLOC = false;
		}
	}

	void POSTPROCESS_GNUFREE(uint64_t addr) {
		// if (SEEN_FIRST_CUDACALL == false)
		// 	return;
		if (DIOGENES_IN_MEMORYFREE == false && DIOGENSE_IN_MEMORYALLOC == false && DIOGENES_TEAR_DOWN == false){
			DIOGENES_IN_MEMORYFREE = true;
			if(DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
				PLUG_BUILD_FACTORY();
				//std::cout << "Processing Free at addr: " << std::hex << addr << std::endl;
				std::shared_ptr<std::unordered_map<DIOG_IDNUMBER,StackPoint,EnumClassHash>> local = DIOG_GLOBAL_SPS;
				std::vector<StackPoint> freeSiteStacks;
				//DIOGENES_CACHED_POINTS.clear();

				int64_t myID = GET_FP_STACKWALK_FAST_NONSW(freeSiteStacks);
				if (myID <= 1) {
					auto n = local->find(E_glibFree);
					if (n == local->end())
						assert(n != local->end());
					freeSiteStacks.push_back(n->second);
					myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(freeSiteStacks));
					freeSiteStacks.pop_back();
					GET_FP_REGISTER_ID(myID, freeSiteStacks);
					//freeSiteStacks.push_back(n->second);
				}

				// bool ret = GET_FP_STACKWALK_NONSW(freeSiteStacks);
				// auto n = local->find(E_glibFree);
				// if (n == local->end())
				// 	assert(n != local->end());
				// freeSiteStacks.push_back(n->second);
				// int64_t myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(freeSiteStacks));
				PLUG_FACTORY_PTR->CPUFreeData(addr, myID);
				DIOGENES_ReleaseGlobalLock();
			}
			DIOGENES_IN_MEMORYFREE = false;
		}
	}

	void DIOGENES_RESET_MALLOCMEM() {
		DIOGENES_MALLOC_MEMLOCATION = NULL;
		DIOGENES_MALLOC_MEMSIZE = 0;
		DIOGENES_SEEN_RUNTIMEMALLOCAPI = false;
	}
	
	void DIOGENES_REC_CudaMallocPost() {
		uint64_t addr;
		if (DIOGENES_MALLOC_MEMLOCATION != NULL) {
			addr = (uint64_t)(*DIOGENES_MALLOC_MEMLOCATION);
			POSTPROCESS_MALLOC(addr, DIOGENES_MALLOC_MEMSIZE,E_cudaMalloc);
		}
		DIOGENES_RESET_MALLOCMEM();
	}

	void DIOGENES_REC_cudaMallocHostpost() {
		uint64_t addr;
		if (DIOGENES_MALLOC_MEMLOCATION != NULL) {
			addr = (uint64_t)(*DIOGENES_MALLOC_MEMLOCATION);
			POSTPROCESS_MALLOC(addr, DIOGENES_MALLOC_MEMSIZE,E_cudaMallocHost);
		}
		DIOGENES_RESET_MALLOCMEM();
	}

	void DIOGENES_REC_cuMemAllocpost() {
		uint64_t addr;
		if(!DIOGENES_SEEN_RUNTIMEMALLOCAPI) {
			if(DIOGENES_MALLOC_MEMLOCATION != NULL) {				
				addr = (uint64_t)(*DIOGENES_MALLOC_MEMLOCATION);
				POSTPROCESS_MALLOC(addr, DIOGENES_MALLOC_MEMSIZE,E_cuMemAlloc_v2);
			}
			DIOGENES_RESET_MALLOCMEM();
		}
	}

	// Driver API wrappers

	void POSTPROCESS_COPY(uint64_t hostptr, DIOG_IDNUMBER idType) {
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			SEEN_FIRST_CUDACALL = true;
			PLUG_BUILD_FACTORY();
			std::shared_ptr<std::unordered_map<DIOG_IDNUMBER,StackPoint,EnumClassHash>> local = DIOG_GLOBAL_SPS;
			DIOGENES_CACHED_POINTS.clear();
			bool ret = GET_FP_STACKWALK(DIOGENES_CACHED_POINTS);
			auto n = local->find(idType);
			if (n == local->end())
				assert(n != local->end());
			DIOGENES_CACHED_POINTS.push_back(n->second);
			int64_t myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(DIOGENES_CACHED_POINTS));
			//std::cout << "Processing Memcpy at addr: " << std::hex << hostptr << std::endl;
			PLUG_FACTORY_PTR->RecordMemTransfer(hostptr, myID);
			DIOGENES_ReleaseGlobalLock();
		}
	}

	void POSTPROCESS_FREE(uint64_t ptr, DIOG_IDNUMBER idType) {
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			SEEN_FIRST_CUDACALL = true;
			PLUG_BUILD_FACTORY();
			std::shared_ptr<std::unordered_map<DIOG_IDNUMBER,StackPoint,EnumClassHash>> local = DIOG_GLOBAL_SPS;
			DIOGENES_CACHED_POINTS.clear();
			bool ret = GET_FP_STACKWALK(DIOGENES_CACHED_POINTS);
			auto n = local->find(idType);
			if (n == local->end())
				assert(n != local->end());
			DIOGENES_CACHED_POINTS.push_back(n->second);
			int64_t myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(DIOGENES_CACHED_POINTS));
			PLUG_FACTORY_PTR->GPUFreeData(ptr, myID);
			DIOGENES_ReleaseGlobalLock();
		} else {
			fprintf(stderr, "COULD NOT PROCESS FREE REQUEST!\n");
		}	
	}


	
	void DIOGENES_REC_cuMemAllocHostpost() {
		uint64_t addr;
		if(!DIOGENES_SEEN_RUNTIMEMALLOCAPI) {
			if(DIOGENES_MALLOC_MEMLOCATION != NULL) {				
				addr = (uint64_t)(*DIOGENES_MALLOC_MEMLOCATION);
				POSTPROCESS_MALLOC(addr, DIOGENES_MALLOC_MEMSIZE,E_cuMemAllocHost_v2);
			}
			DIOGENES_RESET_MALLOCMEM();
		}		
	}
	
	void DIOGENES_REC_cuMemcpyHtoD(void * devptr, void * hostptr, size_t size) {
		if(!DIOGENES_SEEN_RUNTIMECPY){
			POSTPROCESS_COPY((uint64_t)hostptr,E_cuMemcpyHtoD_v2);
		}
	}
	
	void DIOGENES_REC_cuMemcpyDtoH(void * hostptr, void * devptr, size_t size) {
		if(!DIOGENES_SEEN_RUNTIMECPY){
			POSTPROCESS_COPY((uint64_t)hostptr,E_cuMemcpyDtoH_v2);
		}
	}

	void DIOGENES_REC_CudaMemcpyAsync(void * dst, void * src, size_t size, cudaMemcpyKind kind, cudaStream_t stream) {
		void * hostptr = NULL;
		if (kind == cudaMemcpyDeviceToHost) {
			hostptr = dst;
		} else if (kind == cudaMemcpyHostToDevice) {
			hostptr = src;
		} else {
			return;
		}
		DIOGENES_SEEN_RUNTIMECPY = true;
		POSTPROCESS_COPY((uint64_t)hostptr,E_cudaMemcpyAsync);
	}


	void DIOGENES_REC_CudaMemcpy(void * dst, void * src, size_t size, cudaMemcpyKind kind) {
		void * hostptr = NULL;
		if (kind == cudaMemcpyDeviceToHost) {
			hostptr = dst;
		} else if (kind == cudaMemcpyHostToDevice) {
			hostptr = src;
		} else {
			return;
		}
		DIOGENES_SEEN_RUNTIMECPY = true;
		POSTPROCESS_COPY((uint64_t)hostptr,E_cudaMemcpy);
	}

	void DIOGENES_REC_CudaMemcpyAsyncPost() {
		DIOGENES_SEEN_RUNTIMECPY = false;
	}

	void DIOGENES_REC_CudaFree(void * ptr) {
		DIOGENES_SEEN_RUNTIMEFREE = true;
		POSTPROCESS_FREE((uint64_t)ptr, E_cudaFree);
	}
	void DIOGENES_REC_CudaFreePost(void * ptr) {
		DIOGENES_SEEN_RUNTIMEFREE = false;
	}
	void DIOGENES_REC_cuMemFree(void * ptr) {
		if (!DIOGENES_SEEN_RUNTIMEFREE)
			POSTPROCESS_FREE((uint64_t)ptr, E_cuMemFree_v2);
	}

	void DIOGENES_REC_cuMemFreeHost(void * ptr) {
		if(!DIOGENES_SEEN_RUNTIMEFREE)
			POSTPROCESS_FREE((uint64_t)ptr, E_cuMemFreeHost);		
	}


	void * DIOGENES_REC_GLIBMALLOC(size_t size) {
		void * addr =  DIOGENES_libcmalloc_wrapper(size);
		if (DIOGENES_TEAR_DOWN == false)
			POSTPROCESS_GNUMALLOC((uint64_t)addr, size);
		return addr;
	}

	void DIOGENES_REC_GLIBFREE(void* addr) {
		
		if (addr != NULL)
			POSTPROCESS_GNUFREE((uint64_t)addr);
		if (DIOGENES_TEAR_DOWN == false)
			DIOGENES_libcfree_wrapper(addr);
	}

	cudaError_t DIOGENES_cudaFree(void * ptr) {
		DIOGENES_SEEN_RUNTIMEFREE = true;
		if (ptr != NULL)
			POSTPROCESS_FREE((uint64_t)ptr, E_cudaFree);
		cudaError_t ret = DIOGENES_cudaFree_wrapper(ptr);
		DIOGENES_SEEN_RUNTIMEFREE = false;
		return ret;
	}

	cudaError_t DIOGENES_cudaMemcpy(void * dst, void * src, size_t count, cudaMemcpyKind kind) {
		DIOGENES_SEEN_RUNTIMECPY = true;
		void * hostptr = NULL;
		if (kind == cudaMemcpyDeviceToHost) {
			hostptr = dst;
		} else if (kind == cudaMemcpyHostToDevice) {
			hostptr = src;
		}
		if (hostptr != NULL)
			POSTPROCESS_COPY((uint64_t)hostptr,E_cudaMemcpy);		
		auto ret = DIOGENES_cudaMemcpy_wrapper(dst, src, count, kind);
		DIOGENES_SEEN_RUNTIMECPY = false;
		return ret;
	}

	cudaError_t DIOGENES_cudaMemcpyAsync(void * dst, void * src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
		DIOGENES_SEEN_RUNTIMECPY = true;
		void * hostptr = NULL;
		if (kind == cudaMemcpyDeviceToHost) {
			hostptr = dst;
		} else if (kind == cudaMemcpyHostToDevice) {
			hostptr = src;
		}
		if (hostptr != NULL)
			POSTPROCESS_COPY((uint64_t)hostptr,E_cudaMemcpyAsync);	
		auto ret = DIOGENES_cudaMemcpyAsync_wrapper(dst, src, count, kind, stream);
		DIOGENES_SEEN_RUNTIMECPY = false;
		return ret;
	}
/*	void DIOGENES_REC_GLIBMALLOC_PRE(size_t size) {
		DIOGENSE_GLIB_MALLOC_SIZE = size;
	}

	void DIOGENES_REC_GLIBMALLOC_POST(uint64_t retAddress) {
		POSTPROCESS_GNUMALLOC(retAddress,DIOGENSE_GLIB_MALLOC_SIZE);
	}*/

/*	void * DIOGENES_REC_MALLOCWrapper(size_t size)  {
		int64_t cache = DIOGENES_CTX_ID;
		if(DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			// if (debugPrintCount % 10000 == 0)
			// 	std::cerr << "[CACHE] Malloc Cache address is - " << cache << std::endl;
			// debugPrintCount++;
			void * ret = malloc(size);
			PLUG_FACTORY_PTR->CPUMallocData((uint64_t)ret, size, cache);
			DIOGENES_ReleaseGlobalLock();
			return ret;
		} else {
			return malloc(size);
		}
	}


	void DIOGENES_REC_FREEWrapper(void * mem) {
		int64_t cache = DIOGENES_CTX_ID;
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			// if (debugPrintCount % 10000 == 0)
			// 	std::cerr << "[CACHE] Free Cache address is - " << cache << std::endl;
			// debugPrintCount++;
			PLUG_FACTORY_PTR->CPUFreeData((uint64_t)mem, cache);
			DIOGENES_ReleaseGlobalLock();
		}
		free(mem);
	}
	cudaError_t DIOGENES_REC_HostCudaMalloc(void ** data, size_t size) {
		cudaError_t ret = cudaErrorUnknown;
		DIOGENES_SetInWrapper();
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			ret = cudaMallocHost(data, size);
			PLUG_FACTORY_PTR->AllocatePinnedMemory((uint64_t)(*data), size);
			DIOGENES_ReleaseGlobalLock();
		} else {
			ret = cudaMallocHost(data, size);
		}		
		DIOGENES_UnsetInWrapper();
		return ret;		
	}
	CUresult DIOGENES_REC_cuMemAllocHost(void ** data, size_t size) {
		CUresult ret = CUDA_ERROR_UNKNOWN;
		DIOGENES_SetInWrapper();
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			ret = cuMemAllocHost(data, size);
			PLUG_FACTORY_PTR->AllocatePinnedMemory((uint64_t)(*data), size);
			DIOGENES_ReleaseGlobalLock();
		} else {
			ret = cuMemAllocHost(data, size);
		}		
		DIOGENES_UnsetInWrapper();
		return ret;		
	}

	cudaError_t DIOGENES_REC_CudaMalloc(void ** data, size_t size) {
		int64_t cache = DIOGENES_CTX_ID;
		cudaError_t ret = cudaErrorUnknown;
		DIOGENES_SetInWrapper();
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			ret = cudaMalloc(data, size);
			PLUG_FACTORY_PTR->GPUMallocData((uint64_t)(*data), size, cache);
			DIOGENES_ReleaseGlobalLock();
		} else {
			ret = cudaMalloc(data, size);
		}
		DIOGENES_UnsetInWrapper();
		//assert(1==0);
		return ret;
	}


	CUresult DIOGENES_REC_cuMemAlloc(CUdeviceptr* data, size_t size) {
		int64_t cache = DIOGENES_CTX_ID;
		CUresult ret = CUDA_ERROR_UNKNOWN;
		DIOGENES_SetInWrapper();
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			ret = cuMemAlloc(data, size);
			PLUG_FACTORY_PTR->GPUMallocData((uint64_t)(*data), size, cache);
			DIOGENES_ReleaseGlobalLock();
		} else {
			ret = cuMemAlloc(data, size);
		}
		DIOGENES_UnsetInWrapper();
		//assert(1==0);
		return ret;
	}

	cudaError_t DIOGENES_REC_CudaFree(void * data) {
		int64_t cache = DIOGENES_CTX_ID;
		DIOGENES_SetInWrapper();
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			PLUG_FACTORY_PTR->GPUFreeData((uint64_t)(data), cache);
			DIOGENES_ReleaseGlobalLock();
		}
		cudaError_t ret = cudaFree(data);
		DIOGENES_UnsetInWrapper();
		return ret;
	} 

	CUresult DIOGENES_REC_cuMemFree(CUdeviceptr data) {
		int64_t cache = DIOGENES_CTX_ID;
		DIOGENES_SetInWrapper();
		if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
			PLUG_BUILD_FACTORY();
			PLUG_FACTORY_PTR->GPUFreeData((uint64_t)(data), cache);
			DIOGENES_ReleaseGlobalLock();
		}
		CUresult ret = cuMemFree(data);
		DIOGENES_UnsetInWrapper();
		return ret;
	} 
	void ** DIOGENES_CUDA_MALLOC_ARG = NULL;
	size_t DIOGENES_CUDA_MALLOC_SIZE = 0;

	void DIOG_CUDAMallocPreCheck(void ** data, size_t size) {
		//std::cerr << "We are here! (MALLOCPRE)" << std::endl;
		if (DIOGENES_GetWrapperStatus() || DIOGENES_TEAR_DOWN == true)
			return;
		DIOGENES_CUDA_MALLOC_ARG = data;
		DIOGENES_CUDA_MALLOC_SIZE = size;	
	}


	void DIOG_CUDAMallocCheck() {
		if(DIOG_WriteTotals == NULL)
			DIOG_WriteTotals.reset(new WriteTotals(fopen("DIOGENES_UnknownWriteCount.bin", "wb")));

		DIOG_WriteTotals->mallocCount++;
		//std::cerr << "We are here! (MALLOC)" << std::endl;
		if (DIOGENES_GetWrapperStatus() || DIOGENES_TEAR_DOWN == true)
			return;
		//std::cerr << "We are here! (MALLOC)" << std::endl;
		if (DIOGENES_CUDA_MALLOC_ARG == NULL)
			return;

		if (DIOGENES_GetGlobalLock()) {
			PLUG_BUILD_FACTORY();
			// Fallback mode, do slow stack walk and save it to file.
			std::vector<StackPoint> points;
			bool ret = GET_FP_STACKWALK(points);
			int64_t myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(points));
			PLUG_FACTORY_PTR->GPUMallocData((uint64_t)(*DIOGENES_CUDA_MALLOC_ARG), DIOGENES_CUDA_MALLOC_SIZE, myID);
			std::cerr << "[DIOGENES::DIOG_CUDAMallocCheck] Unknown Malloc Entry for Identifier = " << myID << std::endl;
			DIOGENES_CUDA_MALLOC_ARG = NULL;
			DIOGENES_CUDA_MALLOC_SIZE = 0;
			DIOGENES_ReleaseGlobalLock();
		}
	}

	void DIOG_CUDAFreeCheck(void * data) {
		if(DIOG_WriteTotals == NULL)
			DIOG_WriteTotals.reset(new WriteTotals(fopen("DIOGENES_UnknownWriteCount.bin", "wb")));

		if (DIOGENES_TEAR_DOWN == false)
			DIOG_WriteTotals->freeCount++;
		//std::cerr << "We are here! (FREE)" << std::endl;
		if (DIOGENES_GetWrapperStatus() || DIOGENES_TEAR_DOWN == true)
			return;
		//std::cerr << "We are here! (FREE)" << std::endl;
		if (DIOGENES_GetGlobalLock()) {

			PLUG_BUILD_FACTORY();
			// Fallback mode, do slow stack walk and save it to file.
			std::vector<StackPoint> points;
			bool ret = GET_FP_STACKWALK(points);
			int64_t myID = static_cast<int64_t>(DIOGENES_MEM_KEYFILE->InsertStack(points));
			PLUG_FACTORY_PTR->GPUFreeData((uint64_t)(data), myID);
			std::cerr << "[DIOGENES::DIOG_CUDAFreeCheck] Unknown Free Entry for Identifier = " << myID << std::endl;
			DIOGENES_ReleaseGlobalLock();
		}		
	}
	cudaError_t DIOGENES_REC_CudaMemcpy(void * dst, const void * src, size_t size, cudaMemcpyKind kind, cudaStream_t stream) {
		int64_t cache = DIOGENES_CTX_ID;
		if (kind == cudaMemcpyDeviceToHost) {
			if (DIOGENES_GetGlobalLock() && DIOGENES_TEAR_DOWN == false) {
				PLUG_BUILD_FACTORY();
				PLUG_FACTORY_PTR->RecordMemTransfer((uint64_t)dst, cache);
				DIOGENES_ReleaseGlobalLock();
			}
		}
		//assert(1==0);
		return cudaMemcpyAsync(dst,src,size,kind,stream);
	}
	// void * DIOGENES_REC_MALLOCWrapper(size_t size)  {
	// 	int64_t cache = DIOGENES_CTX_ID;
	// 	if(DIOGENES_GetGlobalLock()) {
	// 		if ()
	// 	}

	// }
*/
}



// class MemCollector {
// public:
// 	MemCollector::MemCollector() {}

// 	void MemCollector::CaptureMalloc(void * mem, size_t size, uint64_t loc) {
// 		if (mem )
// 	};
// };



