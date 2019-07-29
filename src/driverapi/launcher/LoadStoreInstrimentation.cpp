#include "LoadStoreInstrimentation.h"

LoadStoreInstrimentation::LoadStoreInstrimentation(std::shared_ptr<DyninstProcess> proc) : _proc(proc) { 
	_bmap.reset(new BinaryLocationIDMap());

}

LoadStoreInstrimentation::~LoadStoreInstrimentation() {

}

void LoadStoreInstrimentation::InsertAnalysis(StackRecMap & recs) {
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	BPatch_object * libcuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
	// Load all the libraries that appear in the stacktraces into the process address space. 
	for (auto i : recs) {
		for (auto z : i.second.GetStackpoints()) {
		 	if (z.libname.find(".so") != std::string::npos)
		 		_proc->LoadLibrary(z.libname);
		}
	}

	// Create the dyninst functions from all the functions in the process.
	std::vector<BPatch_function *> all_functions;
	BPatch_image * img = _proc->GetAddressSpace()->getImage();
	img->getProcedures(all_functions);
	std::shared_ptr<InstrimentationTracker> tracker(new InstrimentationTracker());
	_proc->GetAddressSpace()->allowTraps(false);
	for (auto i : all_functions) {
		//_dyninstFunctions.push_back(std::shared_ptr<DyninstFunction>(new DyninstFunction(_proc, i, tracker, _bmap)));	
		_dyninstFunctions[(uint64_t)i->getBaseAddr()] = std::shared_ptr<DyninstFunction>(new DyninstFunction(_proc, i, tracker, _bmap));
	}

	// Rectify function list to remove anything +0x8
	{
		std::set<uint64_t> removeList;
		for (auto i : _dyninstFunctions) {
			if (_dyninstFunctions.find(i.first - 8) != _dyninstFunctions.end())
				removeList.insert(i.first);
		}
		std::cerr << "[LoadStoreInstrimentation::InsertAnalysis] Removing " << removeList.size() << " duplicate functions without premable" << std::endl;
		for (auto i : removeList)
			_dyninstFunctions.erase(i);
	}

	// Setup apicapture
	APICaptureInstrimentation procTime(_proc);
	procTime.InsertInstrimentation();
	_proc->BeginInsertionSet();
	// Insert Entry/Exit info
	InsertEntryExit(recs);

	// Insert Sync Capture
	BPatch_function * enterSync;
	std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, ops->GetSyncFunctionLocation());
	assert(cudaSyncFunctions.size() == 1);
	assert(ops->FindFuncByName(_proc->GetAddressSpace(), enterSync, std::string("SYNC_CAPTURE_SYNC_CALL")) == 1);
	std::vector<BPatch_point*> * entryPoints = cudaSyncFunctions[0]->findPoint(BPatch_locEntry);
	std::string tmp = cudaSyncFunctions[0]->getModule()->getObject()->pathName();
	_bmap->StorePosition(tmp,ops->GetSyncFunctionLocation());
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*enterSync, recordArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	//_proc->SetTrampGuard();
	for (auto i : _dyninstFunctions)
		i.second->InsertLoadStoreAnalysis();

	// Print Debug Info
	PrintDebug(recs);

	_bmap->DumpLocationMap(std::string("DIOGENES_BINMAP.txt"));
}

void LoadStoreInstrimentation::InsertEntryExit(StackRecMap & recs) {
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	for (auto i : recs) {
		std::vector<StackPoint> points = i.second.GetStackpoints();
		for (auto z : points) {
			BPatch_function * func;
			if (ops->FindFuncByStackPoint(_proc->GetAddressSpace(), func, z) <= 0) {
				std::cerr << "[LoadStoreInstrimentation::InsertEntryExit] Could not find function - " << z.funcName << std::endl;
				continue;
			}
			uint64_t f_addr = (uint64_t)func->getBaseAddr();
			if (_dyninstFunctions.find(f_addr) != _dyninstFunctions.end()) {
				_dyninstFunctions[f_addr]->EntryExitWrapping();
			} else if (_dyninstFunctions.find(f_addr - 8) != _dyninstFunctions.end()) {
				_dyninstFunctions[f_addr - 8]->EntryExitWrapping();
			} else if (_dyninstFunctions.find(f_addr + 8) != _dyninstFunctions.end()) {
				_dyninstFunctions[f_addr + 8]->EntryExitWrapping();
			} else {
				assert("CANT FIND ID" == 0);
			}
			//assert(_funcPositions.find(func) != _funcPositions.end());
			//_dyninstFunctions[_funcPositions[func]]->EntryExitWrapping();
		}
	}
}

void LoadStoreInstrimentation::PrintDebug(StackRecMap & recs) {

	InstStats stats;
	stats.callTracedInsts = 0;
	stats.lsInsts = 0;
	std::ofstream t;
	t.open("DIOGENES_LSDEBUG.txt", std::ofstream::out);
	t << "Printing tracing stack" << std::endl;
	for (auto & i : recs) {
		i.second.PrintStack(t);
	}
	t << "\n";

	for (auto i : _dyninstFunctions) {
		t << i.second->PrintInst(stats);
	}

	t << "\n\nTotal Inst Points: " << stats.callTracedInsts + stats.lsInsts << std::endl;
	std::cout << "\n\nTotal Inst Points: " << stats.callTracedInsts + stats.lsInsts << std::endl;
	t << "Total CallTrace Points: " << stats.callTracedInsts << std::endl;
	std::cout << "Total CallTrace Points: " << stats.callTracedInsts << std::endl;
	t << "Calltraced Instructions: ";
	std::cout << "Calltraced Instructions: ";
	for (auto i : stats.ct_instNames){
		t << i << " ";
		std::cout << i << " ";
	}
	t << "\n";
	std::cout << "\n";

	t << "Total LS Points: " << stats.lsInsts << std::endl;
	std::cout << "Total LS Points: " << stats.lsInsts << std::endl;
	t << "LS Instructions: ";
	std::cout << "LS Instructions: ";
	for (auto i : stats.ls_instNames){
		t << i << " ";
		std::cout << i << " ";
	}
	t << "\n";
	std::cout << "\n";
	
	t.close();
}

void LoadStoreInstrimentation::PostProcessing(StackRecMap & recs, std::vector<StackPoint> & lsPoints) {

	std::shared_ptr<BinaryLocationIDMap> tmpMap = _bmap;
	std::cerr << "Runing onetime code on exit of LS" << std::endl;
	//_binLoc.DumpLocationMap(std::string("LS_BinLoc.txt"));
	ReadLoadStoreFiles tmp(tmpMap.get());
	tmp.OutputTraceKey(std::string("LS_trace.bin"), std::string("LS_tracekey.txt"));
	tmp.CreateStackKey(std::string("LS_stackkey.bin"), std::string("LS_stackkey.txt"));
	lsPoints = tmp.GetTraces();
}
