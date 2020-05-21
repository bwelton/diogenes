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
#include <execinfo.h>
#include <unordered_map>
#include <unistd.h>
#include <dlfcn.h>
#include <mutex>
#include <memory>
#include <thread>
#include "StackPoint.h"
#include "framestepper.h"
#include "walker.h"
#include "procstate.h"
#include "swk_errors.h"
#include "steppergroup.h"
#include "frame.h"
#include "sw_c.h"
#include "Symtab.h"
#include "BPatch.h"
#include "BPatch_process.h"

using namespace Dyninst;
using namespace Dyninst::Stackwalker;
using namespace SymtabAPI;

thread_local Walker *  local_walker = NULL;

volatile bool STACKWALK_SHUTDOWN_MODE = false;
std::mutex SW_RECURISVEMAP_MUTEX;
struct SWRecursiveMap{
	uint64_t _total_count;
	int64_t _id;
	std::map<uint64_t, SWRecursiveMap *> _map;
	SWRecursiveMap() : _total_count(0) {};
	void Init() {
		_total_count = 0;
		_id = -1;
		//StackKeyReader r(fopen("AC_AutoCorrectStacks.txt", "rb"));
		//std::map<uint64_t, std::vector<StackPoint> > stacks = r.ReadStacks();

	};
	~SWRecursiveMap() {
		STACKWALK_SHUTDOWN_MODE = true;
	};

	void LockAndInsert(std::vector<StackPoint> & input, int pos, int64_t id){
		if (STACKWALK_SHUTDOWN_MODE == true)
			return;
		std::lock_guard<std::mutex> guard(SW_RECURISVEMAP_MUTEX);
		Insert(input, pos,id);

	};
	void Insert(std::vector<StackPoint> & input, int pos, int64_t id) {
		if(input.size() <= pos){
			_id = id;
			return;
		}
		uint64_t val = input[pos].raFramePos;
		if (_map.find(val) == _map.end()) {
			_map[val] = new SWRecursiveMap();
		}
		_map[val]->Insert(input, pos+1,id);
	};

	// bool Lookup(std::vector<uint64_t> & input, int pos) {
	// 	if(input.size() <= pos)
	// 		return true;
	// 	uint64_t val = input[pos];
	// 	if(_map.find(val) == _map.end())
	// 		return false;
	// 	return _map[val]->Lookup(input,pos+1);
	// };

	int64_t IterativeLookup(uint64_t * input, int size, int pos) {
		if (STACKWALK_SHUTDOWN_MODE == true)
			return -1;
		std::lock_guard<std::mutex> guard(SW_RECURISVEMAP_MUTEX);

		std::map<uint64_t, SWRecursiveMap *> * curMap = &_map;
		int insize = size;
		int64_t idCaptured = -1;
		while (pos < insize) {
			auto it = curMap->find(input[pos]);
			if (it == curMap->end()) 
				return -1;
			curMap = &(it->second->_map);
			idCaptured = it->second->_id;
			pos++;
		}
		_total_count++;
		return idCaptured;
	};
};

std::shared_ptr<SWRecursiveMap> DIOGENES_SW_StackChecker;


//thread_local StackWalkerFastMap fm;
extern "C" {
	void INIT_FP_STACKWALKER() {
		if (DIOGENES_SW_StackChecker.get() == NULL){
			DIOGENES_SW_StackChecker.reset(new SWRecursiveMap());
			DIOGENES_SW_StackChecker->Init();
		}
		if (local_walker != NULL)
			return;
		local_walker = Walker::newWalker();
	}


	int64_t GET_FP_STACKWALK_FAST_NONSW(std::vector<StackPoint> & ret) {
		if (STACKWALK_SHUTDOWN_MODE == true)
			return -1;
		INIT_FP_STACKWALKER();
		void * local_stack[100];
		int tmpretm = backtrace(local_stack, 100);
		int64_t idfound = DIOGENES_SW_StackChecker->IterativeLookup((uint64_t*)local_stack, tmpretm, 0);
		if (idfound > 1)
			return idfound;

		for (int i = 0; i < tmpretm; i++) {
			StackPoint sp;
			sp.libname = std::string("ABSOLUTE_ADDR_REQ_MUTATOR");
			sp.libOffset = ((uint64_t*)local_stack)[i];
			sp.raFramePos = sp.libOffset;
			ret.push_back(sp);
		}
		return idfound;
	}


	void GET_FP_REGISTER_ID(int64_t id, std::vector<StackPoint> & vec) {
		if (STACKWALK_SHUTDOWN_MODE == true)
			return;
		INIT_FP_STACKWALKER();
		DIOGENES_SW_StackChecker->LockAndInsert(vec,0,id);
	}

	bool GET_FP_STACKWALK_NONSW(std::vector<StackPoint> & ret) {
		void * local_stack[100];
		int tmpretm = backtrace(local_stack, 100);
		for (int i = 0; i < tmpretm; i++) {
			StackPoint sp;
			sp.libname = std::string("ABSOLUTE_ADDR_REQ_MUTATOR");
			sp.libOffset = ((uint64_t*)local_stack)[i];
			sp.raFramePos = sp.libOffset;
			ret.push_back(sp);
		}
		return true;
	}


	bool GET_FP_STACKWALK(std::vector<StackPoint> & ret) {

		// void * local_stack[100];
		// int tmpretm = backtrace(local_stack, 100);
		INIT_FP_STACKWALKER();

		std::vector<Frame> stackwalk;
		if(local_walker->walkStack(stackwalk) == false) {
			return false;
		}
		std::string lib;
		void * stab;
		for (auto frame : stackwalk) {
			StackPoint sp;
			uint64_t offset;
			//std::cerr << "RA/FP/SP: " << std::hex << frame.getRA() << ","  << frame.getFP() << "," << frame.getSP() << std::endl;
			frame.getLibOffset(lib, offset, stab);
			sp.libname = lib;
			sp.libOffset = offset;
			sp.raFramePos = (uint64_t)frame.getRA();
			ret.push_back(sp);
		}
		return true;
	}
}