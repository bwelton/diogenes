#include "ProcessControl.h"
#include <boost/filesystem.hpp>
BPatch bpatch;
static ProcessController * curController;
ProcessController::ProcessController(boost::program_options::variables_map vm, LogInfo * log) :
	_vm(vm), _launched(false), _insertedInstrimentation(false), _terminated(false), _log(log), _dontFin(false), _WithLoadStore(false), _ProcessAttached(false) {
}
inline bool exists_test (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

BPatch_addressSpace * ProcessController::LaunchMPIProcess() {
	_binaryEdit = false;
	BPatch_addressSpace * handle = NULL;
	// What this process entails is attaching to the process instead of launching it.
	// This REQUIRES that you use some other method to stop the process at the start of main
	// Such as kill(getpid(), SIGSTOP);
	std::vector<std::string> progName = _vm["prog"].as<std::vector<std::string> >();
	char ** argv = (char**)malloc(progName.size() * sizeof(char *)+1);
	int appPosition = -1;
	for (int i = 0; i < progName.size(); i++) {
		argv[i] = strdup(progName[i].c_str());
		if (i > 1 && appPosition == -1) {
			if (exists_test(progName[i])) {
				appPosition = i;
			}
		}

	}
	argv[progName.size()] = NULL;
	assert(appPosition != -1);
	int pid = -1;
	// Launch the other procees
	pid_t child_pid = fork();
	if (child_pid == 0){
		// Child process
		for (int i = 0; i < 10; i++){
			int retM = execvp(*argv, argv);
			
			std::cerr << "Launch Failed, trying agian.... Error Status: " <<  strerror(errno) << std::endl;
			sleep(2);
		}
		std::cerr << "FAILED TO LAUNCH PROCESS!\n";
		assert(1==0);
	} else {
		sleep(5);
		std::stringstream ss;
		boost::filesystem::path tmp(progName[appPosition]);
		std::string filename = tmp.filename().string();
		ss << "pidof " << filename << std::endl;
		std::cerr << "[ProcessController::LaunchMPIProcess] Waiting on process " << filename << " to start" << std::endl;
		for (int i = 0; i < 10; i++){
			char line[250];
			FILE * command = popen(ss.str().c_str(),"r");
			if(fgets(line,250,command) > 0) {
				pid = atoi(line);
			}
			pclose(command);
			if (pid != -1)
				break;
			sleep(2);
		}
		assert(pid != -1);
	}
	bpatch.setInstrStackFrames(true);
	bpatch.setTrampRecursive(false);
	bpatch.setLivenessAnalysis(false);	
		std::cerr << "[ProcessController::LaunchMPIProcess] Attaching to process " << argv[appPosition] << " at pid " << pid << std::endl;
	handle = bpatch.processAttach((const char *)argv[appPosition], pid);
	bpatch.setLivenessAnalysis(false);
	bpatch.setInstrStackFrames(true);
	bpatch.setTrampRecursive(false);
	assert(handle != NULL);	
	_addrSpace = handle;
	_launched = true;
	_appProc = dynamic_cast<BPatch_process*>(_addrSpace);
	_loadStore.reset(new LoadStoreInst(_addrSpace, _addrSpace->getImage()));
	_timeFirstUse.reset(new TimeFirstUse(_addrSpace, _addrSpace->getImage()));
	_stackTracer = new StacktraceInst(_addrSpace, _addrSpace->getImage());
	_ProcessAttached = true;
	return handle;
}

BPatch_addressSpace * ProcessController::LaunchProcess() {
	_binaryEdit = false;
	BPatch_addressSpace * handle = NULL;
	std::vector<std::string> progName = _vm["prog"].as<std::vector<std::string> >();
	if (progName[0].find("mpirun") != std::string::npos || progName[0].find("mpiexec") != std::string::npos) 
		return LaunchMPIProcess();
	// Setup arguments
	char ** argv = (char**)malloc(progName.size() * sizeof(char *)+1);
	for (int i = 0; i < progName.size(); i++) 
		argv[i] = strdup(progName[i].c_str());

	argv[progName.size()] = NULL;
	for (int i = 0; i < progName.size(); i++){
		std::cerr << "[PROCCTR] Launch Arguments - " <<  std::string(argv[i]) << std::endl;
		_log->Write(std::string("[PROCCTR] Launch Arguments - ") + std::string(argv[i]));
	}

	// Create the bpatch process
	bpatch.setInstrStackFrames(true);
	bpatch.setTrampRecursive(false);
	bpatch.setLivenessAnalysis(false);
	handle = bpatch.processCreate(argv[0],(const char **)argv);
	bpatch.setLivenessAnalysis(false);
	bpatch.setInstrStackFrames(true);
	bpatch.setTrampRecursive(false);
	assert(handle != NULL);

	// Free temporary argv
	for (int i = 0; i < progName.size(); i++)
		free(argv[i]);
	free(argv);

	_addrSpace = handle;
	_launched = true;
	_appProc = dynamic_cast<BPatch_process*>(_addrSpace);
	_loadStore.reset(new LoadStoreInst(_addrSpace, _addrSpace->getImage()));
	_timeFirstUse.reset(new TimeFirstUse(_addrSpace, _addrSpace->getImage()));
	_stackTracer = new StacktraceInst(_addrSpace, _addrSpace->getImage());
	return handle;
}

BPatch_addressSpace * ProcessController::GenerateDebugBinary(std::string bin) {
	_binaryEdit = true;

	BPatch_addressSpace * handle = NULL;
	std::vector<std::string> progName = _vm["prog"].as<std::vector<std::string> >();

	// Setup arguments
	char ** argv = (char**)malloc(progName.size() * sizeof(char *)+1);
	for (int i = 0; i < progName.size(); i++) 
		argv[i] = strdup(progName[i].c_str());

	argv[progName.size()] = NULL;
	for (int i = 0; i < progName.size(); i++)
		_log->Write(std::string("[PROCCTR] Launch Arguments - ") + std::string(argv[i]));

	// Create the bpatch process
	bpatch.setInstrStackFrames(true);
	BPatch_addressSpace * app = bpatch.openBinary(bin.c_str(), true);
	bpatch.setInstrStackFrames(true);
	assert(app != NULL);

	// Free temporary argv
	for (int i = 0; i < progName.size(); i++)
		free(argv[i]);
	free(argv);

	_addrSpace = app;
	_launched = true;
	_appProc =  NULL;//dynamic_cast<BPatch_process*>(_addrSpace);
	_loadStore.reset(new LoadStoreInst(_addrSpace, app->getImage()));
	_appBE = dynamic_cast<BPatch_binaryEdit*>(app);
	_stackTracer = new StacktraceInst(_addrSpace, _addrSpace->getImage());
	_timeFirstUse.reset(new TimeFirstUse(_addrSpace, _addrSpace->getImage()));
	return handle;
}

void ProcessController::WriteOutput(std::string outputName){
	assert(_binaryEdit == true);
	if(!_appBE->writeFile(outputName.c_str()))
		std::cerr << "Could not generate output binary" << std::endl;
}

void ProcessController::GetModules(std::map<std::string, BPatch_object *> & objs){
	std::vector<BPatch_object *> objects;
	BPatch_image * img = _addrSpace->getImage();
	img->getObjects(objects);
	for (auto i : objects) {
		objs[i->pathName()] = i;
	}
}

void ProcessController::InsertStacktracing() {
	LoadWrapperLibrary(std::string("libcuda.so.1"));
	LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libStacktrace.so"));
	_stackTracer->InsertStackInst();

	_appProc->dumpImage("currentImage");
}

void ProcessController::InsertAPICapture() {
	LoadWrapperLibrary(std::string("libcuda.so.1"));
	LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/libDriverAPIWrapper.so"));
	//_addrSpace->beginInsertionSet();
	BPatch_image * img = _addrSpace->getImage();
	DynOpsClass ops;
	//BPatch_function * main;
	BPatch_function * defineBinders;
	//assert(1 == ops.FindFuncByName(_addrSpace, main, std::string("main")));
	assert(1 == ops.FindFuncByName(_addrSpace, defineBinders, std::string("DefineBinders")));
	//std::vector<BPatch_point*> * entryPoints = main->findPoint(BPatch_locEntry);
	//main = ops.GetPOWERFunction(main);
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*defineBinders, recordArgs);
	std::cerr << "Fireing off one time call to setup binders\n";
	dynamic_cast<BPatch_process*>(_addrSpace)->oneTimeCode(entryExpr);
	//assert(_addrSpace->insertSnippet(entryExpr,*entryPoints) != NULL);
	//_addrSpace->finalizeInsertionSet(false);
}

