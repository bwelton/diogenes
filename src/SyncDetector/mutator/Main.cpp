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
#include "AddressSymbolizer.h"
#include "SymbolInfo.h"

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
    char collisionFileName[] = "DIOG_SYNC_StackCollision.txt";
    char procMapName[] = "ProcMap.txt";

    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> stacks = ReadStacksFromMutatee(stackFileName);
    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> collisions = ReadStacksFromMutatee(collisionFileName);
    DiogenesCommon::ParseProcMap pmap(procMapName);
    DiogenesCommon::AddressSymbolizer symbols;

    std::cout << "[SyncDetector::Mutator::Main] Dumping DIOG_SYNC_StackCapture.txt...." << std::endl;
    for (auto & i : stacks) {
        std::cout << "Stack Number: " << std::dec << i.first << std::endl;
        uint64_t depth = 1;
        for (auto & x : stacks[i.first]){
            if(pmap.GetLibraryAndOffset(x)) {
                symbols.GetSymbolAtAddress(x);
                // for (auto n : x.symbolInfo.funcName)
                //     std::cerr << "MAIN - " << n << std::endl;
                //std::cout << "  " << std::dec << depth << ": " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
                //std::cout << x.symbolInfo.Print(5);
            } else {
                //std::cout << "  " << std::dec << depth << ": UNKNOWN_LIB@" << std::hex << x.processAddress << std::endl;
            }
            depth++;
        }
    }
    for (auto i : collisions) {
        assert(i.second.size() == 2);
        uint64_t syncRequired = i.second[0].processAddress;
        uint64_t useOfData = i.second[1].processAddress;
        std::cout << "REQUIRED SYNCHRONIZATION AT NUMBER: " << std::dec << syncRequired << std::endl;
        uint64_t depth = 1;
        for (auto & x : stacks[syncRequired]){
            std::cout << "  " << std::dec << depth << ": " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
            std::cout <<  x.symbolInfo.Print(5);
            depth++;
        }
        depth = 1;
        std::cout << "DATA USED AT " << std::dec << useOfData << std::endl;
        for (auto & x : stacks[useOfData]){
            std::cout << "  " << std::dec << depth << ": " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
            std::cout <<  x.symbolInfo.Print(5);
            depth++;
        }
    }
}