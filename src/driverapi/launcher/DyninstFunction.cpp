#include "DyninstFunction.h"
uint64_t DYNINST_FUNC_CCOUNT = 0;
DyninstFunction::DyninstFunction(std::shared_ptr<DyninstProcess> proc, BPatch_function * func, std::shared_ptr<InstrimentationTracker> tracker, std::shared_ptr<BinaryLocationIDMap> bmap) : 
		_proc(proc), _track(tracker), _func(func),  _bmap(bmap), _exitEntryDone(false), _lsDone(false), _wrapper(proc) {
	_obj = _func->getModule()->getObject();
	_addedLS = false;
	_addedTRAC = false;
	std::shared_ptr<DynOpsClass> ops = proc->ReturnDynOps();
	ops->GetBasicBlocks(func, _bblocks);
	_ops = ops;
	//std::cerr << "[DyninstFunction] Iterating through " << _bblocks.size() << " blocks" << std::endl;
	std::vector<std::pair<Dyninst::InstructionAPI::Instruction, Dyninst::Address> > instructionVector;

	for (auto i : _bblocks) {
		//while(1) {
		instructionVector.clear();
		i->getInstructions(instructionVector);
		//}
		for (auto n : instructionVector) {
			_instmap[(uint64_t) n.second]  = std::make_pair(n.first, i);
		}
	}
	_entrySize = GetSmallestEntryBlockSize();

}
void DyninstFunction::GetFuncInfo(std::string & libName, std::string & funcName) {
	libName = _obj->pathName();
	funcName = _func->getName();

}
std::string DyninstFunction::PrintInst(InstStats & stats) {
	bool stackTraced = false;
	bool lsInstrimented = false;
	std::stringstream ss;
	if (_insertedInst.size() == 0){
		ss <<"FUNCTION: " << _func->getName()  << " IN MODULE " << _obj->pathName() << " NO INST" << std::endl;
		return ss.str();
	}

	//ss << "FUNCTION: " << _func->getName()  << " IN MODULE " << _obj->pathName() << std::endl;
	for (auto i : _instmap) {
		ss << "0x" << std::hex << i.first << ": " << i.second.first.format(0);
		if (_insertedInst.find(i.first) !=  _insertedInst.end()) {
			if (std::get<0>(_insertedInst[i.first]) == 1) {
				stats.callTracedInsts++;
				std::string tmp = i.second.first.format(0);
				stats.ct_instNames.insert(tmp.substr(0, tmp.find(' ')));
				ss << " CALLTRACED ";
				stackTraced = true;
			} else if (std::get<0>(_insertedInst[i.first]) == -1){
				ss << " FAILED CALLTRACE ";
			}
			if (std::get<1>(_insertedInst[i.first]) == 1) {
				stats.lsInsts++;
				std::string tmp = i.second.first.format(0);
				stats.ls_instNames.insert(tmp.substr(0, tmp.find(' ')));
				ss << " LS APPLIED ID IS " << std::get<2>(_insertedInst[i.first]);
				lsInstrimented = true;
			} else if (std::get<1>(_insertedInst[i.first]) == -1){
				ss << " FAILED LS ";
			}			
		}
		ss << "\n";
	}
	std::string ret = ss.str();
	ss.str(std::string());
	ss <<  "FUNCTION: " << _func->getName()  << " IN MODULE " << _obj->pathName() << (stackTraced ? " FUNC_STACKTRACED " : " ") << (lsInstrimented ? " FUNC_LSINSTRIMENTED " : " " ) << std::endl;
	return ss.str() + ret;
}

