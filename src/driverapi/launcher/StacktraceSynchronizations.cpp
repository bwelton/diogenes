#include "StacktraceSynchronizations.h"
#define USE_BPATCHINSERT 1
std::vector<std::string> CudaCallList = CUDACALLLIST;



StacktraceSynchronizations::StacktraceSynchronizations(std::shared_ptr<DyninstProcess> proc) : _proc(proc) {
}


void StacktraceSynchronizations::Setup() {
	/***
	 * Setup finds the key functions we need for finding synchronizations:
	 *
	 * 1. Nvidia's internal synchronization function. This function is located at a specific, driver version
	 *    dependent offset into libcuda.so. Right now, we have a directory of md5sum,offset infromation where
	 *    we look up the md5sum of the systems libcuda to identify the location of this function. How these
	 *    locations are determined is via manual testing currently. 
	 * 2. The wrapper that performs the backtrace (using dyninst), inserted at the beginning of the synchronization function above
	 **/

	// Insert libraries needed into process	
	_libcuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
	BPatch_object * libstrace = _proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libStacktrace.so"));
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::vector<BPatch_function *> stackTracer;

	#ifdef USE_BPATCHINSERT
	stackTracer = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("RecordStack"), libstrace);
	#else
	stackTracer = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("STACK_SyncWrapper"), libstrace);
	#endif

	std::cerr << "[StacktraceSynchronizations::Setup] Starting Setup.... " << std::endl;

	std::vector<BPatch_function *> wrapperFunc;
	#ifdef USE_BPATCHINSERT
	wrapperFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("SYNC_RECORD_SYNC_CALL_IDTYPE"), NULL); 
	#else
	wrapperFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("STACK_SyncWrapper"), NULL); 
	#endif
	assert(wrapperFunc.size() > 0);
	_wrapperFunc = wrapperFunc[0];

	BPatch_function * cudaSync = NULL;

	std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), _libcuda, ops->GetSyncFunctionLocation());

	assert(cudaSyncFunctions.size() == 1);

	_cudaSync = cudaSyncFunctions[0];

	wrapperFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("SynchronizationWrapper"), NULL); 
	assert(wrapperFunc.size() == 1);
	_wrapSym = wrapperFunc[0];
}

// void StacktraceSynchronizations::SetupOrig() {
// 	/***
// 	 * Setup finds the key functions we need for finding synchronizations:
// 	 *
// 	 * 1. Nvidia's internal synchronization function. This function is located at a specific, driver version
// 	 *    dependent offset into libcuda.so. Right now, we have a directory of md5sum,offset infromation where
// 	 *    we look up the md5sum of the systems libcuda to identify the location of this function. How these
// 	 *    locations are determined is via manual testing currently. 
// 	 * 2. The wrapper that performs the backtrace (using dyninst), inserted at the beginning of the synchronization function above
// 	 **/

// 	// Insert libraries needed into process	
// 	BPatch_object * libcuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
// 	BPatch_object * libstrace = _proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libStacktrace.so"));
// 	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
// 	std::vector<BPatch_function *> stackTracer;

// 	#ifdef USE_BPATCHINSERT
// 	stackTracer = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("RecordStack"), libstrace);
// 	#else
// 	stackTracer = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("STACK_SyncWrapper"), libstrace);
// 	#endif

// 	std::cerr << "[StacktraceSynchronizations::Setup] Starting Setup.... " << std::endl;

// 	std::vector<BPatch_function *> wrapperFunc;
// 	#ifdef USE_BPATCHINSERT
// 	wrapperFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("RecordStack"), NULL); 
// 	#else
// 	wrapperFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("STACK_SyncWrapper"), NULL); 
// 	#endif
// 	assert(wrapperFunc.size() > 0);
// 	_wrapperFunc = wrapperFunc[0];

// 	BPatch_function * cudaSync = NULL;

// 	std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, ops->GetSyncFunctionLocation());

// 	assert(cudaSyncFunctions.size() == 1);

// 	_cudaSync = cudaSyncFunctions[0];

// 	wrapperFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("SynchronizationWrapper"), NULL); 
// 	assert(wrapperFunc.size() == 1);
// 	_wrapSym = wrapperFunc[0];
// }

