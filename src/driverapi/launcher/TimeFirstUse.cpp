#include "TimeFirstUse.h"
#include "Instruction.h"

TimeFirstUse::TimeFirstUse(BPatch_addressSpace * addrSpace, BPatch_image * img) :
	_addrSpace(addrSpace), _img(img), _started(false), _funcId(0) {
	_runOneTime = false;
	_debugPrinting = 0;	
}


void TimeFirstUse::InsertGotchaEntries() {
	// Find function main and DefineBindings, insert call to DefineBindings into main.
	BPatch_function * main;
	BPatch_function * DefineBinders;
	assert(1 == _dynOps.FindFuncByName(_addrSpace, main, std::string("main")));
	assert(1 == _dynOps.FindFuncByName(_addrSpace, DefineBinders, std::string("DefineBinders")));
	BPatchPointVecPtr entryPoints(main->findPoint(BPatch_locEntry));
	main = _dynOps.GetPOWERFunction(main);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*DefineBinders, recordArgs);
	dynamic_cast<BPatch_process*>(_addrSpace)->oneTimeCode(entryExpr);
	//assert(_addrSpace->insertSnippet(entryExpr,*entryPoints) != NULL);
}

void TimeFirstUse::InsertEntryExitSnippets(BPatch_function * func, std::vector<BPatch_point*> * points) {
	std::string libname = func->getModule()->getObject()->pathName();
	_logFile << "[TimeFirstUse][EntryExit] Inserting entry exit instrimentation into - " << func->getName() << " with ids: ";
	for (int nmp = 0; nmp < points->size(); nmp++){
		auto i = (*points)[nmp];
		std::vector<BPatch_point*> singlePoint;
		singlePoint.push_back(i);
		uint64_t id;
		uint64_t libOffsetAddr = 0;
		if (_dynOps.GetFileOffset(_addrSpace, i, libOffsetAddr, true))
			id = _binLoc.StorePosition(libname, libOffsetAddr);
		else
			id = _binLoc.StorePosition(libname, (uint64_t) i->getAddress());
		_logFile << "[TimeFirstUse][EntryExit] \tInstruction at point " << id << " , " << std::hex << (uint64_t) i->getAddress() << std::dec 
				 << " libname: " << libname << " libOffsetPosition: " << libOffsetAddr << std::endl;
		std::vector<BPatch_snippet*> recordArgs;
		recordArgs.push_back(new BPatch_constExpr(id));
		BPatch_funcCallExpr entryExpr(*_entryFunction, recordArgs);
		BPatch_funcCallExpr exitExpr(*_exitingFunction, recordArgs);
		//_logFile << id << ",";
		_instTracker.RecordInstrimentation(CALL_TRACING, func, singlePoint[0]);
		if (_addrSpace->insertSnippet(entryExpr,singlePoint, BPatch_callBefore) == NULL) {
			_logFile << "[TimeFirstUse][EntryExit] \t\t ERROR! Could not insert entry tracking into " << func->getName() << std::endl;
		}
		if (_addrSpace->insertSnippet(exitExpr,singlePoint,BPatch_callAfter) == NULL) {
			_logFile << "[TimeFirstUse][EntryExit] \t\t ERROR! Could not insert exit tracking into " << func->getName() << std::endl;
		}
	}
}

void TimeFirstUse::WrapEntryAndExit(std::map<uint64_t, StackRecord> & syncStacks) {
	// Get all the functions in the binary
	std::vector<BPatch_object *> objects;
	_img->getObjects(objects);
	int count = 0;
	for (auto i : syncStacks) {
		i.second.PrintStack();
		std::vector<StackPoint> points = i.second.GetStackpoints();
		for (auto z : points) {
			_logFile << "[TimeFirstUse][EntryExit] Attempting to find - " << z.funcName << std::endl;
			std::cerr << "[TimeFirstUse][EntryExit] Attempting to find - " << z.funcName << std::endl;

			BPatch_function * func;
			if(_dynOps.FindFuncByStackPoint(_addrSpace, func, z) <= 0){
				_logFile << "[TimeFirstUse][EntryExit] Could not find function - " << z.funcName << std::endl;
				std::cerr << "[TimeFirstUse][EntryExit] Could not find function - " << z.funcName << std::endl;
				continue;
			}
			// Correct power issues
			func = _dynOps.GetPOWERFunction(func);
			std::cerr << "[TimeFirstUse][EntryExit] Power Function - " << func->getName() << std::endl;
			std::vector<BPatch_point*> * funcCalls = func->findPoint(BPatch_locSubroutine);
			if (_instTracker.ShouldInstriment(func, funcCalls, CALL_TRACING)) {
				_logFile << "[TimeFirstUse][EntryExit] Inserting exit/entry info into - " << z.funcName << "," << func->getModule()->getObject()->pathName() << std::endl;
				InsertEntryExitSnippets(func, funcCalls);
			} else {
				_logFile << "[TimeFirstUse][EntryExit] Rejected function - " << z.funcName << std::endl;
			}		
		}
		count++;
	}		
}	

