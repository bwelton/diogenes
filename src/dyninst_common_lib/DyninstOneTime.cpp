#include "DyninstOneTime.h"

bool OneTime_InsertOneTimeCall(DiogenesCommon::DyninstProcess * proc, std::string libname, std::string funcName) {
    BPatch_object * libLoaded = proc->LoadLibrary(libname.c_str());

    std::vector<BPatch_function *> ret;
	libLoaded->findFunction(funcName, ret);    
    
    if (ret.size() == 0) {
        std::cout << "[OneTime_InsertOneTimeCall] Could not find function for - " << funcName << std::endl;
        return false;
    } else if (ret.size() > 1) {
        std::cout << "[OneTime_InsertOneTimeCall] Found more than one " << funcName << 
                     " function, picking the first one Dyninst found" << std::endl;        
    }

    std::vector<BPatch_snippet*> recordArgs;
	BPatch_funcCallExpr entryExpr(*(ret[0]), recordArgs);
    dynamic_cast<BPatch_process*>(proc->GetAddressSpace())->oneTimeCode(entryExpr);

}