#include "DyninstEntryExit.h"

bool DynEntryExit_InsertAtAddr(DiogenesCommon::DyninstProcess & proc, std::string insertLib, uint64_t addr,  std::string wrapperLib, std::string entryFunc, std::string exitFunc) {
    BPatch_function * toWrap =  DynHelper_GetFuncAtAddress(proc.GetAddressSpace(), insertLib, addr);
    if (toWrap == NULL){
        std::cerr << "[DynEntryExit_InsertAtAddr] could not find function at address " << std::hex << addr << " in libname " << insertLib << std::endl;
        return false;
    }

    BPatch_function * entry = DynHelper_GetFuncByName(proc.GetAddressSpace(), wrapperLib, entryFunc);
    if(entry == NULL) {
        std::cerr << "[DynEntryExit_InsertAtAddr] could not find function with name " << entryFunc << " in libname " << wrapperLib << std::endl;
        return false;
    }

    BPatch_function * exit =  DynHelper_GetFuncByName(proc.GetAddressSpace(), wrapperLib, exitFunc);
    if(exit == NULL) {
        std::cerr << "[DynEntryExit_InsertAtAddr] could not find function with name " << exitFunc << " in libname " << wrapperLib << std::endl;
        return false;
    }
    
    std::vector<BPatch_snippet*> recordArgs;
    BPatch_funcCallExpr entryExpr(*entry, recordArgs);
    BPatch_funcCallExpr exitExpr(*exit, recordArgs);

    std::vector<BPatch_point*> * entryPoints = toWrap->findPoint(BPatch_locEntry);
    std::vector<BPatch_point*> * exitPoints = toWrap->findPoint(BPatch_locExit);
    proc.BeginInsertionSet();
    proc.GetAddressSpace()->insertSnippet(entryExpr,*entryPoints);
    proc.GetAddressSpace()->insertSnippet(exitExpr,*exitPoints);
    return true;
}