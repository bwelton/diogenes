#include "OutputWriter.h"

OutputWriter::OutputWriter() : _curPos(1) {
	_accessFile.reset(new OutputFile(std::string("FI_trace.bin")));
	_stackKeyFile.reset(new OutputFile(std::string("FI_stackkey.bin")));
}

void OutputWriter::RecordAccess(uint64_t id, std::vector<uint64_t> & currentStack, double time) {

	uint64_t hash = HashStack(currentStack);
	if (_prevStacks.find(hash) == _prevStacks.end()) {
		_stackKeyFile->Write(_curPos, currentStack);
		_prevStacks[hash] = _curPos;
	}
	_accessFile->Write(id, _prevStacks[hash],time);
}

uint64_t OutputWriter::HashStack(std::vector<uint64_t> & currentStack) {
	std::stringstream ss;
	for (auto i : currentStack)
		ss << i << ",";
	return std::hash<std::string>()(ss.str());	
}