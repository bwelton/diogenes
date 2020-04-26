#include <stdint.h>
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
#include "GenerateAutocorrect.h"
#include "SyncdetectFilenames.h"
#include "AutocorrectFilenames.h"
#include "MemGraphFilenames.h"
#include "AllocationTracker.h"

void GetSymbolsForStacks(DiogenesCommon::AddressSymbolizer & sym, DiogenesCommon::ParseProcMap & pmap,
                         std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> & s) {
    for (auto & i : s) {
        for (auto & x : s[i.first]){
            if(pmap.GetLibraryAndOffset(x)) {
                sym.GetSymbolAtAddress(x);
            } 
        }
    }
}

std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> NormalizeSyncStacks(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& syncStacks,
                                                                                  std::set<uint64_t> & subsetToPrune) {
    // This function gets rid of stack entries that are inserted by the mutatee
    // such as "syncdetect_cuMemcpyDtoH_v2" and chops the stack off at the first
    // call into libcuda.so.
    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> ret;

    std::string installDirectory = DynHelper_GetInstallDirectory();
    for (auto i : subsetToPrune) {
        std::vector<DiogenesCommon::BinaryAddress> cur;
        for (auto entry : syncStacks[i]) {
            bool skip = false;
            if (cur.size() > 0) {
                if (cur.back().binaryName.get() != NULL && entry.binaryName.get() != NULL){
                    std::string tmpStr = std::string(cur.back().binaryName.get());
                    std::string tmpStr2 = std::string(entry.binaryName.get());
                    if (tmpStr2.find("libcuda.so") != std::string::npos && 
                        tmpStr.find("libcuda.so") != std::string::npos) {
                        cur.clear();
                    }
                }
                if (entry.binaryName.get() != NULL) {
                    std::string tmpStr2 = std::string(entry.binaryName.get());
                    if (tmpStr2.find(installDirectory) != std::string::npos)
                        skip = true;
                }
            }
            if(!skip)
                cur.push_back(entry);
        }
        ret[i] = cur;
    }
    return ret;
}

std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> NormalizeMemGraph(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& memgraphStacks,
                                                                                  std::set<uint64_t> & subsetToPrune) {
    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> ret;                                                                              
    std::map<std::string, std::string> memGraphTranslate = {{"memgraph_cuMemAllocHost_v2","cuMemAllocHost_v2"},
                                                            {"memgraph_malloc","malloc"},
                                                            {"memgraph_cuMemAllocManaged","cuMemAllocManaged"},
                                                            {"memgraph_cuMemcpyHtoD_v2","cuMemcpyHtoD_v2"},
                                                            {"memgraph_cuMemcpyDtoH_v2","cuMemcpyDtoH_v2"},
                                                            {"memgraph_cuMemcpyDtoHAsync_v2","cuMemcpyDtoHAsync_v2"},
                                                            {"memgraph_cuMemcpyHtoDAsync_v2","cuMemcpyHtoDAsync_v2"},
                                                            {"memgraph_cuMemcpyAsync","cuMemcpyAsync"},
                                                            {"memgraph_cuMemFreeHost","cuMemFreeHost"},
                                                            {"memgraph_free","free"},
                                                            {"memgraph_cuMemAlloc","cuMemAlloc_v2"},
                                                            {"memgraph_cuMemFree","cuMemFree_v2"}};
    for(auto i : subsetToPrune) {
        std::vector<DiogenesCommon::BinaryAddress> cur;
        for (auto entry : memgraphStacks[i]) {
            bool found = false;
            std::string newFunc;
            for(auto fname : entry.symbolInfo.funcName) {
                if (memGraphTranslate.find(fname) != memGraphTranslate.end()){
                    cur.clear();
                    newFunc=memGraphTranslate[fname];
                    found = true;
                }
            }
            if (found) {
                entry.symbolInfo.funcName.clear();
                entry.symbolInfo.funcName.push_back(newFunc);
                entry.binaryName = std::shared_ptr<char>(strdup(newFunc.c_str()));
            }
            cur.push_back(entry);
        }
        ret[i] = cur;
    }
    return ret;
}


