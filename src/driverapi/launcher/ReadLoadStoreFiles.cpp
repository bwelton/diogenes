#include "ReadLoadStoreFiles.h"

ReadLoadStoreFiles::ReadLoadStoreFiles(BinaryLocationIDMap * locationMap) : _map(locationMap) {}

std::map<uint64_t, std::vector<StackPoint> > ReadLoadStoreFiles::ReadKeyFile(std::string ifname) {
	std::map<uint64_t, std::vector<StackPoint> > ret;
	FILE * inFile = fopen(ifname.c_str(), "rb");
	std::cerr << "[ReadLoadStoreFiles::ReadKeyFile] Opening File.... " << ifname << std::endl;
	if (inFile == NULL)
		return ret;
	fseek(inFile, 0, SEEK_END);
	uint64_t elementCount = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	uint64_t readCount = 0;
	uint64_t size, stackId, locationId;
	std::cerr << "[ReadLoadStoreFiles::ReadKeyFile] Has  " << elementCount << " bytes of data...." << std::endl;
	while (readCount < elementCount) {
		size = 0;
		fread(&size, 1, sizeof(uint64_t), inFile);
		readCount += sizeof(uint64_t);
		fread(&stackId, 1, sizeof(uint64_t), inFile);
		std::cerr << "[ReadLoadStoreFiles::ReadKeyFile] Element has stack id of " << stackId << " and size of " << size << std::endl;
		ret[stackId] = std::vector<StackPoint>();
		readCount += sizeof(uint64_t);
		size -= sizeof(uint64_t);

		while (size > 0)  {
			fread(&locationId,1,sizeof(uint64_t), inFile);
			std::cerr << "[ReadLoadStoreFiles] Stack Location ID: " << locationId << std::endl;
			ret[stackId].push_back(_map->BuildStackPoint(locationId));
			size -= sizeof(uint64_t);
			readCount += sizeof(uint64_t);
		}
	}
	fclose(inFile);
	return ret;
}	

void ReadLoadStoreFiles::OutputTraceKey(std::string inDataFile, std::string outFile) {
	std::set<uint64_t> seenIds;
	FILE * inFile = fopen(inDataFile.c_str(), "rb");
	if (inFile == NULL)
		return;
	FILE * out = fopen(outFile.c_str(), "wb");
	StackKeyWriter writer(out);
	fseek(inFile, 0, SEEK_END);
	uint64_t elementCount = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	uint64_t readCount = 0;
	uint64_t stackId, locationId;
	while (readCount < elementCount) {
		fread(&locationId, 1, sizeof(uint64_t), inFile);
		fread(&stackId, 1, sizeof(uint64_t), inFile);
		std::vector<StackPoint> tmp;
		if(seenIds.find(locationId) == seenIds.end()){
			tmp.push_back(_map->BuildStackPoint(locationId));
			_traceStacks.push_back(tmp[0]);

			std::cerr << tmp[0].libname << "," << tmp[0].libOffset << std::endl;
			writer.InsertStack(locationId, tmp);
			seenIds.insert(locationId);
		}
		readCount += (sizeof(uint64_t) * 2);
	}
	fclose(inFile);
}

void ReadLoadStoreFiles::OutputTraceKeyFirstUse(std::string inDataFile, std::string outFile) {
	std::set<uint64_t> seenIds;
	FILE * inFile = fopen(inDataFile.c_str(), "rb");
	if (inFile == NULL)
		return;
	FILE * out = fopen(outFile.c_str(), "wb");
	StackKeyWriter writer(out);
	fseek(inFile, 0, SEEK_END);
	uint64_t elementCount = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	uint64_t readCount = 0;
	uint64_t stackId, locationId;
	double time; 
	while (readCount < elementCount) {
		fread(&locationId, 1, sizeof(uint64_t), inFile);
		fread(&stackId, 1, sizeof(uint64_t), inFile);
		fread(&time, 1, sizeof(double), inFile);
		if (locationId == 0){
			readCount += (sizeof(uint64_t) * 2) + sizeof(double);
			continue;
		}
		std::vector<StackPoint> tmp;
		if(seenIds.find(locationId) == seenIds.end()){
			tmp.push_back(_map->BuildStackPoint(locationId));
			_traceStacks.push_back(tmp[0]);
			std::cerr << tmp[0].libname << "," << tmp[0].libOffset << std::endl;
			writer.InsertStack(locationId, tmp);
			seenIds.insert(locationId);
		}
		readCount += (sizeof(uint64_t) * 2) + sizeof(double);
	}
	fclose(inFile);
}

void ReadLoadStoreFiles::CreateStackKey(std::string inFile, std::string outFile) {
	std::map<uint64_t, std::vector<StackPoint> > keys = ReadKeyFile(inFile);
	_keyStacks = keys;

	if (keys.size() == 0)
		return;
	FILE * out = fopen(outFile.c_str(), "wb");
	StackKeyWriter writer(out);
	for (auto i : keys) {
		std::cerr << "Writing - " << i.first << " to file " << outFile << std::endl;
		writer.InsertStack(i.first, i.second);
	}
}

std::vector<StackPoint> ReadLoadStoreFiles::GetTraces() {
	return _traceStacks;
}
std::map<uint64_t, std::vector<StackPoint> > ReadLoadStoreFiles::GetKeys() {
	return  _keyStacks;
}
