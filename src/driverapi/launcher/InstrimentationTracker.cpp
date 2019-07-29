#include "InstrimentationTracker.h"
#include "BPatch_flowGraph.h"

#define INST_TRACKER_RECORD 1
InstrimentationTracker::InstrimentationTracker() : _logFile("IT_log.txt", std::ofstream::out | std::ofstream::app) {
	#ifdef INST_TRACKER_RECORD
	_recordInst.open("InstrimentationRecorder.txt", std::ofstream::out);
	#endif
	_libsInstrimented.open("LibsInstrimented.txt", std::ofstream::out);
	_funcsInstrimented.open("FuncsInstrimented.txt", std::ofstream::out);
	// Open File Exlusion lists
	// Grab Defaults 
	_loadStoreFuncSkips =  OpenAndParseExclusionFile(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/LoadStoreFunctions.skip"));
	//_loadStoreModSkips =  OpenAndParseExclusionFile(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/LoadStoreModules.skip"));
	_callTracingFuncSkips =  OpenAndParseExclusionFile(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/CallTracingFunctions.skip"));
	_callTracingModSkips =  OpenAndParseExclusionFile(std::string(LOCAL_INSTALL_PATH) + std::string("/lib/CallTracingModules.skip"));

	// Grab those that are local for this application.
	StringVector LSFunc = OpenAndParseExclusionFile(std::string("LoadStoreFunctions.skip"));
	StringVector LSMod = OpenAndParseExclusionFile(std::string("LoadStoreModules.inst"));
	StringVector CTFunc = OpenAndParseExclusionFile(std::string("CallTracingFunctions.skip"));
	StringVector CTMod = OpenAndParseExclusionFile(std::string("CallTracingModules.skip"));

	// Concate defaults with application specific skips.
	_loadStoreFuncSkips.insert(_loadStoreFuncSkips.end(), LSFunc.begin(), LSFunc.end());
	_loadStoreModSkips.insert(_loadStoreModSkips.end(), LSMod.begin(), LSMod.end());
	_callTracingFuncSkips.insert(_callTracingFuncSkips.end(),CTFunc.begin(), CTFunc.end());
	_callTracingModSkips.insert(_callTracingModSkips.end(), CTMod.begin(), CTMod.end());

	for (auto & i : _callTracingModSkips) {
		std::transform(i.begin(), i.end(), i.begin(), ::tolower);
	}
	for (auto & i : _loadStoreModSkips) {
		std::transform(i.begin(), i.end(), i.begin(), ::tolower);
	}
}

StringVector InstrimentationTracker::OpenAndParseExclusionFile(std::string filename) {
	StringVector ret;
	std::ifstream t(filename.c_str(), std::ios::binary);
	if (!t.good()){
		std::cerr << "[InstrimentationTracker::OpenAndParseExclusionFile] Could not open excludes file: " << filename << std::endl;
		return std::vector<std::string>();
	}

	std::string line;
	while (std::getline(t, line)) {
		if (line.at(0) == '#' || line.at(0) == '\n' || line.size() < 2)
			continue;
		if (line.find('\n') != std::string::npos)
			line.erase(line.find('n'), 1);
		ret.push_back(line);
	}
	return ret;
}


void InstrimentationTracker::RecordInstrimentation(InstType t, BPatch_function * func, std::vector<BPatch_point *> * points) {
#ifdef INST_TRACKER_RECORD
	for(auto i : *points)
		RecordInstrimentation(t, func, i);
#endif
	_recordInst.flush();
}

void InstrimentationTracker::RecordInstrimentation(InstType t, BPatch_function * func, BPatch_point * point) {
#ifdef INST_TRACKER_RECORD
	_recordInst << int(t) << "$" <<  func->getModule()->getObject()->pathName() << "$" << (uint64_t)point->getAddress() << std::endl;
#endif
}

void InstrimentationTracker::AddAlreadyInstrimented(std::vector<std::string> & wrappedFunctions) {
	//_prevWrappedFunctions = wrappedFunctions;
}
bool __In_Emulated;
bool InstrimentationTracker::ShouldInstriment(BPatch_function * func, std::vector<BPatch_point *> * points, InstType t) {
	__In_Emulated = false;
	if (!ShouldInstrimentFunciton(func, t) || !ShouldInstrimentModule(func, t) || _exculdeByAddress.find((uint64_t)(func->getBaseAddr())) != _exculdeByAddress.end()) {
		_logFile << "[InstrimentationTracker] We are rejecting function " << func->getName() <<  " because module/function is labeled as uninstrimentable" << std::endl;
		if (func->getName().find("targ10003b1c") != std::string::npos) {
			_logFile << "[InstrimentationTracker] \t\t Module: " << func->getModule()->getObject()->pathName() << " Offset Address: " <<  std::hex << func->getBaseAddr() << std::endl;
		}
		if (!ShouldInstrimentModule(func, t))
			_logFile << "[InstrimentationTracker]\tRejected because module is set to skip" << std::endl;
		if (!ShouldInstrimentFunciton(func, t))
			_logFile << "[InstrimentationTracker]\tRejected because function is set to skip" << std::endl;
		if (_exculdeByAddress.find((uint64_t)func->getBaseAddr()) != _exculdeByAddress.end())
			_logFile << "[InstrimentationTracker]\tRejected because address is exluded by fixpower" << std::endl;
		return false;
	}
	
	if (_alreadyInstrimented.find(t) == _alreadyInstrimented.end())
		_alreadyInstrimented[t] = std::set<uint64_t>();
	std::string pathName = func->getModule()->getObject()->pathName();
	std::set<uint64_t> removeList;

	for (int i = 0; i < points->size(); i++) {
		if ((*points)[i]->getCalledFunction() != NULL) {
			if (ShouldInstrimentPoint((*points)[i]->getCalledFunction(), t) == false){
				removeList.insert(i);
			} else {
				//std::cerr << "Instrumenting function call - " << (*points)[i]->getCalledFunction()->getName() << "@" << std::hex << (uint64_t)(*points)[i]->getAddress() << std::dec <<std::endl;
			}
		}
		if (!ShouldInstrimentInstruction((*points)[i]))
			removeList.insert(i);
		uint64_t hashValue = HashPoint(func, (*points)[i]);
		if (_alreadyInstrimented[t].find(hashValue) != _alreadyInstrimented[t].end())
			removeList.insert(i);
		else 
			_alreadyInstrimented[t].insert(hashValue);
	}
	std::vector<BPatch_point *> tmpPoints = *points;
	points->clear();
	for (int i = 0; i < tmpPoints.size(); i++) {
		if (removeList.find(i) == removeList.end())
			points->push_back(tmpPoints[i]);
	}
	if (points->size() == 0) 
		return false;
	if (_librariesInstrimented.find(func->getModule()->getObject()->pathName()) == _librariesInstrimented.end()){
		_libsInstrimented << func->getModule()->getObject()->pathName() << std::endl;
		_libsInstrimented.flush();
		_librariesInstrimented.insert(func->getModule()->getObject()->pathName());
	}
	// Function name save
	if (_functionsInstrimented.find(func->getMangledName()) == _functionsInstrimented.end()) {
		_funcsInstrimented << func->getMangledName() << std::endl;
		_functionsInstrimented.insert(func->getMangledName());
	}
	return true;
}

uint64_t InstrimentationTracker::HashPoint(BPatch_function * func, BPatch_point * point) {
	std::stringstream ss;
	ss << func->getModule()->getObject()->pathName() << "," << (uint64_t)point->getAddress();
	return std::hash<std::string>()(ss.str());
}

bool InstrimentationTracker::ShouldInstrimentPoint(BPatch_function * func, InstType t) {
	static StringVector callTracingInstSkips  = {"__stack_chk_fail", "_Unwind_Resume"};
	static StringVector loadStoreInstSkips = {"EMPTY_VECTOR_DONT"};
    StringVector * toSkip;
    if (t == LOAD_STORE_INST)
    	toSkip = &loadStoreInstSkips;
    else
    	toSkip = &callTracingInstSkips;	
    std::string funcName = func->getName();
    for (auto i : *toSkip) {
    	if (funcName.find(i) != std::string::npos)
    		return false;
    }
    return true;
}


bool InstrimentationTracker::ShouldInstrimentInstruction(BPatch_point * point) {
	if ((uint64_t)point->getAddress() == 0x102b3308)
		std::cerr << "MYPOINTISHERE - " <<  point->getInsnAtPoint().format() << std::endl;
	if (point->getInsnAtPoint().isValid() == false){
		std::cerr << "[InstrimentationTracker::ShouldInstrimentInstruction] Could not get instruction at " << std::hex << point->getAddress() << std::endl;
		return true;		
	}
	std::string tmp =  point->getInsnAtPoint().format();
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
    // Reservation Instructions
	if (tmp.find("lwarx") != std::string::npos || 
		tmp.find("lbarx") != std::string::npos ||
		tmp.find("lharx") != std::string::npos ||
		tmp.find("ldarx") != std::string::npos ||
		tmp.find("lqarx") != std::string::npos ){
		__In_Emulated = true;
		return false;
	}
	// Use of reservation instructions
	if (tmp.find("stbcx") != std::string::npos ||
		tmp.find("sthcx") != std::string::npos ||
		tmp.find("stwcx") != std::string::npos ||
		tmp.find("stdcx") != std::string::npos || 
		tmp.find("stqcx") != std::string::npos ) {
		__In_Emulated = false;
		return false;	
	}
	// if (tmp.find("[R31 +") != std::string::npos)
	// 	return false;
	// xxswapd
	if (tmp.find("stfq fpr0, fpr1, [0 + 250]") != std::string::npos)
		return false;

	if (tmp.find("stfq fpr") != std::string::npos)
		return false; 

	// Load/Stores to/from stack.
	if (tmp.find("[r1 +") != std::string::npos || tmp.find("[r1 -") != std::string::npos)
		return false;

	// if ((pointAddress >= 0x10500510 && pointAddress <= 0x10500530) ||
	//     (pointAddress >= 0x10500598 && pointAddress <= 0x105005b8) ||
	//     (pointAddress >= 0x105006d4 && pointAddress <= 0x105006f8) ||
	//     (pointAddress >= 0x10500738 && pointAddress <= 0x10500750))
	uint64_t pointAddress = (uint64_t) point->getAddress();
	if (
	   //(pointAddress >= 0x10500598 && pointAddress <= 0x105005b8) ||
	    (pointAddress >= 0x105006ec && pointAddress <= 0x105006f8) ||
	    (pointAddress >= 0x10500738 && pointAddress <= 0x10500750))
		return false;
	if (__In_Emulated)
		return false;
	return true;
}


uint64_t GetContiguousSize(BPatch_function * func) {
	uint64_t size = 0;
	BPatch_flowGraph * fg = func->getCFG();
    std::vector<BPatch_basicBlock *> entry;
    if (fg->getEntryBasicBlock(entry) != true)
    	assert(fg->getEntryBasicBlock(entry) == true);
    if (entry.size() > 1) 
    	fprintf(stderr, "%s %s %s\n", "Function ", func->getName().c_str(), "has multiple entries");
    uint64_t tmp = 999999999;
    uint64_t pos = 0;
    for (int i = 0; i < entry.size(); i++) {
    	if (entry[i]->size() < tmp){
    		tmp = entry[i]->size();
    		pos = i;
    	}
    }
    if (tmp == 999999999)
    	assert(tmp != 999999999);

    if (tmp >= 6 * 0x4)
    	return tmp;
    std::vector<BPatch_basicBlock *> targets; 
    entry[pos]->getTargets(targets);
    uint64_t lastInstructionAddress = entry[pos]->getLastInsnAddress();
    uint64_t nextBlockAddr = 0;
    for (auto i : targets)
    	if (i->getStartAddress() == lastInstructionAddress || i->getStartAddress() + 0x4 == lastInstructionAddress || i->getStartAddress() + 0x8 == lastInstructionAddress){    	
    		nextBlockAddr = i->getLastInsnAddress();
    		break;
    	}
   if (uint64_t(func->getFootprint()) > std::abs(nextBlockAddr - entry[pos]->getStartAddress()))
   		return std::abs(nextBlockAddr - entry[pos]->getStartAddress());
   return uint64_t(func->getFootprint());
}	

bool CheckForPreamble(BPatch_function * func) {
	BPatch_flowGraph * fg = func->getCFG();
    std::vector<BPatch_basicBlock *> entry;
    if (fg->getEntryBasicBlock(entry) != true)
    	assert(fg->getEntryBasicBlock(entry) == true);
    std::vector<std::pair<Dyninst::InstructionAPI::Instruction, Dyninst::Address> > instructions;
    entry[0]->getInstructions(instructions);
    if (instructions.size() < 2) 
    	return true;
    if ((instructions[0].first.format().find("lis R2") != std::string::npos && instructions[1].first.format().find("addi R2") != std::string::npos) ||
        (instructions[0].first.format().find("addis R2") != std::string::npos && instructions[1].first.format().find("addi R2") != std::string::npos)) {
      return true; 
    }
    // if(func->getAddSpace()->findFunctionByAddr((void*)(((uint64_t) func->getBaseAddr()) + 0x8)))
    // 	return true;

    return false;

}

void InstrimentationTracker::PowerFunctionFix(std::vector<BPatch_function*> & functions) {
	// Handle the POWER ABI issues, look for functions within distance of 0x8 (two instructions).
	// Exclude all functions with size < 5 instructions.

	// This Map MUST Be ordered...
	std::map<uint64_t, BPatch_function *> orderMap;
	for (auto i : functions) {
		if (orderMap.find((uint64_t)i->getBaseAddr()) != orderMap.end())
			assert("WE SHOULDN'T BE HERE" != 0);
		orderMap[(uint64_t)i->getBaseAddr()] = i;
	}

	for (auto i : orderMap) {
		//if (orderMap.find(i.first + 0x8) != orderMap.end() || orderMap.find(i.first + 0x10) != orderMap.end() || CheckForPreamble(i.second) == true ) {
		if (orderMap.find(i.first + 0x8) != orderMap.end() || orderMap.find(i.first + 0x10) != orderMap.end()  ) {
			// Exclude this function
			_recordInst << "-1" << "$" <<  i.second->getModule()->getObject()->pathName() << "$" << std::hex << (uint64_t)i.second->getBaseAddr() << std::dec << "$" << i.second->getName() << std::endl;
			_exculdeByAddress.insert(i.first);
			continue;
		} 
		// Check the size
		if(uint64_t(i.second->getFootprint()) < (0x4 * 6) || GetContiguousSize(i.second) < (0x4 * 6)) {
			_recordInst << "-2" << "$" <<  i.second->getModule()->getObject()->pathName() << "$" << std::hex << (uint64_t)i.second->getBaseAddr() << std::dec << "$" << i.second->getName() << std::endl;
			_exculdeByAddress.insert(i.first);
		}
		// Exclude everything in the std library
		if (i.second->getDemangledName().find("std::") != std::string::npos) {
			_exculdeByAddress.insert(i.first);	
			_exculdeByAddress.insert(i.first+0x8);	
			_recordInst << "-3" << "$" <<  i.second->getModule()->getObject()->pathName() << "$" << std::hex << (uint64_t)i.second->getBaseAddr() << std::dec << "$" << i.second->getDemangledName() << std::endl;
			_recordInst << "-3" << "$" <<  i.second->getModule()->getObject()->pathName() << "$" << std::hex << (uint64_t)i.second->getBaseAddr() + 0x8 << std::dec << "$" << i.second->getDemangledName() << std::endl;
		}

	}
}

bool InstrimentationTracker::ShouldInstrimentFunciton(BPatch_function * func, InstType t) {
//	static StringVector callTracingSkips  = {"dyninst","Dyninst","main","___stack_chk_fail","__run_exit_handlers","exit","libc","__GI_"," __malloc","abort","__random","__stack_chk_fail","deregister_tm_clones","register_tm_clones","backtrace_and_maps","__GI__IO_unsave_markers","_IO_setb","__GI___mempcpy","__munmap","__GI___twalk","__GI__IO_adjust_column"};
	// static StringVector loadStoreSkips =  {"_fini","atexit","main", "dyninst", "boost","pthread", "clock","timer","__aio_",
	// "__libc_csu_init", "__libc_csu_fini","malloc","printf","fwrite","strlen","abort","assert","strnlen","new_heap","fflush",
	// "__static_initialization_and_destruction_0","_start", "__GI___backtrace","__GI___libc_secure_getenv","__GI_exit","cudart","_IO_puts","__new_fopen","fopen","_Unwind_Resume","__run_exit_handlers","free","open",
	// "_init", "cudart::cuosInitializeCriticalSection","cudart::", "cudaLaunch","__timer","pthread","elf","dwarf",
	// "cudart::cuosInitializeCriticalSectionShared","cudart::cuosMalloc", "basic_ostringstream","basic_istringstream",
	// "cudart::cuosInitializeCriticalSectionWithSharedFlag","cudaLaunch","dim3::dim3","std::num_get","std::time_get",
	// "__printf","__GI_fprintf","_IO_vfprintf_internal","buffered_vfprintf","printf_positional","__printf_fp","__printf_fphex","__fxprintf","__GI___printf_fp_l","vfwprintf","__GI___asprintf","buffered_vfprintf","printf_positional","_IO_vasprintf","__snprintf","vsnprintf",
    // "__GI___libc_malloc","_int_malloc","__malloc_assert","malloc_consolidate","sysmalloc","malloc_printerr", "cudaHostGetDevicePointer", "cudaHostGetFlags"};

    StringVector * toSkip;
    if (t == LOAD_STORE_INST)
    	toSkip = &_loadStoreFuncSkips;
    else
    	toSkip = &_callTracingFuncSkips;
    std::string funcName = func->getName();
    std::string unalteredFunc = func->getName();

    // Strip parameters/etc
    // if (funcName.find("<") != std::string::npos){
    // 	funcName.erase(funcName.find("<"));
    // }
    if (funcName.find("(") != std::string::npos){
    	funcName.erase(funcName.find("("));
    }

    std::string funcNameMangled = func->getMangledName();
    std::cerr << "Testing My FuncName = " << funcName << " with original " << unalteredFunc << " and mangled " << funcNameMangled <<  std::endl;

    for (auto i : *toSkip) {
    	if (funcName.find(i) != std::string::npos || funcNameMangled.find(i) != std::string::npos)
    		return false;
    }
    if (t == LOAD_STORE_INST)
	    for (auto i : _prevWrappedFunctions) {
	    	if ( unalteredFunc == i || funcName.find(i) == 0 || funcNameMangled.find(i) == 0)
	    		return false;
	    }

	// if (funcNameMangled.find("thunk") != std::string::npos)
	// 	return false;
	// if (t == LOAD_STORE_INST){
	// 	if (func->getMangledName().find("_ZN6SAMRAI4algs25HyperbolicLevelIntegrator37coarsenDataForRichardsonExtrapolationERKSt10shared_ptrINS_4hier14PatchHierarchyEEiRKS2_INS3_10PatchLevelEEdb") == std::string::npos)
	// 		return false;
	// }
    return true;
}

bool InstrimentationTracker::ShouldInstrimentModule(BPatch_function * func, InstType t) {
	//static StringVector loadStoreModSkips = {"libicui18n","libnvtoolsext","libgfortran","libnvidia-fatbinaryloader","libxlsmp","nvidia-384","libc","libmpi","liblzma","libxerces","libunwind.so","gettext","libxml2","openmpi","libgcc_s","libstdc++","pthread","elf","dwarf","boost","libcuda","boost","Dyninst", "dyninst", "cudadedup","libcudnn.so","libaccinj64.so","libcublas.so","libcudart.so","libcufft.so","libcufftw.so","libcuinj64.so","libcurand.so","libcusolver.so","libcusparse.so","libnppc.so","libnppial.so","libnppicc.so","libnppicom.so","libnppidei.so","libnppif.so","libnppig.so","libnppim.so","libnppist.so","libnppisu.so","libnppitc.so","libnpps.so","libnvblas.so","libnvgraph.so","libnvrtc-builtins.so","libnvrtc.so","libdl-2.23.so","libpthread-2.23.so", "cudadedup", "libcuda.so","libcuptieventhandler.so","libecho.so","libsynchtool.so","libtimecall.so","libtransfertimeline.so","libstublib.so", "dyninst", "dyninst", "libdriverapiwrapper", "libgotcha", "cudadedup"};
	//static StringVector callTracingModSkips = {"libc.so","ld-linux-x86-64","libgcc_s.so","libstdc++.so", "libdl.so", "libpthread.so", "cudadedup", "libdl-2.23.so","dyninst","dyninst","boost", "libc.so", "linux-vdso.so", "libpthread-2.23.so","libcuptieventhandler.so","libecho.so","libsynchtool.so","libtimecall.so","libtransfertimeline.so","libstublib.so", "dyninst","Dyninst", "libdriverapiwrapper", "libgotcha", "cudadedup"};
    StringVector * toSkip;
    if (t == LOAD_STORE_INST)
    	toSkip = &_loadStoreModSkips;
    else
    	toSkip = &_callTracingModSkips;
    std::string modname = func->getModule()->getObject()->pathName();
    std::transform(modname.begin(), modname.end(), modname.begin(), ::tolower);
    for (auto i : *toSkip) {
    	if (t == LOAD_STORE_INST) {
    		if (modname.find(i) != std::string::npos)
    			return true;
    	} else {
    		if (modname.find(i) != std::string::npos)
    			return false;
    	}

    	//if (modname.find("libcuda.so") == std::string::npos)
    	//	return false;
    }
    // if (t == LOAD_STORE_INST){
    // 	if (modname.find("stencil") != std::string::npos)
    // 		return true;
    // 	return false;
    // }
   	if (t == LOAD_STORE_INST)
    	return false;
    return true;
}
