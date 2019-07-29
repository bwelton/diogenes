#pragma once
#include <string.h>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <cassert>
#include <deque>
#include <sys/time.h>
#include <cstdlib>
#include <sstream>
#include <tuple>
#include <utility> 
#include <stdarg.h>
#include <map>
#include <set> 
#include <iomanip>
#include <string>
#include <cassert>
#include <sys/types.h>
#include <unistd.h>
#include <mutex>
#include <queue>
#include <boost/program_options.hpp>

// Dyninst includes
#include "CodeObject.h"
#include "CFG.h"
#include "PatchObject.h"
#include "PatchMgr.h"
#include "Point.h"
#include "BPatch_object.h"
#include "BPatch_snippet.h"
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_image.h"
#include "BPatch_function.h"
#include "BPatch_Vector.h"
#include "BPatch_point.h"
#include "BPatch_addressSpace.h"
#include "BPatch_statement.h"
#include "dynC.h"
#include "set"
#include "LogInfo.h"
#include "Constants.h"

//#define SP_DEBUG 1

struct StackPoint {
	std::string libname;
	std::string funcName;
	uint64_t libOffset;
	uint64_t funcOffset;
	uint64_t timerID;
	bool inMain;
	bool empty;
	// Not used for anything other than giving to python analysis tool....
	uint64_t lineNum;
	std::string fileName;
	StackPoint(std::string _libname, std::string _funcName, uint64_t _libOffset, uint64_t _funcOffset) : empty(false), libname(_libname), funcName(_funcName), libOffset(_libOffset), funcOffset(_funcOffset) 
	{
		fileName = std::string("");
		lineNum = 0;
		inMain = false;
	};

	StackPoint() : empty(true), libOffset(0), funcOffset(0), funcName(std::string("")), libname(std::string("")) {
		fileName = std::string("");
		lineNum = 0;
		inMain = false;
	};

	bool IsEqual(StackPoint & other) {
		if(libname == other.libname && libOffset == other.libOffset)
			return true;
		return false;
	};

	void Print() {
		std::cerr << "[StackPoint] Stack Point Debug" << std::endl;
		std::cerr << "[StackPoint] \t libname: " << libname << std::endl;
		std::cerr << "[StackPoint] \t FuncName: " << funcName << std::endl;
		std::cerr << "[StackPoint] \t libOffset: " << std::hex << libOffset << std::endl;
		std::cerr << "[StackPoint] \t funcOffset: " << std::hex << funcOffset << std::endl;
	};
	int Serialize(char * data, int size) {
		int pos = 0;
		if (size < sizeof(uint64_t) + libname.size() + sizeof(uint64_t))
			return -1;

#ifdef SP_DEBUG
		std::cerr << "Encoding: " << libname << "," << libOffset << std::endl;
#endif
		uint64_t stringSize = libname.size();
		std::memcpy(&(data[0]), &stringSize, sizeof(uint64_t));
		pos += sizeof(uint64_t);
		std::memcpy(&(data[pos]),libname.c_str(), libname.size());
		pos += libname.size();
		std::memcpy(&(data[pos]), &libOffset, sizeof(uint64_t));
		return pos + sizeof(uint64_t);
	}

	int Deserialize(char * data, int len) {
		char tmp[1024];
		uint64_t size = 0;
		int pos = 0;
		std::memcpy(&size, data, sizeof(uint64_t));
		pos += sizeof(uint64_t);
		if (len < size + sizeof(uint64_t) + sizeof(uint64_t))
			assert(len < size + sizeof(uint64_t) + sizeof(uint64_t));
		if (size >= 1024)
			assert(size < 1024);
		std::memcpy(tmp, &(data[pos]), size);

		tmp[size] = '\000';
		libname = std::string(tmp);
		pos += size;
		std::memcpy(&libOffset, &(data[pos]), sizeof(uint64_t));
#ifdef SP_DEBUG
		std::cerr << "Decoding: " << libname << "," << libOffset << std::endl;
#endif
		empty = false;
		pos += sizeof(uint64_t);
		return pos;
	}

};