void DyninstFunction::EntryExitWrapping() {
	//return;
	// Find the function calls being made
	if(_exitEntryDone == true)
		return;
	std::string libname = _obj->pathName();
	// Skip entry/exit instrimentation on libcuda
	if (libname.find("libcuda.so") != std::string::npos) {
		_exitEntryDone = true;
		return;
	}

	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::vector<BPatch_function *> entryFuncs = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("RECORD_FUNCTION_ENTRY"), NULL);
	std::vector<BPatch_function *> exitFuncs = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("RECORD_FUNCTION_EXIT"), NULL);
	assert(entryFuncs.size() == 1);
	assert(exitFuncs.size() == 1);
	BPatchPointVecPtr funcCalls(_func->findPoint(BPatch_locSubroutine));
	if (funcCalls == NULL)
		return;
	for (auto i : *funcCalls)  {
		std::vector<BPatch_point*> singlePoint;
		singlePoint.push_back(i);
		uint64_t id;
		int writeValue = 0;
		uint64_t libOffsetAddr = 0;
		if (!ops->GetFileOffset(_proc->GetAddressSpace(), i, libOffsetAddr, true))
			libOffsetAddr = (uint64_t) i->getAddress();

		if (_bmap->AlreadyExists(libname, libOffsetAddr)) {
			writeValue = -1;
		} else {
			writeValue = 1;
			id = _bmap->StorePosition(libname, libOffsetAddr);
			std::vector<BPatch_snippet*> recordArgs;
			recordArgs.push_back(new BPatch_constExpr(id));
			BPatch_funcCallExpr entryExpr(*entryFuncs[0], recordArgs);
			BPatch_funcCallExpr exitExpr(*exitFuncs[0], recordArgs);
			_addedTRAC = true;
			if (_proc->GetAddressSpace()->insertSnippet(entryExpr,singlePoint, BPatch_callBefore) == NULL) {
				std::cerr << "[LoadStoreInst][EntryExit] \t\t ERROR! Could not insert entry tracking into " << _func->getName() << std::endl;
			}
			if (_proc->GetAddressSpace()->insertSnippet(exitExpr,singlePoint,BPatch_callAfter) == NULL) {
				std::cerr << "[LoadStoreInst][EntryExit] \t\t ERROR! Could not insert exit tracking into " << _func->getName() << std::endl;
			}
			std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Entry Exit Wrapping inserted into function inserted at point - " << libname << "@" << std::dec << libOffsetAddr << std::endl;
		}

		if(_insertedInst.find((uint64_t)i->getAddress()) == _insertedInst.end()){
			_insertedInst[(uint64_t)i->getAddress()] = std::make_tuple(writeValue,0,0);
		} else {
			assert(std::get<0>(_insertedInst[(uint64_t)i->getAddress()]) == 0);
			std::get<0>(_insertedInst[(uint64_t)i->getAddress()]) = writeValue;
		}
	}
	if (_addedTRAC == true)
		std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Entry Exit Wrapping inserted into function: " << _func->getName() << " in library " << libname << std::endl;
	_exitEntryDone = true;
}

void DyninstFunction::InsertLoadStoreAnalysis() {
	if(_wrapper.InsertLoadStoreInstrimentation(_func, _bmap))
		return;
	if (_func->getName().find("xerces") != std::string::npos || _func->getName().find("RuntimeException") != std::string::npos || _func->getName().find("Exception") != std::string::npos) {
		std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Function " << _func->getName() << " is excluded from LS analysis by being in xerces (and unsupported library)" << std::endl;
		return;
	}
	// if (_obj->pathName().find("qb") != std::string::npos)
	//  	_func->relocateFunction();
	
	if (IsExcludedFunction(LOAD_STORE_INST) || _lsDone || _entrySize < (0x4 * 5) ){
		_lsDone = true;
		std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Function " << _func->getName() << " is excluded from LS analysis by IsExcludedFunction or its size is too small (" << _entrySize << ")" << std::endl;

		if (_obj->pathName().find("cuibm") != std::string::npos && _addedTRAC == false )
			_func->relocateFunction();
		return;
	}
	if (_obj->pathName().find("libcufft.") != std::string::npos){
		std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Function " << _func->getName() << " is excluded from LS analysis by being libcufft" << std::endl;
		return;
	}
	_lsDone = true;
	if (_func->getName().find("__device_stub__") != std::string::npos || 
		//_func->getName().find("thrust::") != std::string::npos ||
		_func->getName().find("std::string::_Rep") != std::string::npos ||
		_func->getName().find("cudaRegisterAll") != std::string::npos ||
		//_func->getName().find("YAML::") != std::string::npos ||
		_func->getName().find("__tcf_0") != std::string::npos)
		return;
//	if (_func->getName().find("thrust::") != std::string::npos && DYNINST_FUNC_CCOUNT > 200)
//		return;
//	DYNINST_FUNC_CCOUNT++;


	//return;
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::vector<BPatch_function *> recordMemAccess = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("SYNC_RECORD_MEM_ACCESS"), NULL);
	assert(recordMemAccess.size() == 1);
	std::string libname = _obj->pathName();
 	if (libname.find("/lib64/ld64") != std::string::npos || 
	    libname.find("lib/spectrum_mpi/") != std::string::npos) {
 		std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Function " << _func->getName() << " is excluded from LS analysis by being in spectrum_mpi/ld64" << std::endl;
 		return;
 	}
	std::set<BPatch_opCode> axs;
	axs.insert(BPatch_opLoad);
	axs.insert(BPatch_opStore);
	BPatchPointVecPtr loadsAndStores(_func->findPoint(axs));
	if (loadsAndStores == NULL){
		std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Function " << _func->getName() << " is excluded from LS analysis due to no load stores" << std::endl;
		return;
	}
	std::set<uint64_t> exclude;
	uint64_t setID = 0;
	bool AddedLoadStoreInst = false;
	// Check for non-returning functions (not supported)
	if (!GenExclusionSet(exclude)) {
		std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Function " << _func->getName() << " is excluded from LS analysis due being a non-returning function" << std::endl;
		return;
	}
	for (auto i : *loadsAndStores) {
		int writeValue = 0;
		uint64_t addr = (uint64_t)i->getAddress();
		if (exclude.find(addr) != exclude.end()) {
			writeValue = -1;
		} else {
			uint64_t libOffsetAddr = 0;
			if (!ops->GetFileOffset(_proc->GetAddressSpace(), i, libOffsetAddr, true))
				libOffsetAddr = (uint64_t) i->getAddress();
			if(_bmap->AlreadyExists(libname, libOffsetAddr)){
				writeValue = -1;
			} else {
				AddedLoadStoreInst = true;
				std::vector<BPatch_point*> singlePoint;
				singlePoint.push_back(i);
				writeValue = 1;
				uint64_t id = _bmap->StorePosition(libname, libOffsetAddr);
				std::vector<BPatch_snippet*> recordArgs;
				BPatch_snippet * loadAddr = new BPatch_effectiveAddressExpr();
				recordArgs.push_back(loadAddr);
				recordArgs.push_back(new BPatch_constExpr(id));
				setID = id;
				BPatch_funcCallExpr recordAddrCall(*(recordMemAccess[0]), recordArgs);
				assert(_proc->GetAddressSpace()->insertSnippet(recordAddrCall,singlePoint) != NULL);
				_addedLS = true;
			}
		}
		if(_insertedInst.find((uint64_t)i->getAddress()) == _insertedInst.end()){
			_insertedInst[(uint64_t)i->getAddress()] = std::make_tuple(0,writeValue,setID);
		} else {
			assert(std::get<1>(_insertedInst[(uint64_t)i->getAddress()]) == 0);
			std::get<1>(_insertedInst[(uint64_t)i->getAddress()]) = writeValue;
		}
	}
	//if (AddedLoadStoreInst == true)
	std::cerr << "[DyninstFunction::InsertLoadStoreAnalysis] Load store analysis inserted into function: " << _func->getName() << " in library " << libname << std::endl;
	if (_obj->pathName().find("cuibm") != std::string::npos && _addedTRAC == false  && _addedLS == false)
               _func->relocateFunction();

}

