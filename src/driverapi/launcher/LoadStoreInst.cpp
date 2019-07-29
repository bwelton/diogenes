#include "LoadStoreInst.h"
#include "Instruction.h"

LoadStoreInst::LoadStoreInst(BPatch_addressSpace * addrSpace, BPatch_image * img) :
	_addrSpace(addrSpace), _img(img), _started(false), _funcId(0) {
	_runOneTime = false;
	_debugPrinting = 0;	
}


void LoadStoreInst::InsertGotchaEntries() {
	// Find function main and DefineBindings, insert call to DefineBindings into main.
	BPatch_function * main;
	BPatch_function * DefineBinders;
	assert(1 == _dynOps.FindFuncByName(_addrSpace, main, std::string("main")));
	assert(1 == _dynOps.FindFuncByName(_addrSpace, DefineBinders, std::string("DefineBinders")));
	std::vector<BPatch_point*> * entryPoints = main->findPoint(BPatch_locEntry);
	main = _dynOps.GetPOWERFunction(main);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*DefineBinders, recordArgs);
	dynamic_cast<BPatch_process*>(_addrSpace)->oneTimeCode(entryExpr);
	//assert(_addrSpace->insertSnippet(entryExpr,*entryPoints) != NULL);
}

void LoadStoreInst::InsertEntryExitSnippets(BPatch_function * func, std::vector<BPatch_point*> * points) {
	std::string libname = func->getModule()->getObject()->pathName();
	_logFile << "[LoadStoreInst][EntryExit] Inserting entry exit instrimentation into - " << func->getName() << " with ids: ";
	for (int nmp = 0; nmp < points->size(); nmp++){
		auto i = (*points)[nmp];
		// if (nmp != 6)
		// 	continue;
		// if (i->getCalledFunction() == NULL)
		// 	continue;
		// std::vector<std::pair<Dyninst::InstructionAPI::Instruction::Ptr, Dyninst::Address> > instructionVector;
		// i->getBlock()->getInstructions(instructionVector);
		// Dyninst::InstructionAPI::Instruction::Ptr pointInstruction = NULL;
		// for (auto z : instructionVector)
		// 	if(z.second == (uint64_t)i->getAddress())
		// 	{
		// 		pointInstruction = z.first;
		// 		break;
		// 	}
		// std::string instString;
		// if (pointInstruction != NULL)
		// 	instString = pointInstruction->format();
		// else 
		// 	continue;
		// if (instString.find("call") == std::string::npos)
		// 	continue;
		std::vector<BPatch_point*> singlePoint;
		singlePoint.push_back(i);
		uint64_t id;
		uint64_t libOffsetAddr = 0;
		if (_dynOps.GetFileOffset(_addrSpace, i, libOffsetAddr, true))
			id = _binLoc.StorePosition(libname, libOffsetAddr);
		else
			id = _binLoc.StorePosition(libname, (uint64_t) i->getAddress());
		_logFile << "[LoadStoreInst][EntryExit] \tInstruction at point " << id << " , " << std::hex << (uint64_t) i->getAddress() << std::dec 
				 << " libname: " << libname << " libOffsetPosition: " << libOffsetAddr << std::endl;
		std::vector<BPatch_snippet*> recordArgs;
		recordArgs.push_back(new BPatch_constExpr(id));
		BPatch_funcCallExpr entryExpr(*_entryFunction, recordArgs);
		BPatch_funcCallExpr exitExpr(*_exitingFunction, recordArgs);
		//_logFile << id << ",";
		_instTracker.RecordInstrimentation(CALL_TRACING, func, singlePoint[0]);
		if (_addrSpace->insertSnippet(entryExpr,singlePoint, BPatch_callBefore) == NULL) {
			_logFile << "[LoadStoreInst][EntryExit] \t\t ERROR! Could not insert entry tracking into " << func->getName() << std::endl;
		}
		if (_addrSpace->insertSnippet(exitExpr,singlePoint,BPatch_callAfter) == NULL) {
			_logFile << "[LoadStoreInst][EntryExit] \t\t ERROR! Could not insert exit tracking into " << func->getName() << std::endl;
		}
	}
}