std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> GetUnnecessaryCudaFreeSyncs(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& syncStacks,
                                                                                         std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& syncCollisions, 
                                                                                         std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& memgraphStacks) {
    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> ret;
    std::set<uint64_t> requiredSyncs;
    for (auto i : syncCollisions) {
        assert(i.second.size() == 2);
        requiredSyncs.insert(i.second[0].processAddress);
    }
    DiogenesCommon::BinaryAddress empty;
    DiogenesCommon::BinaryAddressTree buildUniqueIdentiferTree(empty, false);
    
    std::set<uint64_t> syncs = DiogenesCommon::FindSynchronizations(syncStacks);
    std::map<uint64_t, std::shared_ptr<std::set<uint64_t>> > aliasMap;
    aliasMap.clear();
    for(auto i : syncs) {
        aliasMap[i] = std::shared_ptr<std::set<uint64_t>>(new std::set<uint64_t>());
        aliasMap[i]->insert(i);
    }

    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> normalizedSync = NormalizeSyncStacks( syncStacks,syncs);
    for (auto t : normalizedSync) {
        uint64_t i = t.first;
        uint64_t existing = buildUniqueIdentiferTree.InsertElement(t.second, t.first, 0);
        if (i != existing) {
            if (aliasMap[i] != aliasMap[existing]) {
                aliasMap[i]->insert(aliasMap[existing]->begin(), aliasMap[existing]->end());
                aliasMap[i]->insert(i);
                aliasMap[i]->insert(existing);
                aliasMap[existing] = aliasMap[i];
            }
        }
    }
    std::set<uint64_t> tmpRequired = requiredSyncs;
    for (auto i : tmpRequired) {
        requiredSyncs.insert(aliasMap[i]->begin(), aliasMap[i]->end());
    }
    std::set<uint64_t> libcFreeCalls = DiogenesCommon::FindFree(memgraphStacks);
    std::set<uint64_t> libcMemoryAllocs = DiogenesCommon::FindMalloc(memgraphStacks);
    libcMemoryAllocs.insert(libcFreeCalls.begin(), libcFreeCalls.end());
    std::set<uint64_t> tmpFree = DiogenesCommon::FindCudaFree(memgraphStacks);
    
    // libc free can take place in a transfer call, we want transfers only
    std::set<uint64_t> cudaFreeOps = tmpFree;
    for (auto i : libcMemoryAllocs) {
        if (cudaFreeOps.find(i) != cudaFreeOps.end())
            cudaFreeOps.erase(i);
    }
    std::set<uint64_t> cudaFreeToCorrect;
    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> normalizedMG =  NormalizeMemGraph( memgraphStacks,cudaFreeOps);    
     for(auto i : cudaFreeOps) {
        if (normalizedMG.find(i) == normalizedMG.end())
            continue;
        std::vector<DiogenesCommon::BinaryAddress> cur = normalizedMG[i];
        // std::cout << "NEW STACK START" << std::endl;
        // for (auto & x : cur){
        //     std::cout << "  TESTDUMP: " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
        //     std::cout <<  x.symbolInfo.Print(5);
        // }
        uint64_t syncStackID = 0;
        for(auto x : buildUniqueIdentiferTree._children){
            if (x->_binAddr.symbolInfo.funcName.size() > 0 && cur.size() > 0 && cur[0].symbolInfo.funcName.size() > 0) {
                if (x->_binAddr.symbolInfo.funcName[0] == cur[0].symbolInfo.funcName[0]){
                    //std::cout << "FOUND MATCH " << std::endl;
                    syncStackID = x->FindElement(cur, 1);
                    break;
                }
            }
        }
        std::cout << "Matched - MemGraph Stack " << std::dec << i  << " with SyncDetect Stack " << syncStackID << std::endl;
        if (requiredSyncs.find(syncStackID) == requiredSyncs.end() && syncStackID != 0){
            //std::cout << "UNNECESSARY SYNCHRONIZATION AT " << i << std::endl;
            cudaFreeToCorrect.insert(i);
        }
    }
    for (auto i : cudaFreeToCorrect) {
        ret[i]=normalizedMG[i];
    }
    return ret;
}