typedef std::vector<StackPoint> StackPointVec;

struct StackHasher{
	std::stringstream ss;
	uint64_t HashStack(std::vector<StackPoint> & points) {
		if (points.size() == 0)
			return 0;
		ss.str(std::string());
		for (auto i : points)
			ss << i.libname << "," << i.libOffset << "|";
		return std::hash<std::string>()(ss.str());
	}
};


// Key file for the stacks outputted
struct StackKeyWriter {
	char buffer[512000];
	uint64_t curPos;
	std::map<uint64_t, uint64_t> prevStacks;
	StackHasher h;
	FILE * out;
	StackKeyWriter(FILE * fp) {
		out = fp;
		curPos = 1;
	}
	StackKeyWriter(FILE * fp, uint64_t startPos) {
		out = fp;
		curPos = startPos;
	}
	~StackKeyWriter() {
		fclose(out);
	}
	uint64_t InsertStack(std::vector<StackPoint> & points){
		uint64_t hash = h.HashStack(points);
		if (hash == 0)
			return 0;
		if (prevStacks.find(hash) != prevStacks.end()) {
			hash = prevStacks[hash];
			return hash;
		} else {
			prevStacks[hash] = curPos;
			hash = curPos;
			curPos++;
		}
		int pos = 0;
		std::stringstream outStr;
		outStr << hash << "$";
		for (auto i : points)
			outStr << i.libname << "@" << i.libOffset << "$";

		std::string t = outStr.str();
		t.pop_back();
		t = t + std::string("\n");
		do {
			const char * myString = t.c_str();
			pos += fwrite(&myString[pos], 1, t.size() - pos, out);
		} while(pos != t.size());
		std::cerr << "Wrote stack with hash id: " << hash << std::endl;
		return hash;
	}
	void InsertStack(uint64_t id, std::vector<StackPoint> & points){
		uint64_t hash = id;
		int pos = 0;
		std::stringstream outStr;
		outStr << hash << "$";
		for (auto i : points)
			outStr << i.libname << "@" << i.libOffset << "$";

		std::string t = outStr.str();
		t.pop_back();
		t = t + std::string("\n");
		assert(fwrite(t.c_str(), 1, t.size(), out) == t.size());
		
		// do {
		// 	const char * myString = t.c_str();
		// 	pos += fwrite(&myString[pos], 1, t.size() - pos, out);
		// } while(pos != t.size());
		std::cerr << "Wrote stack with hash id: " << hash << std::endl;
	}
};

struct LSDependency;

typedef std::shared_ptr<LSDependency> LSDependencyPtr;
typedef std::vector<LSDependencyPtr> LSDependencyVec;

struct LSDependency{
	uint64_t id;
	uint64_t newDependents;
	uint64_t lastDependent;

	inline static uint64_t GetSize() {
		return sizeof(uint64_t) * 3;
	};
	static LSDependencyPtr Deserialize(char * data) {
		LSDependencyPtr ret;
		ret.reset(new LSDependency());
		uint64_t * tmp = (uint64_t*)data;
		ret->id = tmp[0];
		ret->newDependents = tmp[1];
		ret->lastDependent = tmp[2];
		return ret;
	};
};

struct ReadDependencyFile {
	FILE * _fid;
	ReadDependencyFile(FILE * fp) : _fid(fp){ };

	~ReadDependencyFile() { fclose(_fid);}

	void Read(LSDependencyVec & vec) {
		fseek(_fid, 0, SEEK_END);
		size_t size = ftell(_fid);
		fseek(_fid, 0, SEEK_SET);
		char * data = new char[size];
		fread(data, 1, size, _fid);
		int pos = 0;
		while (size - pos >= LSDependency::GetSize()){
			vec.push_back(LSDependency::Deserialize(data+pos));
			pos += LSDependency::GetSize();
		}
	};
};

