#pragma once
#define BOOST_TEST_DYN_LINK
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <sys/wait.h>
#include <stdio.h>
#include <memory>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <streambuf>
#include <utility>
#include <tuple>
#include <iostream>
typedef boost::function<void(void)> ActionCall;

void RedirectOutputToFile() {
	remove("test_output.txt");
	freopen("test_output.txt","w",stdout);
	dup2(fileno(stdout), fileno(stderr));
}

std::pair<int, int> ParseTotalTransferAndSize() {
	// Parses the total transfer size and count from redirected output
	std::ifstream t("test_output.txt");
	std::string input((std::istreambuf_iterator<char>(t)),
	                   std::istreambuf_iterator<char>());
	std::cerr << "Output File:\n" << input << std::endl;
	size_t startPos = input.find("Total Transfer:");
	startPos += strlen("Total Transfer:");
	std::string intermediate = input.substr(startPos, input.size());
	std::string numberOfTransfers = intermediate.substr(0, intermediate.find(","));
	startPos =  intermediate.find("Total Size:") + strlen("Total Size:");
	intermediate = intermediate.substr(startPos, intermediate.size());
	std::string sizeOfTransfers = intermediate.substr(0, intermediate.find("\n"));

	int sz = 0;
	int nt = 0;
	try {
		nt = std::stoi(numberOfTransfers, nullptr, 10);
		sz = std::stoi(sizeOfTransfers, nullptr, 10);
	} catch(...) {
		std::cout << "Parsed number of transfers: " << numberOfTransfers << " - Size of transfers: " << sizeOfTransfers << std::endl;
		BOOST_FAIL("Could not read Transfer Size or Number of transfers from deduplicator output");
	}
	return std::make_pair(nt,sz);
}

std::pair<int, int> ParseCollisionCountAndSize() {
	// Parses the total transfer size and count from redirected output
	std::ifstream t("test_output.txt");
	std::string input((std::istreambuf_iterator<char>(t)),
	                   std::istreambuf_iterator<char>());
	size_t startPos = input.find("Collisions:");
	startPos += strlen("Collisions:");
	std::string intermediate = input.substr(startPos, input.size());
	std::string numberOfTransfers = intermediate.substr(0, intermediate.find(","));
	startPos =  intermediate.find("Size:") + strlen("Size:");
	intermediate = intermediate.substr(startPos, intermediate.size());
	std::string sizeOfTransfers = intermediate.substr(0, intermediate.find("\n"));

	int sz = 0;
	int nt = 0;
	try {
		nt = std::stoi(numberOfTransfers, nullptr, 10);
		sz = std::stoi(sizeOfTransfers, nullptr, 10);
	} catch(...) {
		std::cout << "Parsed number of Collisions: " << numberOfTransfers << " - Size of Collisions: " << sizeOfTransfers << std::endl;
		BOOST_FAIL("Could not read Collision Size or Number of Collisions from deduplicator output");
	}
	return std::make_pair(nt,sz);
}



// Create random data
void * GenRandom(size_t bcount) {
	void * d = (void *) malloc(bcount);
	for (int x = 0; x < bcount; x++){
		((char *)d)[x] = (char)(rand() % 256);
	}
	return d;
}

#define CheckGPUResult(ans) { gpuAssertion((ans), __FILE__, __LINE__); }
inline void gpuAssertion(cudaError_t ecode, const char *file, int line)
{
   if (ecode != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(ecode), file, line);
      exit(ecode);
   }
}

struct CudaCtx
{
	cudaStream_t _stream;
	CudaCtx() {
		CheckGPUResult(cudaStreamCreate(&_stream));
	}
	~CudaCtx() {
		cudaStreamDestroy(_stream);
	}
	cudaStream_t GetStream() {
		return _stream;
	}

	void SynchronizeStream() {
		fprintf(stderr, "%s\n", "Stream Synch");
		CheckGPUResult(cudaStreamSynchronize(_stream));
	}
	void DeviceSynchronization() {
		fprintf(stderr, "%s\n", "Device Synch");
		CheckGPUResult(cudaDeviceSynchronize());
	}
	void CtxSynchronization() {
		fprintf(stderr, "%s\n", "CTx Synch");
		CheckGPUResult((cudaError_t)cuCtxSynchronize());
	}
};

template <typename T> 
struct DeviceMemory 
{
	T * store;
	size_t _size;
	std::shared_ptr<T> _cpuStore;
	DeviceMemory(size_t size) {
		CheckGPUResult(cudaMalloc((void **)&store, size * sizeof(T)));
		_size = size;
		_cpuStore.reset((T*) GenRandom(size * sizeof(T)), free);
	}
	DeviceMemory(std::shared_ptr<T> d, size_t size) {
		CheckGPUResult(cudaMalloc((void **)&store, size * sizeof(T)));
		_size = size;
		_cpuStore = d;
	}
	~DeviceMemory() {
		cudaFree(store);
	}

	void ReadAsync(std::shared_ptr<CudaCtx> ctx) {
		CheckGPUResult(cudaMemcpyAsync(_cpuStore.get(), store, _size * sizeof(T), cudaMemcpyDeviceToHost, ctx.get()->GetStream()));
	}

	void WriteAsync(std::shared_ptr<CudaCtx> ctx) {
		CheckGPUResult(cudaMemcpyAsync(store, _cpuStore.get(), _size * sizeof(T), cudaMemcpyHostToDevice, ctx.get()->GetStream()));		
	}

	void ReadSync() {
		CheckGPUResult(cudaMemcpy(_cpuStore.get(), store, _size * sizeof(T), cudaMemcpyDeviceToHost));	
	}

	void WriteSync() {
		CheckGPUResult(cudaMemcpy(store, _cpuStore.get(), _size * sizeof(T), cudaMemcpyHostToDevice));			
	}

	bool CompareDevToCPU() {
		T * local_cache = (T*) malloc(_size * sizeof(T));
		memcpy(local_cache, _cpuStore.get(), _size * sizeof(T));
		ReadSync();
		int ret = memcmp(local_cache, _cpuStore.get(), _size * sizeof(T));
		free(local_cache);
		if (ret != 0)
			return false;
		return true;
	}

};

struct TestingFixture {
		TestingFixture() {
			remove("dedup_out.txt");
		}
		int LaunchAndWait(ActionCall a) {
			pid_t child_pid = fork();
			if (child_pid == 0){
				// Child process, perform action then exit
				a();
				exit(0);
			} else {
				pid_t tpid = 0;
				int status = 1;
				while (tpid != child_pid) {
					tpid = wait(&status);
					if(tpid != child_pid) {
						BOOST_FAIL("Could not wait on child process, fork may not be functional on this machine");
					}
				}
				BOOST_CHECK(status == 0);
				return status;
			}
			return -1;
		}

		~TestingFixture() {
			remove("dedup_out.txt");
		}
};

