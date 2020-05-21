#include "FixCudaProblems.h"
#include <fstream>
#include <sstream>
#include <set>
#include "DetectDuplicateStackpoints.h"
#define FIX_CUDAMEMFREE 1
//#define FIX_ASYNCTRANS_NOWRAP 1
// #define FIX_ASYNCTRANS_WWRAP 0

FixCudaProblems::FixCudaProblems(std::shared_ptr<DyninstProcess> proc) : _proc(proc) {}


uint64_t FixCudaProblems::GetAbsoluteAddress(StackPoint & point) {
	boost::filesystem::path p(point.libname);
	std::string tmpfilename = p.stem().string();
	if (tmpfilename.find('.') != std::string::npos) {
		tmpfilename = tmpfilename.substr(0, tmpfilename.find('.'));
	}	
	assert(_object_offsets.find(tmpfilename) != _object_offsets.end());
	return _object_offsets[tmpfilename] + point.libOffset;
};

void FixCudaProblems::InsertAnalysis(StackRecMap & recs) {
	DebugInstrimentationTemp debugOutput(std::string("DIOGENES_limitFunctions.txt"), std::string("DIOGENES_funcsInstrimented.txt"));

	_bmap.reset(new BinaryLocationIDMap());
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	BPatch_object * libcuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
	BPatch_object * libcudart = _proc->LoadLibrary(std::string("libcudart.so"));
	BPatch_object * wrapper = _proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libDiogenesMemManager.so"));

	std::vector<BPatch_object *> objects = ops->GetObjects(_proc->GetAddressSpace());
	for (auto i : objects) {
		if (i->pathName().find(".so") != std::string::npos) {

			boost::filesystem::path p(i->pathName());
			std::string tmpfilename = p.stem().string();
			if (tmpfilename.find('.') != std::string::npos) {
				tmpfilename = tmpfilename.substr(0, tmpfilename.find('.'));
			}

			std::cout << "[FixCudaProblems::InsertAnalysis] Adding entry for module " << tmpfilename << std::endl;
			std::vector<BPatch_module *> mods;
			i->modules(mods);
			if (mods.size() == 0){
				std::cerr << "Could not find module for library " << i->pathName() << std::endl;
				continue;
			}
			_object_offsets[tmpfilename] = (uint64_t)mods[0]->getBaseAddr();
		} else {
			boost::filesystem::path p(i->pathName());
			std::string tmpfilename = p.stem().string();
			if (tmpfilename.find('.') != std::string::npos) {
				tmpfilename = tmpfilename.substr(0, tmpfilename.find('.'));
			}	
			_object_offsets[tmpfilename] = 0;
			std::cout << "[FixCudaProblems::InsertAnalysis] Adding entry for module " << tmpfilename << std::endl;		
		}
	}

	HostToDeviceLimiter readLimiter(fopen("AC_translimit.bin","rb"));
	StackKeyReader r(fopen("AC_AutoCorrectStacks.txt","rb"));
	std::map<uint64_t, std::vector<StackPoint> > m = r.ReadStacks();

	{
		RAStackReaderWriter writeAbsolutes(fopen("AC_BinStacks.bin","wb"));
		std::vector<uint64_t> readBins = readLimiter.ReadLimiter();
		uint64_t currentCount = 0;
		for (auto i : m) {
			std::vector<uint64_t> absoluteOut;
			for(auto x : i.second){
				if (x.libname.find("libc.so") != std::string::npos)
					continue;
				absoluteOut.push_back(GetAbsoluteAddress(x));
			}
			std::reverse(absoluteOut.begin(), absoluteOut.end());
			if (currentCount < readBins.size()) {
				absoluteOut.push_back(readBins[currentCount]);
			}
			currentCount++;
			writeAbsolutes.WriteRAStack(absoluteOut);
			std::cerr << "Writing stack" << std::endl;
			for (auto x : absoluteOut) {
				std::cerr << x << ",";
			}
			std::cerr << std::endl;
		}
	}


	std::vector<BPatch_function *> binderFunctions = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_INIT_GOTCHA"), wrapper);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*(binderFunctions[0]), recordArgs);
	std::cerr << "[FixCudaProblems::InsertAnalysis] Fireing off one time call to setup API Capture Instrimentation\n";
	dynamic_cast<BPatch_process*>(_proc->GetAddressSpace())->oneTimeCode(entryExpr);

	std::vector<BPatch_function *> wrapperFunc = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_POSTSYNC_WRAPPER"), wrapper);
	std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, ops->GetSyncFunctionLocation());
	assert(cudaSyncFunctions.size() == 1);
	assert(wrapperFunc.size() == 1);
	std::vector<BPatch_point*> * entryLocations = cudaSyncFunctions[0]->findPoint(BPatch_locExit);
	std::vector<BPatch_snippet*> testArgs;
	BPatch_funcCallExpr recordFuncEntry(*(wrapperFunc[0]), testArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(recordFuncEntry,*entryLocations) != false);









