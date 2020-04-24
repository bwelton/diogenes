#include <map>
#include <unordered_map>
#include <inttypes.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include "CPPLocker.h"



typedef uint64_t AllocSize;
typedef uint64_t MemPtr;

namespace DiogenesPlugin{
    class CPPPageLocker {
        public:
            void AddMemoryAllocation(uint64_t addr, uint64_t size);
            void FreeMemoryAllocation(uint64_t addr);
            void AddTransferPage(uint64_t mem, size_t size);
            bool LockMemory();
            int UnlockMemory();
            void ClearToLockPages();
            RelockIndex TempUnlockAddress(uint64_t addr, uint64_t size);
            void RelockIndex_CPP(RelockIndex index);
        private:
            MemPtr _tmpUnlockedMem;
            AllocSize _tmpUnlockedSize;
            std::unordered_map<MemPtr,AllocSize> _pagesToLock;
            std::unordered_map<MemPtr,AllocSize> _pagesLocked;
            std::map<MemPtr, AllocSize> _allocatedData;
    };
}

void DiogenesPlugin::CPPPageLocker::ClearToLockPages() {
    _pagesToLock.clear();
}

void DiogenesPlugin::CPPPageLocker::AddMemoryAllocation(uint64_t addr, uint64_t size) {
    _allocatedData[addr] = size;
}