std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> GetUnnecessarySyncTrans(std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& syncStacks,
                                                                      std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& syncCollisions, 
                                                                      std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>>& memgraphStacks){
    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> ret;
    std::set<uint64_t> requiredSyncs;
    for (auto i : syncCollisions) {
        assert(i.second.size() == 2);
        requiredSyncs.insert(i.second[0].processAddress);
    }
    DiogenesCommon::BinaryAddress empty;
    DiogenesCommon::BinaryAddressTree buildUniqueIdentiferTree(empty, false);
    
    std::set<uint64_t> syncs = DiogenesCommon::FindSynchronizations(syncStacks);
    std::map<uint64_t, std::shared_ptr<std::set<uint64_t>> > aliasMap;

    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> normalizedSync = NormalizeSyncStacks( syncStacks,syncs);
    
    aliasMap.clear();
    for(auto i : syncs) {
        aliasMap[i] = std::shared_ptr<std::set<uint64_t>>(new std::set<uint64_t>());
        aliasMap[i]->insert(i);
    }   

    for (auto t : normalizedSync) {
        uint64_t i = t.first;
        uint64_t existing = buildUniqueIdentiferTree.InsertElement(t.second, t.first, 0);
        if (i != existing) {
            if (aliasMap[i] != aliasMap[existing]) {
                aliasMap[i]->insert(aliasMap[existing]->begin(), aliasMap[existing]->end());
                aliasMap[i]->insert(i);
                aliasMap[i]->insert(existing);
                aliasMap[existing] = aliasMap[i];
            }
        }
        //std::cout << "NEW STACK START With ID " << std::dec << existing << std::endl;
        // for (auto & x : t.second){
        //     std::cout << "  TESTDUMP: " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
        //     std::cout <<  x.symbolInfo.Print(5);
        // }     
    }

    std::set<uint64_t> tmpRequired = requiredSyncs;
    for (auto i : tmpRequired) {
        requiredSyncs.insert(aliasMap[i]->begin(), aliasMap[i]->end());
    }
    std::cout << "Printing first level of unique tree " << std::endl;
    for(auto x : buildUniqueIdentiferTree._children){
        std::cout << x->_binAddr.symbolInfo.Print(4) << std::endl;
    }
    std::set<uint64_t> libcFreeCalls = DiogenesCommon::FindFree(memgraphStacks);
    std::set<uint64_t> libcMemoryAllocs = DiogenesCommon::FindMalloc(memgraphStacks);
    libcMemoryAllocs.insert(libcFreeCalls.begin(), libcFreeCalls.end());
    std::set<uint64_t> tempTrans = DiogenesCommon::FindTransfers(memgraphStacks);
    
    // libc malloc can take place in a transfer call, we want transfers only
    std::vector<uint64_t> transfersVec;
    std::set<uint64_t> transfers = tempTrans;
    for (auto i : libcMemoryAllocs) {
        if (transfers.find(i) != transfers.end())
            transfers.erase(i);
    }
    
    transfers.insert(transfersVec.begin(), transfersVec.end());

    std::map<uint64_t,std::vector<DiogenesCommon::BinaryAddress>> normalizedMG =  NormalizeMemGraph( memgraphStacks,transfers);
    std::set<uint64_t> transToCorrect;
    for(auto i : transfers) {
        if (normalizedMG.find(i) == normalizedMG.end())
            continue;
        std::vector<DiogenesCommon::BinaryAddress> cur = normalizedMG[i];
        // std::cout << "NEW STACK START" << std::endl;
        // for (auto & x : cur){
        //     std::cout << "  TESTDUMP: " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
        //     std::cout <<  x.symbolInfo.Print(5);
        // }
        uint64_t syncStackID = 0;
        for(auto x : buildUniqueIdentiferTree._children){
            if (x->_binAddr.symbolInfo.funcName.size() > 0 && cur.size() > 0 && cur[0].symbolInfo.funcName.size() > 0) {
                if (x->_binAddr.symbolInfo.funcName[0] == cur[0].symbolInfo.funcName[0]){
                    //std::cout << "FOUND MATCH " << std::endl;
                    syncStackID = x->FindElement(cur, 1);
                    break;
                }
            }
        }
        std::cout << "Matched - MemGraph Stack " << std::dec << i  << " with SyncDetect Stack " << syncStackID << std::endl;
        if (requiredSyncs.find(syncStackID) == requiredSyncs.end() && syncStackID != 0){
            //std::cout << "UNNECESSARY SYNCHRONIZATION AT " << i << std::endl;
            transToCorrect.insert(i);
        }
    }
    for (auto i : transToCorrect) {
        ret[i]=normalizedMG[i];
    }

    return ret;
}

