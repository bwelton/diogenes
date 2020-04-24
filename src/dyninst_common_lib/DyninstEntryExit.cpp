#include "DyninstEntryExit.h"

bool DynEntryExit_InsertAtAddr(DiogenesCommon::DyninstProcess & proc, std::string insertLib, uint64_t addr,  std::string wrapperLib, std::string entryFunc, std::string exitFunc) {
    BPatch_function * entry = NULL;
    BPatch_function * exit = NULL;

    BPatch_function * toWrap =  DynHelper_GetFuncAtAddress(proc.GetAddressSpace(), insertLib, addr);
    if (toWrap == NULL){
        std::cerr << "[DynEntryExit_InsertAtAddr] could not find function at address " << std::hex << addr << " in libname " << insertLib << std::endl;
        return false;
    }

    if (entryFunc.size() != 0) {
        entry = DynHelper_GetFuncByName(proc.GetAddressSpace(), wrapperLib, entryFunc);
        if(entry == NULL) {
            std::cerr << "[DynEntryExit_InsertAtAddr] could not find function with name " << entryFunc << " in libname " << wrapperLib << std::endl;
            return false;
        }
    }

    if (exitFunc.size() != 0) {
        exit =  DynHelper_GetFuncByName(proc.GetAddressSpace(), wrapperLib, exitFunc);
        if(exit == NULL) {
            std::cerr << "[DynEntryExit_InsertAtAddr] could not find function with name " << exitFunc << " in libname " << wrapperLib << std::endl;
            return false;
        }
    }
    proc.BeginInsertionSet();
    std::vector<BPatch_snippet*> recordArgs;
    if (entry != NULL) {
        BPatch_funcCallExpr entryExpr(*entry, recordArgs);
        std::vector<BPatch_point*> * entryPoints = toWrap->findPoint(BPatch_locEntry);
        proc.GetAddressSpace()->insertSnippet(entryExpr,*entryPoints);
    }

    if (exit != NULL) {
        BPatch_funcCallExpr exitExpr(*exit, recordArgs);
        std::vector<BPatch_point*> * exitPoints = toWrap->findPoint(BPatch_locExit);
        proc.GetAddressSpace()->insertSnippet(exitExpr,*exitPoints);
    }
    return true;
}

bool DynEntryExit_InsertAtAddrBinary(DiogenesCommon::DyninstBinaryEdit & proc, std::string insertLib, uint64_t addr,  std::string wrapperLib, std::string entryFunc, std::string exitFunc){
    BPatch_function * entry = NULL;
    BPatch_function * exit = NULL;

    BPatch_function * toWrap =  DynHelper_GetFuncAtAddress(proc.GetAddressSpace(), insertLib, addr);
    if (toWrap == NULL){
        std::cerr << "[DynEntryExit_InsertAtAddr] could not find function at address " << std::hex << addr << " in libname " << insertLib << std::endl;
        return false;
    }

    if (entryFunc.size() != 0) {
        entry = DynHelper_GetFuncByName(proc.GetAddressSpace(), wrapperLib, entryFunc);
        if(entry == NULL) {
            std::cerr << "[DynEntryExit_InsertAtAddr] could not find function with name " << entryFunc << " in libname " << wrapperLib << std::endl;
            return false;
        }
    }

    if (exitFunc.size() != 0) {
        exit =  DynHelper_GetFuncByName(proc.GetAddressSpace(), wrapperLib, exitFunc);
        if(exit == NULL) {
            std::cerr << "[DynEntryExit_InsertAtAddr] could not find function with name " << exitFunc << " in libname " << wrapperLib << std::endl;
            return false;
        }
    }
    proc.BeginInsertionSet();
    std::vector<BPatch_snippet*> recordArgs;
    if (entry != NULL) {
        BPatch_funcCallExpr entryExpr(*entry, recordArgs);
        std::vector<BPatch_point*> * entryPoints = toWrap->findPoint(BPatch_locEntry);
        proc.GetAddressSpace()->insertSnippet(entryExpr,*entryPoints);
    }

    if (exit != NULL) {
        BPatch_funcCallExpr exitExpr(*exit, recordArgs);
        std::vector<BPatch_point*> * exitPoints = toWrap->findPoint(BPatch_locExit);
        proc.GetAddressSpace()->insertSnippet(exitExpr,*exitPoints);
    }
    return true;

}