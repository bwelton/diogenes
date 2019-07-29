#include "CheckAccesses.h"
#include <iostream>
CheckAccesses::CheckAccesses() : _doNotCheck(false) {};

void CheckAccesses::AddMemoryTransfer(MemoryRange & range) {
	_doNotCheck = true;
	_current.push_back(range);
	_doNotCheck = false;
}

void CheckAccesses::AddUnifiedMemRange(MemoryRange & range) {
	_doNotCheck = true;
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
void CheckAccesses::SyncCalled() {
	_prev = _current;
	_memoryRangesPrev = _memoryRanges;
}

bool CheckAccesses::IsAddressProtected(uint64_t addr) {
	if (_doNotCheck)
		return false;
	for (auto i : _prev)
		if(i.IsInRange(addr))
			return true;
	if (contains(_memoryRangesPrev,addr) == true)
		return true;
	//for (auto i : _unifiedMemory)
	//	if(i.IsInRange(addr))
	//		return true;		
	return false;
}
