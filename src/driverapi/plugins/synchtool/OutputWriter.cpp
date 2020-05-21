#include "OutputWriter.h"

OutputWriter::OutputWriter(bool timeType) : _curPos(1), _timeType(timeType), _prevDependency(0) {
	if (timeType == false) {
		_accessFile.reset(new OutputFile(std::string("LS_trace.bin")));
		_stackKeyFile.reset(new OutputFile(std::string("LS_stackkey.bin")));
		_syncAccess.reset(new OutputFile(std::string("LS_syncaccess.bin")));
	} else {
		_firstUse.reset(new OutputFile(std::string("FI_stackkey.bin")));
		_timingValues.reset(new OutputFile(std::string("FI_trace.bin")));
	}
}

// LS_stackkey.bin = stacks that synchronize
// LS_trace.bin = stacks that write/read locations of GPU written data
// LS_syncaccess.bin = sequence info. 

void OutputWriter::RecordAccess(uint64_t id, std::vector<uint64_t> & currentStack, double timeID) {
	uint64_t hash = HashStack(currentStack);
	if (_prevStacks.find(hash) == _prevStacks.end()) {
		if (_timeType == false)
			_stackKeyFile->Write(_curPos, currentStack);
		else 
			_firstUse->Write(_curPos, currentStack);
		_prevStacks[hash] = _curPos;
		_curPos++;
	}
	if (_timeType == false) {
		_accessFile->Write(id, _prevStacks[hash]);
	} else {
		_timingValues->Write(id, _prevStacks[hash], timeID);
	}
}

void OutputWriter::FlushAll() {
	if (_accessFile)
		_accessFile->FlushFile();
	if (_stackKeyFile)
		_stackKeyFile->FlushFile();
	if (_syncAccess)
		_syncAccess->FlushFile();
	if (_timingValues)
		_timingValues->FlushFile();
	if (_firstUse)
		_firstUse->FlushFile();
}

void OutputWriter::WriteSequenceInfo(std::vector<uint64_t> & currentStack, bool newDependents) {
	if (_timeType == true) 
		return;
	uint64_t hash = HashStack(currentStack);	
	if (_prevStacks.find(hash) == _prevStacks.end()) {
		_stackKeyFile->Write(_curPos, currentStack);
		_prevStacks[hash] = _curPos;
		_curPos++;
	}
	if (newDependents) {
		_prevDependency = _prevStacks[hash];
		_syncAccess->WriteSequenceInfo(_prevDependency, 1, 0);
	} else {
		_syncAccess->WriteSequenceInfo(_prevStacks[hash], 0, _prevDependency);		
	}

}

uint64_t OutputWriter::HashStack(std::vector<uint64_t> & currentStack) {
	std::stringstream ss;
	for (auto i : currentStack)
		ss << i << ",";
	return std::hash<std::string>()(ss.str());	
}