#ifndef DYNINST_COMMON_BINARYEDIT
#define DYNINST_COMMON_BINARYEDIT 1
#include <string>
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
#include "BPatch_basicBlock.h"
#include "dynC.h"
#include "set"

#include "DynHelper.h"
//#include "Constants.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;
namespace DiogenesCommon{
class DyninstBinaryEdit {
public:
	DyninstBinaryEdit(std::string binName, bool output = false, std::string outName = std::string(""));
	BPatch_image * GetImage();
	BPatch_addressSpace * GetAddressSpace();
	~DyninstBinaryEdit();
	void BeginInsertionSet();
private:
	BPatch_addressSpace * _as;
	BPatch_binaryEdit * _be;
	std::string _binName;
	bool _output;
	std::string _outName;
	bool _openInsertions;

};
};
#endif