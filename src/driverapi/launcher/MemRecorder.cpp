#include "MemRecorder.h"
#include <fstream>
#include <sstream>
#include <set>

MemRecorder::MemRecorder(std::shared_ptr<DyninstProcess> proc) : _proc(proc) {}

void MemRecorder::InsertAnalysis(StackRecMap & recs) {
	DebugInstrimentationTemp debugOutput(std::string("DIOGENES_limitFunctions.txt"), std::string("DIOGENES_funcsInstrimented.txt"));

	std::remove("DIOGENES_MemRecords.bin");
	_bmap.reset(new BinaryLocationIDMap());
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	BPatch_object * libcuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
	// BPatch_object * libcudart = _proc->LoadLibrary(std::string("libcudart.so.1"));
	BPatch_object * wrapper = _proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libDiogenesMemRecorder.so"));

	std::vector<BPatch_function *> cudaFreeWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_REC_CudaFree"), wrapper);
	std::vector<BPatch_function *> cudaMallocWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_REC_CudaMalloc"), wrapper);
	std::vector<BPatch_function *> cudaMemcpyWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_REC_CudaMemcpy"), wrapper);
	std::vector<BPatch_function *> mallocWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_REC_MALLOCWrapper"), wrapper);
	std::vector<BPatch_function *> freeWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_REC_FREEWrapper"), wrapper);
	std::vector<BPatch_function *> cudaMallocHostWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_REC_HostCudaMalloc"), wrapper);

	assert(cudaFreeWrapper.size() > 0);
	assert(cudaMallocWrapper.size() > 0);
	assert(cudaMemcpyWrapper.size() > 0);
	assert(mallocWrapper.size() > 0);
	assert(freeWrapper.size() > 0);
	assert(cudaMallocHostWrapper.size() > 0);
	
	std::shared_ptr<InstrimentationTracker> tracker(new InstrimentationTracker());
	std::vector<BPatch_function *> all_functions;
	BPatch_image * img = _proc->GetAddressSpace()->getImage();
	img->getProcedures(all_functions);
	for (auto i : all_functions) {
		if (ops->IsNeverInstriment(i))
			continue;
		_dyninstFunctions[(uint64_t)i->getBaseAddr()] = std::shared_ptr<DyninstFunction>(new DyninstFunction(_proc, i, tracker, _bmap));
	}
	DetectDuplicateStackpoints dupCheck;

	std::string tmpLibname = std::string("");
	std::string tmpFuncName = std::string("");
	_proc->BeginInsertionSet();
	int64_t ident = 2;
	for (auto i : _dyninstFunctions) {
		i.second->GetFuncInfo(tmpLibname, tmpFuncName);

		if (tmpFuncName.find("DIOGENES_") != std::string::npos || tmpLibname.find("/lib/plugins/") != std::string::npos)
			continue;
		if (tmpLibname.find("/usr/lib64/libc-2.17.so") != std::string::npos || tmpLibname.find("libcudart") != std::string::npos || tmpLibname.find("libcuda") != std::string::npos)
			continue;
		
		//if (tmpLibname.find(binary_name) != std::string::npos) {
			std::vector<DyninstCallsite> callsites;
			i.second->GetCallsites(callsites);
			for (auto x : callsites) {
				//std::cerr << "[DB]CS Function Name - " << *(x.GetCalledFunction()) << std::endl;
				if (*(x.GetCalledFunction()) == std::string("cudaFree")){
					if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
						continue;
					// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
					// 	continue;
					std::cerr << "[MemRecorder::InsertAnalysis] Found function call to cudaFree in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
					x.ReplaceFunctionCallWithID(cudaFreeWrapper[0], ident);
					_idToStackPoint[ident] = x.GetStackPoint();
					ident++;
				}
				if (*(x.GetCalledFunction()) == std::string("cudaMalloc")) {
					if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
						continue;
					// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
					// 	continue;
					std::cerr << "[MemRecorder::InsertAnalysis] Found function call to cudaMalloc in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
					x.ReplaceFunctionCallWithID(cudaMallocWrapper[0], ident);
					_idToStackPoint[ident] = x.GetStackPoint();
					ident++;					

					//x.ReplaceFunctionCall(cudaMallocWrapper[0]);
					
					//return;
				}
				if (*(x.GetCalledFunction()) == std::string("__GI___libc_malloc")){
					if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
						continue;
					// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
					// 	continue;
					x.ReplaceFunctionCallWithID(mallocWrapper[0], ident);
					_idToStackPoint[ident] = x.GetStackPoint();
					ident++;
					std::cerr << "[MemRecorder::InsertAnalysis] Found function call to malloc in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				}
				if (*(x.GetCalledFunction()) == std::string("__libc_free")){
					if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
						continue;
					// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
					// 	continue;
					x.ReplaceFunctionCallWithID(freeWrapper[0], ident);
					_idToStackPoint[ident] = x.GetStackPoint();
					ident++;
					std::cerr << "[MemRecorder::InsertAnalysis] Found function call to free in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				}
				if (*(x.GetCalledFunction()) == std::string("cudaMemcpyAsync")){
					if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
						continue;
					// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
					// 	continue;
					std::cerr << "[MemRecorder::InsertAnalysis] Found function call to cudaMemcpyAsync in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
					x.ReplaceFunctionCallWithID(cudaMemcpyWrapper[0], ident);
					_idToStackPoint[ident] = x.GetStackPoint();
					ident++;					
				}
				if (*(x.GetCalledFunction()) == std::string("cudaMallocHost")){
					if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
						continue;
					// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
					// 	continue;
					std::cerr << "[MemRecorder::InsertAnalysis] Found function call to cudaMallocHost in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
					x.ReplaceFunctionCallWithID(cudaMallocHostWrapper[0], ident);
					_idToStackPoint[ident] = x.GetStackPoint();
					ident++;					
				}
			}
		//}
	}


	std::vector<BPatch_function*> cudaMalloc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("cudaMalloc"), NULL);
	std::vector<BPatch_function*> cudaFree = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("cudaFree"), NULL);
	std::vector<BPatch_function*> cudaMallocPreWrap = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOG_CUDAMallocPreCheck"), wrapper);
	std::vector<BPatch_function*> cudaMallocPostwrap = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOG_CUDAMallocCheck"), wrapper);
	std::vector<BPatch_function*> cudaFreeWrap = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOG_CUDAFreeCheck"), wrapper);

	if (cudaMalloc.size() > 1)
		for (auto i : cudaMalloc){
			std::cerr << "CudaMalloc Duplicate at function name - " << i->getName() << " in library - " << i->getModule()->getObject()->pathName() << std::endl;
		}
	assert(cudaMalloc.size() == 1);
	assert(cudaFree.size() == 1 );
	assert(cudaMallocPreWrap.size() == 1);
	assert(cudaMallocPostwrap.size() == 1);
	assert(cudaFreeWrap.size() == 1);
	InsertPrePostCall(cudaMalloc[0], cudaMallocPreWrap[0], false, 2);
	InsertPrePostCall(cudaMalloc[0], cudaMallocPostwrap[0], true, 0);
	InsertPrePostCall(cudaFree[0], cudaFreeWrap[0], false, 1);

	// std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, ops->GetSyncFunctionLocation());
	// assert(cudaSyncFunctions.size() > 0);

	// std::vector<BPatch_point*> * entryPoints = cudaSyncFunctions[0]->findPoint(BPatch_locExit);
	// std::vector<BPatch_snippet*> recordArgs;
	// BPatch_funcCallExpr entryExpr(*(syncExit[0]), recordArgs);	
	// assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	// 
}



void MemRecorder::InsertPrePostCall(BPatch_function * origFunction, BPatch_function * instrimentation, bool postCall, int numParams) {
	BPatchPointVecPtr locationEntry;
	if (postCall == false)
		locationEntry.reset(origFunction->findPoint(BPatch_locEntry));
	else
		locationEntry.reset(origFunction->findPoint(BPatch_locExit));
	std::vector<BPatch_snippet*> recordArgs;
	for (int i = 0; i < numParams; i++){
		recordArgs.push_back(new BPatch_paramExpr(i));
	}	
	assert(locationEntry->size() > 0);
	BPatch_funcCallExpr recordAddrCall(*instrimentation, recordArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(recordAddrCall,*locationEntry) != NULL);
}

CallTransPtr MemRecorder::PostProcessing() {
	MemTransVec memVec;
	GPUMallocVec gMallocVec;
	CPUMallocVec cMallocVec;
	MemRecDataFile readRecs(fopen("DIOGENES_MemRecords.bin", "rb"));
	readRecs.Read(memVec, gMallocVec, cMallocVec);
	CallTransPtr transformer;
	transformer.reset(new CallTransformation(gMallocVec, cMallocVec, memVec, _idToStackPoint));
	return transformer;
}