// 	std::vector<BPatch_function *> cudaFreeWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_cudaFreeWrapper"), wrapper);
// 	std::vector<BPatch_function *> cudaFreeSyncWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_synchronousCudaFree"), wrapper);
// 	std::vector<BPatch_function *> cudaMallocWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_cudaMallocWrapper"), wrapper);
// 	std::vector<BPatch_function *> cudaMemcpyWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_cudaMemcpyAsyncWrapper"), wrapper);
// 	std::vector<BPatch_function *> mallocWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_MALLOCWrapper"), wrapper);
// 	std::vector<BPatch_function *> freeWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_FREEWrapper"), wrapper);
// 	std::vector<BPatch_function *> syncExit = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_SIGNALSYNC"), wrapper);

// 	std::vector<BPatch_function *> testCudaMemcpySyncWrapper = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_cudaMemcpySyncWrapper"), wrapper);
// 	// std::vector<BPatch_function *> syncExit = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_SyncExit"), wrapper);
// 	// std::vector<BPatch_function *> syncGetStream = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DIOGENES_SUB_333898PRECALL"), wrapper);
// 	assert(cudaFreeWrapper.size() > 0);
// 	assert(cudaMallocWrapper.size() > 0);
// 	assert(cudaMemcpyWrapper.size() > 0);
// 	assert(mallocWrapper.size() > 0);
// 	assert(freeWrapper.size() > 0);
// 	assert(syncExit.size() > 0);
// 	assert(cudaFreeSyncWrapper.size() > 0);
// 	assert(testCudaMemcpySyncWrapper.size() > 0);
// 	// assert(syncGetStream.size() > 0);

// 	RemovePointsPtr remPoints(new RemovePoints());
// 	remPoints->Deserialze(); //callTrans->GetRemoveCalls();
//     remPoints->BuildTreeMap();
// 	std::shared_ptr<InstrimentationTracker> tracker(new InstrimentationTracker());
// 	std::vector<BPatch_function *> all_functions;
// 	BPatch_image * img = _proc->GetAddressSpace()->getImage();
// //	std::vector<BPatch_object *> objs;
// //	img->getObjects(objs);
// 	/*{
// 	    StackPointVec tmpPoints = remPoints->GetAllStackPoints();
//     //    std::set<std::string> libsToCheck = remPoints->GetLibs();
// 	    for (auto n : tmpPoints) {
// 	        std::vector<BPatch_function *> funcsInClass = ops->FindFunctionsByLibnameOffset(_proc->GetAddressSpace(), n.libname, n.libOffset, false);
// 	        assert(funcsInClass.size() > 0);
// 	        std::unordered_set<uint64_t> tmpTracker;

// 	        for (auto i : funcsInClass)
// 	        	tmpTracker.insert((uint64_t)i->getBaseAddr());
// 	        for (auto i : funcsInClass)
// 	        	if (tmpTracker.find((uint64_t)i->getBaseAddr()+0x8) == tmpTracker.end())
// 	            	_dyninstFunctions[(uint64_t)i->getBaseAddr()] = std::shared_ptr<DyninstFunction>(new DyninstFunction(_proc, i, tracker, _bmap));
// 	    }	    
// 	}*/


