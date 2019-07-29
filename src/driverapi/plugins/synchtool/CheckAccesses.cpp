#include "CheckAccesses.h"
#include <iostream>
//#define DEBUG_CHECK 1
CheckAccesses::CheckAccesses() : _doNotCheck(false), _reset(false), _newDependents(false) {};

void CheckAccesses::AddMemoryTransfer(MemoryRange & range) {
	_doNotCheck = true;
	_current.push_back(range);
	_newDependents = true;
	_doNotCheck = false;
}

void CheckAccesses::AddUnifiedMemRange(MemoryRange & range) {
	_doNotCheck = true;
	_newDependents = true;
	_unifiedMemory.push_back(range);
	_memoryRanges += interval<uint64_t>::closed(range.StartAddr(), range.EndAddr());
	//std::cerr << "Range interval after adding range - " << interval<uint64_t>::closed(range.StartAddr(), range.EndAddr()) << std::endl;
	// for(auto i : _memoryRanges)
	// 	std::cerr << i << std::endl;
	_doNotCheck = false;
}
void CheckAccesses::RemoveUnifiedMemoryRange(MemoryRange & range) {
	_doNotCheck = true;
	int removePos = -1;
	for (int i = 0; i < _unifiedMemory.size(); i++){
		if (_unifiedMemory[i].DoesRangeMatch(range)){
			removePos = i;
			break;
		}
	}
	// std::cerr << "Range interval before removal of range - " << interval<uint64_t>::closed(_unifiedMemory[removePos].StartAddr(), _unifiedMemory[removePos].EndAddr()) << std::endl;
	// for(auto i : _memoryRanges)
	// 	std::cerr << i << std::endl;
	if (removePos == -1){
		_doNotCheck = false;
		return;
	}
	_memoryRanges.erase(interval<uint64_t>::closed(_unifiedMemory[removePos].StartAddr(), _unifiedMemory[removePos].EndAddr()));
	// std::cerr << "Range interval after removal of range - " << interval<uint64_t>::closed(_unifiedMemory[removePos].StartAddr(), _unifiedMemory[removePos].EndAddr()) << std::endl;
	// for(auto i : _memoryRanges)
	// 	std::cerr << i << std::endl;
	
	if (removePos >= 0)
		_unifiedMemory.erase(_unifiedMemory.begin() + removePos);

	_doNotCheck = false;

}

bool CheckAccesses::GetNewDependents() {
	return _newDependents;
}
void CheckAccesses::SyncCalled() {
	//_reset = true;
	if (_reset == true) {
		#ifdef DEBUG_CHECK
		std::cerr << "[CheckAccesses::SyncCalled] Resetting Range" << std::endl;
		#endif
		_prev = _current;
		_memoryRangesPrev = _memoryRanges;
		_reset = false;
	} else {
		#ifdef DEBUG_CHECK
		std::cerr << "[CheckAccesses::SyncCalled] Adding to previous range since sync previous sync produced nothing" << std::endl;
		#endif
		_prev.insert(_prev.end(), _current.begin(), _current.end());
		_memoryRangesPrev = _memoryRanges;
	}
	_newDependents = false;
	_current.clear();
}

bool CheckAccesses::IsAddressRangeProtected(uint64_t addr, uint64_t count) {
	if (_doNotCheck)
		return false;
	for (auto i : _prev){ 
		if(i.IsInRangeWithCount(addr,count)){
			_reset = true;
			return true;
		}
	}
	for (auto mn : _memoryRangesPrev){
		if ((addr >= mn.lower() && addr <= mn.upper()) || (addr <= mn.lower() && addr +count >= mn.lower())){
			_reset = true;
			return true;
		}
	}
	//for (auto i : _unifiedMemory)
	//	if(i.IsInRange(addr))
	//		return true;		
	return false;
}


bool CheckAccesses::IsAddressProtected(uint64_t addr) {
	#ifdef DEBUG_CHECK 
	std::cerr << "[CheckAccesses::IsAddressProtected] Checking address: " << std::hex << addr << std::endl;
	#endif

	if (_doNotCheck)
		return false;
	#ifdef DEBUG_CHECK 
	std::cerr << "[CheckAccesses::IsAddressProtected] Past Check" << std::endl;
	#endif		
	for (auto i : _prev) {
		#ifdef DEBUG_CHECK 
		std::cerr << "[CheckAccesses::IsAddressProtected] Checking range: " << std::hex << i.StartAddr() << " to " << std::hex << i.EndAddr() << std::endl;
		#endif		
		if(i.IsInRange(addr)){
			_reset = true;
			//#ifdef DEBUG_CHECK 
			std::cerr << "[CheckAccesses::IsAddressProtected] Found in range: " << std::hex << addr << std::endl;
			//#endif
			return true; 

		}
	}
	if (contains(_memoryRangesPrev,addr) == true) {
		#ifdef DEBUG_CHECK 
		std::cerr << "[CheckAccesses::IsAddressProtected] Found in range (unified): " << std::hex << addr << std::endl;
		#endif
		_reset = true;
		return true;
	}
	//for (auto i : _unifiedMemory)
	//	if(i.IsInRange(addr))
	//		return true;		
	return false;
}
