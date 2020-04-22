#include <stdint.h>
#include <string>
#include <sstream> 
#include <iostream>  
#include <vector>
#include <map>
#include "DynHelper.h"
#include "DyninstProcess.h"
#include "DyninstOneTime.h"
#include "DyninstEntryExit.h"
#include "ReadStacks.h"
#include "BinaryAddress.h"
#include "ParseProcMap.h"

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
    
    char stackFileName[] = "DIOG_SYNC_StackCapture.txt";
    char procMapName[] = "ProcMap.txt";

    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> stacks = ReadStacksFromMutatee(stackFileName);
    DiogenesCommon::ParseProcMap pmap(procMapName);

    std::cout << "[SyncDetector::Mutator::Main] Dumping DIOG_SYNC_StackCapture.txt...." << std::endl;
    for (auto & i : stacks) {
        std::cout << "\tStack Number: " << i.first << std::endl;
        for (auto & x : i.second){
            if(pmap.GetLibraryAndOffset(x)) 
                std::cout << "\t\t" << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
            else
                std::cout << "\t\tUNKNOWN_LIB@" << std::hex << x.processAddress << std::endl;
        }
    }
}