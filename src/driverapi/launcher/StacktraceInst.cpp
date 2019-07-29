#include "StacktraceInst.h"
#define USE_BPATCHINSERT 1
StacktraceInst::StacktraceInst(BPatch_addressSpace * addrSpace, BPatch_image * img) :
	_addrSpace(addrSpace), _img(img), _curID(0) {
	

}

void StacktraceInst::InsertStackInst() {
	Setup();
	// Begin the insertion set
	_addrSpace->beginInsertionSet();

#ifdef USE_BPATCHINSERT
	// Testing
	//assert(cuCtxSync.size() > 0);
	std::vector<BPatch_point*> * entryLocations = _cudaSync->findPoint(BPatch_locEntry);
	std::vector<BPatch_snippet*> testArgs;
	BPatch_funcCallExpr recordFuncEntry(*_wrapperFunc, testArgs);
	assert(_addrSpace->insertSnippet(recordFuncEntry,*entryLocations) != false);
#else
	assert(_addrSpace->wrapFunction(_cudaSync, _wrapperFunc, _wrapSym) != false);
#endif
	_addrSpace->finalizeInsertionSet(false);

	BPatch_process * tmpProc = dynamic_cast<BPatch_process*>(_addrSpace);	
	// tmpProc->dumpImage("processImage.img");
	// tmpProc->dumpCore("processCore.core", false);
}

void StacktraceInst::Setup() {
	std::cerr << "[StacktraceInst] Starting Setup.... " << std::endl;
	std::vector<BPatch_function *> wrapperFunc;
#ifdef USE_BPATCHINSERT
	_img->findFunction("RecordStack", wrapperFunc);
#else
	_img->findFunction("STACK_SyncWrapper", wrapperFunc);
#endif
	assert(wrapperFunc.size() > 0);
	_wrapperFunc = wrapperFunc[0];
	
	BPatch_function * cudaSync = NULL;
	std::vector<BPatch_object *> imgObjs;
	_img->getObjects(imgObjs);
	for (auto i : imgObjs){
		if (i->name().find("libcuda.so") == std::string::npos)
			continue;
		// Found libcuda
		uint64_t offsetAddress = i->fileOffsetToAddr(INTERNAL_SYNC_ST);
		cudaSync = _img->findFunction(offsetAddress);
		break;
	}
	assert(cudaSync != NULL);
	std::cerr << "[StacktraceInst] Cudasync has been found " << std::endl;
	_cudaSync = cudaSync;

	// Find the symbol to wrap
	BPatch_object * obj = _wrapperFunc->getModule()->getObject();
	Dyninst::SymtabAPI::Symtab * symt = Dyninst::SymtabAPI::convert(obj);
	std::vector<Dyninst::SymtabAPI::Symbol *> tmp;
	symt->getAllSymbols(tmp);
	_wrapSym = NULL;
	std::cerr << "[StacktraceInst] Finding wrapper symbol.... " << std::endl;
	for (auto i : tmp) {
		if (i->getPrettyName().find("SynchronizationWrapper") != std::string::npos) {
			if (i->getModule() != NULL)
				std::cerr << i->getModule()->fullName() << std::endl;
			// if (i->getModule()->fileName().find("STraceWrapSync") == std::string::npos)
			// 	continue;
			_wrapSym = i;
			std::cerr << "Found synchtonization Wrapper" << std::endl;
			//break;
		}
	}
	assert(_wrapSym != NULL);

	// _img->findFunction("STACK_SyncWrapper", wrapperFunc);
}
