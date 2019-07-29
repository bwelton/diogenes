#include "APICaptureInstrimentation.h"
APICaptureInstrimentation::APICaptureInstrimentation(std::shared_ptr<DyninstProcess> proc) : _proc(proc), _compleated(false) { 

}

bool APICaptureInstrimentation::InsertInstrimentation() {
	/**
	 * Inserts API Capture instrimentation. Currently this works by first loading in the driverapiwrapper library and 
	 * making a one time call to DefineBinders in that library. Definebinders will use libgotcha to warp all of the 
	 * public API functions (with a few exceptions). 
	 *
	 * Assumptions: The process is alive at time of entry. 
	 */ 
	if (_compleated == true)
		return true;
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	BPatch_object * libCuda = _proc->LoadLibrary(std::string("libcuda.so.1"));
	BPatch_object * driverAPIWrapper = _proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/libDriverAPIWrapper.so"));
	_proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libDriverAPITrace.so"));
	_proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libDataTransfer.so"));
	_proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libDataSyncTimer.so"));
	_proc->LoadLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libSynchTool.so"));
	std::vector<BPatch_function *> binderFunctions = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("DefineBinders"), driverAPIWrapper);

	// We expect only a single call with this name, fail if there is more than one.
	//assert(binderFunction.size() == 1);
	BPatch_function * binderFunction;
	assert(1 == ops->FindFuncByName(_proc->GetAddressSpace(), binderFunction, std::string("DefineBinders")));

	std::cerr << "Function returned by old interface: " << binderFunction->getName() << " at offset: " << binderFunction->getBaseAddr() << " with pointer value " << binderFunction << std::endl;
	std::cerr << "Function returned by new interface: " << binderFunctions[0]->getName() << " at offset: " << binderFunctions[0]->getBaseAddr() << " with pointer value " << binderFunctions[0] << std::endl;
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*(binderFunctions[0]), recordArgs);
	std::cerr << "[APICaptureInstrimentation::InsertInstrimentation] Fireing off one time call to setup API Capture Instrimentation\n";
	dynamic_cast<BPatch_process*>(_proc->GetAddressSpace())->oneTimeCode(entryExpr);
	_compleated = true;
	return true;
}
