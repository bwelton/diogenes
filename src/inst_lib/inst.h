#include <stdio.h>
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

// Dyninst includes
#include "CodeObject.h"
#include "CFG.h"
#include "PatchObject.h"
#include "PatchMgr.h"
#include "Point.h"
#include "BPatch_snippet.h"
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_image.h"
#include "BPatch_function.h"
#include "BPatch_object.h"
#include "BPatch_Vector.h"
#include "BPatch_point.h"
#include "BPatch_addressSpace.h"
#include "BPatch_statement.h"
#include "dynC.h"
#include "set"

// Python Includes
#include <python2.7/Python.h>

using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;
struct InstStorage {
	BPatch bpatch;
	BPatch_binaryEdit * app;
	std::string binaryName;
	uint64_t unnamedCount;
	std::map<char *, std::pair<char *, char *>> replaceFuncs;
	std::map<char *, std::tuple<char *, char *, char *>> wrapFunctions;
	std::map<char *, std::tuple<char *, char *>> wrapAllFunctions;
	std::map<char *, std::tuple<char *, char *>> InsertAtEntry;
	std::map<uint64_t, std::string> SymbolsToWrite;
	std::map<std::string, uint64_t> SymbolToAddr;

};


extern "C" {
	InstStorage * OpenBinary(char * app_binary); 
	int ReplaceFunction(InstStorage * storage, char * binary_function, char * replacement_function, 
						char * replacement_library);
	int WrapFunction(InstStorage * storage, char * binary_function, char * wrapper_function, 
					 char * wrapper_library, char * wrapper_hookName);
	int PerformRewrite(InstStorage * storage, char * outputName);
	int WrapAllFunctions(InstStorage * storage, char * binary_function, char * wrapper_function, 
				 char * wrapper_library);

	PyObject * GetBinarySymbolsForModule(InstStorage * storage, char * module_name);
	PyObject * GetModuleNames(InstStorage * storage);
	PyObject * FindAllSymbolsWithPrefix(InstStorage * storage, char * prefix);
	int AddSymbolAtOffset(InstStorage * storage, uint64_t offset, char * preferredName);
	int InsertAtFunctionEntry(InstStorage * storage, char * binary_function, char * wrapper_function, 
				 char * wrapper_library);
	void InsertSymbols(InstStorage * storage, char * outputName);
}