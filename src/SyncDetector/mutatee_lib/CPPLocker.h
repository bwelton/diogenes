#ifndef CPPLOCKED_DEFINE
#define CPPLOCKED_DEFINE 1
#include <assert.h>
#include "PageLocker.h"
typedef void * CPPLocker;
#ifdef __cplusplus
extern "C" {
#endif
void CPPPageLocker_ClearToLockPages(CPPLocker locker);
CPPLocker CPPPageLocker_GetThreadSpecific();
CPPLocker CPPPageLocker_GetThreadSpecificSignalSafe();
void CPPPageLocker_FreeMemoryAllocation(CPPLocker locker, void * mem);
void CPPPageLocker_AddMemoryAllocation(CPPLocker locker,void * mem, size_t size);
void CPPPageLocker_AddTransferPage(CPPLocker locker, void * mem, size_t size);
bool CPPPageLocker_LockMemory(CPPLocker locker);
int CPPPageLocker_UnlockMemory(CPPLocker locker);
RelockIndex CPPPageLocker_TempUnlockAddress(CPPLocker locker, void * addr, uint64_t size);
void CPPPageLocker_RelockIndex(CPPLocker locker, RelockIndex index);
#ifdef __cplusplus
}
#endif
#endif