void ProcessController::InsertTimers(std::vector<StackPoint> points) {
	// Load the timing plugin
	LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libTimeCall.so"));
	std::vector<BPatch_function *> startFunc;
	std::vector<BPatch_function *> stopFunc;
	BPatch_image * img = _addrSpace->getImage();
	img->findFunction("TIMER_SIMPLE_TIME_START", startFunc);
	img->findFunction("TIMER_SIMPLE_TIME_STOP", stopFunc);
	
	assert(startFunc.size() > 0);
	assert(stopFunc.size() > 0);
	std::map<std::string, BPatch_object *> objs;
	GetModules(objs);
	BPatch_object * libcudaObj = NULL;
	_addrSpace->beginInsertionSet();
	// Insert timer to add one every time a syncrhonization is called
	{

		for (auto z : objs) {
			if (z.first.find("libcuda.so") != std::string::npos){
				libcudaObj = z.second;
				break;
			}
		}
		assert(libcudaObj != NULL);
		BPatch_function * syncFunction = NULL;
		syncFunction = img->findFunction(libcudaObj->fileOffsetToAddr(INTERNAL_SYNC_C));
		assert(syncFunction != NULL);
		std::vector<BPatch_function *> addFunction;
		img->findFunction("TIMER_SIMPLE_COUNT_ADD_ONE", addFunction);
		assert(addFunction.size() > 0);
		std::vector<BPatch_point*> * funcEntry = syncFunction->findPoint(BPatch_locEntry);
		std::vector<BPatch_snippet*> testArgs;
		BPatch_funcCallExpr recordFuncEntry(*(addFunction[0]), testArgs);
		assert(_addrSpace->insertSnippet(recordFuncEntry,*funcEntry) != NULL);
	}
	std::cerr << "Inserting timers into " << points.size() << " functions " << std::endl;
	// Insert Entry/Exit calls for synchronous functions
	for (auto i : points) {
		BPatch_object * curObj = NULL;
		if (i.libname.find("libcuda.so") != std::string::npos){
			// special case for libcuda
			assert(libcudaObj != NULL);
			curObj = libcudaObj;
		} else {
			// Manual search for the library
			for (auto z : objs) {
				if (i.libname == z.first){
					curObj = z.second;
					break;
				}
			}
		}
		if (curObj == NULL) {
			std::cerr << "Could not find - " << i.libname << " resorting to manual insertion" << std::endl;
			LoadWrapperLibrary(i.libname);
			objs[i.libname] = _loadedLibraries[i.libname];
			curObj = objs[i.libname];
		}

		BPatch_function * instFunc = NULL;
		if (i.inMain)
			instFunc = img->findFunction(i.funcOffset);
		else{
			assert(curObj->fileOffsetToAddr(i.funcOffset) != -1);
			instFunc = img->findFunction(curObj->fileOffsetToAddr(i.funcOffset));
		}
		std::cerr << "Inserting timing Instrimentation into - " << instFunc->getName() << std::endl;
		std::cerr << "Function Name: " << i.funcName << ","<< i.funcName << "," << i.timerID << std::endl;
		assert(instFunc != NULL);
		assert(instFunc->getName() == i.funcName);
		std::vector<BPatch_point*> * funcEntry = instFunc->findPoint(BPatch_locEntry);
		std::vector<BPatch_point*> * funcExit = instFunc->findPoint(BPatch_locExit);
		assert(funcEntry->size() > 0);
		assert(funcExit->size() > 0);
		std::vector<BPatch_snippet*> testArgs;
		testArgs.push_back(new BPatch_constExpr(i.timerID));
		BPatch_funcCallExpr recordFuncEntry(*(startFunc[0]), testArgs);
		BPatch_funcCallExpr recordFuncExit(*(stopFunc[0]), testArgs);
		assert(_addrSpace->insertSnippet(recordFuncEntry,*funcEntry) != NULL);
		assert(_addrSpace->insertSnippet(recordFuncExit,*funcExit) != NULL); 
	}
	assert(_addrSpace->finalizeInsertionSet(false) == true);
}