void DyninstFunction::InsertTimingAtPoint(StackPoint p) {
	if (p.libname.find("libc-2.17.so") != std::string::npos)
		return;
	assert(p.libOffset != 0);
	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::vector<BPatch_function *> recordMemAccess = ops->FindFuncsByName(_proc->GetAddressSpace(), std::string("SYNC_CAPTURE_TIMEACCESS"), NULL);	
	assert(recordMemAccess.size() == 1);
	std::set<BPatch_opCode> axs;
	std::string libname = _obj->pathName();
	axs.insert(BPatch_opLoad);
	axs.insert(BPatch_opStore);
	BPatchPointVecPtr loadsAndStores(_func->findPoint(axs));
	bool found = false;
	for (auto i : *loadsAndStores) {
		uint64_t libOffsetAddr = 0;
		if (!ops->GetFileOffset(_proc->GetAddressSpace(), i, libOffsetAddr, true))
			libOffsetAddr = (uint64_t) i->getAddress();	
		if (p.libOffset == libOffsetAddr) {
			found = true;
			std::vector<BPatch_point*> singlePoint;
			singlePoint.push_back(i);
			uint64_t id = _bmap->StorePosition(libname, libOffsetAddr);
			std::vector<BPatch_snippet*> recordArgs;
			BPatch_snippet * loadAddr = new BPatch_effectiveAddressExpr();
			recordArgs.push_back(loadAddr);
			recordArgs.push_back(new BPatch_constExpr(id));
			BPatch_funcCallExpr recordAddrCall(*(recordMemAccess[0]), recordArgs);
			assert(_proc->GetAddressSpace()->insertSnippet(recordAddrCall,singlePoint) != NULL);
			break;
		}
	}
	assert(found == true);


}