void StacktraceSynchronizations::InsertEntryInst() {
	// Insert main entry/exit timing
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::vector<BPatch_function *> mainFunc;
	std::vector<BPatch_function *> entryMain;
	std::vector<BPatch_function *> exitMain;
	mainFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("main"), NULL);
	entryMain = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_START_MAIN_TIMER"), NULL);
	exitMain = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_END_MAIN_TIMER"), NULL);
	assert(mainFunc.size() == 1);
	assert(entryMain.size() == 1);
	assert(exitMain.size() == 1);
	std::vector<BPatch_point*> * mainEntryLocations = mainFunc[0]->findPoint(BPatch_locEntry);
	std::vector<BPatch_point*> * mainExitLocations = mainFunc[0]->findPoint(BPatch_locExit);	

	std::vector<BPatch_snippet*> testArgs2;
	BPatch_funcCallExpr mainRecordFuncEntry(*(entryMain[0]), testArgs2);
	BPatch_funcCallExpr mainRecordFuncExit(*(exitMain[0]), testArgs2);
	assert(_proc->GetAddressSpace()->insertSnippet(mainRecordFuncEntry,*mainEntryLocations) != false);	
	assert(_proc->GetAddressSpace()->insertSnippet(mainRecordFuncExit,*mainExitLocations) != false);		

	std::vector<BPatch_function *> stackTracer;
	std::vector<BPatch_function *> stackTracerExit;

	stackTracer = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("ENTER_CUDA_FUNCT"), NULL);
	stackTracerExit = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("EXIT_CUDA_FUNCT"), NULL);
	assert(stackTracer.size() == 1);
	std::set<std::string> alreadyInst;

	std::vector<BPatch_function *> funcList;
	int idCount = 1;

	for (auto i : CudaCallList) {
		std::string tmpName = std::string(i);
		std::cerr << "[StacktraceSynchronizations::InsertEntryInst] Searching for function " << i << std::endl;
		funcList = ops->FindFuncsByName(_proc->GetAddressSpace(), tmpName, _libcuda); 
		for (auto z : funcList) {
			std::cerr << "[StacktraceSynchronizations::InsertEntryInst] Found function with label - " << z->getName() << std::endl;
			if (alreadyInst.find(z->getName()) == alreadyInst.end() && alreadyInst.find(z->getName() + std::string("_v2")) == alreadyInst.end()) {
				std::vector<BPatch_point*> * entryLocations = z->findPoint(BPatch_locEntry);
				std::vector<BPatch_point*> * exitLocations = z->findPoint(BPatch_locExit);
				std::vector<BPatch_snippet*> testArgs;
				testArgs.push_back(new BPatch_constExpr(idCount));
				BPatch_funcCallExpr recordFuncEntry(*(stackTracer[0]), testArgs);
				BPatch_funcCallExpr recordFuncExit(*(stackTracerExit[0]), testArgs);
				assert(_proc->GetAddressSpace()->insertSnippet(recordFuncEntry,*entryLocations) != false);	
				assert(_proc->GetAddressSpace()->insertSnippet(recordFuncExit,*exitLocations) != false);	
				uint64_t libOffsetAddr = 0;
				if (!ops->GetFileOffset(_proc->GetAddressSpace(), (*entryLocations)[0], libOffsetAddr, true))
					libOffsetAddr = (uint64_t) (*entryLocations)[0]->getAddress();		
				StackPoint n;
				n.libOffset = libOffsetAddr;
				n.libname = _libcuda->pathName();
				n.funcName = z->getName();
				_idToPoint[idCount] = n;
				std::cout << "[CUDAID] " << n.funcName <<  " = " << idCount << std::endl;
				idCount++;	
				alreadyInst.insert(z->getName());

				//(_libcuda->pathName(),z->getName(), libOffsetAddr)
			}
		}
	}
}

void StacktraceSynchronizations::InsertStacktracing() {
	/** 
	 * Inserts stack tracing to identify the locations of synchronizations in libcuda. 
	 *
	 * Currently either wraps the syncrhonization function (if function wrapping enabled, this is currently broken in dyninst) 
	 * or viainsertion of a function call to instrimentation into the beginning of the synchronization function (current method).
	 *
	 */

	Setup();
	
	_proc->BeginInsertionSet();
#ifdef USE_BPATCHINSERT
	// Testing
	//assert(cuCtxSync.size() > 0);
	std::vector<BPatch_point*> * entryLocations = _cudaSync->findPoint(BPatch_locEntry);
	std::vector<BPatch_snippet*> testArgs;
	BPatch_funcCallExpr recordFuncEntry(*_wrapperFunc, testArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(recordFuncEntry,*entryLocations) != false);

	InsertEntryInst();
#else
	assert(_proc->GetAddressSpace()->wrapFunction(_cudaSync, _wrapperFunc, _wrapSym) != false);
#endif
// 	// _addrSpace->finalizeInsertionSet(false);
// 	BPatch_process * tmpProc = dynamic_cast<BPatch_process*>(_addrSpace);		
}


void StacktraceSynchronizations::ReadResults(StackRecMap & recs) {
	// ReadStackKeys reader(std::string("DIOGENES_SyncCalls.key"), std::string("DIOGENES_SyncCalls.bin"));
	// reader.GetStackRecords(recs, std::bind(&ReadStackKeys::ProcessStacktraceSynch, &reader, std::placeholders::_1));
	// std::set<std::string> cudaCalls;
	// std::cout << "[StacktraceSynchronizations::ReadResults] ORIG BEGIN" << std::endl;
	// for (auto i : recs) {
	// 	i.second.PrintStack();
	// 	cudaCalls.insert(i.second.GetFirstCudaCall().funcName);
	// }
	// std::cout << "[StacktraceSynchronizations::ReadResults] ORIG END" << std::endl;
	// for (auto i : cudaCalls)
	// 	std::cout << "[StacktraceSynchronizations::ReadResults] Cuda call at - " << i << std::endl;
	recs.clear();
	// new test
	FILE * inFile = fopen("DIOGENES_SyncCallKeys.bin","rb");
	int valTmp = 0;
	uint64_t pos = 1;
	while (fread(&valTmp, 1, sizeof(int), inFile) > 0){
		if (_idToPoint.find(valTmp) == _idToPoint.end()) 
			std::cerr << "[StacktraceSynchronizations::ReadResults] Could not find stack point for id - " << valTmp << std::endl;
		else {
			std::vector<StackPoint> tmpPoints;
			tmpPoints.push_back(_idToPoint[valTmp]);
			StackRecord rec(pos, tmpPoints);
			recs[pos] = rec;
			std::cerr << "[StacktraceSynchronizations::ReadResults] Synchronization at: " << _idToPoint[valTmp].funcName << std::endl;
			pos++;
		}
	}
	std::cerr << "[StacktraceSynchronizations::ReadResults] Size of output map = " << recs.size() << std::endl;
	assert(recs.size() > 0);

}