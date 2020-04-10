#include "SyncDetector.h"
uint64_t syncdetect_write_id_coll = 99;

void syncdetect_catchSegFault(int sig, siginfo_t *siginfo, void *context) {
	PageLocker * local_locker = PageLocker_GetThreadSpecific();
	if (local_locker == NULL)
		exit(-1);
	if (PageLocker_UnlockMemory(local_locker) == 0 && syncdetect_exitinit == false){
		puts("SOMETHING SERIOUSLY WRONG, EXITING NOW!");
		exit(-1);
	}
	uint64_t myid = GetStackID();
	if (myid == 0)
		return;
	
	uint64_t tmp[2];
	uint64_t data[2];	
	tmp[0] = currentStackID;
	tmp[1] = myid;
	data[0] = 0;
	data[1] = syncdetect_write_id_coll;
	syncdetect_write_id_coll++;
	if (syncdetect_necessary_syncs != NULL)
		StackTrie_InsertStack(syncdetect_necessary_syncs,tmp, (void**)data,2);
}