void GenerateAutocorrection() {
    char syncProcMap[] = "ProcMap.txt";
    char syncCaptureFile[] = SYNC_STACKCAPTURE_FILE;
    char collisionFileName[] = SYNC_STACKCOLLISION_FILE;
    char memoryGraphStack[] = MEMGRAPH_STACKFILE;
    char memoryGraphProcmap[] = MEMGRAPH_PROCMAP;
    char memoryGraphData[] = MEMGRAPH_GRAPHDATA;

    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> syncStacks;
    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> syncCollisions;
    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> memgraphStacks;


    syncStacks = ReadStacksFromMutatee(syncCaptureFile);
    syncCollisions = ReadStacksFromMutatee(collisionFileName);
    memgraphStacks = ReadStacksFromMutatee(memoryGraphStack);

    DiogenesCommon::ParseProcMap syncPMap(syncProcMap);
    DiogenesCommon::ParseProcMap memGraphPMap(memoryGraphProcmap);

    DiogenesCommon::AddressSymbolizer symbols;
    GetSymbolsForStacks(symbols, syncPMap, syncStacks);
    //GetSymbolsForStacks(symbols, syncPMap, syncCollisions);
    GetSymbolsForStacks(symbols, memGraphPMap, memgraphStacks);
    auto tmp2 = GetUnnecessarySyncTrans(syncStacks,syncCollisions,memgraphStacks);
    auto tmp3 = GetUnnecessaryCudaFreeSyncs(syncStacks,syncCollisions,memgraphStacks);


    FILE *f = fopen(memoryGraphData, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  
    char *dataRead = (char*) malloc(fsize);
    fread(dataRead, 1, fsize, f);
    fclose(f);

    std::ofstream outputToAC;
    outputToAC.open(AUTOCORRECT_STACKFILE);
    uint64_t globalOutCount = 1;

    MemGraphBuild::AllocationTracker track;
    track.DeSerializeData(dataRead, fsize);
    std::set<uint64_t> cudaHostAllocs = DiogenesCommon::FindCudaHostAllocs(memgraphStacks);
    std::stringstream syncTransUnnecessary;
    for (auto i : tmp2) {
        MemGraphBuild::TransferMap tmap = track.GetTransferMapForID(i.first);
        if (tmap.unknownAllocations == 0)
            syncTransUnnecessary << "[AUTOCORR] Unnecessary Synchronous Transfer At...." << std::endl;
        else
            syncTransUnnecessary << "[MANUAL] Unnecessary Synchronous Transfer At...." << std::endl;
        for (auto & x : i.second){
            syncTransUnnecessary << "    " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
            syncTransUnnecessary << x.symbolInfo.Print(4);
        }        
        syncTransUnnecessary << "  Non-CUDA Managed Memory Allocated At...." << std::endl;
        for (auto y : tmap.allocSites) {
            if (cudaHostAllocs.find(y) != cudaHostAllocs.end())
                continue;
            for (auto & x : memgraphStacks[y]) {
                syncTransUnnecessary << "      " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
                syncTransUnnecessary << x.symbolInfo.Print(6);
            }
        }
        syncTransUnnecessary << "  Non-CUDA Managed Memory Freed At...." << std::endl;
        for (auto y : tmap.freeSites) {
            if (cudaHostAllocs.find(y) != cudaHostAllocs.end())
                continue;
            for (auto & x : memgraphStacks[y]) {
                syncTransUnnecessary << "      " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
                syncTransUnnecessary << x.symbolInfo.Print(6);
            }
        }
        if (tmap.unknownAllocations == 0) {
            outputToAC << std::dec << globalOutCount << "$";
            globalOutCount++;
            bool first = true;
            for (auto & x : i.second){
                if (first)
                    outputToAC << x.binaryName << "@" << std::hex << x.libraryOffset;
                else
                    outputToAC << "," << x.binaryName << "@" << std::hex << x.libraryOffset;
                first = false;
            }
            outputToAC<< std::endl;                
        }
    }

    //    class FreeMap {
    //     public:
    //         uint64_t stackID;
    //         std::set<uint64_t> allocSites;
    //         uint64_t unknownAlloc;
    //         FreeMap(uint64_t s) : stackID(s) {
    //             unknownAlloc = 0;
    //         };
    // };
    for (auto i : tmp3) {
        MemGraphBuild::FreeMap mfree = track.GetFreeMapForID(i.first);
         if (mfree.unknownAlloc == 0)
            syncTransUnnecessary << "[AUTOCORR] Unnecessary cudaFree Synchronization at...." << std::endl;
        else
            syncTransUnnecessary << "[MANUAL] Unnecessary cudaFree Synchronization at...." << std::endl;
        for (auto & x : i.second){
            syncTransUnnecessary << "    " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
            syncTransUnnecessary << x.symbolInfo.Print(4);
        }
        syncTransUnnecessary << "  Memory Allocated At...." << std::endl;
        for (auto y : mfree.allocSites) {
            for (auto & x : memgraphStacks[y]) {
                syncTransUnnecessary << "      " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
                syncTransUnnecessary << x.symbolInfo.Print(6);
            }
        }
        if (mfree.unknownAlloc == 0) {
            outputToAC << std::dec << globalOutCount << "$";
            globalOutCount++;
            bool first = true;
            for (auto & x : i.second){
                if (first)
                    outputToAC << x.binaryName << "@" << std::hex << x.libraryOffset;
                else
                    outputToAC << "," << x.binaryName << "@" << std::hex << x.libraryOffset;
                first = false;
            }
            outputToAC<< std::endl;                
        }               
    }
    std::cout << "[BEGIN_FINAL_OUTPUT]" << std::endl;
    std::cout << syncTransUnnecessary.str()  << std::endl;
    outputToAC.close();
    // std::set<uint64_t> requiredSyncs;
    // for (auto i : syncCollisions) {
    //     assert(i.second.size() == 2);
    //     requiredSyncs.insert(i.second[0].processAddress);
    // }

    // std::set<uint64_t> syncs = DiogenesCommon::FindSynchronizations(syncStacks);
    // std::map<uint64_t, std::shared_ptr<std::set<uint64_t>> > aliasMap;
    // for(auto i : syncs) {
    //     aliasMap[i] = std::shared_ptr<std::set<uint64_t>>(new std::set<uint64_t>());
    //     aliasMap[i]->insert(i);
    // }

    // DiogenesCommon::BinaryAddress empty;
    // DiogenesCommon::BinaryAddressTree buildUniqueIdentiferTree(empty, false);
    // std::string installDirectory = DynHelper_GetInstallDirectory();
    // for (auto i : syncs) {
    //     std::vector<DiogenesCommon::BinaryAddress> cur;
    //     for (auto entry : syncStacks[i]) {
    //         bool skip = false;
    //         if (cur.size() > 0) {
    //             if (cur.back().binaryName.get() != NULL && entry.binaryName.get() != NULL){
    //                 std::string tmpStr = std::string(cur.back().binaryName.get());
    //                 std::string tmpStr2 = std::string(entry.binaryName.get());
    //                 if (tmpStr2.find("libcuda.so") != std::string::npos && 
    //                     tmpStr.find("libcuda.so") != std::string::npos) {
    //                     cur.clear();
    //                 }
    //             }
    //             if (entry.binaryName.get() != NULL) {
    //                 std::string tmpStr2 = std::string(entry.binaryName.get());
    //                 if (tmpStr2.find(installDirectory) != std::string::npos)
    //                     skip = true;
    //             }
    //         }
    //         if(!skip)
    //             cur.push_back(entry);
    //     }
    //     std::cout << "Inserting stack of size " << cur.size() << std::endl;
    //     uint64_t existing = buildUniqueIdentiferTree.InsertElement(cur, i, 0);
    //     if (i != existing) {
    //         if (aliasMap[i] != aliasMap[existing]) {
    //             aliasMap[i]->insert(aliasMap[existing]->begin(), aliasMap[existing]->end());
    //             aliasMap[i]->insert(i);
    //             aliasMap[i]->insert(existing);
    //             aliasMap[existing] = aliasMap[i];
    //         }
    //     }
    //     std::cout << "NEW STACK START With ID " << std::dec << existing << std::endl;
    //     for (auto & x : cur){
    //         std::cout << "  TESTDUMP: " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
    //         std::cout <<  x.symbolInfo.Print(5);
    //     }
    // }
    // std::set<uint64_t> tmpRequired = requiredSyncs;
    // for (auto i : tmpRequired) {
    //     requiredSyncs.insert(aliasMap[i]->begin(), aliasMap[i]->end());
    // }
    // std::cout << "Printing first level of unique tree " << std::endl;
    // for(auto x : buildUniqueIdentiferTree._children){
    //     std::cout << x->_binAddr.symbolInfo.Print(4) << std::endl;
    // }
    // std::set<uint64_t> libcMemoryAllocs = DiogenesCommon::FindMalloc(memgraphStacks);
    // std::set<uint64_t> tempTrans = DiogenesCommon::FindTransfers(memgraphStacks);
    
    // // libc malloc can take place in a transfer call, we want transfers only
    // std::vector<uint64_t> transfersVec;
    // //std::set_difference(tempTrans.begin(), tempTrans.end(), libcMemoryAllocs.begin(), libcMemoryAllocs.end(), transfersVec.begin());
    // std::set<uint64_t> transfers = tempTrans;
    // for (auto i : libcMemoryAllocs) {
    //     if (transfers.find(i) != transfers.end())
    //         transfers.erase(i);
    // }
    
    // transfers.insert(transfersVec.begin(), transfersVec.end());

    // std::map<std::string, std::string> memGraphTranslate = {{"memgraph_cuMemAllocHost_v2","cuMemAllocHost_v2"},
    //                                                         {"memgraph_malloc","malloc"},
    //                                                         {"memgraph_cuMemAllocManaged","cuMemAllocManaged"},
    //                                                         {"memgraph_cuMemcpyHtoD_v2","cuMemcpyHtoD_v2"},
    //                                                         {"memgraph_cuMemcpyDtoH_v2","cuMemcpyDtoH_v2"},
    //                                                         {"memgraph_cuMemcpyDtoHAsync_v2","cuMemcpyDtoHAsync_v2"},
    //                                                         {"memgraph_cuMemcpyHtoDAsync_v2","cuMemcpyHtoDAsync_v2"},
    //                                                         {"memgraph_cuMemcpyAsync","cuMemcpyAsync"},
    //                                                         {"memgraph_cuMemFreeHost","cuMemFreeHost"},
    //                                                         {"memgraph_free","free"},
    //                                                         {"memgraph_cuMemAlloc","cuMemAlloc_v2"},
    //                                                         {"memgraph_cuMemFree","cuMemFree_v2"}};
    // std::set<uint64_t> transToCorrect;
    // for(auto i : transfers) {
    //     std::vector<DiogenesCommon::BinaryAddress> cur;
    //     for (auto entry : memgraphStacks[i]) {
    //         bool found = false;
    //         std::string newFunc;
    //         for(auto fname : entry.symbolInfo.funcName) {
    //             if (memGraphTranslate.find(fname) != memGraphTranslate.end()){
    //                 cur.clear();
    //                 newFunc=memGraphTranslate[fname];
    //                 found = true;
    //             }
    //         }
    //         if (found) {
    //             entry.symbolInfo.funcName.clear();
    //             entry.symbolInfo.funcName.push_back(newFunc);
    //         }
    //         cur.push_back(entry);
    //     }
    //     std::cout << "NEW STACK START" << std::endl;
    //     for (auto & x : cur){
    //         std::cout << "  TESTDUMP: " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
    //         std::cout <<  x.symbolInfo.Print(5);
    //     }
    //     uint64_t syncStackID = 0;
    //     for(auto x : buildUniqueIdentiferTree._children){
    //         if (x->_binAddr.symbolInfo.funcName.size() > 0 && cur.size() > 0 && cur[0].symbolInfo.funcName.size() > 0) {
    //             if (x->_binAddr.symbolInfo.funcName[0] == cur[0].symbolInfo.funcName[0]){
    //                 std::cout << "FOUND MATCH " << std::endl;
    //                 syncStackID = x->FindElement(cur, 1);
    //                 break;
    //             }
    //         }
    //     }
    //     std::cout << "Matched - MemGraph Stack " << std::dec << i  << " with SyncDetect Stack " << syncStackID << std::endl;
    //     if (requiredSyncs.find(syncStackID) == requiredSyncs.end()){
    //         std::cout << "UNNECESSARY SYNCHRONIZATION AT " << i << std::endl;
    //         transToCorrect.insert(i);
    //     }
    // }



    // std::cout << "Required Sync Keys before merge: ";
    // for (auto i : tmpRequired) {
    //     std::cout << std::dec << i << ",";
    // }
    // std::cout << std::endl << "After Merge: ";
    // for (auto i : requiredSyncs) {
    //     std::cout << std::dec << i << ",";
    // }    


    /*
    std::set<uint64_t> libcMemoryAllocs = DiogenesCommon::FindMalloc(memgraphStacks);
    std::set<uint64_t> tempTrans = DiogenesCommon::FindTransfers(memgraphStacks);
    
    // libc malloc can take place in a transfer call, we want transfers only
    std::set<uint64_t> transfers;
    std::set_difference(tempTrans.begin(), tempTrans.end(), libcMemoryAllocs.begin(), libcMemoryAllocs.end(), transfers.begin());
    */
    // std::set<uint64_t> transfersPerformingSync;
    // for (auto i : )

}

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
    assert(proc.LoadLibrary(DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so")) != NULL);
    DynEntryExit_InsertAtAddr(proc,std::string("libcuda.so"), cudaOffset,DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so"), std::string(""), std::string("DIOG_Synchronization_Post"));
    OneTime_InsertOneTimeCall(&proc,DynHelper_GetInstallDirectory() +std::string("/lib/libSyncDetectorMutatee.so") ,std::string("mutatee_init"));
    proc.DetachForDebug();
    //proc.RunUntilCompleation(std::string(""));
    
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
		assert(x.binaryName != NULL);
                std::cout << "  " << std::dec << depth << ": " << x.binaryName << "@" << std::hex << x.libraryOffset << std::endl;
	        std::cout <<  x.symbolInfo.Print(5);
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
            
	    assert(x.binaryName != NULL);
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
    std::cout << std::flush;
    GenerateAutocorrection();
    return  -1;
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