std::map<uint64_t, StackPoint> ProcessController::GetFirstUse() {
	assert(_loadStore != NULL);
	return _loadStore->_firstUses;
}


std::map<uint64_t, std::vector<StackPoint> > ProcessController::GetThreadStacks() {
	std::map<uint64_t, std::vector<StackPoint> > ret;
	return ret;
}

BPatch_addressSpace * ProcessController::LaunchProcessInstrimenter(std::string WrapperDef) {
	assert(1 == 0);
	BPatch_addressSpace * handle = NULL;

	std::vector<std::string> progName = _vm["prog"].as<std::vector<std::string> >();

	// Insert Instrimentation Library into binary before launching.

	bpatch.setInstrStackFrames(true);
	BPatch_binaryEdit *app = bpatch.openBinary(progName[0].c_str(), true);
	ReadDefinition(WrapperDef);
	std::set<std::string> libs = WrapperLibraries();
	for (auto i : libs) {
		assert(app->loadLibrary(i.c_str()) != NULL);
	}
	progName[0] = progName[0] + std::string("_withlibs");
	if(!app->writeFile(progName[0].c_str())) {
		_log->Write(std::string("[PROCCTR] Could not write output binary"));

		exit(-1);
	}

	// Setup arguments
	char ** argv = (char**)malloc(progName.size() * sizeof(char *)+1);
	for (int i = 0; i < progName.size(); i++) 
		argv[i] = strdup(progName[i].c_str());

	argv[progName.size()] = NULL;
	for (int i = 0; i < progName.size(); i++)
		_log->Write(std::string("[PROCCTR] Launch Arguments - ") + std::string(argv[i]));
	// Create the bpatch process
	handle = bpatch.processCreate(argv[0],(const char **)argv);
	assert(handle != NULL);

	// Free temporary argv
	for (int i = 0; i < progName.size(); i++)
		free(argv[i]);
	free(argv);

	_addrSpace = handle;
	_launched = true;
	_appProc = dynamic_cast<BPatch_process*>(_addrSpace);
	return handle;
}