void TimeFirstUse::InsertSyncNotifierSnippet(BPatch_function * func, uint64_t offset) {
	assert(func != NULL);
	std::vector<BPatch_point*> * entryPoints = func->findPoint(BPatch_locEntry);
	std::string tmp = func->getModule()->getObject()->pathName();
	uint64_t id = _binLoc.StorePosition(tmp,offset);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*_enterSync, recordArgs);
	if (_addrSpace->insertSnippet(entryExpr,*entryPoints) == NULL) {
		std::cerr << "[TimeFirstUse][SyncNotifier] FATAL ERROR! Insertion of notifier into libcuda.so - failed! Callname: " << func->getName() << std::endl;
	}
}

void TimeFirstUse::InsertSyncCallNotifier() {
	InsertSyncNotifierSnippet(_libcudaSync, INTERNAL_SYNC_LS);
}

void TimeFirstUse::InsertLoadStoreSnippets(BPatch_function * func, std::vector<BPatch_point*> * points, uint64_t id) {
	std::string libname = func->getModule()->getObject()->pathName();
	_logFile << "[TimeFirstUse][LoadStoreSnippet] Inserting load store instrimentation into - " << func->getName() << "," << func->getModule()->getObject()->pathName() << "\n";
	_instTracker.RecordInstrimentation(LOAD_STORE_INST, func, points);
	for (auto i : *points) {
		uint64_t libOffsetAddr = 0;
		std::vector<BPatch_point*> singlePoint;
		singlePoint.push_back(i);
		// if (_dynOps.GetFileOffset(_addrSpace, i, libOffsetAddr))
		// 	id = _binLoc.StorePosition(libname, libOffsetAddr);
		// else
		// 	id = _binLoc.StorePosition(libname, (uint64_t) i->getAddress());
		std::vector<BPatch_snippet*> recordArgs;
		BPatch_snippet * loadAddr = new BPatch_effectiveAddressExpr();
		recordArgs.push_back(loadAddr);
		recordArgs.push_back(new BPatch_constExpr(id));
		_logFile << "[DEBUGEMERG] Inst point " << id << " Original Address: " << std::hex << i->getAddress() << std::dec <<  " in function " << func->getName() << std::endl;
		// if (_debugPrinting < 40 || id == 4988 || id == 4989){
		// 	std::cerr << "[DEBUGEMERG] Inst point " << id << " Original Address: " << std::hex << i->getAddress() << std::dec <<  " in function " << func->getName() << std::endl;
		// 	//_logFile << "[DEBUGEMERG] Inst point " << id << " Original Address: " << std::hex << i->getAddress() << std::dec <<  " in function " << func->getName() << std::endl;
		// 	_debugPrinting++;
		// 	// if (id == 4989)
		// 	//  	assert(id != 4989);
		// }
		BPatch_funcCallExpr recordAddrCall(*_recordMemAccess, recordArgs);
		if (_addrSpace->insertSnippet(recordAddrCall,singlePoint) == NULL) {
			std::cerr << "[TimeFirstUse][LoadStoreSnippet]\t\tCould not insert load store instrimentation into " << id << " in function " << func->getName() << std::endl;
		}
	}
}


