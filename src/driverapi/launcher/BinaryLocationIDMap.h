#pragma once
#include <string>
#include <fstream>
#include <map> 
#include <utility>
#include <algorithm>
#include "StackPoint.h"

class BinaryLocationIDMap {
public:
	BinaryLocationIDMap();
	uint64_t StorePosition(std::string & libname, uint64_t offsetAddr);
	uint64_t GetOffsetForID(uint64_t id);
	std::string * GetLibnameForID(uint64_t id);
	StackPoint BuildStackPoint(uint64_t id);
	bool SetAbsoluteID(uint64_t id, std::string & libname, uint64_t offsetAddr);
	void DumpLocationMap(std::string outFile);
	bool AlreadyExists(std::string & libname, uint64_t offsetAddr);
private:
	uint64_t _curPos, _libids;
	std::map<uint64_t, std::map<uint64_t,uint64_t> > _libIdOffsetToID;
	std::map<uint64_t,std::pair<uint64_t, uint64_t>> _idToLibOffset;
	std::map<std::string, uint64_t> _libnameToLibID;
	std::map<uint64_t,std::string> _libIdtoLibname;
};
