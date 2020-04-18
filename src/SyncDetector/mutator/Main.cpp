#include "DynHelper.h"
#include "DyninstProcess.h"
#include "DyninstOneTime.h"
#include "DyninstEntryExit.h"
#include <stdint.h>

int main(int argc, char * argv[]) {
    uint64_t cudaOffset = DynHelper_GetSynchronizationOffset();
    std::cout << "We found cuda offset at location: " << std::hex << cudaOffset << std::endl;

    if (argc < 2) {
        std::cerr << "Expected arguments  <application> <application args>" << std::endl;
        return -1;
    }
    DiogenesCommon::DyninstProcess proc(argc - 1, &(argv[1]));
    proc.LaunchProcess();
    proc.LoadLibrary("libcuda.so");
    proc.LoadLibrary(DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so"));
    DynEntryExit_InsertAtAddr(proc,std::string("libcuda.so"), cudaOffset,DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so"), std::string(""), std::string("DIOG_Synchronization_Post"));
    OneTime_InsertOneTimeCall(&proc,DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so") ,std::string("mutatee_init"));
    proc.RunUntilCompleation(std::string(""));
}