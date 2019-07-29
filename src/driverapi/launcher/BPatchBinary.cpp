#include "BPatchBinary.h"

BPatchBinary::BPatchBinary(std::string binName, bool output, std::string outName)  :
	_binName(binName), _output(output), _outName(outName) {
	_as = bpatch.openBinary(_binName.c_str(), false);
	assert(_as != NULL);
	_be = dynamic_cast<BPatch_binaryEdit*>(_as);
}

BPatchBinary::~BPatchBinary() {
	if(_output)
		if(!_be->writeFile(_outName.c_str()))
			std::cerr << "Could not generate output binary - " << _outName << std::endl;
}

BPatch_Image * BPatchBinary::GetImage() {
	return _be->getImage();
}

BPatch_addressSpace * BPatchBinary::GetAddressSpace() {
	return _as;
}