void TimeFirstUse::InsertTimeFirstUserimentation(std::map<uint64_t, std::vector<StackPoint> > LSpoints) {
	for (auto tf : LSpoints) {
		assert(tf.second.size() == 1);
		auto i = tf.second[0];
		auto ident = tf.first;
		std::vector<BPatch_function *> funcs;
		funcs = _dynOps.FindFunctionsByLibnameOffset(_addrSpace, i.libname, i.libOffset, false);
		if (funcs.size() == 0) {
			std::cerr << "[TimeFirstUse] ERROR - Could not find function - " << i.libname << "@" << i.libOffset << std::endl;
			continue;
		}
		assert(funcs.size() >= 1);
		BPatch_function * chosenOne = funcs[0];
		// Power BS
		if (funcs.size() > 1){ 
			for (auto nm : funcs) {
				if (((uint64_t)chosenOne->getBaseAddr()) +0x8 == (uint64_t)nm->getBaseAddr())
					chosenOne = nm;
			}
		}
		// Find the specific load store
		std::set<BPatch_opCode> axs;
		axs.insert(BPatch_opLoad);
		axs.insert(BPatch_opStore);
		std::vector<BPatch_point*> * loadsAndStores = chosenOne->findPoint(axs);
		std::vector<BPatch_point *> * tmpPoints = new std::vector<BPatch_point *>();
		std::vector<uint64_t> idents;
		for (auto p : *loadsAndStores) {
			if ((uint64_t)p->getAddress() == i.libOffset)
				tmpPoints->push_back(p);
		}
		assert(tmpPoints->size() == 1);
		if (_instTracker.ShouldInstriment(chosenOne, tmpPoints, LOAD_STORE_INST)) {
			InsertLoadStoreSnippets(chosenOne, tmpPoints, ident);
		}		
	}
}

void TimeFirstUse::FixLSStackFiles() {
	// Converts:
	// LS_trace.bin -> {LS_trace.bin, LS_tracekey.txt}
	// LS_stackkey.bin -> LS_stackkey.txt
	std::cerr << "Runing onetime code on exit of LS" << std::endl;
	ReadLoadStoreFiles tmp(&_binLoc);
	//tmp.OutputTraceKey(std::string("FI_trace.bin"), std::string("FI_tracekey.txt"));
	tmp.CreateStackKey(std::string("FI_stackkey.bin"), std::string("FI_stackkey.txt"));

}

TimeFirstUse::~TimeFirstUse() {
	if(_runOneTime)
		FixLSStackFiles();
}

void TimeFirstUse::SetWrappedFunctions(std::vector<std::string> & wrappedFunctions ) {
	_wrappedFunctions = wrappedFunctions;
	_instTracker.AddAlreadyInstrimented(wrappedFunctions);
}

bool TimeFirstUse::InstrimentAllModules(bool finalize, std::vector<uint64_t> & skips, uint64_t & instUntil, std::vector<std::string> & syncFunctions, std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks) {
	Setup();
	BeginInsertionSet();
	FILE * keyFile = fopen("LS_tracekey.txt", "rb");
	assert(keyFile != NULL);
	StackKeyReader reader(keyFile);
	std::map<uint64_t, std::vector<StackPoint> > ret = reader.ReadStacks();
	if (ret.size() == 0) 
		return true;
	assert(ret.size() > 0);

	std::vector<BPatch_function *> all_functions;
	_img->getProcedures(all_functions);
	_instTracker.PowerFunctionFix(all_functions);
	InsertGotchaEntries();
	WrapEntryAndExit(syncStacks);
	InsertSyncCallNotifier();
	//InsertSyncCallNotifier(points);
	InsertTimeFirstUserimentation(ret);
	_runOneTime =true;
	if (finalize)
		Finalize();

	return true;
}


void TimeFirstUse::Setup() {
	std::cerr << "[TimeFirstUse] Setup" << std::endl;
	_logFile.open("FI_log.txt", std::ofstream::out);
	_addrSpace->loadLibrary("libDriverAPIWrapper.so");
	assert(_dynOps.FindFuncByName(_addrSpace, _entryFunction, std::string("RECORD_FUNCTION_ENTRY")) == 1);
	assert(_dynOps.FindFuncByName(_addrSpace, _exitingFunction, std::string("RECORD_FUNCTION_EXIT")) == 1);
	assert(_dynOps.FindFuncByName(_addrSpace, _enterSync, std::string("SYNC_CAPTURE_SYNC_CALL")) == 1);
	assert(_dynOps.FindFuncByName(_addrSpace, _recordMemAccess, std::string("SYNC_RECORD_MEM_ACCESS")) == 1);
	assert(_dynOps.FindFuncByLibnameOffset(_addrSpace, _libcudaSync, std::string("libcuda.so"), INTERNAL_SYNC_LS, false) == 1);
}


bool TimeFirstUse::InstrimentNewModule(BPatch_object * obj, bool finalize) {
	// Adds instrimentation to a module loaded at a later point in time.
	return false;
}

void TimeFirstUse::Finalize() {
	// Finalizes the insertion set of dyninst
	if (_started)
		_addrSpace->finalizeInsertionSet(false);
	_started = false;
}


void TimeFirstUse::BeginInsertionSet() {
	// Begin the insertion set;
	if(!_started)
		_addrSpace->beginInsertionSet();
	_started = true;
}
