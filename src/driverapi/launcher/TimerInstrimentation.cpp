#include "TimerInstrimentation.h"
#include "SymbolLookup.h"
TimerInstrimentation::TimerInstrimentation(std::shared_ptr<DyninstProcess> proc) : _proc(proc) { }

void TimerInstrimentation::InsertTimers(StackRecMap & recs) {
	// standard library insertions
	_libcuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
	_libtime = _proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libTimeCall.so"));
	//SymbolLookup tmp("/usr/tce/packages/cuda/cuda-9.2.88/lib64/libcublas.so.9.2.88");
	//_proc->LoadLibrary(std::string("/usr/tce/packages/cuda/cuda-9.2.88/lib64/libcublas.so.9.2.88"));
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();

	// Find the cuda synchronization
	BPatch_function * cudaSync = NULL;
	std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), _libcuda, ops->GetSyncFunctionLocation());
	assert(cudaSyncFunctions.size() == 1);

	std::vector<BPatch_function*> instFuncs;
	FindUniqueCudaFuncs(recs, instFuncs);

	std::vector<BPatch_function *> addFunction;
	addFunction = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("TIMER_SIMPLE_COUNT_ADD_ONE"), _libtime); 
	assert(addFunction.size() == 1);

	_proc->BeginInsertionSet();

	std::vector<BPatch_point*> * funcEntry = cudaSyncFunctions[0]->findPoint(BPatch_locEntry);
	std::vector<BPatch_snippet*> testArgs;
	BPatch_funcCallExpr recordFuncEntry(*(addFunction[0]), testArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(recordFuncEntry,*funcEntry) != NULL);


	addFunction.clear();
	addFunction = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("TIMER_END_SYNCHRONIZATION_CALL"), _libtime); 
	assert(addFunction.size() == 1);

	std::vector<BPatch_point*> * funcExit = cudaSyncFunctions[0]->findPoint(BPatch_locExit);
	BPatch_funcCallExpr recordFuncExit(*(addFunction[0]), testArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(recordFuncExit,*funcExit) != NULL);

	uint64_t curId = 1;
	for (auto i : instFuncs) {
		//if (curId != 3)
		InsertTimer(i, curId);
		_idToFunc[curId] = GetPointFromBpatchFunc(i);
		curId++;
		//break;
	}

}

StackPoint TimerInstrimentation::GetPointFromBpatchFunc(BPatch_function * func) {
	uint64_t addr = 0;
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::vector<BPatch_point*> * funcEntry = func->findPoint(BPatch_locEntry);
	assert(funcEntry->size() > 0);

	assert(ops->GetFileOffset(_proc->GetAddressSpace(), (*funcEntry)[0], addr) == true);
	assert(addr > 0);

	return StackPoint(func->getModule()->getObject()->pathName(),func->getName(), addr, addr);

}

void TimerInstrimentation::PostProcessing(StackRecMap & recs) {
	StackRecMap tmp;
	ReadStackKeys reader(std::string("TF_timekey.bin"), std::string("TF_trace.bin"));
	reader.GetStackRecords(tmp, std::bind(&ReadStackKeys::ProcessTFTimingData, &reader, std::placeholders::_1));
	StackKeyWriter outKey(fopen("TF_timekey.txt","wb"));

	std::cout << "Printing decoded stack" << std::endl;
	for (auto & i : tmp) {
		i.second.PrintStack();
		if (i.first > 0) {
			assert(i.second._timing.size() > 0);
			uint64_t dynID = i.second._timing[0].s.dynId;
			for (auto n : i.second._timing)
				assert(n.s.dynId == dynID);
			assert(_idToFunc.find(dynID) != _idToFunc.end());
			i.second.ReplaceLibDynRT(_idToFunc[dynID], std::string("libTimeCall.so"));
		}
		i.second.PrintStack();
		std::vector<StackPoint> tmp_map = i.second.GetStackpoints();
		outKey.InsertStack(i.first, tmp_map);
	}
		// i.second.PrintStack();

	std::cout << "Printing original stack" << std::endl;
	for (auto i : recs) 
		i.second.PrintStack();

	recs = tmp;
}

void TimerInstrimentation::InsertTimer(BPatch_function * func, uint64_t ident) {
	if (_processed.find(func) != _processed.end()) {
		std::cerr << "[TimerInstrimentation::InsertTimer] Function " << func->getName() << " has already been instrimented with timers!" << std::endl;
		assert(_processed.find(func) == _processed.end());
	}
	std::cerr << "[TimerInstrimentation::InsertTimer] Inserting timers into function " << func->getName() << std::endl;
	_processed.insert(func);

	std::vector<BPatch_function *> startTime;
	std::vector<BPatch_function *> endTime;

	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	startTime = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("TIMER_SIMPLE_TIME_START"), _libtime); 
	endTime = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("TIMER_SIMPLE_TIME_STOP"), _libtime); 

	assert(startTime.size() == 1);
	assert(endTime.size() == 1);
	
	std::vector<BPatch_point*> * funcEntry = func->findPoint(BPatch_locEntry);
	std::vector<BPatch_point*> * funcExit = func->findPoint(BPatch_locExit);	
	//std::cerr << "[TimerInstrimentation::InsertTimer] Entry points found: " << funcEntry->size() << std::endl;
	//std::cerr << "[TimerInstrimentation::InsertTimer] Exit points found: " << funcExit->size() << std::endl;
	assert(funcEntry->size() > 0);
	assert(funcExit->size() > 0);

	std::vector<BPatch_snippet*> testArgs;
	testArgs.push_back(new BPatch_constExpr(ident));
	BPatch_funcCallExpr recordFuncEntry(*(startTime[0]), testArgs);
	BPatch_funcCallExpr recordFuncExit(*(endTime[0]), testArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(recordFuncEntry,*funcEntry) != NULL);
	assert(_proc->GetAddressSpace()->insertSnippet(recordFuncExit,*funcExit) != NULL); 	
}

void TimerInstrimentation::FindUniqueCudaFuncs(StackRecMap & recs, std::vector<BPatch_function *> & ret) {
	std::cerr << "[TimerInstrimentation::FindUniqueCudaFuncs] Starting search for cuda functions... " << std::endl;
	std::cerr << "[TimerInstrimentation::FindUniqueCudaFuncs] Stack count = " << recs.size() << std::endl;
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::set<uint64_t> alreadyPresent;
	BPatch_function * func;
	for (auto i : recs) {
		func = NULL;
		StackPoint p = i.second.GetFirstCudaCall();
		p.Print();
		if (p.empty == true){
			continue;
		}
		if (alreadyPresent.find(p.libOffset) == alreadyPresent.end()) {
			alreadyPresent.insert(p.libOffset);
			ops->FindFuncByStackPoint(_proc->GetAddressSpace(), func, p);
			ret.push_back(func);
			std::cerr << "[TimerInstrimentation::FindUniqueCudaFuncs] Inserting timing into - " << func->getName() << std::endl; 
		}
	}
}