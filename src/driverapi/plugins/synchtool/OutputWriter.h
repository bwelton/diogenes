#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <functional>
#include <sstream> 
struct OutputFile {
	FILE * outFile;
	OutputFile(std::string filename) {
		outFile = fopen(filename.c_str(),"w");
	}
	void Write(uint64_t id, uint64_t stackId) {
		fwrite(&id, 1, sizeof(uint64_t), outFile);
		fwrite(&stackId, 1, sizeof(uint64_t), outFile);
		fflush(outFile);
	}
	void Write(uint64_t id, uint64_t stackId, double timeID) {
		fwrite(&id, 1, sizeof(uint64_t), outFile);
		fwrite(&stackId, 1, sizeof(uint64_t), outFile);
		fwrite(&timeID, 1, sizeof(double), outFile);
		fflush(outFile);
	}

	void WriteSequenceInfo(uint64_t stackId, uint64_t newDependents, uint64_t dependency) {
		fwrite(&stackId, 1, sizeof(uint64_t), outFile);
		fwrite(&newDependents, 1, sizeof(uint64_t), outFile);
		fwrite(&dependency, 1, sizeof(uint64_t), outFile);
		fflush(outFile);		
	}

	void Write(uint64_t stackId, std::vector<uint64_t> & currentStack) {
		uint64_t size = sizeof(uint64_t) + currentStack.size() * sizeof(uint64_t);
		fwrite(&size, 1, sizeof(uint64_t), outFile);
		fwrite(&stackId, 1, sizeof(uint64_t), outFile);
		//std::cerr << "[OutputFile] Writing currentStack of size - " << currentStack.size() << std::endl;
		for (auto i : currentStack)
			fwrite(&i, 1, sizeof(uint64_t), outFile);
		fflush(outFile);
	}

	~OutputFile() {
		fflush(outFile);
		fclose(outFile);
	}
};

class OutputWriter {
public:
	OutputWriter(bool timeType = false);
	void RecordAccess(uint64_t id, std::vector<uint64_t> & currentStack, double timeID = 0.0);
	uint64_t HashStack(std::vector<uint64_t> & currentStack);
	void WriteSequenceInfo(std::vector<uint64_t> & currentStack, bool newDependents);
private:
	uint64_t _prevDependency;
	uint64_t _curPos;
	bool _timeType;
	std::map<uint64_t, uint64_t> _prevStacks;
	std::shared_ptr<OutputFile> _accessFile;
	std::shared_ptr<OutputFile> _stackKeyFile;
	std::shared_ptr<OutputFile> _firstUse;
	std::shared_ptr<OutputFile> _timingValues;
	std::shared_ptr<OutputFile> _syncAccess;
};

typedef std::shared_ptr<OutputWriter> OutputWriterPtr;