uint64_t DyninstFunction::HandleEmulated(BPatch_basicBlock * block) {
	// Returns the address of the reserveration use instruction.
	std::vector<std::pair<Dyninst::InstructionAPI::Instruction, Dyninst::Address> > instructionVector;
	block->getInstructions(instructionVector);
	for (auto i : instructionVector) {
		std::string tmp = i.first.format(0);
		// Use of reservation instructions
		if (tmp.find("stbcx") != std::string::npos ||
			tmp.find("sthcx") != std::string::npos ||
			tmp.find("stwcx") != std::string::npos ||
			tmp.find("stdcx") != std::string::npos || 
			tmp.find("stqcx") != std::string::npos ) {
			return i.second + 4;
		}		
	}
	return 0;
}
bool DyninstFunction::GenExclusionSet(std::set<uint64_t> & excludedAddress) {
	// This is a set of instructions that should never be profiled.
	// Right now in this set is all instructions with the following properties:
	// 1. Ranges of emulated instructions. 
	// 2. stfq instructions
	bool FoundReturn = false;
	for (auto i : _instmap) {
		std::string tmp = i.second.first.format(0);
		if (i.second.first.getCategory() == InstructionAPI::InsnCategory::c_ReturnInsn)
			FoundReturn = true;
		// Reservation Instructions
		// Find emulated start/end
		if (tmp.find("lwarx") != std::string::npos || 
			tmp.find("lbarx") != std::string::npos ||
			tmp.find("lharx") != std::string::npos ||
			tmp.find("ldarx") != std::string::npos ||
			tmp.find("lqarx") != std::string::npos ){
			uint64_t endingAddress = HandleEmulated(i.second.second);
			// We have noidea where the emulation ends, it is unsafe to do anything here.
			if (endingAddress == 0)
				return false;
			excludedAddress.insert(i.first);
			assert(endingAddress > i.first);
			for (uint64_t t = i.first; t < endingAddress + 4; t = t + 4) 
				excludedAddress.insert(t);				
		} else {
			if (tmp.find("stfq ") != std::string::npos)
				excludedAddress.insert(i.first);
			else if (tmp.find("[r1 +") != std::string::npos || tmp.find("[r1 -") != std::string::npos)
				excludedAddress.insert(i.first);
			else if (tmp.find("(r1)") != std::string::npos || tmp.find("(r31)") != std::string::npos)
				excludedAddress.insert(i.first);
			// if (tmp.find("lbz") != std::string::npos)
			// 	excludedAddress.insert(i.first);
			// if (tmp.find("lfs ") == std::string::npos)
			// 	excludedAddress.insert(i.first);
			// if (i.first < 0x10002fe0 || i.first > 0x10002fec)
			// 	excludedAddress.insert(i.first);
		} 
	}
	return FoundReturn;
}

void DyninstFunction::GetCallsites(std::vector<DyninstCallsite> & ret) {
	BPatchPointVecPtr points = _ops->GetPoints(_func, BPatch_subroutine);
	// points.reset(_func->findPoint(BPatch_subroutine));
	if (points == NULL)
		return;
	for (auto x : *points) {
		ret.push_back(DyninstCallsite(_proc, _func, *x));
	}
}


uint64_t DyninstFunction::GetSmallestEntryBlockSize() {
	BPatch_flowGraph * fg = _func->getCFG();
	std::vector<BPatch_basicBlock *> entry;
	uint64_t smallest = 100000000;
	assert(fg->getEntryBasicBlock(entry) == true);
	for (auto i : entry) {
		uint64_t cur = (uint64_t) i->getStartAddress();
		uint64_t size = _instmap[cur].first.size();
		while (1) {
			uint64_t next = cur + _instmap[cur].first.size();
			if (_instmap.find(next) == _instmap.end())
				break;
			cur = next;
			size += _instmap[cur].first.size();
		}
		if (size < smallest)
			smallest = size;
	}
	return smallest;
}

bool DyninstFunction::IsExcludedFunction(InstType T) {
	//if(_track->ShouldInstrimentFunciton(_func, T))
	//if (!_track->ShouldInstrimentFunciton(_func, T))
	//	std::cerr << "[DyninstFunction::IsExcludedFunction] Returned false for function" << std::endl;
	//if (!_track->ShouldInstrimentModule(_func, T))
	//	std::cerr << "[DyninstFunction::IsExcludedFunction] Returned false for module" << std::endl;
	if (_track->ShouldInstrimentFunciton(_func, T) && (_obj->pathName().find("qb_cuda8_mpirun") != std::string::npos || _obj->pathName().find("libcufftw") != std::string::npos 
		|| _obj->pathName().find("/main") != std::string::npos || _obj->pathName().find("/cuibm") != std::string::npos))
		return false;
	if (_track->ShouldInstrimentFunciton(_func, T) && _track->ShouldInstrimentModule(_func, T))
		return false;
	return true;

}
