#pragma once
struct MemoryRange {
	uint64_t begin;
	uint64_t end; 
	uint64_t size; 
	uint64_t transferID;
	uint64_t unifiedMemory;
	int stream;
	uint64_t StartAddr() {
		return begin;
	}
	uint64_t EndAddr() {
		return begin+size;
	}
	bool IsInRange(uint64_t addr) {
		if (addr >= begin && addr <= end)
			return true;
		return false;
	}
	bool DoesRangeMatch(MemoryRange & range) {
		if (range.begin == begin && unifiedMemory == 1)
			return true;
		return false;
	}
};
