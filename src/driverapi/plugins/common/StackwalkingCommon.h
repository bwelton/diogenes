#pragma once
#include <map>
#include <vector>
#include <iostream> 
#include <fstream>    
#include <string>		
#include <cassert> 
#include <unistd.h>
#include <sys/syscall.h>
#include <sstream>
#include <cstring>
#include <memory>
#include <execinfo.h>
#include "StackPoint.h"
#include "framestepper.h"
#include "walker.h"
#include "procstate.h"
#include "swk_errors.h"
#include "steppergroup.h"
#include "frame.h"
#include "sw_c.h"
#include "Symtab.h"
#include "BPatch.h"
#include "BPatch_process.h"

extern "C" {
	void INIT_FP_STACKWALKER();
	bool GET_FP_STACKWALK(std::vector<StackPoint> & ret);
}
