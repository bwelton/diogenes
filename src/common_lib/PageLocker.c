#include "PageLocker.h"
#include <assert.h>
#define MAX_ENTRIES 4096
static pthread_key_t pagelocker_key;
static pthread_once_t pagelocker_key_once = PTHREAD_ONCE_INIT;

void PageMemArrays_Initalize(PageMemArrays ** memArray) {
	(*memArray) = (PageMemArrays*)valloc(sizeof(PageMemArrays));
	(*memArray)->pages = (void**)valloc(MAX_ENTRIES * sizeof(void*));
	(*memArray)->sizes = (size_t*)valloc(MAX_ENTRIES * sizeof(size_t));
	(*memArray)->count = 0;
}

void PageLocker_Initalize(PageLocker ** locker) {
	fprintf(stderr, "Initing page locker\n");
	(*locker) = (PageLocker*)valloc(sizeof(PageLocker));
	(*locker)->pagesToLock = NULL;
	(*locker)->pagesLocked = NULL;
	PageMemArrays_Initalize(&((*locker)->pagesToLock));
	PageMemArrays_Initalize(&((*locker)->pagesLocked));
}
static void PageLocker_make_key()
{
    (void) pthread_key_create(&pagelocker_key, NULL);
}
PageLocker * tmpGlobalPage = NULL;
PageLocker * PageLocker_GetThreadSpecific() {
	pthread_once(&pagelocker_key_once, PageLocker_make_key);
	void *ptr;
	if ((ptr = pthread_getspecific(pagelocker_key)) == NULL) {
		PageLocker_Initalize((PageLocker**)&ptr);
		pthread_setspecific(pagelocker_key, ptr);
	}
	return (PageLocker *)ptr;
}
PageLocker * PageLocker_GetThreadSpecificSignalSafe() {
	return pthread_getspecific(pagelocker_key);
}

void PageLocker_AddMemoryAllocation(void * mem, size_t size) {
	SkipListHandle_AddElement((uint64_t)mem, size);
}

void PageLocker_FreeMemoryAllocation(PageLocker * locker, void * mem) {
	SkipListHandle_RemoveElement((uint64_t)mem);
	PageMemArrays * pagesToLock = locker->pagesToLock;
	PageMemArrays * lockedPages = locker->pagesLocked;
	bool moveBack = false;
	for (int i = 0; i < lockedPages->count; i++) {
		if (lockedPages->pages[i] == mem){
			//mprotect(lockedPages->pages[i], lockedPages->sizes[i], PROT_READ | PROT_WRITE);
			moveBack = true;
		} else {
			if(moveBack) {
				lockedPages->pages[i-1] = lockedPages->pages[i];
				lockedPages->sizes[i-1] = lockedPages->sizes[i];
			}
		}	
	}
	if (moveBack)
		lockedPages->count--;

	moveBack = false;
	for (int i = 0; i < pagesToLock->count; i++) {
		if (pagesToLock->pages[i] == mem){
			moveBack = true;
		} else {
			if(moveBack) {
				pagesToLock->pages[i-1] = pagesToLock->pages[i];
				pagesToLock->sizes[i-1] = pagesToLock->sizes[i];
			}
		}	
	}
	if (moveBack)
		pagesToLock->count--;
}

void PageLocker_AddTransferPage(PageLocker * locker, void * mem, size_t size) {
	if (locker == NULL){
		fprintf(stderr, "%s\n", "LOCKER IS NULL!!!");
		exit(1);
	}
	PageMemArrays * memArray = locker->pagesToLock;
	if(memArray == NULL){
		fprintf(stderr, "%s\n", "MEMARRAY IS NULL!!!");
		exit(1);
	}
	if(memArray->count >= MAX_ENTRIES) {
		//fprintf(stderr, "%s\n", "[PageLocker - Warning] Cant add page to lock, at maximum limit!");
		return;
	}
	memArray->pages[memArray->count] = mem;
	memArray->sizes[memArray->count] = size;
	memArray->count++;
}