// 	BPatch_variableExpr * cmemcpyvar = img->findVariable("DIOGENES_TMP_GLOBALC_CMEMCPY_PTR", true);
// 	BPatch_variableExpr * cmemcpyasyncvar = img->findVariable("DIOGENES_TMP_GLOBALC_CMEMCPYASYNC_PTR", true);
// 	{
// 		std::vector<BPatch_function *> cmemcpy = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("cuMemcpyDtoH_v2"), libcuda);
// 		//std::string tmpLib = cmemcpy[0]->getModule()->getObject()->pathName();
// 		void * tempAddr = (cmemcpy[0]->getBaseAddr());
// 		cmemcpyvar->writeValue(&tempAddr, 8);
// 		//bool found = false;
// 		//for (auto i : cmemcpy) {
// 			//std::string tmpLib = i->getModule()->getObject()->pathName();
// 			//if (tmpLib.find("cufft") != std::string::npos) 
// 		/*		if (found == false) {
// 					found = true;
// 					void * tempAddr = (i->getBaseAddr());
// 					cmemcpyvar->writeValue(&tempAddr, 8);
// 				} else {
// 					assert("FOUND 2 CUDAMEMCPY FUNCTIONS!!!!" == 0);
// 				}*/
// 		//}
// 		//assert(found != false);
// 	}
// 	{
// 		std::vector<BPatch_function *> cmemcpy = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("cuMemcpyDtoHAsync_v2"), libcuda);
// 		void * tempAddr = (cmemcpy[0]->getBaseAddr());
// 		cmemcpyasyncvar->writeValue(&tempAddr, 8);
// 		/*bool found = false;
// 		for (auto i : cmemcpy) {
// 			std::string tmpLib = i->getModule()->getObject()->pathName();
// 			if (tmpLib.find("cufft") != std::string::npos) 
// 				if (found == false) {
// 					found = true;
// 					void * tempAddr = (i->getBaseAddr());
// 					cmemcpyasyncvar->writeValue(&tempAddr, 8);
// 				} else {
// 					assert("FOUND 2 CUDAMEMCPYASYNC FUNCTIONS!!!!" == 0);
// 				}
// 		}
// 		assert(found != false);*/
// 	}	
//     img->getProcedures(all_functions);
//     for (auto i : all_functions) {
// 	    std::string tmpLib = i->getModule()->getObject()->pathName();
// 	    void * start;
// 	    void * end;
// 	    i->getAddressRange(start, end);
// 	    std::shared_ptr<std::vector<BPatch_point *> > tmp(i->findPoint(BPatch_locEntry));
// 	    uint64_t lOffset = (uint64_t) start;
// 	    uint64_t dist = ((uint64_t) end) - ((uint64_t) start); 
// 	    if (tmp->size() > 0) {
// 	        if (!ops->GetFileOffset(_proc->GetAddressSpace(), (*tmp)[0], lOffset, true))
// 		        lOffset = (uint64_t)start;
// 	    }
// 		_dyninstFunctions[(uint64_t)i->getBaseAddr()] = std::shared_ptr<DyninstFunction>(new DyninstFunction(_proc, i, tracker, _bmap));
// 	}

// 	std::vector<uint64_t> deleteList;
// 	/*for (auto i : _dyninstFunctions) {
// 		if(_dyninstFunctions.find(i.first + 0x8) != _dyninstFunctions.end())
// 			deleteList.push_back(i.first);
// 	}
// 	for (auto i : deleteList) 
// 		if (_dyninstFunctions.find(i) != _dyninstFunctions.end())
// 			_dyninstFunctions.erase(i);
// 	*/
// 	std::string binary_name = std::string("main");
// 	std::string tmpLibname = std::string("");
// 	std::string tmpFuncName = std::string("");
// 	_proc->BeginInsertionSet();

// 	DetectDuplicateStackpoints dupCheck;

// 	int freesReplaced = 0;
// 	int freesSkipped = 0;
// 	int mallocsReplaced = 0;
// 	int mallocsSkipped = 0;
// 	for (auto i : _dyninstFunctions) {
// 		// if (i.second->IsExcludedFunction(LOAD_STORE_INST))
// 		// 	continue;
// 		i.second->GetFuncInfo(tmpLibname, tmpFuncName);

// 		if (tmpFuncName.find("DIOGENES_") != std::string::npos || tmpLibname.find("/lib/plugins/") != std::string::npos)
// 			continue;
// 		if (tmpLibname.find("/usr/lib64/libc-2.17.so") != std::string::npos || tmpLibname.find("libcudart") != std::string::npos  || tmpLibname.find("libcuda") != std::string::npos)
// 			continue;
		
