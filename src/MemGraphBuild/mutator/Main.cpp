#include <stdint.h>
#include <string>
#include <sstream> 
#include <iostream>  
#include <vector>
#include <unistd.h>
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
#include "DyninstBinaryEdit.h"
int main(int argc, char * argv[]) {
    std::string libcudaTest;
    uint64_t cudaOffset = DynHelper_GetSynchronizationOffset(libcudaTest); 
    std::cout << "We found cuda offset at location: " << std::hex << cudaOffset << std::endl;

    if (argc < 2) {
        std::cerr << "Expected arguments  <application> <application args>" << std::endl;
        return -1;
    }

    DiogenesCommon::DyninstProcess proc(argc - 1, &(argv[1]));
    proc.LaunchProcess();
    proc.LoadLibrary("libcuda.so");
    proc.LoadLibrary(DynHelper_GetInstallDirectory() +std::string("/lib/libMemGraphMutateeLib.so"));
    //DynEntryExit_InsertAtAddr(proc,std::string("libcuda.so"), cudaOffset,DynHelper_GetInstallDirectory() +std::string("/lib/libMemGraphMutateeLib.so"), std::string(""), std::string("DIOG_Synchronization_Post"));
    //proc.BreakAtMain();
    OneTime_InsertOneTimeCall(&proc,DynHelper_GetInstallDirectory() +std::string("/lib/libMemGraphMutateeLib.so") ,std::string("mutatee_init"));
    //proc.RunUntilCompleation(std::string(""));
    proc.DetachForDebug();
    
    
    char stackFileName[] = "DIOG_MemGraph_Stack.txt";
    char procMapName[] = "DIOG_MemGraph_Procmap.txt";

    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> stacks = ReadStacksFromMutatee(stackFileName);
    DiogenesCommon::ParseProcMap pmap(procMapName);
    DiogenesCommon::AddressSymbolizer symbols;

    std::cout << "[SyncDetector::Mutator::Main] Dumping DIOG_MemGraph_Stack.txt...." << std::endl;
    for (auto & i : stacks) {
        std::cout << "Stack Number: " << std::dec << i.first << std::endl;
        uint64_t depth = 1;
        for (auto & x : stacks[i.first]){
            if(pmap.GetLibraryAndOffset(x)) {
                symbols.GetSymbolAtAddress(x);
                std::cout << "  " << std::dec << depth << ": " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
                std::cout <<  x.symbolInfo.Print(5);
            } 
            depth++;
        }
    }
    /*
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
    }*/
}
    // Code for writing binaries, stackwalking just doesn't work right now though
    // {
    //     std::string loadingLibrary = std::string(buff);
    //     DiogenesCommon::DyninstBinaryEdit editor(libcudaTest, true, loadingLibrary + std::string("/libcuda.so"));
    //     loadingLibrary = DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so");
    //     editor.GetAddressSpace()->loadLibrary(loadingLibrary.c_str());
    //     DynEntryExit_InsertAtAddrBinary(editor,std::string("libcuda.so"), cudaOffset,DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so"), std::string(""), std::string("DIOG_Synchronization_Post"));
    // }
    // {
    //     std::string loadingLibrary = std::string(buff);
    //     DiogenesCommon::DyninstBinaryEdit editor(std::string(argv[1]), true, loadingLibrary + std::string("/inst_app"));
    //     loadingLibrary = DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so");
    //     std::vector<BPatch_function *> funcs;
    //     BPatch_function * found = NULL;
    //     editor.GetImage()->getProcedures(funcs, false);
    //     for (auto i : funcs) {
    //         if (i->getMangledName() == std::string("_start")){
    //             found  = i;
    //             break;
    //         }
    //     }
    //     editor.GetAddressSpace()->loadLibrary(loadingLibrary.c_str());
    //     DynEntryExit_InsertAtAddrBinary(editor,std::string(argv[1]), (uint64_t)found->getBaseAddr(),DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so"), std::string("mutatee_init"), std::string(""));
    // }
    // exit(0);