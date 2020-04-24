#include "DyninstBinaryEdit.h"
extern 	BPatch bpatch; // Defined in DyninstProcess
DiogenesCommon::DyninstBinaryEdit::DyninstBinaryEdit(std::string binName, bool output, std::string outName)  :
	_binName(binName), _output(output), _outName(outName) {
	_as = bpatch.openBinary(_binName.c_str(), false);
	assert(_as != NULL);
	_be = dynamic_cast<BPatch_binaryEdit*>(_as);
}

DiogenesCommon::DyninstBinaryEdit::~DyninstBinaryEdit() {
	if (_openInsertions)
		GetAddressSpace()->finalizeInsertionSet(true);
	if(_output)
		if(!_be->writeFile(_outName.c_str()))
			std::cerr << "Could not generate output binary - " << _outName << std::endl;
}

BPatch_image * DiogenesCommon::DyninstBinaryEdit::GetImage() {
	return _be->getImage();
}

BPatch_addressSpace * DiogenesCommon::DyninstBinaryEdit::GetAddressSpace() {
	return _as;
}

void DiogenesCommon::DyninstBinaryEdit::BeginInsertionSet() {
	if (_openInsertions)
		return;
	GetAddressSpace()->beginInsertionSet();
	_openInsertions = true;
}