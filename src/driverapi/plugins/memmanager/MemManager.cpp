#include <cassert>
#include <iostream>
#include <fstream> 
#include <memory>   
#include <string>
#include <stdio.h> 
#include <map>
#include <sstream> 
#include <unordered_set>
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
#include "StackPoint.h"
#include <execinfo.h>
#include <unistd.h>
#include <string.h>

bool DIOGENES_SHUTDOWN_MODE = false;

struct DataRecorderCountTool {
	uint64_t total_malloc;
	uint64_t total_free;
	uint64_t total_trans;
	uint64_t trans_applied;
	uint64_t memApplied;

	DataRecorderCountTool() : total_malloc(0), total_free(0), total_trans(0), trans_applied(0), memApplied(0) {};
	inline void AddMalloc() {
		total_malloc++;
	}
	inline void AddFree() {
		total_free++;
	}
	inline void AddTrans() {
		total_trans++;
	}
	inline void TransApplied() {
		trans_applied++;
	}
	inline void FreeApplied() {
		memApplied++;
	}

	~DataRecorderCountTool() {
		std::cout << "[DataRecorderCountTool] Total Malloc Ops = " <<  total_malloc << std::endl;
		std::cout << "[DataRecorderCountTool] Total Free Ops = " <<  total_free << std::endl;
		std::cout << "[DataRecorderCountTool] Total Trans Ops = " <<  total_trans << std::endl;
		std::cout << "[DataRecorderCountTool] Total Trans Applied = " <<  trans_applied << std::endl;
		std::cout << "[DataRecorderCountTool] Total Mem Free Applied Ops = " <<  memApplied << std::endl;
	};
};
struct RecursiveMap{
	uint64_t _total_count;
	bool onlyIfHtoD;
	std::map<uint64_t, RecursiveMap *> _map;
	RecursiveMap() : _total_count(0), onlyIfHtoD(true) {};
	void Init() {
		RAStackReaderWriter rs(fopen("AC_BinStacks.bin", "rb"));
		std::vector<std::vector<uint64_t>>  stacks = rs.ReadStacks();
		for (auto i : stacks) {
			// std::cerr << "\nTEST STACK = ";
			// for (auto n : i) {
			// 	std::cerr << n << ",";
			// }
			// std::cerr << std::endl;
			Insert(i, 0);
		}
		_total_count = 0;
		//StackKeyReader r(fopen("AC_AutoCorrectStacks.txt", "rb"));
		//std::map<uint64_t, std::vector<StackPoint> > stacks = r.ReadStacks();

	};
	~RecursiveMap() {
		DIOGENES_SHUTDOWN_MODE = true;
		if (_total_count != 0)
			std::cout << "NUMBER OF OPTI HITS = " << std::dec << _total_count << std::endl;
	};
	void Insert(std::vector<uint64_t> & input, int pos) {
		if(input.size() <= pos)
			return;
		uint64_t val = input[pos];
		if (val == 0) {
			onlyIfHtoD = false;
			return;
		} else if (val == 1) {
			onlyIfHtoD = true;
			return;
		}
		if (_map.find(val) == _map.end()) {
			_map[val] = new RecursiveMap();
		}
		_map[val]->Insert(input, pos+1);
	};

	bool Lookup(std::vector<uint64_t> & input, int pos) {
		if(input.size() <= pos)
			return true;
		uint64_t val = input[pos];
		if(_map.find(val) == _map.end())
			return false;
		return _map[val]->Lookup(input,pos+1);
	};

	bool IterativeLookup(uint64_t * input, int size, int pos, bool & htodlimit) {
		// std::cerr << "\nLOOKUP STACK = ";
		// for (int i = pos; i < size; i++) {
		// 	std::cerr << input[i] << ",";
		// }
		// std::cerr << std::endl;
		std::map<uint64_t, RecursiveMap *> * curMap = &_map;
		int insize = size;
		htodlimit = true;
		while (pos < insize) {
			auto it = curMap->find(input[pos]);
			if (it == curMap->end()) 
				return false;
			curMap = &(it->second->_map);
			htodlimit = it->second->onlyIfHtoD;
			pos++;
		}
		_total_count++;
		return true;
	};
};


// struct GOT_Wrapping {
// 	std::vector<std::string> funcsToWrap;
// 	std::vector<gotcha_wrappee_handle_t> funcHandles;
// 	//gotcha_binding_t * DIOGNESE_gotfuncs;
// 	GOT_Wrapping() {
// 		funcsToWrap = {"cudaMemcpy","cuMemcpyDtoH_v2","cuMemcpyHtoD_v2","cudaMemcpyAsync ","cudaFree","cudaMalloc","cuMemAlloc_v2","cuMemFree","cuMemAllocHost_v2", "cuMemFreeHost","cudaMallocHost"};
// 		struct gotcha_binding_t DIOGNESE_gotfuncs[] = {
// 					{"cudaFreeHost",(void*)DIOGENES_CudaFreeHostWrapper,&DIOGENES_wrappee_cudaFreeHost_handle},
// 					{"cudaMallocHost",(void*)DIOGENES_CudaMallocHostWrapper,&DIOGENES_wrappee_cudaMallocHost_handle}};

// 	}


// };

//Runtime API Wrappers
extern "C" cudaError_t DIOGENES_cudaFree(void * mem);
extern "C" cudaError_t DIOGENES_cudaFreeHost(void * mem);
extern "C" cudaError_t DIOGENES_cudaMalloc(void ** mem, size_t size);
extern "C" cudaError_t DIOGENES_cudaMallocHost(void ** mem, size_t size);
extern "C" cudaError_t DIOGENES_cudaMemcpy(void * dst, void * src, size_t count, cudaMemcpyKind kind);
extern "C" cudaError_t DIOGENES_cudaMemcpyAsync(void * dst, void * src, size_t count, cudaMemcpyKind kind, cudaStream_t stream);

// Driver API Wrappers
extern "C" CUresult DIOGENES_cuMemFree(void * mem);
extern "C" CUresult DIOGENES_cuMemFreeHost(void * mem);
extern "C" CUresult DIOGENES_cuMemAlloc(void ** mem, size_t size);
extern "C" CUresult DIOGENES_cuMemAllocHost(void ** mem, size_t size);
extern "C" CUresult DIOGENES_cuMemcpyDtoH(void * dst, CUdeviceptr src, size_t count);
extern "C" CUresult DIOGENES_cuMemcpyHtoD(CUdeviceptr dst, void * src, size_t count);

gotcha_wrappee_handle_t DIOGENES_cudaFree_handle;
gotcha_wrappee_handle_t DIOGENES_cudaFreeHost_handle;
gotcha_wrappee_handle_t DIOGENES_cudaMalloc_handle;
gotcha_wrappee_handle_t DIOGENES_cudaMallocHost_handle;
gotcha_wrappee_handle_t DIOGENES_cudaMemcpy_handle;
gotcha_wrappee_handle_t DIOGENES_cudaMemcpyAsync_handle;
gotcha_wrappee_handle_t DIOGENES_cuMemFree_handle;
gotcha_wrappee_handle_t DIOGENES_cuMemFreeHost_handle;
gotcha_wrappee_handle_t DIOGENES_cuMemAlloc_handle;
gotcha_wrappee_handle_t DIOGENES_cuMemAllocHost_handle;
gotcha_wrappee_handle_t DIOGENES_cuMemcpyDtoH_handle;
gotcha_wrappee_handle_t DIOGENES_cuMemcpyHtoD_handle;


typeof(&DIOGENES_cudaFree) DIOGENES_cudaFree_wrapper;
typeof(&DIOGENES_cudaFreeHost) DIOGENES_cudaFreeHost_wrapper;
typeof(&DIOGENES_cudaMalloc) DIOGENES_cudaMalloc_wrapper;
typeof(&DIOGENES_cudaMallocHost) DIOGENES_cudaMallocHost_wrapper;
typeof(&DIOGENES_cudaMemcpy) DIOGENES_cudaMemcpy_wrapper;
typeof(&DIOGENES_cudaMemcpyAsync) DIOGENES_cudaMemcpyAsync_wrapper;
typeof(&DIOGENES_cuMemFree) DIOGENES_cuMemFree_wrapper;
typeof(&DIOGENES_cuMemFreeHost) DIOGENES_cuMemFreeHost_wrapper;
typeof(&DIOGENES_cuMemAlloc) DIOGENES_cuMemAlloc_wrapper;
typeof(&DIOGENES_cuMemAllocHost) DIOGENES_cuMemAllocHost_wrapper;
typeof(&DIOGENES_cuMemcpyDtoH) DIOGENES_cuMemcpyDtoH_wrapper;
typeof(&DIOGENES_cuMemcpyHtoD) DIOGENES_cuMemcpyHtoD_wrapper;
std::shared_ptr<RecursiveMap> DIOGENES_StackChecker;
std::shared_ptr<DataRecorderCountTool> DIOGENES_MemStatTool;
uint64_t matchCount = 0;


