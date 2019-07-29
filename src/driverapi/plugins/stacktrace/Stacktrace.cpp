#include <map>
#include <vector>
#include <iostream> 
#include <fstream>    
#include <string>		
#include <cassert> 
#include <unistd.h>
#include <sys/syscall.h>
#include <sstream>
#include <cstring>
#include <memory>
#include <vector>
#include <execinfo.h>
#include <chrono>
#include <dlfcn.h>
#include <sstream>
#include "StackPoint.h"
#include "StackwalkingCommon.h"
//std::shared_ptr<std::vector<bool>> DIOGENES_seenCalls(1024, false);
int DIOGENES_CURRENT_CALL_ID = -1;
//bool DIOGENES_CHECK_KERN_START =false;

struct OutputFile {
	FILE * outFile;
	OutputFile(std::string filename) {
		outFile = fopen(filename.c_str(),"w");
	}

	~OutputFile() {
		fflush(outFile);
		fclose(outFile);
	}
};

class WriteIDKeys {
public:
	WriteIDKeys(std::string filename) : _seenCalls(1024, false), _callCounts(1024,0), _outFile(filename) {

	}
	// void CheckKey(int id) {
	// 	if (id >= 0 && id < 1024){
	// 		_callCounts[id]++;
	// 	}
	// }

	void RecordKey(int id) {
		// if (id == 291) {
		// 	void (*tf)(uint64_t);
		// 	fprintf(stderr, "%s\n", "In cudaMemcpyAsyncHtoD_v2");
		// 	void * handle = dlopen("/g/g17/welton2/scratch/nfs/apps/cuIBM/examples/lidDrivenCavity/Re100/logs/mylib.so", RTLD_NOW);
		// 	assert(handle != NULL);

		// 	tf = (void (*)(uint64_t)) dlsym(handle, "_Z11StartKernelP11CUstream_st");
		// 	assert(tf != NULL);
		// 	tf(0);
		// 	fprintf(stderr, "%s\n", "Started Kernel");
		// 	DIOGENES_CHECK_KERN_START = true;
		// }
		if (id >= 0 && id < 1024){
			_seenCalls[id] = true;
			//_callCounts[id]++;
		}
		else {
			fprintf(stderr, "%s\n", "Unknown ID Seen");
		}
	};

	~WriteIDKeys() {
		for (int i = 0; i < _seenCalls.size(); i++) {
			if (_seenCalls[i] == true)
				fwrite(&i, 1, sizeof(int),_outFile.outFile);
			// if (_callCounts[i] > 0)
			// 	std::cerr << "[CallCount] " << i << " - " << _callCounts[i] << std::endl;
		}
	};
private:
	std::vector<bool> _seenCalls;
	std::vector<int> _callCounts;
	OutputFile _outFile;
};

thread_local uint64_t skippedStacks;
thread_local pid_t my_thread_id = -1;
thread_local std::shared_ptr<OutputFile> outputFile;
thread_local std::shared_ptr<StackKeyWriter> keyFile;
thread_local std::shared_ptr<WriteIDKeys> DIOGENES_WRITEKEYID;