void LoadStoreInst::WrapEntryAndExit(std::map<uint64_t, StackRecord> & syncStacks) {
	// Get all the functions in the binary
	std::vector<BPatch_object *> objects;
	_img->getObjects(objects);
	int count = 0;
	std::ofstream functionDump;
	functionDump.open("temporary_list.txt", std::ofstream::out);

	for (auto i : syncStacks) {
		i.second.PrintStack();
		std::vector<StackPoint> points = i.second.GetStackpoints();
		for (auto z : points) {
			_logFile << "[LoadStoreInst][EntryExit] Attempting to find - " << z.funcName << std::endl;
			std::cerr << "[LoadStoreInst][EntryExit] Attempting to find - " << z.funcName << std::endl;

			BPatch_function * func;
			if(_dynOps.FindFuncByStackPoint(_addrSpace, func, z) <= 0){
				_logFile << "[LoadStoreInst][EntryExit] Could not find function - " << z.funcName << std::endl;
				std::cerr << "[LoadStoreInst][EntryExit] Could not find function - " << z.funcName << std::endl;
				continue;
			}
			// Correct power issues
			func = _dynOps.GetPOWERFunction(func);
			std::cerr << "[LoadStoreInst][EntryExit] Power Function - " << func->getName() << std::endl;
			std::vector<BPatch_point*> * funcCalls = func->findPoint(BPatch_locSubroutine);
			if (_instTracker.ShouldInstriment(func, funcCalls, CALL_TRACING)) {
				_logFile << "[LoadStoreInst][EntryExit] Inserting exit/entry info into - " << z.funcName << "," << func->getModule()->getObject()->pathName() << std::endl;
				InsertEntryExitSnippets(func, funcCalls);
				functionDump << func->getMangledName() << std::endl;
			} else {
				_logFile << "[LoadStoreInst][EntryExit] Rejected function - " << z.funcName << std::endl;
			}		
		}
		count++;
	}	
	functionDump.close();	
}	

void LoadStoreInst::InsertSyncNotifierSnippet(BPatch_function * func, uint64_t offset) {
	assert(func != NULL);
	std::vector<BPatch_point*> * entryPoints = func->findPoint(BPatch_locEntry);
	std::string tmp = func->getModule()->getObject()->pathName();
	uint64_t id = _binLoc.StorePosition(tmp,offset);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*_enterSync, recordArgs);
	if (_addrSpace->insertSnippet(entryExpr,*entryPoints) == NULL) {
		std::cerr << "[LoadStoreInst][SyncNotifier] FATAL ERROR! Insertion of notifier into libcuda.so - failed! Callname: " << func->getName() << std::endl;
	}
}

void LoadStoreInst::InsertSyncCallNotifier() {
	InsertSyncNotifierSnippet(_libcudaSync, INTERNAL_SYNC_LS);
}