bool InRegionCheck(std::vector<BPatch_object::Region> & regions, void * addr) {
	for (auto i : regions) {
		if ((uint64_t)addr > (uint64_t)i.base && (uint64_t)addr < ((uint64_t)i.base + i.size))
			return true;
	}
	return false;
}

bool ProcessController::IsObjectInList(std::vector<std::string> li, BPatch_object * obj) {
	std::string libname = obj->name();
	std::string pathname = obj->pathName();
	std::cerr << "My Pathname: " << pathname << std::endl;
	std::transform(libname.begin(), libname.end(), libname.begin(), ::tolower);
	std::transform(pathname.begin(), pathname.end(), pathname.begin(), ::tolower);	
	for (auto x : li)
		if (pathname.find(x) != std::string::npos || libname.find(x) != std::string::npos)
			return true;
	return false;
}


bool ProcessController::IsFunctionInList(std::vector<std::string> li, std::string functionName) {
	return false;
}


void ProcessController::InsertLoadStoreSingle(std::string funcName) {
    // BPatch_effectiveAddressExpr,BPatch_originalAddressExpr, 
	// assert(LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libSynchTool.so")) != false);
	//_appProc->stopExecution();
	_idToFunction.clear();
	std::vector<BPatch_snippet*> recordArgs;
	BPatch_snippet * loadAddr = new BPatch_effectiveAddressExpr();
	BPatch_snippet * instAddr = new BPatch_originalAddressExpr();
	recordArgs.push_back(loadAddr);
	recordArgs.push_back(instAddr);

	std::vector<BPatch_function *> all_functions;
	std::vector<BPatch_function *> callFunc;
	std::vector<BPatch_function *> tracerCall;
	std::vector<BPatch_point*> points; 

	BPatch_image * img = _addrSpace->getImage();
	img->findFunction("SYNC_RECORD_MEM_ACCESS", callFunc);
	img->findFunction("SYNC_RECORD_FUNCTION_ENTRY", tracerCall);
	BPatch_funcCallExpr recordAddrCall(*(callFunc[0]), recordArgs);
	assert(callFunc.size() > 0);
	assert(tracerCall.size() > 0);
	
	img->getProcedures(all_functions);

	std::set<BPatch_opCode> axs;
	axs.insert(BPatch_opLoad);
	axs.insert(BPatch_opStore);

	_addrSpace->beginInsertionSet();
	uint64_t curId = 0;

	BPatch_variableExpr * intCounter = _addrSpace->malloc(*(img->findType("int")), "TEMPORARY_COUNTERS");

	for (auto x : all_functions) {
		if (x->getName() != funcName)
			continue;
//		std::vector<BPatch_point*> * funcEntry = x->findPoint(BPatch_locEntry);
//		std::vector<BPatch_snippet*> testArgs;
//		BPatch_arithExpr addOne(BPatch_assign, *intCounter, BPatch_arithExpr(BPatch_plus, *intCounter, BPatch_constExpr(1)));
		std::vector<BPatch_point*> * funcEntry = x->findPoint(BPatch_locEntry);
		std::vector<BPatch_snippet*> testArgs;
		testArgs.push_back(new BPatch_constExpr(curId));
		BPatch_funcCallExpr recordFuncEntry(*(tracerCall[0]), testArgs);
		std::cerr << x->getName() << "," << curId << "," << std::hex <<  (uint64_t) x->getModule()->getBaseAddr() - (uint64_t) x->getBaseAddr()  << std::dec << "," << std::hex << (uint64_t) x->getBaseAddr() - (uint64_t) x->getModule()->getBaseAddr() << std::dec << std::endl;
		// curId += 1;
		if (_addrSpace->insertSnippet(recordFuncEntry,*funcEntry) == NULL) 
			std::cerr << "could not insert func entry snippet" << std::endl;

		// // Find all load/store's in this funciton.
		std::vector<BPatch_point*> * tmp = x->findPoint(axs);
		if (tmp != NULL){
			points.insert(points.end(), tmp->begin(), tmp->end());
			std::cerr << "Inserting Load/Store Instrimentation into : " << x->getName() << std::endl;
			if (points.size() >= 1)
				if (_addrSpace->insertSnippet(recordAddrCall,points) == NULL) 
					std::cerr << "could not insert snippet" << std::endl;
			points.clear();
		} else {
			std::cerr << "Could not find any load/stores for function : " << x->getName() << std::endl;
		}
	}
	std::cerr << "Finalizing insertion set" << std::endl;
	_addrSpace->finalizeInsertionSet(false);	
}