struct gotcha_binding_t DIOGNESE_gotfuncs[] = {{"cudaFree", (void*)DIOGENES_cudaFree,&DIOGENES_cudaFree_handle},
			{"cudaFreeHost", (void*)DIOGENES_cudaFreeHost,&DIOGENES_cudaFreeHost_handle},
			{"cudaMalloc", (void*)DIOGENES_cudaMalloc,&DIOGENES_cudaMalloc_handle},
			{"cudaMallocHost", (void*)DIOGENES_cudaMallocHost,&DIOGENES_cudaMallocHost_handle},
			{"cudaMemcpy", (void*)DIOGENES_cudaMemcpy,&DIOGENES_cudaMemcpy_handle},
			{"cudaMemcpyAsync", (void*)DIOGENES_cudaMemcpyAsync,&DIOGENES_cudaMemcpyAsync_handle},
			{"cuMemFree_v2", (void*)DIOGENES_cuMemFree,&DIOGENES_cuMemFree_handle},
			{"cuMemFreeHost", (void*)DIOGENES_cuMemFreeHost,&DIOGENES_cuMemFreeHost_handle},
			{"cuMemAlloc_v2", (void*)DIOGENES_cuMemAlloc,&DIOGENES_cuMemAlloc_handle},
			{"cuMemAllocHost_v2", (void*)DIOGENES_cuMemAllocHost,&DIOGENES_cuMemAllocHost_handle},
			{"cuMemcpyDtoH_v2", (void*)DIOGENES_cuMemcpyDtoH,&DIOGENES_cuMemcpyDtoH_handle},
			{"cuMemcpyHtoD_v2", (void*)DIOGENES_cuMemcpyHtoD,&DIOGENES_cuMemcpyHtoD_handle} };

bool __attribute__ ((noinline)) CheckStack(bool & htodlimit) {
	void * local_stack[75];
	int ret = backtrace(local_stack, 75);
	return DIOGENES_StackChecker->IterativeLookup((uint64_t*)local_stack, ret - 2, 2,htodlimit);
}	

bool __attribute__ ((noinline)) CheckStackInternal(bool & htodlimit) {
	void * local_stack[75];
	int ret = backtrace(local_stack, 75);
	return DIOGENES_StackChecker->IterativeLookup((uint64_t*)local_stack, ret - 2, 3, htodlimit);
}	

struct DevCached{
	void * cpuDst;
	void * gpuDst;
	size_t size;	

	int IsOverlap(void * compCpuDst, size_t compCpuSize) {
		if (compCpuDst < cpuDst)
			if (((uint64_t)compCpuDst)+compCpuSize > ((uint64_t)cpuDst))
				return -1;
		if (compCpuDst > cpuDst)
			if (((uint64_t)cpuDst) + size > (uint64_t)compCpuDst)
				return 1;
		return 0;
	};

};

struct CudaMemhostPageManager {
	std::unordered_multimap<size_t, void *> cachedPages;
	std::unordered_multimap<size_t, void *>::iterator _currentIt;

	std::unordered_map<void *, size_t> spoiledPages;
	std::unordered_map<void *, void *> copyPages;

	std::unordered_map<void *, void *> copyPagesRev;


	std::vector<DevCached> transfersActive;

	CudaMemhostPageManager()  {
		_currentIt = cachedPages.end();
	};
	~CudaMemhostPageManager() {
		DIOGENES_SHUTDOWN_MODE = true;
	};

	void * GetPinnedPage(size_t size) {
		auto it = cachedPages.find(size);
		if (it == cachedPages.end()) {
			void * tmp;
			if (CUDA_SUCCESS != cuMemAllocHost(&tmp,size))
				assert("CANNOT ALLOCATE PINNED HOST MEMORY" != 0);
			cachedPages.insert(std::make_pair(size, tmp));
			it = cachedPages.find(size);
		}
		_currentIt = it;
		return it->second;
	};

	void AtSync() {
		for(auto i : spoiledPages) {
			cachedPages.insert(std::make_pair(i.second,i.first));
			auto it = copyPages.find(i.first);
			if(it != copyPages.end()) {
				memcpy(it->second, i.first, i.second);
			}
		}
		//std::cerr << "Clearing pages" << std::endl;
		copyPagesRev.clear();
		spoiledPages.clear();
		transfersActive.clear();
		copyPages.clear();
	};

	bool IsCachedPages(void * dst, void ** ptr) {
		if(copyPagesRev.find(dst) == copyPagesRev.end())
			return false;

		*ptr = copyPagesRev[dst];
		return true;
	};
	void SpoilLastPage(bool copy, void * dst) {
		if(_currentIt != cachedPages.end()) {
			spoiledPages[_currentIt->second] = _currentIt->first;
			if(copy) {
				copyPages[_currentIt->second] = dst;
				//copyPagesRev[dst] = _currentIt->second;
			}
			cachedPages.erase(_currentIt);
			_currentIt = cachedPages.end();
		}
	};




	void SetDtoHMemcpyAddress(void * dst, void * gpuAddr, size_t size) {
		DevCached tmp;
		tmp.cpuDst = dst;
		tmp.gpuDst = gpuAddr;
		tmp.size = size;
		transfersActive.push_back(tmp);
	};

	bool IsOverlap(void * dst, size_t size) {
		for (int i = transfersActive.size() - 1;  i >= 0; i--) {
			if (transfersActive[i].IsOverlap(dst,size) != 0)
				return true;
		}
		return false;
	};
	// void IsOverlap(void * dst, size_t size, CUdeviceptr origDst, CUdeviceptr & sourceStart, CUdeviceptr & dstStart, 
	// 			   size_t & dtodsize, void * & cpuStart, CUdeviceptr & cpuDst, size_t & htodsize) {
	// 	cpuStart = dst;
	// 	htodsize = size;
	// 	cpuDst = origDst;

	// 	for (int i = transfersActive.size() - 1;  i >= 0; i--) {
	// 		int overlapId = transfersActive[i].IsOverlap(dst,size);
	// 		if (overlapId == 0)
	// 			continue;
	// 		if (transfersActive[i].IsOverlap(dst,size))	
	// 	}

	// }
	// void SpoilLastPageWSize(bool copy, void * dst, size_t size) {
	// 	if(_currentIt != cachedPages.end()) {
	// 		spoiledPages[_currentIt->second] = _currentIt->first;
	// 		if(copy) {
	// 			copyPages[_currentIt->second] = dst;
	// 			copyPagesRev[dst] = _currentIt->second;
	// 			sizeMap[dst] = size;
	// 		}
	// 		cachedPages.erase(_currentIt);
	// 		_currentIt = cachedPages.end();
	// 	}
	// };


};
struct CudaMemoryManager {
	std::unordered_multimap<size_t, void *> cachedPages;

	std::unordered_map<void *, size_t> spoiledPages;

	~CudaMemoryManager() {
		DIOGENES_SHUTDOWN_MODE = true;
	};
	void * GetMemoryPage(size_t size) {
		void * ret;
		auto it = cachedPages.find(size);
		if (it == cachedPages.end()) {
			if (cudaSuccess != DIOGENES_cudaMalloc_wrapper(&ret, size)) 
				assert("COULD NOT ALLOCATE MEMORY!!!!" != 0);
		} else {
			ret = it->second;
			cachedPages.erase(it);
		}
		spoiledPages[ret] = size;
		return ret;
	};

	bool FreeMemory(void * mem) {
		auto it = spoiledPages.find(mem);
		if (it == spoiledPages.end()){
			DIOGENES_cudaFree_wrapper(mem);
			return true;
		}
		cachedPages.insert(std::make_pair(it->second, mem));
		spoiledPages.erase(it);
		return false;

	};
};
struct PinnedPageManager {
	std::unordered_set<void *> cachedPages;
	~PinnedPageManager() {
		DIOGENES_SHUTDOWN_MODE = true;
	}
	void AddMallocHostPage(void * mem) {
		cachedPages.insert(mem);
	};

	bool IsManagedPage(void * mem) {
		auto it = cachedPages.find(mem);
		if (it != cachedPages.end())
			return true;
		return false;
	};

};

thread_local bool DIOGENES_IN_RUNTIME = false;