void LoadStoreInst::InsertLoadStoreSnippets(BPatch_function * func, std::vector<BPatch_point*> * points) {
	std::string libname = func->getModule()->getObject()->pathName();
	_logFile << "[LoadStoreInst][LoadStoreSnippet] Inserting load store instrimentation into - " << func->getName() << "," << func->getModule()->getObject()->pathName() << "\n";
	_instTracker.RecordInstrimentation(LOAD_STORE_INST, func, points);
	int count = 0;
	for (auto i : *points) {
		uint64_t libOffsetAddr = 0;
		uint64_t id = 0;
		std::vector<BPatch_point*> singlePoint;
		singlePoint.push_back(i);

		if (_dynOps.GetFileOffset(_addrSpace, i, libOffsetAddr))
			id = _binLoc.StorePosition(libname, libOffsetAddr);
		else
			id = _binLoc.StorePosition(libname, (uint64_t) i->getAddress());
		// if (libOffsetAddr != 0x102b35c8)
		// 	continue;
		// if (libOffsetAddr == 0x102b3720 || libOffsetAddr == 0x102b3718 || libOffsetAddr == 0x102b36e0 || libOffsetAddr == 0x102b36c4 || libOffsetAddr == 0x102b36c0 || libOffsetAddr == 0x102b36bc ||
		// 	libOffsetAddr == 0x102b36b8 || libOffsetAddr == 0x102b36b4 || libOffsetAddr == 0x102b369c ||
		// 	libOffsetAddr == 0x102b358c ||
		// 	libOffsetAddr == 0x102b35b8 ||
		// 	//libOffsetAddr == 0x102b35c8 ||
		// 	libOffsetAddr == 0x102b35d4 ||
		// 	libOffsetAddr == 0x102b35c0)
		// 	continue;

			/*
			//libOffsetAddr == 0x102b35c8 ||
			//libOffsetAddr == 0x102b35d4 ||
			libOffsetAddr == 0x102b35e4 ||
			libOffsetAddr == 0x102b3618 ||
			libOffsetAddr == 0x102b3620 ||
			libOffsetAddr == 0x102b362c ||
			libOffsetAddr == 0x102b3630 ||

			libOffsetAddr == 0x102b3650 ||
			libOffsetAddr == 0x102b3684 ||
			libOffsetAddr == 0x102b368c ||
			libOffsetAddr == 0x102b3698)*/
			//continue;
		std::vector<BPatch_snippet*> recordArgs;
		BPatch_snippet * loadAddr = new BPatch_effectiveAddressExpr();
		recordArgs.push_back(loadAddr);
		recordArgs.push_back(new BPatch_constExpr(id));
		_logFile << "[DEBUGEMERG] Inst point " << id << " Original Address: " << std::hex << i->getAddress() << std::dec <<  " in function " << func->getName() << std::endl;
		if (_debugPrinting < 40 || id == 4988 || id == 4989){
			std::cerr << "[DEBUGEMERG] Inst point " << id << " Original Address: " << std::hex << i->getAddress() << std::dec <<  " in function " << func->getName() << std::endl;
			//_logFile << "[DEBUGEMERG] Inst point " << id << " Original Address: " << std::hex << i->getAddress() << std::dec <<  " in function " << func->getName() << std::endl;
			_debugPrinting++;
			// if (id == 4989)
			//  	assert(id != 4989);
		}
		BPatch_funcCallExpr recordAddrCall(*_recordMemAccess, recordArgs);
		if (_addrSpace->insertSnippet(recordAddrCall,singlePoint) == NULL) {
			std::cerr << "[LoadStoreInst][LoadStoreSnippet]\t\tCould not insert load store instrimentation into " << id << " in function " << func->getName() << std::endl;
		}
		// count++;
		// if (count >= 32)
		// 	break;
	}
}

void LoadStoreInst::AddSpecialCase(std::string functionName, std::string libName, uint64_t id) {
	// std::vector<BPatch_object *> objects;
	// _img->getObjects(objects);
	// for (auto i : objects)
	// 	std::cerr << i->pathName() << std::endl;
	std::vector<BPatch_function *> specFuncs;
	_img->findFunction(functionName.c_str(), specFuncs);
	for (auto i : specFuncs) {
		std::cerr << i->getMangledName() << std::endl;
	}

	std::string specModule = libName;
	BPatch_object * specObj = _dynOps.FindObjectByName(_addrSpace,  specModule, false);
	uint64_t specID = id;
	assert(specObj != NULL);

	//specObj->findFunction(functionName,specFuncs,false);
	BPatch_function * actual = NULL;
	for (auto i : specFuncs) {
		std::cerr << i->getMangledName() << std::endl;
		std::cerr << i->getModule()->getObject()->pathName() << std::endl;
		if (i->getMangledName() == functionName){
			actual = i;
			break;
		}
	}
	//assert(specFuncs.size() == 1);
	assert(actual != NULL);
	std::set<BPatch_opCode> axs;
	axs.insert(BPatch_opLoad);
	axs.insert(BPatch_opStore);
	BPatch_function * memcpyFunc = _dynOps.GetPOWERFunction(actual);
	assert(memcpyFunc != NULL);
	std::vector<BPatch_point*> * loadsAndStores = memcpyFunc->findPoint(axs);
	assert(loadsAndStores->size() > 0);

	uint64_t libOffset = 0;
	assert(_dynOps.GetFileOffset(_addrSpace, (*loadsAndStores)[0], libOffset) == true);
	std::string specObjPath = specObj->pathName();
	_binLoc.SetAbsoluteID(specID, specObjPath, libOffset);
}


