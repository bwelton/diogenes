#include "StackStorage.h"



StackRecord::StackRecord() {}

StackRecord::StackRecord(uint64_t id, std::vector<StackPoint> & points) : _id(id), _points(points) {

}

StackPoint StackRecord::GetFirstCudaCall() {
	for (int i = _points.size() - 1; i >= 0; i = i - 1){
		if(_points[i].libname.find("libcuda.so") != std::string::npos){
			_points[i].empty = false;
#ifdef DEBUG_STACKRECORD
			std::cout << "[StackRecord] First entry into libcuda for Stack Record " << _id << " is " << _points[i].funcName << std::endl;
#endif
			return _points[i];
		}
	}	
	StackPoint empty;
	empty.empty = true;
	return empty;
}

std::vector<StackPoint> StackRecord::GetStackpoints() {
	return _points;
}

// Walking out of instrimented frames causes the frame being walked out of to look like it is
// coming from libdyninstAPI_RT.so, we need to get that position
uint64_t StackRecord::GetFirstLibDynRTPosition() {
	for (int i = _points.size() - 1; i >= 0; i = i - 1){
		if(_points[i].libname.find("libdyninstAPI_RT.so") != std::string::npos){
			return i;
		}
	}	
	return 0;
}

void StackRecord::AddOccurance(uint64_t pos) {
	_occurances.push_back(pos);
}

void StackRecord::ReturnPreCudaCalls(std::vector<StackPoint> & ret) {
	for (int i = _points.size() - 1; i >= 0; i = i - 1) 
		if (_points[i].libname.find("libcuda.so") == std::string::npos && 
			_points[i].libname.find("libdyninstAPI_RT.so") == std::string::npos)
			ret.push_back(_points[i]);
		else
			break;
}

bool StackRecord::IsEqual(StackRecord & other) {
	std::vector<StackPoint> a,b;
	ReturnPreCudaCalls(a);
	other.ReturnPreCudaCalls(b);
	if (a.size() != b.size())
		return false;
	for (int i = 0; i < a.size(); i++) {
		if(!a[i].IsEqual(b[i]))
			return false;
	}
	return true;
}

bool StackRecord::ReplaceLibDynRT(StackPoint p) {
	uint64_t pos = GetFirstLibDynRTPosition();
	if (pos == 0)
		return false;
	std::vector<StackPoint> tmp;
	tmp.push_back(p);
	for (int i = pos + 1; i < _points.size(); i++) 
		tmp.push_back(_points[i]);
	_points = tmp;
	//_points.erase(_points.begin() + pos, _points.end());
	//_points.push_back(p);
	return true;
}


void StackRecord::ChangePointAtPosition(StackPoint p, uint64_t pos) {
	if (pos < _points.size()) {
		//std::cerr << "Inserting " << p.libname << " into position " << pos << std::endl;
		_points[pos] = p;
	}
}

void StackRecord::AddStackRecord(uint64_t pos) {
	if (_ranges.size() == 0){
		_ranges.push_back(SyncRangeRecord(pos));
		return;
	}
	if (_ranges.back().ExtendRange(pos) == false) {
		_ranges.push_back(SyncRangeRecord(pos));
	}
}

std::vector<std::string> StackRecord::GetLibNames() {
	std::vector<std::string> ret;
	for (auto i : _points) {
		ret.push_back(i.libname);
	}
	return ret;
}

void StackRecord::GetStackSymbols(SymbolMap & m) {
	for (int i = 0; i < _points.size(); i++) {
		if (m.find(_points[i].libname) == m.end())
			continue;
		std::pair<std::string, LineInfo> tmp;
		m[_points[i].libname]->GetInfoAtLocation(_points[i].libOffset, tmp);
		//std::cerr << _points[i].libname << " " << _points[i].libOffset << std::endl;
		_points[i].funcOffset = m[_points[i].libname]->GetFunctionOffset(_points[i].libOffset);
		_points[i].funcName = m[_points[i].libname]->GetFuncName(_points[i].libOffset);
		_points[i].fileName = tmp.second.filename;
		_points[i].lineNum = tmp.second.lineNum;
		_points[i].empty = false;
	}
#ifdef DEBUG_STACKRECORD
	PrintStack();
#endif 
}

void StackRecord::PrintStack() {
	std::cout << "[StackRecord] Decoded Stack for ID - " << _id << std::endl;
	for (auto i : _points)
		std::cout << "[StackRecord]\t" << i.funcName << "@" << i.libOffset << " in " << i.libname << std::endl;
}


void StackRecord::PrintStack(std::ofstream & outStream) {
	outStream << "[StackRecord] Decoded Stack for ID - " << _id << std::endl;
	for (auto i : _points)
		outStream << "[StackRecord]\t" << i.funcName << "@" << i.libOffset << " in " << i.libname << std::endl;
}
void StackRecord::PrintStack(std::stringstream & outStream) {
	outStream << "[StackRecord] Decoded Stack for ID - " << _id << std::endl;
	for (auto i : _points)
		outStream << "[StackRecord]\t" << i.funcName << "@" << i.libOffset << " in " << i.libname << std::endl;
}

void StackRecord::PrintEncodedStack(std::ofstream & outStream) {
	for (auto i : _points) {
		outStream << "$" << i.libname << "@" << i.funcName << "@" << std::hex << i.libOffset << "@" << i.fileName << "|" << i.lineNum;
	}
}

CudaCallMap::CudaCallMap() : _pos(1) {
	_nameToGeneralID[std::string("unknown")] = 0;
	_stackToGeneral[0] = 0;
}

void CudaCallMap::InsertStackID(std::string s, uint64_t id) {
	if (_nameToGeneralID.find(s) == _nameToGeneralID.end()){
		_nameToGeneralID[s] = _pos;
		_stackToGeneral[id] = _pos;
		_pos++;
	} else {
		_stackToGeneral[id] = _nameToGeneralID[s];
	}
}

uint64_t CudaCallMap::GeneralToStackID(uint64_t id) {
	for (auto i : _stackToGeneral) {
		if (i.second == id) {
			return i.first;
		}
	}
	return 0;
}

std::string CudaCallMap::GeneralToName(uint64_t id) {
	for (auto i : _nameToGeneralID)
		if (i.second == id)
			return i.first;
	return std::string("");
}

uint64_t CudaCallMap::StackIDToGeneral(uint64_t id) {
	if (_stackToGeneral.find(id) == _stackToGeneral.end())
		assert(1 == 0);
	return _stackToGeneral[id];
}

uint64_t CudaCallMap::NameToGeneral(std::string s) {
	if (_nameToGeneralID.find(s) == _nameToGeneralID.end())
		assert(1 == 0);
	return _nameToGeneralID[s];
}