thread_local CudaMemhostPageManager * pageAllocator;
thread_local CudaMemoryManager * mallocManager;
thread_local PinnedPageManager * pinManage;
extern "C" {
	void DIOGENES_INIT_GOTCHA() {
		DIOGENES_StackChecker.reset(new RecursiveMap());
		DIOGENES_StackChecker->Init();
		DIOGENES_MemStatTool.reset(new DataRecorderCountTool());
		// make sure libcudart and libcuda are loaded into the binary.
		void * cudarthandle = dlopen("libcudart.so", RTLD_LAZY);
		void * libcudahandle = dlopen("libcuda.so", RTLD_LAZY);

		DIOGENES_cudaFree_wrapper = (typeof(&DIOGENES_cudaFree))dlsym(cudarthandle,"cudaFree");
		DIOGENES_cuMemFreeHost_wrapper = (typeof(&DIOGENES_cuMemFreeHost))dlsym(libcudahandle,"cuMemFreeHost");
		DIOGENES_cudaFreeHost_wrapper = (typeof(&DIOGENES_cudaFreeHost))dlsym(cudarthandle,"cudaFreeHost");
		DIOGENES_cudaMalloc_wrapper = (typeof(&DIOGENES_cudaMalloc))dlsym(cudarthandle,"cudaMalloc");
		DIOGENES_cudaMallocHost_wrapper = (typeof(&DIOGENES_cudaMallocHost))dlsym(cudarthandle,"cudaMallocHost");
		DIOGENES_cudaMemcpy_wrapper = (typeof(&DIOGENES_cudaMemcpy))dlsym(cudarthandle,"cudaMemcpy");
		DIOGENES_cudaMemcpyAsync_wrapper = (typeof(&DIOGENES_cudaMemcpyAsync))dlsym(cudarthandle,"cudaMemcpyAsync");
		DIOGENES_cuMemFree_wrapper = (typeof(&DIOGENES_cuMemFree))dlsym(libcudahandle,"cuMemFree_v2");
		DIOGENES_cuMemAlloc_wrapper = (typeof(&DIOGENES_cuMemAlloc))dlsym(libcudahandle,"cuMemAlloc_v2");
		DIOGENES_cuMemAllocHost_wrapper = (typeof(&DIOGENES_cuMemAllocHost))dlsym(libcudahandle,"cuMemAllocHost_v2");
		DIOGENES_cuMemcpyDtoH_wrapper = (typeof(&DIOGENES_cuMemcpyDtoH))dlsym(libcudahandle,"cuMemcpyDtoH_v2");
		DIOGENES_cuMemcpyHtoD_wrapper = (typeof(&DIOGENES_cuMemcpyHtoD))dlsym(libcudahandle,"cuMemcpyHtoD_v2");
		// Wrap functions
		gotcha_wrap(DIOGNESE_gotfuncs, sizeof(DIOGNESE_gotfuncs)/sizeof(struct gotcha_binding_t), "diogenes");
	}
//Runtime API Wrappers
	void DIOGENES_POSTSYNC_WRAPPER() {
		if (DIOGENES_SHUTDOWN_MODE)
			return;
		if(pageAllocator == NULL)
			pageAllocator = new CudaMemhostPageManager();
		if(pageAllocator != NULL)
			pageAllocator->AtSync();
	}
	cudaError_t DIOGENES_cudaFree(void * mem) {
		if (DIOGENES_SHUTDOWN_MODE)
			return cudaSuccess;
		if(mallocManager == NULL)
			mallocManager = new CudaMemoryManager();
		DIOGENES_IN_RUNTIME = true;
		bool htodlimit;
		DIOGENES_MemStatTool->AddFree();
		bool sync = mallocManager->FreeMemory(mem);
		if (!CheckStack(htodlimit) && !sync) {
			cudaDeviceSynchronize();			
		} else {
			DIOGENES_MemStatTool->FreeApplied();
		}
		DIOGENES_IN_RUNTIME = false;
		return cudaSuccess;
		//std::cerr << "In cuda free" << std::endl;
		//return DIOGENES_cudaFree_wrapper(mem);
	}
	cudaError_t DIOGENES_cudaFreeHost(void * mem) {
		//std::cerr << "In DIOGENES_cudaFreeHost free" << std::endl;
		return DIOGENES_cudaFreeHost_wrapper(mem);
	}
	cudaError_t DIOGENES_cudaMalloc(void ** mem, size_t size) {
		//std::cerr << "In DIOGENES_cudaMalloc free" << std::endl;
		if(mallocManager == NULL)
			mallocManager = new CudaMemoryManager();
		DIOGENES_IN_RUNTIME = true;
		*mem =  mallocManager->GetMemoryPage(size);
		DIOGENES_MemStatTool->AddMalloc();
		DIOGENES_IN_RUNTIME = false;
		return cudaSuccess;
		//return DIOGENES_cudaMalloc_wrapper(mem, size);

	}
	cudaError_t DIOGENES_cudaMallocHost(void ** mem, size_t size) {
		if (pinManage == NULL)
			pinManage = new PinnedPageManager();
		DIOGENES_IN_RUNTIME = true;
		//std::cerr << "In DIOGENES_cudaMallocHost free" << std::endl;		
		cudaError_t ret = DIOGENES_cudaMallocHost_wrapper(mem, size);
		if (ret == cudaSuccess)
			pinManage->AddMallocHostPage(*mem);
		DIOGENES_IN_RUNTIME = false;
		return ret;
		return DIOGENES_cudaMallocHost_wrapper(mem,size);

	}
	cudaError_t DIOGENES_cudaMemcpy(void * dst, void * src, size_t count, cudaMemcpyKind kind) {
		DIOGENES_IN_RUNTIME = true;
		if (pinManage == NULL)
			pinManage = new PinnedPageManager();
		if(pageAllocator == NULL)
			pageAllocator = new CudaMemhostPageManager();
		bool htodlimit;
		bool performOpt = CheckStack(htodlimit);
		void * stackAddr = NULL;
		if (kind == cudaMemcpyHostToDevice) {

			if (!(pinManage->IsManagedPage(src))){
				void * tmp = pageAllocator->GetPinnedPage(count);
				memcpy(tmp, src, count);
				if (performOpt)
					pageAllocator->SpoilLastPage(false, NULL);
				src = tmp;
			}
		} else if (kind == cudaMemcpyDeviceToHost) {
			if (!(pinManage->IsManagedPage(dst))){
				void * tmp = pageAllocator->GetPinnedPage(count);
				//if (performOpt)
				pageAllocator->SpoilLastPage(true, dst);
				performOpt = false;
				dst = tmp;				
			} else {
				performOpt = false;
			}
		} else {
			DIOGENES_IN_RUNTIME = false;
			return DIOGENES_cudaMemcpy_wrapper(dst, src, count, kind);
		}

		cudaError_t ret = DIOGENES_cudaMemcpyAsync_wrapper(dst, src, count, kind, 0);
		DIOGENES_MemStatTool->AddTrans();
		if (!performOpt)
			ret = cudaDeviceSynchronize();
		else 
			DIOGENES_MemStatTool->TransApplied();
		DIOGENES_IN_RUNTIME = false;
		//std::cerr << "In DIOGENES_cudaMemcpy free" << std::endl;		
		return ret;
		return DIOGENES_cudaMemcpy_wrapper(dst, src, count, kind);

	}
	cudaError_t DIOGENES_cudaMemcpyAsync(void * dst, void * src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
		DIOGENES_IN_RUNTIME = true;
		if (pinManage == NULL)
			pinManage = new PinnedPageManager();
		if(pageAllocator == NULL)
			pageAllocator = new CudaMemhostPageManager();
		bool htodlimit;
		bool performOpt = CheckStack(htodlimit);
		void * stackAddr = NULL;
		if (kind == cudaMemcpyHostToDevice) {
			//std::cerr << "DTOHTRANSFER!" << std::endl;
			performOpt = true;
			if (!(pinManage->IsManagedPage(src))){
				void * tmp = pageAllocator->GetPinnedPage(count);
				memcpy(tmp, src, count);
				if (performOpt)
					pageAllocator->SpoilLastPage(false, NULL);
				src = tmp;
			}
		} else if (kind == cudaMemcpyDeviceToHost) {
			//performOpt = false;
			// if (((void *)&stackAddr > dst)){
			// 	performOpt = false;
			// }
			if (htodlimit == true)
				performOpt = false;
			if (!(pinManage->IsManagedPage(dst))) {
				void * tmp = pageAllocator->GetPinnedPage(count);
				pageAllocator->SpoilLastPage(true, dst);
				dst = tmp;
			}

			// if (!(pinManage->IsManagedPage(dst)) && ((void *)&stackAddr < dst) && performOpt){
			// 	void * tmp = pageAllocator->GetPinnedPage(count);
			// 	//if (performOpt)
			// 	pageAllocator->SpoilLastPage(true, dst);
			// 	dst = tmp;		
			// 	performOpt = false;		
			// } else {
			// 	performOpt = false;
			// }
		} else {
			DIOGENES_IN_RUNTIME = false;
			return DIOGENES_cudaMemcpyAsync_wrapper(dst, src, count, kind, stream);
		}
		//std::cerr << "Args = " << std::hex << dst << "," << src << "," << count << "," << kind << "," << stream << std::endl;
		cudaError_t ret = DIOGENES_cudaMemcpyAsync_wrapper(dst, src, count, kind, stream);
		if (kind == cudaMemcpyDeviceToHost)
			DIOGENES_MemStatTool->AddTrans();
		//assert(ret == cudaSuccess);
		DIOGENES_IN_RUNTIME = false;
		if (!performOpt)
		 	ret = cudaDeviceSynchronize();
		else {
		 	DIOGENES_MemStatTool->TransApplied();
		}
		//std::cerr << "In DIOGENES_cudaMemcpyAsync free" << std::endl;	
		return ret;
	}

// Driver API Wrappers
	CUresult DIOGENES_cuMemFree(void * mem) {
		//std::cerr << "In DIOGENES_cuMemFree free" << std::endl;	
		return DIOGENES_cuMemFree_wrapper(mem);

	}
	CUresult DIOGENES_cuMemFreeHost(void * mem) {
		//std::cerr << "In DIOGENES_cuMemFreeHost free" << std::endl;	
		return DIOGENES_cuMemFreeHost_wrapper(mem);
	}
	CUresult DIOGENES_cuMemAlloc(void ** mem, size_t size) {
		//std::cerr << "In DIOGENES_cuMemAlloc free" << std::endl;
		return DIOGENES_cuMemAlloc_wrapper(mem, size);

	}
	CUresult DIOGENES_cuMemAllocHost(void ** mem, size_t size) {
		//std::cerr << "In DIOGENES_cuMemAllocHost free" << std::endl;
		return DIOGENES_cuMemAllocHost_wrapper(mem, size);

	}
	CUresult DIOGENES_cuMemcpyDtoH(void * dst, CUdeviceptr src, size_t count) {
		if (DIOGENES_SHUTDOWN_MODE)
			return DIOGENES_cuMemcpyDtoH_wrapper(dst, src, count);
		if (DIOGENES_IN_RUNTIME)
			return DIOGENES_cuMemcpyDtoH_wrapper(dst, src, count);

		//std::cerr << "DTOH - DST = " << std::hex << dst << " SIZE = " << std::dec << count << std::endl;
		void* stackAddr = NULL;
		void * tmp = NULL;
		bool IsManagedPage = false;
		bool htodlimit = false;
		if (pinManage == NULL)
			pinManage = new PinnedPageManager();

		if(pageAllocator == NULL)
			pageAllocator = new CudaMemhostPageManager();

		if(!(pinManage->IsManagedPage(dst))){
			tmp = pageAllocator->GetPinnedPage(count);
		} else {
			tmp = dst;
			IsManagedPage = true;
		}
		
		CUresult ret = cuMemcpyDtoHAsync(tmp, src, count, 0);
		DIOGENES_MemStatTool->AddTrans();
		if(CheckStackInternal(htodlimit)) {
			if (htodlimit == false) {
				if (!IsManagedPage)
					pageAllocator->SpoilLastPage(true, dst);
				DIOGENES_MemStatTool->TransApplied();
				pageAllocator->SetDtoHMemcpyAddress(dst,(void*)src, count);
//				cuCtxSynchronize();
				return ret;
			}
		}
		ret = cuCtxSynchronize();
		if (!IsManagedPage)
			memcpy(dst, tmp, count);

		return ret;
	}
	CUresult DIOGENES_cuMemcpyHtoD(CUdeviceptr dst, void * src, size_t count) {
		//cuCtxSynchronize();
		if (DIOGENES_SHUTDOWN_MODE)
			return DIOGENES_cuMemcpyHtoD_wrapper(dst, src, count);
		if (DIOGENES_IN_RUNTIME)
			return DIOGENES_cuMemcpyHtoD_wrapper(dst, src, count);

		if(pageAllocator == NULL)
			pageAllocator = new CudaMemhostPageManager();
		if (pinManage == NULL)
			pinManage = new PinnedPageManager();
		void * tmp = NULL;
		bool IsManagedPage = false;
		bool htodlimit = false;
		bool syncPerformed = false;
		//std::cerr << "HTOD - SRC = " << std::hex << src << " SIZE = " << std::dec << count << std::endl;
		bool checkInternal = CheckStackInternal(htodlimit);
		if (!checkInternal || pageAllocator->IsOverlap(src,count)){
		 	cuCtxSynchronize();
		 	syncPerformed = true;
		}
		if(!(pinManage->IsManagedPage(src))){
			//if(!pageAllocator->IsCachedPages(src,&tmp)){
				tmp = pageAllocator->GetPinnedPage(count);
				memcpy(tmp, src, count);
			//} else {
			//	IsManagedPage = true;
			//}
		} else {
			tmp = src;
			IsManagedPage = true;
		}

		CUresult ret = cuMemcpyHtoDAsync(dst, tmp, count, 0);
		DIOGENES_MemStatTool->AddTrans();
		if(checkInternal){
			if (!IsManagedPage)
				pageAllocator->SpoilLastPage(false, NULL);
			if (!syncPerformed)
			DIOGENES_MemStatTool->TransApplied();
			//cuCtxSynchronize();
			return ret;
		}
		if (ret != CUDA_SUCCESS)
			assert(ret == CUDA_SUCCESS);
		return CUDA_SUCCESS; //cuCtxSynchronize();
	}

}