void ProcessController::InsertLoadStores(std::vector<uint64_t> & skips, uint64_t & instUntil, std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks) {
	// Check this....
	//LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libStubLib.so"));
	LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libSynchTool.so"));
	for (auto i : syncStacks) {
		for (auto z : i.second.GetStackpoints()) {
		 	if (z.libname.find(".so") != std::string::npos)
		 		LoadWrapperLibrary(z.libname);
		}
	}
	std::vector<std::string> synchFunctions;
	std::vector<std::string> wrappedFunctionNames;
	for (auto i : _wrapFunctions)
		wrappedFunctionNames.push_back(std::get<1>(i));

	for (auto i : points) {
		synchFunctions.push_back(i.funcName);
		//synchFunctions.push_back(i.fname);
	}

	_loadStore->SetWrappedFunctions(wrappedFunctionNames);
	_loadStore->InstrimentAllModules(true, skips, instUntil, synchFunctions, points,syncStacks);
	_WithLoadStore = true;
	char * val;                                                                        
    val = getenv( "DIOGENES_DEBUG_DETACH");    
    if (val != NULL)  {
    	_appProc->detach(false);
		std::cerr << "[LoadStore] Detacthing process for debugging and leaving stopped...." << std::endl;
		std::cerr << "Unset DIOGENES_DEBUG_DETACH if this is not desired behavior...." << std::endl;
		while (1)
	 		sleep(10);
	 }

	//exit(0);
}
//zSAMRAI::tbox::StartupShutdownManager::Handler::initialize
void ProcessController::InsertFirstUse(std::vector<uint64_t> & skips, uint64_t & instUntil, std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks) {
	// Check this....
	//LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libStubLib.so"));
	LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/plugins/libTimeFirstUse.so"));
	for (auto i : syncStacks) {
		for (auto z : i.second.GetStackpoints()) {
		 	if (z.libname.find(".so") != std::string::npos)
		 		LoadWrapperLibrary(z.libname);
		}
	}
	std::vector<std::string> synchFunctions;
	std::vector<std::string> wrappedFunctionNames;
	for (auto i : _wrapFunctions)
		wrappedFunctionNames.push_back(std::get<1>(i));

	for (auto i : points) {
		synchFunctions.push_back(i.funcName);
		//synchFunctions.push_back(i.fname);
	}

	_timeFirstUse->SetWrappedFunctions(wrappedFunctionNames);
	_timeFirstUse->InstrimentAllModules(true, skips, instUntil, synchFunctions, points,syncStacks);
	_WithLoadStore = true;
	//_appProc->terminateExecution();
	//exit(0);
}