// 		//if (tmpLibname.find(binary_name) != std::string::npos) {
// 			std::vector<DyninstCallsite> callsites;
// 			i.second->GetCallsites(callsites);
// 			for (auto x : callsites) {
// 				if (*(x.GetCalledFunction()) == std::string("cuMemcpyDtoH_v2")){
// 					std::cerr << "[DB]CS Function Name - " << *(x.GetCalledFunction()) <<  " @ address= " << std::hex << x.GetPointAddress() << " in function " << tmpFuncName << std::dec << std::endl;
// 					x.ReplaceFunctionCall(testCudaMemcpySyncWrapper[0]);
// 				}

				// std::cerr << "[DB]CS Function Name - " << *(x.GetCalledFunction()) <<  " @ address= " << std::hex << x.GetPointAddress() << std::dec << std::endl;
				// if (*(x.GetCalledFunction()) == std::string("cudaFree")){
				// 	#ifdef FIX_CUDAMEMFREE
				// 	//if (remPoints->CheckArray(CUFREE_REP, x.GetStackPoint())) {		
				// 	    if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
    // 					    continue;			
				// 		freesReplaced++;
				// 		std::cerr << "Found function call to cudaFree in " << tmpFuncName 
				// 		          << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
				// 		x.ReplaceFunctionCall(cudaFreeWrapper[0]);
				// 	/*
				// 	} else if (remPoints->CheckArray(CUFREE_REQUIRED, x.GetStackPoint())) {		
				// 	    if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
    // 					    continue;			
				// 		freesReplaced++;
				// 		std::cerr << "Found function call to cudaFree in " << tmpFuncName << " within library " 
				// 		          << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
				// 		x.ReplaceFunctionCall(cudaFreeSyncWrapper[0]);
				// 	} else {
				// 		freesSkipped++;
				// 	}*/
				// 	#endif
				// }
				// if (*(x.GetCalledFunction()) == std::string("cudaMalloc")) {
				// 	#ifdef FIX_CUDAMEMFREE
				// 	//if (remPoints->CheckArray(CUMALLOC_REP, x.GetStackPoint())) {
				// 		std::cerr << "Found function call to cudaMalloc in " << tmpFuncName << " within library " 
				// 		          << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
    //     				//if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
    //     				//	continue;    					
				// 		mallocsReplaced++;

				// 		x.ReplaceFunctionCall(cudaMallocWrapper[0]);
				// 	//} else {
				// 	//	mallocsSkipped++;
				// 	//}
				// 	#endif
				// }
				// if (*(x.GetCalledFunction()) == std::string("__GI___libc_malloc")){
				// 	#ifdef FIX_ASYNCTRANS_WWRAP
				// 	if (remPoints->CheckArray(MALLOC_REP, x.GetStackPoint())) {
    //     				if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
    //     					continue;    		
    //     				x.ReplaceFunctionCall(mallocWrapper[0]);						
    //     				std::cerr << "Found function call to malloc in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				// 	}
				// 	#endif
				// 	// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
				// 	// 	continue;
				// 	// x.ReplaceFunctionCall(mallocWrapper[0]);
				// 	// std::cerr << "Found function call to malloc in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				// }
				// if (*(x.GetCalledFunction()) == std::string("__libc_free")){
				// 	#ifdef FIX_ASYNCTRANS_WWRAP
				// 	if (remPoints->CheckArray(FREE_REP, x.GetStackPoint())) {
    //     				if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
    //     					continue;    		
    //     				x.ReplaceFunctionCall(freeWrapper[0]);						
    //     				std::cerr << "Found function call to free in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				// 	}
				// 	#endif
				// 	// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
				// 	// 	continue;
				// 	// x.ReplaceFunctionCall(freeWrapper[0]);
				// 	// std::cerr << "Found function call to free in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;
				// }
				// if (*(x.GetCalledFunction()) == std::string("cudaMemcpyAsync")){
				// 	#if defined(FIX_ASYNCTRANS_WWRAP) || defined(FIX_ASYNCTRANS_NOWRAP)
				// 	if (remPoints->CheckArray(CUMEMCPY_REP, x.GetStackPoint())) {
    //     				if (dupCheck.CheckAndInsert(tmpLibname, x.GetPointFileAddress()) == false)
    //     					continue;  
				// 		//x.ReplaceFunctionCall(cudaMemcpyWrapper[0]);						
    //     				std::cerr << "Found function call to cudaMemcpyAsync in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")" << std::endl;        				  								
				// 	}
				// 	#endif
				// 	// if (!debugOutput.InstrimentFunction(tmpLibname, tmpFuncName,x.GetPointFileAddress()))
				// 	// 	continue;
				// 	// std::cerr << "Found function call to cudaMemcpyAsync in " << tmpFuncName << " within library " << tmpLibname << " (calling " << *(x.GetCalledFunction()) << ")"  << std::endl;
				// 	// x.ReplaceFunctionCall(cudaMemcpyWrapper[0]);
				// }
	//		}
		//}
	//}

