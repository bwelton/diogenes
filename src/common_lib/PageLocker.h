#pragma ONCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include "SkipList.h"
typedef struct PageMemArrays{
	void ** pages;
	size_t * sizes;
	int count;
} PageMemArrays;

typedef struct PageLocker{
	PageMemArrays * pagesToLock;
	PageMemArrays * pagesLocked;
} PageLocker;
PageLocker * PageLocker_GetThreadSpecific();
PageLocker * PageLocker_GetThreadSpecificSignalSafe();
void PageLocker_FreeMemoryAllocation(void * mem);
void PageLocker_AddMemoryAllocation(void * mem, size_t size);
void PageMemArrays_Initalize(PageMemArrays ** memArray);
void PageLocker_Initalize(PageLocker ** locker);
void PageLocker_AddTransferPage(PageLocker * locker, void * mem, size_t size);
void PageLocker_LockMemory(PageLocker * locker);
int PageLocker_UnlockMemory(PageLocker * locker);