bool PageLocker_LockMemory(PageLocker * locker) {
	PageMemArrays * pagesToLock = locker->pagesToLock;
	PageMemArrays * lockedPages = locker->pagesLocked;

	int iter = pagesToLock->count;
	int lockPageCount = lockedPages->count;
	for (int i = 0; i < iter; i++) {
		if (lockPageCount >= MAX_ENTRIES){
			fprintf(stderr, "%s\n", "Reached max page lock count!");
			break;
		}
		size_t sizeToLock = 0;
		void * ptrToLock = (void *)SkipListHandle_FindNearestElement((uint64_t)pagesToLock->pages[i], &sizeToLock);

		if (ptrToLock != NULL && mprotect(ptrToLock, sizeToLock, PROT_NONE) == 0) {
			lockedPages->pages[lockPageCount] = ptrToLock;
			lockedPages->sizes[lockPageCount] = sizeToLock;	
			lockPageCount++;
			if (FileIO_CheckDebug())		
				fprintf(stderr, "[PageLocker_LockMemory] Succesfully locked page - %p %"PRIu64"\n", ptrToLock, sizeToLock);
		} else {
			if (FileIO_CheckDebug())
				fprintf(stderr, "[PageLocker_LockMemory] Could not memory at address %p with size  %"PRIu64", could be stack address (not supported)\n",  pagesToLock->pages[i],  pagesToLock->sizes[i]);
			
			return false;
			//assert(1==0);
		}
	}
	lockedPages->count = lockPageCount;
	pagesToLock->count = 0;
	return true;
}

RelockIndex PageLocker_TempUnlockAddress(PageLocker * locker, void * addr, uint64_t size) {
	RelockIndex ret = -1;
	PageMemArrays * lockedPages = locker->pagesLocked;
	for (int i = 0; i < lockedPages->count; i++) {
		uint64_t taddr = (uint64_t)lockedPages->pages[i];
		uint64_t tsize = lockedPages->sizes[i];
		if (taddr <= (uint64_t)addr &&  taddr + tsize >= (uint64_t)addr){
			if (FileIO_CheckDebug())
				fprintf(stderr, "[PageLocker_TempUnlockAddress] Temporarily Unlocked - %p %"PRIu64"\n", lockedPages->pages[i], lockedPages->sizes[i]);
			mprotect(lockedPages->pages[i], lockedPages->sizes[i], PROT_READ|PROT_WRITE);
			ret = i;
			break;
		}
	}
	return ret;
}

void PageLocker_RelockIndex(PageLocker * locker, RelockIndex index) {
	if(index < 0)
		return;
	PageMemArrays * lockedPages = locker->pagesLocked;
	if (index >= lockedPages->count)
		return;
	if (FileIO_CheckDebug())
		fprintf(stderr, "[PageLocker_RelockIndex] Relocking - %p %"PRIu64"\n", lockedPages->pages[index], lockedPages->sizes[index]);
	mprotect(lockedPages->pages[index], lockedPages->sizes[index], PROT_NONE);
}

int PageLocker_UnlockMemory(PageLocker * locker) {
	PageMemArrays * lockedPages = locker->pagesLocked;
	PageMemArrays * pagesToLock = locker->pagesToLock;
	int iter = lockedPages->count;
	int relcount = 0;
	for (int i = 0; i < iter; i++) {
		relcount++;
		if (FileIO_CheckDebug())
			fprintf(stderr, "[PageLocker_UnlockMemory] Unlocking - %p %"PRIu64"\n", lockedPages->pages[i], lockedPages->sizes[i]);
		if (mprotect(lockedPages->pages[i], lockedPages->sizes[i], PROT_READ | PROT_WRITE) != 0) {
			fprintf(stderr, "%s\n", "COULD NOT UNLOCK MEMORY! COULD RESULT IN BAD BEHAVIOR!!!");
		}
	}
	pagesToLock->count = 0;
	lockedPages->count = 0;
	return relcount;
}