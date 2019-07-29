#include "FixCudaFree.h"
#include <fstream>
#include <sstream>
#include <set>

FixCudaFree::FixCudaFree(std::shared_ptr<DyninstProcess> proc) : _proc(proc) {}

void FixCudaFree::InsertAnalysis(StackRecMap & recs) {
	DebugInstrimentationTemp debugOutput(std::string("DIOGENES_limitFunctions.txt"), std::string("DIOGENES_funcsInstrimented.txt"));

	_bmap.reset(new BinaryLocationIDMap());
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	BPatch_object * libcuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
	BPatch_object * wrapper = _proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libDiogenesMemManager.so"));


	std::vector<BPatch_function *> cudaFreeWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_cudaFreeWrapper"), wrapper);
	std::vector<BPatch_function *> cudaMallocWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_cudaMallocWrapper"), wrapper);
	std::vector<BPatch_function *> cudaMemcpyWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_cudaMemcpyAsyncWrapper"), wrapper);
	std::vector<BPatch_function *> mallocWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_MALLOCWrapper"), wrapper);
	std::vector<BPatch_function *> freeWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_FREEWrapper"), wrapper);
	std::vector<BPatch_function *> syncExit = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_SyncExit"), wrapper);

	assert(cudaFreeWrapper.size() > 0);
	assert(cudaMallocWrapper.size() > 0);
	assert(cudaMemcpyWrapper.size() > 0);
	assert(mallocWrapper.size() > 0);
	assert(freeWrapper.size() > 0);
	assert(syncExit.size() > 0);
	std::shared_ptr<InstrimentationTracker> tracker(new InstrimentationTracker());
	std::vector<BPatch_function *> all_functions;
	BPatch_image * img = _proc->GetAddressSpace()->getImage();
	img->getProcedures(all_functions);
	for (auto i : all_functions) {
		_dyninstFunctions[(uint64_t)i->getBaseAddr()] = std::shared_ptr<DyninstFunction>(new DyninstFunction(_proc, i, tracker, _bmap));
	}

	std::string binary_name = std::string("main");
	std::string tmpLibname = std::string("");
	std::string tmpFuncName = std::string("");
	_proc->BeginInsertionSet();
	for (auto i : _dyninstFunctions) {
		// if (i.second->IsExcludedFunction(LOAD_STORE_INST))
		// 	continue;
		i.second->GetFuncInfo(tmpLibname, tmpFuncName);

		if (tmpFuncName.find("DIOGENES_") != std::string::npos || tmpLibname.find("/lib/plugins/") != std::string::npos)
			continue;
		if (tmpLibname.find("/usr/lib64/libc-2.17.so") != std::string::npos || tmpLibname.find("libcudart") != std::string::npos)
			continue;
		
		//if (tmpLibname.find(binary_name) != std::string::npos) {
			std::vector<DyninstCallsite> callsites;
			i.second->GetCallsites(callsites);
			for (auto x : callsites) {
				std::cerr << "[DB]CS Function Name - " << *(x.GetCalledFunction()) << std::endl;
				if (*(x.GetCalledFunction()) == std::string("cudaFree")){
					if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
						continue;
					//if (x.GetPointAddress() == (uint64_t) 0x10006cc4) {
					std::cerr << "Found function call to cudaFree in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
					x.ReplaceFunctionCall(cudaFreeWrapper[0]);
					//return;
					
					//}
				}
				if (*(x.GetCalledFunction()) == std::string("cudaMalloc")) {
					if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
						continue;
					std::cerr << "Found function call to cudaMalloc in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
					x.ReplaceFunctionCall(cudaMallocWrapper[0]);
					//return;
				}
				if (*(x.GetCalledFunction()) == std::string("__GI___libc_malloc")){
					if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
						continue;
					x.ReplaceFunctionCall(mallocWrapper[0]);
					std::cerr << "Found function call to malloc in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				}
				if (*(x.GetCalledFunction()) == std::string("__libc_free")){
					if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
						continue;
					x.ReplaceFunctionCall(freeWrapper[0]);
					std::cerr << "Found function call to free in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				}
				if (*(x.GetCalledFunction()) == std::string("cudaMemcpyAsync")){
					if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
						continue;
					std::cerr << "Found function call to cudaMemcpyAsync in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
					x.ReplaceFunctionCall(cudaMemcpyWrapper[0]);
				}
			}
		//}
	}
	std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, ops->GetSyncFunctionLocation());
	assert(cudaSyncFunctions.size() > 0);

	std::vector<BPatch_point*> * entryPoints = cudaSyncFunctions[0]->findPoint(BPatch_locExit);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*(syncExit[0]), recordArgs);	
	assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	// 
}