// //extern "C" CUresult DIOGENES_cudaMemcpyAsync(void * dst, const void * src, size_t count, cudaMemcpyKind kind, cudaStream_t stream);


// extern "C" cudaError_t DIOGENES_CudaMallocHostWrapper(void ** mem, size_t size);



// "cudaMemcpy","cuMemcpyDtoH_v2","cuMemcpyHtoD_v2","cudaMemcpyAsync ","cudaFree","cudaMalloc","cuMemAlloc_v2","cuMemFree","cuMemAllocHost_v2", "cuMemFreeHost","cudaMallocHost"





// extern "C" void DIOGENES_FREEWrapper(void * mem);
// extern "C" cudaError_t DIOGENES_CudaFreeHostWrapper(void * mem);
// extern "C" cudaError_t DIOGENES_CudaMallocHostWrapper(void ** mem, size_t size);

// typeof(&DIOGENES_FREEWrapper) DIOGENES_LIBCFREE;

// typeof(&DIOGENES_CudaFreeHostWrapper) DIOGENES_CUDAFREEHOST;
// typeof(&DIOGENES_CudaMallocHostWrapper) DIOGENES_CUDAMALLOCHOST;


// gotcha_wrappee_handle_t DIOGENES_wrappee_free_handle;
// gotcha_wrappee_handle_t DIOGENES_wrappee_cudaFreeHost_handle;
// gotcha_wrappee_handle_t DIOGENES_wrappee_cudaMallocHost_handle;
// struct gotcha_binding_t DIOGNESE_gotfuncs[] = {
// 	{"cudaFreeHost",(void*)DIOGENES_CudaFreeHostWrapper,&DIOGENES_wrappee_cudaFreeHost_handle},
// 	{"cudaMallocHost",(void*)DIOGENES_CudaMallocHostWrapper,&DIOGENES_wrappee_cudaMallocHost_handle}};






// extern "C" {




// };



// std::atomic<bool> DIOGENES_Atomic_Malloc(false);
// volatile int DIOGENES_DEBUG_FREE_PASSED = 0;
// volatile int DIOGENES_DEBUG_MALLOC_PASSED = 0;

// volatile bool DIOGENES_MEMMANGE_TEAR_DOWN = false;
// volatile bool IN_INSTRIMENTATION = false;
// volatile void * DIOGENES_CURRENT_STREAM = NULL;

// enum DIOGLockTypes{IN_INIT = 0, IN_OP, IN_NONE, IN_FREECALL, IN_TEARDOWN};

// class DiogAtomicMutex {
// private:
// 	DIOGLockTypes _m;
// 	std::atomic<DIOGLockTypes> _freeLock;
// public:

// 	DiogAtomicMutex() : _m(IN_NONE), _freeLock(IN_NONE) {};
// 	~DiogAtomicMutex() {
// 		DIOGENES_MEMMANGE_TEAR_DOWN = true;
// 	};
// 	bool EnterOp() {
// 		if (_m == IN_NONE) {
// 			_m = IN_OP;
// 		} else if (_m == IN_OP || _m == IN_INIT || _m == IN_TEARDOWN) {
// 			return false;
// 		}
// 		return true;
// 	};

// 	void ExitOp() {
// 		_m = IN_NONE;
// 	};

// 	bool EnterFree() {
// 		DIOGLockTypes lt = IN_FREECALL;
// 		if(_freeLock.compare_exchange_weak(lt, IN_NONE)) {
// 			return true;
// 		}
// 		return false;
// 	};

// 	void ExitFree() {
// 		_freeLock.exchange(IN_NONE);
// 	};

// 	void EnterInit() {
// 		_m = IN_INIT;
// 	};

// 	void ExitInit() {
// 		_m = IN_NONE;
// 	};
// };


// //#define MEMMANAGE_DEBUG 1
// class MemStats {
// public:
// 	MemStats(std::string name);
// 	~MemStats();
// 	void AllocatedMemory(uint64_t size);
// 	void FreedMemory(uint64_t size);
// 	void UsedCache(size_t size);
// 	void TransferAddrKnown(uint64_t addr, bool known);
// private:
// 	std::map<uint64_t, uint64_t> _sizes;
// 	std::map<uint64_t, uint64_t> _transAddresses;
// 	uint64_t _current;
// 	uint64_t _max;
// 	uint64_t _allocatedCount;
// 	uint64_t _freedCount;
// 	uint64_t _knownTransferAddrs;
// 	uint64_t _unknownTransferAddrs;
// 	std::string _type;
// 	uint64_t _cacheHits;
// };


