#include <stdint.h>
#include <unordered_map>
#include <string>
#include <sstream> 
#include <iostream>  
#include <vector>
#include <unistd.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>  
#include "AutocorrectFilenames.h"
#include "DynHelper.h"
#include "DyninstProcess.h"
#include "DyninstOneTime.h"
#include "DyninstEntryExit.h"
#include "ReadStacks.h"

int main(int argc, char * argv[]) {
   char unnecessarySyncs[] = AUTOCORRECT_STACKFILE;
   char binarySyncFile[] = AUTOCORRECT_BINARYFILE;
   if (argc < 2) {
        std::cerr << "Expected arguments  <application> <application args>" << std::endl;
        return -1;
    }
    std::ofstream outFile;
    outFile.open(binarySyncFile);
    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> acStack = ReadStacksFromMutatee(unnecessarySyncs);
    std::unordered_map<char *, std::string> stringConversion;
    for (auto i : acStack){
        // First item will be the cudaCall to intercept (cuMemcpy...) do not translate it
        bool first = true;
        for (auto z : i.second){
            if (z.binaryName.get() == NULL || first == true) {
                first = false;
                continue;
            }
            if (stringConversion.find(z.binaryName.get()) == stringConversion.end())
                stringConversion[z.binaryName.get()] = std::string(z.binaryName.get());
        }
    }
    DiogenesCommon::DyninstProcess proc(argc - 1, &(argv[1]));
    proc.LaunchProcess();
    std::unordered_map<std::string, BPatch_object *> offsetMap;
    std::vector<BPatch_object *> objs = DynHelper_GetObjects(proc.GetAddressSpace());
    for (auto i : objs) {
        offsetMap[i->pathName()] = i;
    }
    uint64_t count = 1;
    for (auto i : acStack){
        outFile << std::dec << count << "$";
        count++;
        bool first = true;
        for (auto z : i.second){
            if (first) {
                outFile << z.binaryName.get() << "@0,";
                first = false;
            } else {
                auto it = offsetMap.find(stringConversion[z.binaryName.get()]);
                if (it == offsetMap.end()) {
                    std::cout << "Missing library " << z.binaryName.get() << " loading into address space" << std::endl;
                    BPatch_object * tobj = proc.LoadLibrary(z.binaryName.get());
                    offsetMap[tobj->pathName()] = tobj;
                    it = offsetMap.find(stringConversion[z.binaryName.get()]);
                }
                outFile << "," << z.binaryName.get() << "@" << std::hex << it->second->fileOffsetToAddr(z.libraryOffset);
            }
        }
        outFile << std::endl;
    }
    outFile.close();
    proc.LoadLibrary("libcuda.so");
    proc.LoadLibrary(DynHelper_GetInstallDirectory() +std::string("/lib/libAutocorrectLibrary.so"));
    //DynEntryExit_InsertAtAddr(proc,std::string("libcuda.so"), cudaOffset,DynHelper_GetInstallDirectory() +std::string("/lib/libMemGraphMutateeLib.so"), std::string(""), std::string("DIOG_Synchronization_Post"));
    OneTime_InsertOneTimeCall(&proc,DynHelper_GetInstallDirectory() +std::string("/lib/libAutocorrectLibrary.so") ,std::string("ac_mutatee_init"));
    proc.RunUntilCompleation(std::string(""));
    //proc.DetachForDebug();
}