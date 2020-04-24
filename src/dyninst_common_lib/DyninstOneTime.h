#ifndef DYNINST_ONETIME
#define DYNINST_ONETIME 1
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <iostream>
#include <memory>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>

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
#include "DyninstProcess.h"
#include "DyninstBinaryEdit.h"
bool OneTime_InsertOneTimeCall(DiogenesCommon::DyninstProcess * proc, std::string libname, std::string funcName);
bool OneTime_InsertOneTimeCallBinary(DiogenesCommon::DyninstBinaryEdit & proc, std::string libname, std::string funcName);
#endif