// class MemManage {
// public:
// 	MemManage();
// 	~MemManage();
// 	cudaError_t GPUAllocate(void ** mem, uint64_t size);
// 	cudaError_t GPUFree(void * mem, bool & sync);
// 	void * CPUAllocate(uint64_t size);
// 	void CheckDestTransMem(void * mem);
// 	void CPUFree(void * mem);

// 	void * MallocPinMim(uint64_t size, void * origLoc);
// 	void ReturnPinMem();
// private:
// 	std::map<uint64_t, uint64_t> _gpuMem;
// 	std::map<uint64_t, std::vector<void *> > _gpuMemSize;
// 	std::map<uint64_t, uint64_t> _cpuMem;
// 	std::shared_ptr<MemStats> _cpuStats;
// 	std::shared_ptr<MemStats> _gpuStats;

// 	std::map<uint64_t, std::vector<void *> > _cpuPinnedPages;
// 	std::map<uint64_t, std::pair<uint64_t, void *> > _cpuPinMem;
// 	//
// };

// MemStats::MemStats(std::string name) :  _current(0), _max(0), _allocatedCount(0), _freedCount(0), 
// 										_type(name), _cacheHits(0), _knownTransferAddrs(0),
// 										_unknownTransferAddrs(0) {}

// MemStats::~MemStats() {
// 	std::stringstream ss;
// 	ss << "==============================================================" << std::endl;
// 	ss << "Allocator Type: " << _type << std::endl;
// 	ss << "Alloc Count: " << _allocatedCount << " Free Count: " << _freedCount << std::endl;
// 	ss << "Max Allocated: " << _max << std::endl;
// 	ss << "Known Transfer Addr: " << _knownTransferAddrs << " Unknown Trans Addr: " << _unknownTransferAddrs << std::endl;
// 	ss << "Cache Hits: " << _cacheHits << std::endl;
// 	ss << "Sizes Allocated... " << std::endl;

// 	for (auto i : _sizes) {
// 		ss << i.first << " " << i.second << std::endl;
// 	}

// 	ss << "Transfer Addresses.... " << std::endl;
// 	for (auto i : _transAddresses)
// 		ss << std::hex << i.first << " " << std::dec << i.second << std::endl;
// 	//std::cerr << ss.str();
// }

// void MemStats::UsedCache(size_t size) {
// #ifdef MEMMANAGE_DEBUG
// 	std::cout << "[MemStats::UsedCache] Cached memory range used for alloc of size - " << size << std::endl;
// #endif
// 	_cacheHits++;
// }


// void MemStats::TransferAddrKnown(uint64_t addr, bool known) {
// 	if (known)
// 		_knownTransferAddrs++;
// 	else
// 		_unknownTransferAddrs++;
// 	if (_transAddresses.find(addr) == _transAddresses.end()){
// 		_transAddresses[addr] = 1;
// 	} else {
// 		_transAddresses[addr] += 1;
// 	}
// }
// void MemStats::AllocatedMemory(uint64_t size) {
// #ifdef MEMMANAGE_DEBUG
// 	std::cout << "[MemStats::AllocatedMemory] Allocating Memory of size - " << size << std::endl;
// #endif
// 	if (_sizes.find(size) == _sizes.end())
// 		_sizes[size] = 1;
// 	else
// 		_sizes[size] += 1;
// 	_current += size;
// 	if (_current > _max)
// 		_max = _current;
// 	_allocatedCount++;
// }

// void MemStats::FreedMemory(uint64_t size) {
// #ifdef MEMMANAGE_DEBUG
// 	std::cout << "[MemStats::FreedMemory] Freeing Memory of size - " << size << std::endl;
// #endif
// 	_freedCount += 1;
// 	if (_current > size) 
// 		_current -= size;
// 	else
// 		//std::cerr << "[MemStats::FreedMemory] We have a free from a memory location we don't know" << std::endl;
// }


// MemManage::MemManage() :  _cpuStats(new MemStats(std::string("CPU"))), _gpuStats(new MemStats(std::string("GPU"))) {
// 	// Allocate if we must...
// 	/*for (int i = 0; i < 30; i++) {
// 		void ** mem;
// 		size_t size = 80000000;
// 		cudaError_t tmp = cudaMalloc(mem, size);
// 		if (tmp == cudaSuccess){
// 			_gpuStats->AllocatedMemory(size);
// 			_gpuMem[*((uint64_t*)mem)] = size;	
// 			if (_gpuMemSize.find(size) == _gpuMemSize.end())
// 				_gpuMemSize[size] = std::vector<void *>();
// 			_gpuMemSize[size].push_back(*mem);
// 		}

// 	}*/
// }

// bool FindFreeMemory(std::map<uint64_t, std::vector<void *> >  & memRegions, void ** mem, uint64_t size) {
// 	/*auto itpair =  memRegions.equal_range(size);
// 	if (itpair.first != memRegions.end() && itpair.first->first == size) {
// 		if(itpair.first->second.size() > 0) {
// 			*mem = itpair.first->second.back();
// 			itpair.first->second.pop_back();
// 			return true;
// 		}
// 	}
// 	if (itpair.second != memRegions.end())
// 		if (itpair.second->first > size) {
// 			if (itpair.second->second.size() > 0) {
// 				*mem = itpair.second->second.back();
// 				itpair.second->second.pop_back();
// 				return true;				
// 			}
// 		}*/
// 	if(memRegions.find(size) != memRegions.end())
// 		if (memRegions[size].size() > 0){
// 			*mem = (memRegions[size].back());
// 			memRegions[size].pop_back();
// 			return true;
// 		}
	
// 	return false;
// }

// void MemManage::ReturnPinMem() {
// 	for (auto i : _cpuPinMem) {
// 		if(_cpuPinnedPages.find(i.second.first) == _cpuPinnedPages.end())
// 			_cpuPinnedPages[i.second.first] = std::vector<void *>();
// 		std::memcpy(i.second.second, (void*)i.first, i.second.first);
// 		_cpuPinnedPages[i.second.first].push_back((void*)i.first);
// 	}
// 	_cpuPinMem.clear();
// }

// void * MemManage::MallocPinMim(uint64_t size, void * orig) {
// 	void * tmp = NULL;

// 	if (FindFreeMemory(_cpuPinnedPages, &tmp, size)) {
// 		_cpuPinMem[(uint64_t)tmp] = std::make_pair(size, orig);
// 		return tmp;
// 	} else {
// 		if (cudaMallocHost(&tmp, size_t(size)) == cudaSuccess){
// 			_cpuPinMem[(uint64_t)tmp] = std::make_pair(size, orig);
// 			return tmp;
// 		}
// 	}

// 	//std::cerr << "We cannot allocate via cudaMallocHost, using original address" << std::endl;
// 	return orig;
// }

// cudaError_t MemManage::GPUAllocate (void** mem, uint64_t size) {
// 	#ifdef MEMMANAGE_DEBUG
// 		std::cout << "[MemManage::GPUAllocate] Entering GPU allocator to get size of " << size << std::endl;
// 	#endif
// 	if (FindFreeMemory(_gpuMemSize, mem, size)) {
// 		#ifdef MEMMANAGE_DEBUG
// 			std::cout << "[MemManage::GPUAllocate] Using a cached address " << std::endl;
// 		#endif
// 		_gpuStats->UsedCache(size);
// 		return cudaSuccess;
// 	}
// 	#ifdef MEMMANAGE_DEBUG
// 		std::cout << "[MemManage::GPUAllocate] Allocating a new GPU memory address " << std::endl;
// 	#endif	
// 	// Allocate if we must...
// 	cudaError_t tmp = cudaMalloc(mem, size);
// 	if (tmp == cudaSuccess){
// 		_gpuStats->AllocatedMemory(size);
// 		if (_gpuMemSize.find(size) == _gpuMemSize.end())
// 			_gpuMemSize[size] = std::vector<void *>();
// 		_gpuMem[*((uint64_t*)mem)] = size;
// 	}
// 	return tmp;
// }

// cudaError_t MemManage::GPUFree(void * mem, bool & sync){
// 	if (_gpuMem.find((uint64_t)mem) != _gpuMem.end()) {
// 		_gpuMemSize[_gpuMem[(uint64_t)mem]].push_back(mem);
// 		sync = false;
// 		return cudaSuccess;
// 	} 
// 	sync = true;
// 	// Free the memory, we have no idea what this is.
// 	//std::cerr << "Freeing Memory we don't know where it was allocated" << std::endl;
// 	return cudaFree(mem);
// }

// void * MemManage::CPUAllocate(uint64_t size) {
// 	void * tmp = malloc(size);
// 	_cpuMem[(uint64_t) tmp] = size;
// 	_cpuStats->AllocatedMemory(size);
// 	return tmp;
// }

