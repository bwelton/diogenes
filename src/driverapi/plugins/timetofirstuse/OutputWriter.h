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
	void Write(uint64_t id, uint64_t stackId, double time) {
		fwrite(&id, 1, sizeof(uint64_t), outFile);
		fwrite(&stackId, 1, sizeof(uint64_t), outFile);
		fwrite(&time, 1, sizeof(double), outFile);
	}

	void Write(uint64_t stackId, std::vector<uint64_t> & currentStack) {
		uint64_t size = sizeof(uint64_t) + currentStack.size() * sizeof(uint64_t);
		fwrite(&size, 1, sizeof(uint64_t), outFile);
		fwrite(&stackId, 1, sizeof(uint64_t), outFile);
		//std::cerr << "[OutputFile] Writing currentStack of size - " << currentStack.size() << std::endl;
		for (auto i : currentStack)
			fwrite(&i, 1, sizeof(uint64_t), outFile);
	}
	~OutputFile() {
		fflush(outFile);
		fclose(outFile);
	}
};

class OutputWriter {
public:
	OutputWriter();
	void RecordAccess(uint64_t id, std::vector<uint64_t> & currentStack, double time);
	uint64_t HashStack(std::vector<uint64_t> & currentStack);
private:
	uint64_t _curPos;
	std::map<uint64_t, uint64_t> _prevStacks;
	std::shared_ptr<OutputFile> _accessFile;
	std::shared_ptr<OutputFile> _stackKeyFile;
};

typedef std::shared_ptr<OutputWriter> OutputWriterPtr;