struct StackKeyReader {
	FILE * in;
	StackKeyReader(FILE * fp) {
		in = fp;
	}
	~StackKeyReader() {
		fclose(in);
	}
	std::map<uint64_t, std::vector<StackPoint> > ReadStacks() {
		std::map<uint64_t, std::vector<StackPoint> > ret;
		fseek(in, 0, SEEK_END);
  		uint64_t size = ftell(in);
  		fseek(in, 0, SEEK_SET);
  		std::shared_ptr<char> tmp(new char[size+1]);
  		uint64_t readPos = 0;
  		do {
  			char * myString = tmp.get();
  			readPos += fread(&myString[readPos], 1, size - readPos, in);
  		} while (readPos != size);
  		tmp.get()[size] = '\000';
  		char * token = strtok(tmp.get(), "\n");
  		while (token != NULL) {
  			uint64_t hash = 0;
  			std::string tmpToken = std::string(token);
  			std::istringstream ifstring(tmpToken);
  			std::string line;
  			while (getline(ifstring, line, '$')) {
  				if (line.find("@") == std::string::npos){
  					hash = std::stoull(line);
  					//std::cerr << "My hash - " << hash << std::endl;
  					ret[hash] = std::vector<StackPoint>();
  				} else {
  					StackPoint sp;
  					sp.libname = line.substr(0, line.find('@'));	
  					sp.libOffset = std::stoull(line.substr(line.find('@')+1)); 	
  					std::cerr <<  hash << "," << sp.libname << "," << sp.libOffset << std::endl;
  					ret[hash].push_back(sp);
  				}
  			}

  			// uint64_t hash = std::stoull(tmpToken.substr(0, tmpToken.find('$')));
  			// uint64_t start = tmpToken.find('$') + 1;
  			
  			// while(start < tmpToken.size()){
  			// 	StackPoint sp;
  			// 	std::string r;
  			// 	if (tmpToken.find('$', start) != std::string::npos){
  			// 		r = tmpToken.substr(start, tmpToken.find('$', start));
  			// 		start = tmpToken.find('$', start) + 1;
	  		// 	}	else{
  			// 		r = tmpToken.substr(start);
  			// 		start = tmpToken.size();
  			// 	}
  			// 	std::cerr << r << std::endl;
  			// 	sp.libname = r.substr(0, r.find('@'));	
  			// 	sp.libOffset = std::stoull(r.substr(r.find('@')));
  			// 	std::cerr <<  hash << "," << sp.libname << "," << sp.libOffset << std::endl;
  			// 	ret[hash].push_back(sp);
  			// }
  			token = strtok(NULL,"\n");
  		}


		// while(fread(&size,1, sizeof(int), in) > 0) {
		// 	if (feof(in))
		// 		break;
		// 	std::vector<StackPoint> points; 
		// 	uint64_t hashId, recCount, pos;
		// 	pos = 0;
		// 	int read = 0;
		// 	do {
		// 		read += fread(&(buffer[read]), 1, size - read, in);
		// 	} while (read < size);
		// 	assert(read == size);
		// 	std::memcpy(&hashId, &(buffer[pos]), sizeof(uint64_t));
		// 	pos += sizeof(uint64_t);
		// 	std::memcpy(&recCount, &(buffer[pos]), sizeof(uint64_t));
		// 	pos += sizeof(uint64_t);
		// 	for (int i = 0; i < recCount; i++) {
		// 		StackPoint sp;
		// 		pos += sp.Deserialize(&(buffer[pos]), size - pos);
		// 		points.push_back(sp);
		// 	}
		// 	ret[hashId] = points;
		// 	std::cerr << "Read stack with hash id: " << hashId << std::endl;
		// }
		return ret;
	}
};


// namespace std {
// 	template<> struct less<StackPoint> {
// 		bool operator() (const StackPoint& lhs, const StackPoint& rhs) {
// 			return lhs.libOffset < rhs.libOffset;
// 		};
// 	};
// };