// void MemManage::CPUFree(void * mem) {
// 	if (_cpuMem.find((uint64_t) mem) != _cpuMem.end()) {
// 		_cpuStats->FreedMemory(_cpuMem[(uint64_t)mem]);
// 		_cpuMem.erase((uint64_t)mem);
// 	} else {
// 		std::cerr << "[MemManage::CPUFree] We have no idea what this memory address is... " << std::hex << mem << std::endl;
// 	}
// 	free(mem);
// }

// uint64_t Diogenes_FindMemKey(std::map<uint64_t,uint64_t> & m, uint64_t addr) {
// 	std::map<uint64_t, uint64_t>::iterator lb = m.lower_bound(addr);
// 	if (lb == m.end())
// 		return 0;
// 	if (lb->first + lb->second >= addr)
// 		return lb->first;
// 	return 0;
// }

// void MemManage::CheckDestTransMem(void * mem) {
// 	uint64_t key = Diogenes_FindMemKey(_cpuMem, (uint64_t) mem);
// 	if (key == 0)
// 		_cpuStats->TransferAddrKnown((uint64_t) mem, false);
// 	else 
// 		_cpuStats->TransferAddrKnown((uint64_t) mem, true);
// 		//std::cerr << "[MemManage::CheckDestTransMem] We do not know about this CPU address in memory transfer - " << std::hex << (uint64_t) mem << std::endl;
// }


// MemManage::~MemManage() {
// 	DIOGENES_Atomic_Malloc.exchange(true);
// }
// std::shared_ptr<MemManage> DIOGENES_MEMORY_MANAGER;
// std::shared_ptr<DiogAtomicMutex> DIOGENES_MUTEX_MANAGER = NULL;
// pthread_t DIOG_PROC_TID = 0;
// // class DiogenesMemLockExchangeReset {
// // 	bool _original;
// // 	std::atomic<bool> * _lock;
// // public:
// // 	DiogenesMemLockExchangeReset(std::atomic<bool> * lock) : _lock(lock), _original(lock->exchange(true)) { };
// // 	~DiogenesMemLockExchangeReset() {_lock->exchange(_original)};
// // };

// thread_local pthread_t L_PID = 0;
// pthread_t gettid() {
// 	if (L_PID == 0)
// 		L_PID = pthread_self();
// 	return L_PID;
// }

// #define PLUG_BUILD_FACTORY(param) \
// 	if (DIOGENES_MUTEX_MANAGER == NULL && DIOGENES_MEMMANGE_TEAR_DOWN == false) {\
// 		DIOGENES_MEMMANGE_TEAR_DOWN = true; \
// 		DIOG_PROC_TID = gettid();\
// 		DIOGENES_MUTEX_MANAGER.reset(new DiogAtomicMutex()); \
// 		DIOGENES_MEMMANGE_TEAR_DOWN = false; \
// 	} \
// 	if (DIOGENES_TRANSFER_MEMMANGE.get() == NULL && DIOGENES_MEMMANGE_TEAR_DOWN == false) { \
// 		DIOGENES_MEMMANGE_TEAR_DOWN = true; \
// 		DIOGENES_MUTEX_MANAGER->EnterInit(); \
// 		DIOGENES_TRANSFER_MEMMANGE.reset(new TransferMemoryManager()); \
// 		DIOGENES_MEMORY_MANAGER.reset(new MemManage(param)); \
// 		DIOGENES_MUTEX_MANAGER->ExitInit(); \
// 		DIOGENES_MEMMANGE_TEAR_DOWN = false; \
// 	} 

// #define PLUG_FACTORY_PTR DIOGENES_MEMORY_MANAGER.get()


// #define NUM_GOTFUNCS 2

// extern "C" void DIOGENES_FREEWrapper(void * mem);
// extern "C" cudaError_t DIOGENES_CudaFreeHostWrapper(void * mem);
// extern "C" cudaError_t DIOGENES_CudaMallocHostWrapper(void ** mem, size_t size);

// typeof(&DIOGENES_FREEWrapper) DIOGENES_LIBCFREE;

// typeof(&DIOGENES_CudaFreeHostWrapper) DIOGENES_CUDAFREEHOST;
// typeof(&DIOGENES_CudaMallocHostWrapper) DIOGENES_CUDAMALLOCHOST;


// gotcha_wrappee_handle_t DIOGENES_wrappee_free_handle;
// gotcha_wrappee_handle_t DIOGENES_wrappee_cudaFreeHost_handle;
// gotcha_wrappee_handle_t DIOGENES_wrappee_cudaMallocHost_handle;
// struct gotcha_binding_t DIOGNESE_gotfuncs[] = {
// 	{"cudaFreeHost",(void*)DIOGENES_CudaFreeHostWrapper,&DIOGENES_wrappee_cudaFreeHost_handle},
// 	{"cudaMallocHost",(void*)DIOGENES_CudaMallocHostWrapper,&DIOGENES_wrappee_cudaMallocHost_handle}};


// class NoFreeVector {
// private:
// 	uint64_t * _storage;
// 	int _size;

// public:
// 	NoFreeVector() : _storage((uint64_t*)malloc(10000*sizeof(uint64_t))), _size(0) { };

// 	~NoFreeVector() {};
// 	int size() {
// 		return _size;
// 	};
// 	void * back() {
// 		if (_size > 0)
// 			return (void*)(_storage[_size - 1]);
// 		return NULL;
// 	};

// 	void pop_back() {
// 		if (_size > 0)
// 			_size--;
// 	};

// 	void push_back(void * m){ 
// 		if (_size < 10000){
// 			_storage[_size] = (uint64_t)m;
// 			_size++;
// 		} else {
// 			assert("WE SHOULD NOT BE HERE!" == 0);
// 		}
// 	};
// };

// class MemAllocatorManager {
// private:
// 	std::map<size_t, NoFreeVector *> _memRanges;
// 	std::map<uint64_t, size_t> _MemAddrToSize;
// 	std::map<uint64_t, size_t> _appInitCudaManaged;
// 	void * InternalAllocate(size_t size) {
// 		void * mem;
// 		if(cudaMallocHost(&mem,size) != cudaSuccess)
// 			assert("Could not malloc anything with cudaMallocHost!" != 0);
// 		_MemAddrToSize[(uint64_t)mem] = size;
// 		auto it = _memRanges.find((uint64_t)size);
// 		if(it == _memRanges.end())
// 			_memRanges[(uint64_t)size] = new NoFreeVector();
// 		return mem;
// 	};

// public:
// 	MemAllocatorManager() {};
// 	void * AllocateMemory(size_t size) {
// 		auto it = _memRanges.find((uint64_t)size);
// 		if (it == _memRanges.end())
// 			return InternalAllocate(size);
// 		if (it->second->size() > 0) {
// 			void * ret = it->second->back();
// 			it->second->pop_back();
// 			return ret;
// 		}
// 		return InternalAllocate(size);
// 	};

// 	void AddManagedMemory(void * mem, size_t size) {
// 		_appInitCudaManaged[(uint64_t)mem] = size;
// 	}

// 	void FreeManagedMemory(void * mem) {
// 		auto it = _appInitCudaManaged.find((uint64_t)mem);
// 		if (it != _appInitCudaManaged.end())
// 			_appInitCudaManaged.erase(it);
// 	}


// 	bool IsManagedMemory(void * mem) {
// 		auto it = _appInitCudaManaged.find((uint64_t)mem);
// 		if (it != _appInitCudaManaged.end() && it->first <= (uint64_t)mem && it->first + it->second >= (uint64_t) mem)
// 			return true;
// 		return false;
// 	}

// 	bool IsOurAllocation(void * mem) {
// 		auto it = _MemAddrToSize.lower_bound((uint64_t)mem);
// 		if (it == _MemAddrToSize.end())
// 			return false;
// 		if (it->first <= (uint64_t)mem && it->first + it->second >= (uint64_t)mem)
// 			return true;
// 		return false;
// 	}
// 	bool ReleaseMemory(void * mem) {
// 		auto it = _MemAddrToSize.find((uint64_t)mem);
// 		if (it == _MemAddrToSize.end())
// 			return false;
// 		_memRanges[it->second]->push_back(mem);
// 		return true;
// 	};
// };

// inline cudaStream_t ConvertInternalCUStream(volatile void * inStream) {
// 	return (cudaStream_t)(((char*)inStream) - 136);
// };

// inline cudaStream_t ConvertUserToInternalCUStream(cudaStream_t inStream)  {
// 	if (inStream == NULL)
// 		return NULL;
// 	return *((cudaStream_t*)inStream);
// };

// struct  DelayedTransferCopy {
// 	uint64_t size;
// 	void * processAddress;
// 	void * diogenesTemp;
// 	cudaStream_t stream;
// 	DelayedTransferCopy(void * _processAddress, void * _diogenesTemp, uint64_t _size, cudaStream_t _stream) : processAddress(_processAddress), 
// 						diogenesTemp(_diogenesTemp), size(_size), stream(_stream) {};

