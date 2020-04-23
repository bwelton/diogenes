#ifndef DYNINST_ADDRESSSYMBOLINFO
#define DYNINST_ADDRESSSYMBOLINFO 1
struct BinaryAddress;
#include <string>
#include <vector>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <map>
#include <set>
#include "BinaryAddress.h"
#include "SymbolInfo.h"
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
#include "BPatch_flowGraph.h"
#include "Function.h"
#include "FileIO.h"
#include "DyninstBinaryEdit.h"

#include "dynC.h"
#include "set"

#include "DynHelper.h"
//#include "Constants.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

namespace DiogenesCommon {
class SymbolStorage {
    public:
        SymbolStorage(std::string libname);
        std::vector<std::string> GetFunctionAtOffset(uint64_t offset);
        std::vector<DiogenesCommon::LineMap> GetLineInfoAtOffset(uint64_t offset);
    private:
        Symtab * _sobj;
        std::shared_ptr<DiogenesCommon::DyninstBinaryEdit> _bedit;
        std::map<uint64_t, Symbol *> _funcSymbols;
        std::map<uint64_t, Dyninst::SymtabAPI::Statement::Ptr> _lineStatements;

        std::map<uint64_t, std::set<BPatch_function *>> _blockMap;
        std::map<uint64_t, std::vector<BPatch_statement>> _statementMap;
};

class AddressSymbolizer {
public:
    void GetSymbolAtAddress(BinaryAddress & addr);

private:
    std::map<std::string,std::shared_ptr<DiogenesCommon::SymbolStorage>> _symbolMap;
};

};
#endif