/*
	std::vector<std::string> streamSynchronousFunctions = {"cuStreamSynchronize"};
	std::vector<std::string> deviceSynchronousFunctions = {"cuMemcpy","cuMemcpy2D","cuMemcpy2DUnaligned","cuMemcpy3D",
									"cuMemcpyAtoD","cuMemcpyAtoH","cuMemcpyDtoA","cuMemcpyDtoH","cuMemcpyHtoA",
									"cuMemcpyHtoD","cuMemcpyPeer","cuMemFree","cuCtxSynchronize","cuMemFree"};

	for (auto i : streamSynchronousFunctions) {
		std::vector<BPatch_function *> cudaFunction = ops->FindFuncsByName(_proc->GetAddressSpace(), i, libcuda);
		assert(cudaFunction.size() == 1);
		std::vector<BPatch_point*> * entryPoints = cudaFunction[0]->findPoint(BPatch_locExit);
		std::vector<BPatch_snippet*> recordArgs;
		recordArgs.push_back(new BPatch_paramExpr(0));
		BPatch_funcCallExpr entryExpr(*(syncExit[0]), recordArgs);	
		assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	}


	for (auto i : deviceSynchronousFunctions) {
		std::vector<BPatch_function *> cudaFunction = ops->FindFuncsByName(_proc->GetAddressSpace(), i, libcuda);
		assert(cudaFunction.size() == 1);
		std::vector<BPatch_point*> * entryPoints = cudaFunction[0]->findPoint(BPatch_locExit);
		std::vector<BPatch_snippet*> recordArgs;
		recordArgs.push_back(new BPatch_constExpr(uint64_t(0x999)));
		BPatch_funcCallExpr entryExpr(*(syncExit[0]), recordArgs);	
		assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	}
*/

	// std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, ops->GetSyncFunctionLocation());
	// std::vector<BPatch_function*> cudaSyncStreamFunc = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, 0x333898);
	// assert(cudaSyncFunctions.size() > 0);
	// assert(cudaSyncStreamFunc.size() > 0);
	// {
	// 	std::vector<BPatch_point*> * entryPoints = cudaSyncFunctions[0]->findPoint(BPatch_locExit);
	// 	std::vector<BPatch_snippet*> recordArgs;
	// 	BPatch_funcCallExpr entryExpr(*(syncExit[0]), recordArgs);	
	// 	assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	// }
	// {
	// 	std::vector<BPatch_point*> * entryPoints = cudaSyncStreamFunc[0]->findPoint(BPatch_locEntry);
	// 	std::vector<BPatch_snippet*> recordArgs;
	// 	recordArgs.push_back(new BPatch_paramExpr(1));
	// 	BPatch_funcCallExpr entryExpr(*(syncGetStream[0]), recordArgs);	
	// 	assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	// }
	// std::cerr << "[FixCudaProblems::InsertAnalysis] CudaFree's Replaced = " << freesReplaced << " Skipped = " << freesSkipped << std::endl;
	// std::cerr << "[FixCudaProblems::InsertAnalysis] cudaMallocs Replaced = " << mallocsReplaced << " Skipped = " << mallocsSkipped << std::endl;
	// std::vector<BPatch_function*> cudaSyncFunctions = ops->GetFunctionsByOffeset(_proc->GetAddressSpace(), libcuda, ops->GetSyncFunctionLocation());
	// assert(cudaSyncFunctions.size() > 0);

	// std::vector<BPatch_point*> * entryPoints = cudaSyncFunctions[0]->findPoint(BPatch_locExit);
	// std::vector<BPatch_snippet*> recordArgs;
	// BPatch_funcCallExpr entryExpr(*(syncExit[0]), recordArgs);	
	// assert(_proc->GetAddressSpace()->insertSnippet(entryExpr,*entryPoints) != NULL);
	// 
}


void FixCudaProblems::PostProcessing() {

}
