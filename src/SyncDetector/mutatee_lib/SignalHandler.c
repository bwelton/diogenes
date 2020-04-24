#include "SyncDetector.h"
#include "CPPLocker.h"
#include "CPPStackTree.h"
uint64_t syncdetect_write_id_coll = 99;

void syncdetect_catchSegFault(int sig, siginfo_t *siginfo, void *context) {
	
	CPPLocker local_locker = CPPPageLocker_GetThreadSpecific();
	if (local_locker == NULL)
		exit(-1);
	if (CPPPageLocker_UnlockMemory(local_locker) == 0 && syncdetect_exitinit == false){
		puts("SOMETHING SERIOUSLY WRONG, EXITING NOW!");
		exit(-1);
	}
	CPPPageLocker_ClearToLockPages(local_locker);
	uint64_t myid = GetStackID();
	if (myid == 0)
		return;

	syncdetect_WriteNecessarySync(currentStackID, myid);
	// uint64_t tmp[2];
	// uint64_t data[2];	
	// tmp[0] = currentStackID;
	// tmp[1] = myid;
	// data[0] = 0;
	// data[1] = syncdetect_write_id_coll;
	// syncdetect_write_id_coll++;
	// if (syncdetect_necessary_syncs != NULL)
	// 	StackTrie_InsertStack(syncdetect_necessary_syncs,tmp, (void**)data,2);
}

void syncdetect_WriteNecessarySync(uint64_t syncLocation, uint64_t useOfData) {
	syncdetect_disablememcapture = true;
	uint64_t tmp[2];
	uint64_t data;	
	tmp[0] = syncLocation;
	tmp[1] = useOfData;
	data = syncdetect_write_id_coll;
	syncdetect_write_id_coll++;
	syncdetect_disablememcapture = true;
	if (syncdetect_necessary_syncs != NULL)
		CPPStackTrie_InsertStack(syncdetect_necessary_syncs,tmp, &data,2);	
	syncdetect_disablememcapture = false;	
}