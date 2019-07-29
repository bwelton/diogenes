#include "DebugInstrimentationTemp.h"

DebugInstrimentationTemp::DebugInstrimentationTemp(std::string infile, std::string outfile) {
	_inFile.open(infile.c_str());
	if (_inFile.good()){
		PopulateInstructions();
		_inFile.close();
	}
	_outFile.open(outfile.c_str());
}
DebugInstrimentationTemp::~DebugInstrimentationTemp() {
	_outFile.close();
}

bool DebugInstrimentationTemp::InstrimentFunction(std::string & funcName, std::string & libraryName, uint64_t offset) {
	std::stringstream ss;
	ss << funcName <<"," << libraryName << "," << std::hex << offset << std::endl;
	std::string tmp = ss.str();
	if (_instFunctions.size() > 0) {
		if (_instFunctions.find(tmp) == _instFunctions.end())
			return false;
	}
	_outFile << tmp;
	return true;
}

void DebugInstrimentationTemp::PopulateInstructions() {
	std::string line;
	while (std::getline(_inFile, line)) {
		std::cout << "[DebugInstrimentationTemp::PopulateInstructions] Reading funciton id - " << line;
		_instFunctions.insert(line);
	}
}