BPatch * ProcessController::GetBPatch() {
	return &bpatch;
}

void ProcessController::Run() {
	//_appProc->continueExecution();
	if (_binaryEdit)
		return;
	bpatch.waitForStatusChange();
}

void ProcessController::RunWithTimeout(int timeout) {
	//_appProc->continueExecution();
	if (_binaryEdit)
		return;
	sleep(timeout);
	std::vector<BPatch_process *> * procs = bpatch.getProcesses();
	for (auto i : *procs)
		i->stopExecution();

	//bpatch.waitForStatusChange();
}
bool ProcessController::IsTerminated() {
	if (_binaryEdit)
		return true;
	if (_terminated == true)
		return _terminated;
	else 
		_terminated = _appProc->isTerminated();
	return _terminated;
}

bool ProcessController::ContinueExecution() {
	if (_ProcessAttached) {
		_appProc->continueExecution();
		_appProc->continueExecution();
		_ProcessAttached = false;
	}
//	_loadStore->RunOneTimeCode();
	if (_binaryEdit)
		return false;
	return _appProc->continueExecution();
}

bool ProcessController::IsStopped() {
	if (_binaryEdit)
		return true;
	return _appProc->isStopped();
}


void ProcessController::ReadDefinition(std::string WrapperDef) {
	std::ifstream f;
	std::string line;
	f.open(WrapperDef.c_str(),std::ifstream::in);
	while (std::getline(f, line)) {
	    std::stringstream ss(line);
	    std::vector<std::string> tokens;
	    std::string item;
	    while (std::getline(ss, item, ',')) {
	        tokens.push_back(item);
	    }
	    if (tokens.size() != 5) {
	    	_log->Write(std::string("Token size is not 5 in wrapper def.... skipping this function"));
	    	_log->Write(std::string("Line skipped: ") + line);
	    } else {
	    	_log->Write(std::string("Inserting Instrimentation Into: ") + line);
	    	std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::tolower);
	    	_wrapFunctions.push_back(std::make_tuple(tokens[0],tokens[1],tokens[2],tokens[3], tokens[4]));
	    }
	}
}

void ProcessController::InsertWrapperDef(std::string type, std::string origName, std::string wrapperFunc, std::string lib, std::string origSymbol) {
	_wrapFunctions.push_back(std::make_tuple(type, origName, wrapperFunc, lib, origSymbol));
}

