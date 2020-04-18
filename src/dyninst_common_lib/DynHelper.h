#ifndef COMMON_DYNINST_HELPER
#define	COMMON_DYNINST_HELPER 1
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <iostream>
#include <memory>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <functional>
// #include "Common.h"

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

#include "CommonDefines.h"
//#include "Constants.h"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

std::vector<std::pair<BPatch_function *, uint64_t>> DynHelper_GetFunctions(BPatch_object * obj);
std::vector<BPatch_object *> DynHelper_GetObjects(BPatch_addressSpace * aspace);
BPatch_function * DynHelper_GetFuncAtAddress(BPatch_addressSpace * aspace, std::string libname, uint64_t offset);
BPatch_function * DynHelper_GetFuncByName(BPatch_addressSpace * aspace, std::string libname, std::string funcName);
std::string DynHelper_GetInstallDirectory();
bool DynHelper_CheckIfFileExists(const std::string& name);
uint64_t DynHelper_GetSynchronizationOffset();
#endif