// 	inline void CopyTempToPro() {
// 		memcpy(processAddress, diogenesTemp, size);
// 	};

// 	void * GetTempAddress() {
// 		return diogenesTemp;
// 	};
// };

// class TransferMemoryStreamTracker {
// private:
// 	std::map<cudaStream_t, uint64_t> _streamsSeen;
// public:
// 	TransferMemoryStreamTracker() {};

// 	void SeenSynchronization(cudaStream_t stream) {
// 		auto it = _streamsSeen.find(stream);
// 		if (it == _streamsSeen.end()) {
// 			_streamsSeen[stream] = 0;
// 			it = _streamsSeen.find(stream);
// 		}
// 		it->second++;
// 	};

// 	std::string Print(cudaStream_t ctxStream, cudaStream_t defaultStream){
// 		std::stringstream ss;
// 		ss << "[TransferMemoryStreamTracker::StreamsSeen] Streams seen during execution " << std::endl;
// 		ss << "[TransferMemoryStreamTracker::StreamsSeen] CTX stream = " << std::hex << (uint64_t)ctxStream << std::endl;
// 		ss << "[TransferMemoryStreamTracker::StreamsSeen] Default stream = " << std::hex << (uint64_t)defaultStream << std::endl;
// 		for (auto i : _streamsSeen) {
// 			ss << "[TransferMemoryStreamTracker::StreamsSeen] " << std::hex << (uint64_t)i.first << " Sync Calls Seen = " << std::dec << i.second << std::endl;
// 		}
// 		return ss.str();
// 	};
// };


// class TransferMemoryManager {
// private:
// 	std::mutex mtx;
// 	std::shared_ptr<MemAllocatorManager> _MemAlloc;
// 	std::map<cudaStream_t, std::vector<std::shared_ptr<DelayedTransferCopy>>> _delayedCopies;
// 	TransferMemoryStreamTracker _streamsSeen;
// 	bool _initStreams;
// 	// hack for ctx synchronization
// 	cudaStream_t _ctxSynchronize;
//  	cudaStream_t _defaultStream;

//  	uint64_t _initTransferCallCount;
//  	void FindDefaultStream(){
//  		_defaultStream = NULL;
//  		_ctxSynchronize = (cudaStream_t)(0x999);

// 		// void * host = malloc(1024);
// 		// void * dst;
// 		// int attempts = 0;
// 		// DIOGENES_CURRENT_STREAM = NULL;
// 		// assert(cudaMalloc(&dst, 1024) == cudaSuccess);
// 		// while (DIOGENES_CURRENT_STREAM == NULL && attempts < 10) {
// 		// 	cudaMemcpyAsync(dst, host, 1024, cudaMemcpyHostToDevice, 0);
// 		// 	DIOGENES_CURRENT_STREAM = NULL;
// 		// 	cudaStreamSynchronize(0);
// 		// 	attempts++;
// 		// }	
// 		// assert(DIOGENES_CURRENT_STREAM != NULL);
// 		// _defaultStream = ConvertInternalCUStream(DIOGENES_CURRENT_STREAM);
// 		// _initStreams = true;
// 		// DIOGENES_LIBCFREE(host);
// 		// cudaFree(dst);
//  	};

// 	void FindCudaDeviceSynchronization() {
// 		// DIOGENES_CURRENT_STREAM = NULL;
// 		// cudaDeviceSynchronize();
// 		// if (DIOGENES_CURRENT_STREAM != NULL) {
// 		// 	_ctxSynchronize = ConvertInternalCUStream(DIOGENES_CURRENT_STREAM);
// 		// 	_initStreams = true;
// 		// 	DIOGENES_CURRENT_STREAM = NULL;
// 		// 	return;
// 		// }
// 		// // Perform a small transfer to get this address...
// 		// void * host = malloc(1024);
// 		// void * dst;
// 		// assert(cudaMalloc(&dst, 1024) == cudaSuccess);
// 		// assert(cudaMemcpyAsync(dst, host, 1024, cudaMemcpyHostToDevice, 0) == cudaSuccess);
// 		// DIOGENES_CURRENT_STREAM = NULL;
// 		// cudaDeviceSynchronize();
// 		// assert(DIOGENES_CURRENT_STREAM != NULL);
// 		// _ctxSynchronize = ConvertInternalCUStream(DIOGENES_CURRENT_STREAM);
// 		// _initStreams = true;
// 		// DIOGENES_LIBCFREE(host);
// 		// cudaFree(dst);
// 		// DIOGENES_CURRENT_STREAM = NULL;
// 	};
// public:

// 	TransferMemoryManager() : _MemAlloc(new MemAllocatorManager()), _initStreams(false), _defaultStream(NULL),_ctxSynchronize(NULL), _initTransferCallCount(0) {
// 		void * handle = dlopen("libcudart.so", RTLD_LAZY);
// 		assert(handle != NULL);
// 		DIOGENES_CUDAMALLOCHOST = (typeof(&DIOGENES_CudaMallocHostWrapper)) dlsym(handle, "cudaMallocHost");
// 		DIOGENES_CUDAFREEHOST = (typeof(&DIOGENES_CudaFreeHostWrapper)) dlsym(handle, "cudaFreeHost");

// 		assert(DIOGENES_CUDAMALLOCHOST != NULL);
// 		assert(DIOGENES_CUDAFREEHOST != NULL);

// 		gotcha_wrap(DIOGNESE_gotfuncs, sizeof(DIOGNESE_gotfuncs)/sizeof(struct gotcha_binding_t), "diogenes");

// 		//DIOGENES_LIBCFREE = (typeof(&DIOGENES_FREEWrapper)) gotcha_get_wrappee(DIOGENES_wrappee_free_handle);
// 		//assert(DIOGENES_LIBCFREE != NULL);
// 	};
// 	~TransferMemoryManager() { 
// 		DIOGENES_MEMMANGE_TEAR_DOWN = true;
// 		std::cerr << "Init transfer call count = " << _initTransferCallCount << std::endl;
// 		std::cerr << _streamsSeen.Print(_ctxSynchronize, _defaultStream) << std::endl;
// 	};

// 	void MallocManaged(void * mem, size_t size) {
// 		_MemAlloc->AddManagedMemory(mem, size);
// 	}

// 	void FreeManaged(void * mem) {
// 		_MemAlloc->FreeManagedMemory(mem);
// 	}

// 	void * MallocMemory(size_t size) {
// 		//mtx.lock();
// 		void * ret = _MemAlloc->AllocateMemory(size);
// 		//mtx.unlock();
// 		return ret;
// 	};

// 	void ReleaseMemory(void * mem) {
// 		//mtx.lock();
// 		if (_MemAlloc->ReleaseMemory(mem) == false) 
// 			DIOGENES_LIBCFREE(mem);
// 		//mtx.unlock();
// 	};

// 	template <typename T, typename D> 
// 	void AddToMapVector(T key, D & value, std::map<T,std::vector<D>> & mmap) {
// 		auto it = mmap.find(key);
// 		if (it == mmap.end()) {
// 			mmap[key] = std::vector<D>();
// 			it = mmap.find(key);
// 		}
// 		it->second.push_back(value);
// 	};


// 	void * InitiateTransfer(void * dst, size_t size, cudaStream_t stream) {
// 		if (_initStreams == false){
// 			FindCudaDeviceSynchronization();
// 			FindDefaultStream();
// 			_initStreams = true;
// 		}
// 		if (stream == NULL || ((uint64_t)stream) == 1)
// 			stream = _defaultStream;
// 		else 
// 			stream = ConvertUserToInternalCUStream(stream);
// 		//_streamsSeen.SeenSynchronization(stream);
// 		//return dst;
// 		// if (stream == 0)
// 		// 	stream = _defaultStream;
// 		// else {
// 		// 	stream = ConvertUserToInternalCUStream(stream);
// 		// }
// 		_initTransferCallCount++;
// 		if (_MemAlloc->IsManagedMemory(dst) == false && _MemAlloc->IsOurAllocation(dst) == false) {
// 			std::shared_ptr<DelayedTransferCopy> tmp(new DelayedTransferCopy(dst, _MemAlloc->AllocateMemory(size), size, stream));
// 			AddToMapVector<cudaStream_t, std::shared_ptr<DelayedTransferCopy>>(stream, tmp, _delayedCopies);
// 			return tmp->GetTempAddress();
// 		}
// 	    return dst;
// 	};