std::vector<BPatch_function *> findFuncByName(BPatch_image * appImage, const char * funcName, LogInfo * _log) {
  std::stringstream ss;
  /* fundFunctions returns a list of all functions with the name 'funcName' in the binary */
  BPatch_Vector<BPatch_function * >funcs;
  if (NULL == appImage->findFunction(funcName, funcs) || !funcs.size() || NULL == funcs[0])
  {
      std::cerr << "Failed to find " << funcName <<" function in the instrumentation library" << std::endl;
      return std::vector<BPatch_function *>();
  }
  ss << "Found " << funcName << " this many times " << funcs.size();
  _log->Write(ss.str());
  ss.clear();
  if (funcs.size() > 1) {
    for(int i = 0; i < funcs.size(); i++ )
    {
       ss << "    " << funcs[i]->getName();
       _log->Write(ss.str());
       ss.clear();
    }
  }
  return funcs;
}

void ProcessController::DontFinalize() {
	_dontFin = true;
}

void ProcessController::InstrimentApplication() {
	BPatch_image * img = _addrSpace->getImage();
	std::map<std::string, std::vector<Symbol *> > instLibSymbols;
	uint64_t wrapCount = 0;
	uint64_t totalFunctions = 0;
	bool print = true;

	for (auto i : _loadedLibraries) {
		BPatch_object * obj = i.second;
		std::vector<Symbol *> tmp;
		Dyninst::SymtabAPI::Symtab * symt = Dyninst::SymtabAPI::convert(obj);
		symt->getAllSymbols(tmp);
		instLibSymbols[i.first].insert(instLibSymbols[i.first].end(),tmp.begin(),tmp.end());
	}
	_addrSpace->beginInsertionSet();
	std::stringstream ss;
	for (auto i : _wrapFunctions) {
		totalFunctions += 1;
		if (std::get<0>(i).find("wrap") == std::string::npos)
			continue;
		BPatch_Vector<BPatch_function *> orig;
		if (std::get<1>(i).find("0x") ==  std::string::npos)
			orig = findFuncByName(img,std::get<1>(i).c_str(), _log);
		else {
			// This is an offset.....
			// We must:
			// 1. Convert the offset to an uint.
			uint64_t offset = uint64_t(std::stoull(std::get<1>(i),0,16));
			Dyninst::Address offsetAddress = 0;
			bool foundCuda = false;
			for (auto obj : _loadedLibraries){
				if (obj.first.find(std::string("libcuda.so")) == std::string::npos)
					continue;
				// Potentially more stable method 
				std::vector<BPatch_module *> soModules;
				obj.second->modules(soModules);
				assert(soModules.size() == 1);
				offsetAddress = soModules[0]->getLoadAddr() + offset;


				// // We have found the cuda lib....
				// // Find the function by offset within that library
				// offsetAddress = obj.second->fileOffsetToAddr(offset);
				std::stringstream ss;
				ss << "Found offset for function " << offset << " at location " << offsetAddress;
				_log->Write(ss.str());
				assert(offsetAddress != 0);
				//break;
			}	
			_addrSpace->findFunctionsByAddr(offsetAddress, orig);
			std::stringstream ss;
			ss << "Found function " << std::get<1>(i) << " at location " << offsetAddress << " " << orig.size() << " number of times";
			_log->Write(ss.str());
			assert(orig.size() != 0);
		}
		//std::vector<BPatch_function *> * orig2 =  mod->findFunction(std::get<1>(i).c_str(), funcs, true, false, false, false);//findFuncByName(img,std::get<1>(i).c_str());
		//std::vector<BPatch_function *> orig = *orig2;		
		if (orig.size() == 0) {
			std::cerr << "[PROCCTR] Could not find function with name - " << std::get<1>(i) << std::endl;
			continue;
		}
		std::vector<BPatch_function *> wrapfunc = findFuncByName(img,std::get<2>(i).c_str(), _log);
		if (wrapfunc.size() == 0){
			std::cerr << "[PROCCTR] Could not find wrapper function - " << std::get<2>(i) << std::endl;
			continue;
		}
		
		// Find Hook Symbol	
		if (instLibSymbols.find(std::get<3>(i)) == instLibSymbols.end()) {
			assert(1 == 0);
			std::vector<Symbol *> tmp;
			Dyninst::SymtabAPI::Module *symtab =  Dyninst::SymtabAPI::convert(wrapfunc[0]->getModule());	
			instLibSymbols[std::get<3>(i)] = tmp;
		}

		ss << "Replacing " << orig[0]->getName() << " with " << wrapfunc[0]->getName() << " and new hook " << std::get<4>(i);
		_log->Write(ss.str());
		ss.clear();

		for (Symbol * sym : instLibSymbols[std::get<3>(i)]) {
			if (sym->getPrettyName() == std::string(std::get<4>(i))) {
				uint64_t ptr;
				BPatch_object * obj = _loadedLibraries[std::get<3>(i)];
				Dyninst::SymtabAPI::Symtab * symt = Dyninst::SymtabAPI::convert(obj);

				if (_addrSpace->wrapFunction(orig[0], wrapfunc[0], sym) == true){
					ss << "Function " << orig[0]->getName() << " wrapped successful";
					_log->Write(ss.str());
					ss.clear();
					wrapCount += 1;
				} else {
					std::cerr << "[PROCCTR] Function " << orig[0]->getName() << " WRAPPING FAILED" << std::endl;	
					ss << "Function " << orig[0]->getName() << " WRAPPING FAILED" << std::endl;	
					_log->Write(ss.str());
					ss.clear();
				}
				break;
			}
		}
	}
	if (!_dontFin)
		_addrSpace->finalizeInsertionSet(false);
	_insertedInstrimentation =  true;
}

