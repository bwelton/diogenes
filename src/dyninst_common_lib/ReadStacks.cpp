#include "ReadStacks.h"
std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> ReadStacksFromMutatee(char * infile) {
    std::map<std::string, std::shared_ptr<char>> nameMap;
    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> ret;
    uint64_t fileSize = 0;
    char * fileData = FileIO_ReadFile(malloc, infile, &fileSize);
    if (fileData == NULL){
        std::cerr << "ERROR COULD NOT OPEN STACKFILE - " << infile << std::endl;
        exit(-1);
    }
    std::cerr << "Building hashmap from file with filesize " << fileSize << std::endl;
    HashMap * map = ReadStackKeys(malloc,free, fileData, fileSize);

    fileSize = 0;
    uint64_t * keys = ReadStackKeys_GetIds(map, &fileSize);
    if (keys == NULL){
        std::cerr << "[ReadStacksFromMutatee] Could not read keys from file - " << infile << std::endl;
        return ret;
    }
    for(uint64_t i = 0; i < fileSize; i++) {
        uint64_t stackCount = 0;
        std::vector<DiogenesCommon::BinaryAddress> addrs;
        StackKeyEntry * entries = ReadStackKeys_GetElementAt(map, keys[i], &stackCount);
        if (entries == NULL) {
            std::cerr << "[ReadStacksFromMutatee] Could not read stack keys for entry - " << i << std::endl;
            continue;
        }
        for (uint64_t j = 0; j < stackCount; j++) {
            DiogenesCommon::BinaryAddress t;
            std::cerr << "Entry Value: Addr = " << entries[j].addr << " libname = " << entries[j].libname << std::endl;
            t.libraryOffset = 0; t.processAddress = 0; t.binaryName = NULL;
            std::string entryName = std::string(entries[j].libname);
            if (entryName.find("VIRTADDR") != std::string::npos)
                t.processAddress = entries[j].addr;
            else {
                if (nameMap.find(entryName) == nameMap.end())
                    nameMap[entryName] = std::shared_ptr<char>(strdup(entryName.c_str()));
                t.binaryName = nameMap[entryName];
                t.libraryOffset = entries[j].addr;
            } 
            addrs.push_back(t);
        }
        ret[keys[i]] = addrs;
    }
}