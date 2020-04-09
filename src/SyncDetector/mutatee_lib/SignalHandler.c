#include "SyncDetector.h"

void syncdetect_catchSegFault(int sig, siginfo_t *siginfo, void *context) {
	PageLocker * local_locker = PageLocker_GetThreadSpecific();
	if (local_locker == NULL)
		exit(-1);
	if (PageLocker_UnlockMemory(local_locker) == 0){
		puts("SOMETHING SERIOUSLY WRONG, EXITING NOW!");
		exit(-1);
	}
}