void ProcessController::InsertBreakpoints(std::vector<std::string> functionNames) {
	BPatch_image * img = _addrSpace->getImage();
	BPatch_breakPointExpr bp;
	BPatch_Vector<BPatch_point *> points;
	for (auto i : functionNames) {
		std::vector<BPatch_function *> wrapfunc; //= findFuncByName(img,i.c_str(), _log);
		std::cerr << "Searching for breakpoint function " << i << std::endl; 
		for (auto z : _loadedLibraries) {
			z.second->findFunction(i, wrapfunc, false);
			std::cerr << "Searching object: " << z.second->name() << std::endl;
			if (wrapfunc.size() > 0) 
				break;
		}
		assert(wrapfunc.size() != 0);
		BPatch_Vector<BPatch_point *> * entry_points;
		entry_points = wrapfunc[0]->findPoint(BPatch_entry);
		std::stringstream ss;
		ss << "Inserting breakpoint at function: " << i;
		_log->Write(ss.str());
		points.insert(points.end(), entry_points->begin(), entry_points->end());
	}	
	if(!_addrSpace->insertSnippet(bp, points)) {
		fprintf(stderr, "%s\n", "InsertFailed");
		exit(-1);
	}
}

bool ProcessController::LoadWrapperLibrary(std::string libname) {
	if (_loadedLibraries.find(libname) != _loadedLibraries.end())
		return true;
	_log->Write(std::string("Loading library ") + libname + std::string(" into address space"));
	BPatch_object * tmp;
	if (_binaryEdit == false)
		tmp = _appProc->loadLibrary(libname.c_str());
	else
		tmp = _addrSpace->loadLibrary(libname.c_str());
	if(tmp == NULL) {
		std::cerr << "[PROCCTR] Failed to load library - " << libname << " into address space!" << std::endl;
		return false;
	}
	_loadedLibraries[libname] = tmp;
	return true;
}	

std::set<std::string> ProcessController::WrapperLibraries() {
	std::set<std::string> ret;
	for (auto i : _wrapFunctions) {
		if (std::get<0>(i).find("wrap") != std::string::npos) {
			ret.insert(std::get<3>(i));
		}
	}
	return ret;
}

bool ProcessController::InsertInstrimentation(std::string WrapperDef) {
	// Force libcuda to be loaded
	assert(LoadWrapperLibrary(std::string("libcuda.so.1")) != false);
	assert(LoadWrapperLibrary(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/libStubLib.so")) != false);
	if (WrapperDef != std::string(""))
    	ReadDefinition(WrapperDef);

	std::set<std::string> libsToLoad = WrapperLibraries();
	for (auto i : libsToLoad)
		LoadWrapperLibrary(i);
	InstrimentApplication();
}