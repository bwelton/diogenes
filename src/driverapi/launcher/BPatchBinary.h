#pragma once
#include "DyninstIncludes.h"
class BPatchBinary {
public:
	BPatchBinary(std::string binName, bool output = false, std::string outName = std::string(""));
	BPatch_Image * GetImage();
	BPatch_addressSpace * GetAddressSpace();
	~BPatchBinary();

private:
	BPatch_addressSpace * _as;
	BPatch_binaryEdit * _be;
	std::string _binName;
	bool _output;
	std::string _outName;
	BPatch bpatch;
};