void DiogenesPlugin::CPPPageLocker::FreeMemoryAllocation(uint64_t addr) {
    auto it = _allocatedData.find(addr);
    if (it != _allocatedData.end())
        _allocatedData.erase(it);
}
void DiogenesPlugin::CPPPageLocker::AddTransferPage(uint64_t mem, size_t size) {
    if(_pagesToLock.find(mem) == _pagesToLock.end()) {
        _pagesToLock[mem] = size;
    }
}
bool DiogenesPlugin::CPPPageLocker::LockMemory() {
    for (auto i : _pagesToLock) {
        MemPtr memAddress = i.first;
        AllocSize memSize = i.second;
        auto it = _allocatedData.lower_bound(i.first);
        if (it == _allocatedData.end()){
            return false;
        }
        if (it != _allocatedData.begin() && it->first != memAddress)
            std::advance(it,-1);    
        if (it->first <= memAddress && it->first + it->second >= memAddress) {
            if (_pagesLocked.find(it->first) == _pagesLocked.end()){
                if (mprotect((void*)it->first, it->second, PROT_NONE) == 0) {
                    _pagesLocked[it->first] = it->second;
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    _pagesToLock.clear();
    return true;
}

int DiogenesPlugin::CPPPageLocker::UnlockMemory() {
    int count = 0;
    for (auto i : _pagesLocked) {
        if(mprotect((void*)i.first, i.second, PROT_READ | PROT_WRITE)!= 0){
            fprintf(stderr, "Could not unlock memory at address %llx of size %llx\n", i.first, i.second);
        } else{
            count++;
        }
    }
    _pagesLocked.clear();
    return count;
}

RelockIndex DiogenesPlugin::CPPPageLocker::TempUnlockAddress(uint64_t addr, uint64_t size) {
    _tmpUnlockedMem = 0;
    _tmpUnlockedSize = 0;
    auto it = _allocatedData.lower_bound(addr);
    if (it == _allocatedData.end()){
        return -1;
    }    
    if (it != _allocatedData.begin() && it->first != addr)
        std::advance(it,-1);     
    if (it->first <= addr && it->first + it->second >= addr) {  
        if(_pagesLocked.find(it->first) != _pagesLocked.end()) {
            mprotect((void*)it->first, it->second, PROT_READ | PROT_WRITE);
            _tmpUnlockedMem = it->first;
            _tmpUnlockedSize = it->second;
            return 1;
        }
    }
    return -1;
}
void DiogenesPlugin::CPPPageLocker::RelockIndex_CPP(RelockIndex index) {
    if (index != 1)
        assert(index == 1);
    if(_tmpUnlockedMem == 0)
        return;
    mprotect((void*)_tmpUnlockedMem, _tmpUnlockedSize, PROT_NONE);
    _tmpUnlockedSize = 0;
    _tmpUnlockedMem = 0;
}

thread_local DiogenesPlugin::CPPPageLocker * cppp_ptr = NULL;
thread_local int cpp_lock_engage = false;
extern "C" {

    int CPPPageLocker_IsMemLocked() {
        return cpp_lock_engage;
    }
 
    extern volatile bool syncdetect_disablememcapture;
    bool IN_MEMALLOC = false;
    CPPLocker CPPPageLocker_GetThreadSpecific() {
        if (cppp_ptr == NULL) {
            syncdetect_disablememcapture = true;
            cppp_ptr = new DiogenesPlugin::CPPPageLocker();
            syncdetect_disablememcapture = false;
        }
        return (CPPLocker) cppp_ptr;
    }
    CPPLocker CPPPageLocker_GetThreadSpecificSignalSafe() {
        return CPPPageLocker_GetThreadSpecific();   
    }

    void CPPPageLocker_FreeMemoryAllocation(CPPLocker locker, void * mem) {
        syncdetect_disablememcapture = true;
        if (locker != NULL)
            ((DiogenesPlugin::CPPPageLocker *)locker)->FreeMemoryAllocation((uint64_t)mem);
        syncdetect_disablememcapture = false;
    }

    void CPPPageLocker_AddMemoryAllocation(CPPLocker locker,void * mem, size_t size) {
        syncdetect_disablememcapture = true;
        if (locker != NULL)
            ((DiogenesPlugin::CPPPageLocker *)locker)->AddMemoryAllocation((uint64_t)mem, (uint64_t)size);
        syncdetect_disablememcapture = false;
    }
    void CPPPageLocker_AddTransferPage(CPPLocker locker, void * mem, size_t size){
        syncdetect_disablememcapture = true;
        if (locker != NULL)
             ((DiogenesPlugin::CPPPageLocker *)locker)->AddTransferPage((uint64_t)mem, (uint64_t)size);
        syncdetect_disablememcapture = false;    
    }
    bool CPPPageLocker_LockMemory(CPPLocker locker) {
        syncdetect_disablememcapture = true;
        bool ret = false;
        if (locker != NULL)
            ret = ((DiogenesPlugin::CPPPageLocker *)locker)->LockMemory();
        syncdetect_disablememcapture = false;        
        return ret;
    }
    int CPPPageLocker_UnlockMemory(CPPLocker locker) {
        syncdetect_disablememcapture = true;
        int ret = 0;
        if (locker != NULL)
            ret = ((DiogenesPlugin::CPPPageLocker *)locker)->UnlockMemory();
        syncdetect_disablememcapture = false;        
        return ret;    
    }
    RelockIndex CPPPageLocker_TempUnlockAddress(CPPLocker locker, void * addr, uint64_t size) {
        RelockIndex ret = -1;
        syncdetect_disablememcapture = true;
        if (locker != NULL)
            ret = ((DiogenesPlugin::CPPPageLocker *)locker)->TempUnlockAddress((uint64_t)addr, (uint64_t)size);
        syncdetect_disablememcapture = false; 
        return ret;
    }
    void CPPPageLocker_RelockIndex(CPPLocker locker, RelockIndex index) {
        syncdetect_disablememcapture = true;
        if (locker != NULL)
            ((DiogenesPlugin::CPPPageLocker *)locker)->RelockIndex_CPP(index);
        syncdetect_disablememcapture = false;     
    }
    void CPPPageLocker_ClearToLockPages(CPPLocker locker) {
        syncdetect_disablememcapture = true;
        if (locker != NULL)
            ((DiogenesPlugin::CPPPageLocker *)locker)->ClearToLockPages();
        syncdetect_disablememcapture = false;           
    }
}