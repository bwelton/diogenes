#include "LoadStoreDriver.h"
extern "C" void DYNINST_disableCondInst();
extern "C" void DYNINST_enableCondInst();
InstrimentationControl::InstrimentationControl() : _state(DISABLED) {}

void InstrimentationControl::EnteringInst() {
//	DYNINST_disableCondInst();
}

void InstrimentationControl::SyncCalled() {
	_state = SYNC_NOUSE;
}

void InstrimentationControl::FoundUse() {
	_state = SYNC_USEFOUND;
}

void InstrimentationControl::ExitingInst() {
	if (_state == SYNC_NOUSE) {
//		DYNINST_enableCondInst();
	}
}

LoadStoreDriver::LoadStoreDriver(CheckAccessesPtr access, bool timefu) : _firstSync(true), _syncTriggered(false), _firstWrite(true), _found(false), _access(access), _timefu(timefu) {
	_start = std::chrono::high_resolution_clock::now();
	_checkCount = 0;
	_totalCheck = 0;
}

void LoadStoreDriver::RecordAccessRange(uint64_t id, uint64_t addr, uint64_t count) {
	//_checkCount++;
	if (_syncTriggered && !_found) {
		if(_access->IsAddressRangeProtected(addr, count)) {
			if (_firstWrite)
				_writer.reset(new OutputWriter(_timefu));
			_firstWrite = false;
			_writer->RecordAccess(id,_stackAtSync);
			_found = true;
			_instControler.FoundUse();
		}
	}
}

void LoadStoreDriver::RecordAccess(uint64_t id, uint64_t addr) {
	//_checkCount++;
	if (_syncTriggered && !_found) {
		if(_access->IsAddressProtected(addr)) {
			if (_firstWrite)
				_writer.reset(new OutputWriter(_timefu));

			_firstWrite = false;
			_writer->RecordAccess(id,_stackAtSync);
			_found = true;
			/*if (_checkCount > 100000){
				_totalCheck += _checkCount;
				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> diff =  std::chrono::high_resolution_clock::now() - _start;
				std::cerr << "[LoadStoreDriver::RecordAccess] Flushing files at checkcount " << std::dec << _totalCheck << " at time " << std::dec << diff.count() << std::endl;
				_writer->FlushAll();
				if (diff.count()/60 > 40)
					exit(0);
				_checkCount = 0;
			}*/			
			_instControler.FoundUse();
		}
	}
}

void LoadStoreDriver::EnterInstrimentation() {
	_instControler.EnteringInst();
}

void LoadStoreDriver::ExitingInstrimentation() {
	_instControler.ExitingInst();
}

void LoadStoreDriver::PushStack(uint64_t stack) {
	_storedStack.push_back(stack);
}

bool LoadStoreDriver::ShouldSignalSync(bool inSync) {
	// This kludge is needed to catch cases where the low-level synchronize is called by 
	// a libcuda function more than once
	
	// No synchronization occured, return immediately.
	if (!inSync && _firstSync)
		return false;

	// Dont stop on the first sync, record the stack depth and continue.
	if(_firstSync) {
		_syncStackDepth = _storedStack.size();
		_stackAtSync = _storedStack;
		_firstSync = false;
		return false;
	} 

	// We have synchronized prior, check if we are a few stack positions away from
	// the low level synchronization.
	if (!inSync && _syncStackDepth >= _storedStack.size() + 3) {
		_firstSync = true;
		return true;
	}

	return false;
}

void LoadStoreDriver::PopStack(uint64_t stack) {
	if (_storedStack.back() == stack) 
		_storedStack.pop_back();
	else {
		for (int i = _storedStack.size() - 1; i >= 0; i = i - 1) {
			if (_storedStack[i] == stack){
				_storedStack.erase(_storedStack.begin() + i, _storedStack.end());
				break;
			}
		}
	}
	// if (ShouldSignalSync(false))
	// 	SignalSync();
}

void LoadStoreDriver::SyncCalled() {
	//ShouldSignalSync();
	SignalSync();
}

void LoadStoreDriver::SignalSync() {
	if (_firstWrite){
		_firstWrite = false;
		_writer.reset(new OutputWriter(_timefu));
	}
	_writer->WriteSequenceInfo(_storedStack,_access->GetNewDependents());
	_access->SyncCalled();
	_found = false;
	_syncTriggered = true;
	_stackAtSync = _storedStack;
	_instControler.SyncCalled();
	// for(auto i : _stackAtSync){
	// 	std::cerr << "[LoadStoreDriver::SignalSync] Stack record at sync: " << i << std::endl;
	// }
}

void LoadStoreDriver::RecordAccessWithTime(uint64_t id, uint64_t addr, double timetv) {
	if (_syncTriggered && !_found) {
		if(_access->IsAddressProtected(addr)) {
			WriteStackTime(id, timetv);
			_found = true;
		}
	}
}

void LoadStoreDriver::WriteStackTime(uint64_t id, double tv) { 
	if (_firstWrite){
		_writer.reset(new OutputWriter(_timefu));
		_firstWrite = false;
	}
	_writer->RecordAccess(id,_stackAtSync,tv);
}