extern "C" {

	void SETUP_NEWINTERCEPTOR() {
		if (DIOGENES_WRITEKEYID.get() != NULL)
			return;
		DIOGENES_WRITEKEYID.reset(new WriteIDKeys(std::string("DIOGENES_SyncCallKeys.bin")));
	}

	void SETUP_INTERCEPTOR() {
		if (outputFile.get() != NULL)
			return;

		skippedStacks = 0;
		if (my_thread_id == -1)
			my_thread_id = (pid_t) syscall(__NR_gettid);

		std::stringstream ss;
		ss << "DIOGENES_SyncCalls.bin";
		outputFile.reset(new OutputFile(ss.str()));
		assert(outputFile.get() != NULL);
		ss.str(std::string());
		ss << "DIOGENES_SyncCalls.key";
		keyFile.reset(new StackKeyWriter(fopen(ss.str().c_str(),"w")));
		assert(keyFile.get() != NULL);
	}



	__attribute__ ((noinline)) void RecordStack() {
//		fprintf(stderr,"We are recording a stack position....\n");
		SETUP_INTERCEPTOR();
		uint64_t pos = 0;
		std::vector<StackPoint> points;
		bool ret = GET_FP_STACKWALK(points);
		if(ret == false) {
			std::cout << "Could not walk stack, returning nothing" << std::endl;
			skippedStacks += 1; 
			std::cout << "Skipped Stacks: " << skippedStacks << std::endl;
			fwrite(&pos, 1, sizeof(uint64_t), outputFile->outFile);
		} else {
			pos = keyFile->InsertStack(points);
			fwrite(&pos, 1, sizeof(uint64_t), outputFile->outFile);			
		}

		
		// uint64_t pos = 0;
		// StackPoint sp;
		// std::string lib;
		// void * stab;
		// if(local_walker->walkStack(stackwalk) == false) {
		// 	std::cout << "Could not walk stack, returning nothing" << std::endl;
		// 	skippedStacks += 1; 
		// 	std::cout << "Skipped Stacks: " << skippedStacks << std::endl;
		// 	fwrite(&pos, 1, sizeof(uint64_t), outputFile->outFile);
		// 	return;
		// }
		// for (auto frame : stackwalk) {
		// 	StackPoint sp;
		// 	uint64_t offset;
		// 	frame.getLibOffset(lib, offset, stab);
		// 	sp.libname = lib;
		// 	sp.libOffset = offset;
		// 	points.push_back(sp);
		// }
		// pos = keyFile->InsertStack(points);
		// fwrite(&pos, 1, sizeof(uint64_t), outputFile->outFile);
	}


	__attribute__ ((noinline)) int SynchronizationWrapper(void * a, void * b, void * c);


	__attribute__ ((noinline)) int STACK_SyncWrapper(void * a, void * b, void * c) {
		RecordStack();
		return SynchronizationWrapper(a,b,c);
	}

	void SYNC_RECORD_SYNC_CALL() {
		std::vector<StackPoint> points;
		bool ret = GET_FP_STACKWALK(points);
		// for (auto i : points) {
		// 	std::cerr << i.libname <<  "," << i.libOffset << std::endl;
		// }
	}

	void SYNC_RECORD_SYNC_CALL_IDTYPE() {
		SETUP_NEWINTERCEPTOR();
		DIOGENES_WRITEKEYID->RecordKey(DIOGENES_CURRENT_CALL_ID);
	}

	void ENTER_CUDA_FUNCT(int id) {
		//SETUP_NEWINTERCEPTOR();
		// DIOGENES_WRITEKEYID->CheckKey(id);
		//if (DIOGENES_CURRENT_CALL_ID == -1)
		DIOGENES_CURRENT_CALL_ID = id;
	}

	void EXIT_CUDA_FUNCT(int id) {
		//if (DIOGENES_CURRENT_CALL_ID == id) {
		DIOGENES_CURRENT_CALL_ID = -1;
		//}
		// if (DIOGENES_CHECK_KERN_START == true)
		// 	exit(0);
	}

	std::chrono::high_resolution_clock::time_point DIOGENES_TOTAL_TIME_START;
	bool DIOGENES_TIMER_SET = false;
	void DIOGENES_START_MAIN_TIMER() {
		SETUP_NEWINTERCEPTOR();
		DIOGENES_TIMER_SET = true;
		DIOGENES_TOTAL_TIME_START = std::chrono::high_resolution_clock::now();
	}

	void DIOGENES_END_MAIN_TIMER() {
		auto end = std::chrono::high_resolution_clock::now();
		if (DIOGENES_TIMER_SET == false){
			fprintf(stderr, "%s\n", "Diogenes timer not set, exiting");
			return;
		}
		std::stringstream ss;

		std::chrono::duration<double> diff = end - DIOGENES_TOTAL_TIME_START;
		std::cerr << "[DIOGENES_RTIME] " << diff.count() << std::endl;
		ss << diff.count() << std::endl;
		std::ofstream ofs("ttime_new.txt", std::ofstream::out);
		ofs << ss.str();
		ofs.close();

	}
}