#include <map>
#include <vector>
#include <iostream> 
#include <fstream>    
#include <string>		
#include <cassert> 
#include <unistd.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sstream>
#include <cstring>
#include <memory>
#include <execinfo.h>
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

using namespace Dyninst;
using namespace Dyninst::Stackwalker;
using namespace SymtabAPI;

thread_local Walker *  local_walker = NULL;
thread_local std::vector<Frame> stackwalk_vector;
extern "C" {
	void FPStackWalker_Init() {
		if (local_walker != NULL)
			return;
		local_walker = Walker::newWalker();
	}

    size_t FPStackWalker_GetStackFP(uint64_t * items, size_t maxSize) {
        FPStackWalker_Init();
        Frame curFrame;
        Frame next;
        size_t ret = 0;
        if(local_walker->getInitialFrame(curFrame) == false)
            return 0;
        items[ret] = (uint64_t)curFrame.getRA();
        ret++;
        while (local_walker->walkSingleFrame(curFrame,next)) {
            items[ret] = (uint64_t)next.getRA();
            fprintf(stderr,"Frame RA: %lx, FP: %lx, SP: %lx\n",(uint64_t)next.getRA(),(uint64_t)next.getFP(), (uint64_t)next.getSP());
            ret++;
            curFrame = next;
        }
		//stackwalk_vector.clear();

        // getInitialFrame

		// if(local_walker->walkStack(stackwalk_vector) == false) {
		// 	return 0;
		// }

        // for (auto i : stackwalk_vector) {
        //     items[ret] = (uint64_t)i.getRA();
        //     ret++;
        // }
        return ret;
    }

}