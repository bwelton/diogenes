#include <stdio.h>
#include "BPatch.h"
#include "BPatch_addressSpace.h"
#include "BPatch_process.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_point.h"
#include "BPatch_function.h"
#include <stdlib.h>
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
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <unistd.h>

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
#include "BPatch_Vector.h"
#include "BPatch_point.h"
#include "BPatch_addressSpace.h"
#include "BPatch_statement.h"
#include "dynC.h"
#include "set"

//using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;

// Pulls all function symbols from binary, places them into textfile. 

int main(const int argc, const char * argv[]){
	BPatch patch;
	BPatch_binaryEdit *app = patch.openBinary(argv[1], false);
	std::ofstream pfile;
	pfile.open(argv[2]);	
	BPatch_image * img = app->getImage();
	std::vector<BPatch_function *> funcs;
	img->getProcedures(funcs, false);
	for (auto i : funcs) {
		if (i->getName().find("targ") == std::string::npos)
			pfile << i->getName() << std::endl;
		// if (i->getName().at(0) == 'c' && i->getName().at(1) == 'u') {
		// 	if (i->getName().find(std::string("__")) == std::string::npos) {
		// 		pfile << i->getName() << std::endl;
		// 		// pfile << "" << std::hex << i->getBaseAddr() << std::dec << " " << i->getName() 
		// 		//       << " 0x" << std::hex << i->getFootprint() << std::dec << std::endl;		
		// 	}
		// }
	}
	pfile.close();
}