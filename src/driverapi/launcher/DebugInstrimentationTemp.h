#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>

class DebugInstrimentationTemp {
public:
	DebugInstrimentationTemp(std::string infile, std::string outfile);
	void PopulateInstructions();
	bool InstrimentFunction(std::string & funcName, std::string & libraryName, uint64_t offset);
	~DebugInstrimentationTemp();
private:
	std::ofstream _outFile;
	std::ifstream _inFile;
	std::set<std::string> _instFunctions;
};