// 	void PerformSynchronizationAction(cudaStream_t stream) {
// 		//volatile void * local = DIOGENES_CURRENT_STREAM;
// 		cudaStream_t myStream = stream;
// 		if (myStream == NULL || ((uint64_t) myStream) == 1) {
// 			myStream = _defaultStream;
// 		} else if (((uint64_t)myStream) == 0x999) {
// 			myStream = _ctxSynchronize;
// 		} else {
// 			myStream = ConvertUserToInternalCUStream(myStream);
// 		}
// 		//cudaStream_t myStream = ConvertInternalCUStream(local);
// 		_streamsSeen.SeenSynchronization(myStream);
// 		if (myStream == _ctxSynchronize) {
// 			// Synchronize everything
// 			for (auto & i : _delayedCopies) {
// 				for(auto n : i.second) {
// 					n->CopyTempToPro();
// 					ReleaseMemory(n->GetTempAddress());
// 				}
// 				i.second.clear();
// 			}
// 		} else {
// 			auto it = _delayedCopies.find(myStream);
// 			if (it != _delayedCopies.end()) {
// 				for (auto n : it->second) {
// 					n->CopyTempToPro();
// 					ReleaseMemory(n->GetTempAddress());
// 				}
// 				it->second.clear();
// 			}
// 		}
	
// 	};
// };

// std::shared_ptr<TransferMemoryManager> DIOGENES_TRANSFER_MEMMANGE;


// extern "C" {

// cudaError_t DIOGENES_CudaMallocHostWrapper(void ** mem, size_t size) {
// 	cudaError_t ret;
// 	if (DIOGENES_CUDAMALLOCHOST != NULL)
// 		ret = DIOGENES_CUDAMALLOCHOST(mem, size);
// 	else 
// 		assert("WE SHOULD NOT BE HERE! " == 0);

// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false && ret == cudaSuccess) {
// 		PLUG_BUILD_FACTORY()
// 		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
// 			DIOGENES_TRANSFER_MEMMANGE->MallocManaged(*mem, size);
// 		}
// 		DIOGENES_MUTEX_MANAGER->ExitOp();	
// 	}
// 	return ret;
// }


// cudaError_t DIOGENES_CudaFreeHostWrapper(void * mem) {
// 	cudaError_t ret;
// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
// 			DIOGENES_TRANSFER_MEMMANGE->FreeManaged(mem);
// 		}
// 		DIOGENES_MUTEX_MANAGER->ExitOp();
// 	}
// 	if (DIOGENES_CUDAFREEHOST != NULL)
// 		return DIOGENES_CUDAFREEHOST(mem);
// 	else 
// 		assert("WE SHOULD NOT BE HERE" == 0);
// 	return cudaSuccess;

// }
// cudaError_t  DIOGENES_cudaFreeWrapper(void * mem) {
// 	cudaError_t ret;
// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
// 			bool sync;
// 			ret = PLUG_FACTORY_PTR->GPUFree(mem, sync);
// 			DIOGENES_MUTEX_MANAGER->ExitOp();
// 		} else {
// 			assert( "WE SHOULD NOT BE HERE!" == 0);
// 		}
// 		return ret;
// 	} 
// 	return cudaSuccess;
// 	// //fprintf(stderr,"I am freeing an address of %p \n", mem);
// 	// return cudaFree(mem);
// }

// cudaError_t  DIOGENES_synchronousCudaFree(void * mem) {
// 	// bool original = DIOGENES_Atomic_Malloc.exchange(true); 
// 	// if (IN_INSTRIMENTATION == false) {
// 	// 	IN_INSTRIMENTATION = true;
// 	cudaError_t ret;
// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
// 			bool sync;
// 			ret = PLUG_FACTORY_PTR->GPUFree(mem, sync);
// 			if (sync == false)
// 				cudaDeviceSynchronize();
// 			DIOGENES_MUTEX_MANAGER->ExitOp();
// 		} else {
// 			assert("WE SHOULD NOT BE HERE!" == 0);
// 		}
// 		return ret;
// 	}
// 	return cudaSuccess;
// 	// //fprintf(stderr,"I am freeing an address of %p \n", mem);
// 	// return cudaFree(mem);
// }


// cudaError_t DIOGENES_cudaMallocWrapper(void ** mem, size_t size) {
// 	cudaError_t  ret;
// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
// 		    ret = PLUG_FACTORY_PTR->GPUAllocate(mem, uint64_t(size));
// 			DIOGENES_MUTEX_MANAGER->ExitOp();
// 		} else {
// 			assert("WE SHOULD NOT BE HERE!" == 0);
// 		}
// 	} else {
// 		assert("WE SHOULD NOT BE HERE!" == 0);
// 	}
// 	return ret;
// 	// cudaError_t tmp = cudaMalloc(mem, size);
// 	// if (tmp == cudaSuccess)
// 	// 	std::cerr << "I alloced an address at " << std::hex << *((uint64_t**)mem)  <<  " of size " << size << std::endl;
// 	// return tmp;
// }



// // void DIOGENES_SUB_333898PRECALL(void * param2) {
// // 	PLUG_BUILD_FACTORY()
// // 	DIOGENES_CURRENT_STREAM = param2;
// // };


// void DIOGENES_SIGNALSYNC(cudaStream_t stream) {
// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		DIOGENES_TRANSFER_MEMMANGE->PerformSynchronizationAction(stream);
// 	}
// };

// cudaError_t DIOGENES_cudaMemcpyAsyncWrapper(void * dst, const void * src, size_t size, cudaMemcpyKind kind, cudaStream_t stream) {
// 	bool original = DIOGENES_Atomic_Malloc.exchange(true); 
// 	if (kind == cudaMemcpyDeviceToHost && DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		dst = DIOGENES_TRANSFER_MEMMANGE->InitiateTransfer(dst, size, stream);
// 		//void * mem = PLUG_FACTORY_PTR->MallocPinMim(size, dst);
// 		//st = mem;
// 		//PLUG_FACTORY_PTR->CheckDestTransMem(dst);
// 	}
// 	//std::cerr << "Initiating a transfer between  " << std::hex << dst <<  " and " << std::hex << src << " of size " << size << std::endl;
// 	//DIOGENES_Atomic_Malloc.exchange(original);
// 	return cudaMemcpyAsync(dst, src, size, kind, stream);
// }

// void * DIOGENES_MALLOCWrapper(size_t size) {
// 	void * tmp = NULL;
// 	bool setVal = false;

// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		if (DIOG_PROC_TID != gettid()) {
// 			tmp = malloc(size);
// 		} else if (DIOGENES_MUTEX_MANAGER->EnterOp()) {
// 			tmp = DIOGENES_TRANSFER_MEMMANGE->MallocMemory(size);
// 			DIOGENES_MUTEX_MANAGER->ExitOp();
// 		} else {
// 			tmp = malloc(size);
// 		}
// 	} else {
// 		tmp = malloc(size);
// 	}
// 	return tmp;
// 	// if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 	// 	if(DIOGENES_Atomic_Malloc.compare_exchange_weak(setVal, true)) {
// 	// 		PLUG_BUILD_FACTORY()	
// 	// 		tmp = DIOGENES_TRANSFER_MEMMANGE->MallocMemory(size);
// 	// 		//tmp = PLUG_FACTORY_PTR->CPUAllocate(uint64_t(size));
// 	// 		DIOGENES_Atomic_Malloc.exchange(false);
// 	// 	} else {
// 	// 		tmp = malloc(size);
// 	// 	}
// 	// } else {
// 	// 	tmp = malloc(size);
// 	// } 
// 	// //std::cerr << "Malloced data at  " << std::hex << tmp << " of size " << size << std::endl;
// 	// return tmp;
// }

// // void DIOGENES_EXITWrapper() {
// // 	DIOGENES_Atomic_Malloc.exchange(true);
// // }

// void DIOGENES_SyncExit() {
// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		//DIOGENES_TRANSFER_MEMMANGE->PerformSynchronizationAction();
// 	}
// }

// void DIOGENES_FREEWrapper(void * mem) {
// 	bool setVal = false;
// 	if (DIOGENES_MEMMANGE_TEAR_DOWN == false) {
// 		PLUG_BUILD_FACTORY()
// 		if (DIOG_PROC_TID != gettid()) {
// 			//std::cerr << "Different TID Detected! " << gettid() << std::endl;
// 			DIOGENES_LIBCFREE(mem);
// 			return;
// 		}
// 		DIOGENES_TRANSFER_MEMMANGE->ReleaseMemory(mem);
// 	} 
// }
// int (*DIOGENES_TMP_GLOBALC_CMEMCPY_PTR)(void *, const void *, size_t);
// int (*DIOGENES_TMP_GLOBALC_CMEMCPYASYNC_PTR)(void *, const void *, size_t, cudaStream_t);


// int DIOGENES_cudaMemcpySyncWrapper(void * src, const void * dst, size_t count){
// 	fprintf(stderr, "Pointer to CMEMCPY is... %p\n",DIOGENES_TMP_GLOBALC_CMEMCPY_PTR);
// 	//if (kind != cudaMemcpyHostToDevice)
// 		return DIOGENES_TMP_GLOBALC_CMEMCPY_PTR(src, dst, count);
// 	//return DIOGENES_TMP_GLOBALC_CMEMCPY_PTR(src, dst, count);
// 	//return DIOGENES_TMP_GLOBALC_CMEMCPYASYNC_PTR(src, dst, count, kind, 0);
// }
// }