void LoadStoreInst::InsertLoadStoreInstrimentation() {
	std::vector<BPatch_function *> all_functions;
	_img->getProcedures(all_functions);
	//_instTracker.PowerFunctionFix(all_functions);
	std::set<BPatch_opCode> axs;
	axs.insert(BPatch_opLoad);
	axs.insert(BPatch_opStore);
	for (auto i : all_functions) {
		std::vector<BPatch_point*> * loadsAndStores = i->findPoint(axs);
		if (_instTracker.ShouldInstriment(i, loadsAndStores, LOAD_STORE_INST)) {
			InsertLoadStoreSnippets(i, loadsAndStores);
		}
	}

	// Now lets add the special cases
	// Start at -1 from limit max
	//AddSpecialCase(std::string("memcpy"), std::string("/usr/lib64/libc-2.17"), std::numeric_limits<uint64_t>::max() - 2);

	//specObj->pathName()
}

void LoadStoreInst::FixLSStackFiles() {
	// Converts:
	// LS_trace.bin -> {LS_trace.bin, LS_tracekey.txt}
	// LS_stackkey.bin -> LS_stackkey.txt
	std::cerr << "Runing onetime code on exit of LS" << std::endl;
	_binLoc.DumpLocationMap(std::string("LS_BinLoc.txt"));
	ReadLoadStoreFiles tmp(&_binLoc);
	tmp.OutputTraceKey(std::string("LS_trace.bin"), std::string("LS_tracekey.txt"));
	tmp.CreateStackKey(std::string("LS_stackkey.bin"), std::string("LS_stackkey.txt"));

}

LoadStoreInst::~LoadStoreInst() {
	if(_runOneTime)
		FixLSStackFiles();
}

void LoadStoreInst::SetWrappedFunctions(std::vector<std::string> & wrappedFunctions ) {
	_wrappedFunctions = wrappedFunctions;
	_instTracker.AddAlreadyInstrimented(wrappedFunctions);
}

bool LoadStoreInst::InstrimentAllModules(bool finalize, std::vector<uint64_t> & skips, uint64_t & instUntil, std::vector<std::string> & syncFunctions, std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks) {
	Setup();
	BeginInsertionSet();
	std::vector<BPatch_function *> all_functions;
	_img->getProcedures(all_functions);
	_instTracker.PowerFunctionFix(all_functions);
	InsertGotchaEntries();
	WrapEntryAndExit(syncStacks);
	InsertSyncCallNotifier();
	//InsertSyncCallNotifier(points);
	InsertLoadStoreInstrimentation();
	_runOneTime =true;
	if (finalize)
		Finalize();

	return true;
}


void LoadStoreInst::Setup() {
	std::cerr << "[LoadStoreInst] Setup" << std::endl;
	_logFile.open("LS_log.txt", std::ofstream::out);
	_addrSpace->loadLibrary("libDriverAPIWrapper.so");
	assert(_dynOps.FindFuncByName(_addrSpace, _entryFunction, std::string("RECORD_FUNCTION_ENTRY")) == 1);
	assert(_dynOps.FindFuncByName(_addrSpace, _exitingFunction, std::string("RECORD_FUNCTION_EXIT")) == 1);
	assert(_dynOps.FindFuncByName(_addrSpace, _enterSync, std::string("SYNC_CAPTURE_SYNC_CALL")) == 1);
	assert(_dynOps.FindFuncByName(_addrSpace, _recordMemAccess, std::string("SYNC_RECORD_MEM_ACCESS")) == 1);
	assert(_dynOps.FindFuncByLibnameOffset(_addrSpace, _libcudaSync, std::string("libcuda.so"), INTERNAL_SYNC_LS, false) == 1);
}


bool LoadStoreInst::InstrimentNewModule(BPatch_object * obj, bool finalize) {
	// Adds instrimentation to a module loaded at a later point in time.
	return false;
}

void LoadStoreInst::Finalize() {
	// Finalizes the insertion set of dyninst
	if (_started)
		_addrSpace->finalizeInsertionSet(false);
	_started = false;
}


void LoadStoreInst::BeginInsertionSet() {
	// Begin the insertion set;
	if(!_started)
		_addrSpace->beginInsertionSet();
	_started = true;
}
