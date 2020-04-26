#include "ReadStacks.h"

std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> ReadStacksFromMutatee(char * infile) {
    std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress> > ret;
    std::ifstream inFileStream;
    inFileStream.open(std::string(infile));
    std::string line;
    while (std::getline(inFileStream, line)) {
        std::vector<DiogenesCommon::BinaryAddress> e;
        size_t splitPos = line.find('$');        
        if (splitPos == 0)
            break;
        std::string idNumLine = line.substr(0, splitPos);
        uint64_t id = std::stoull(idNumLine);
        size_t curPos = splitPos +1;
        while (curPos != std::string::npos && curPos < line.size() - (splitPos + 1)) {
            DiogenesCommon::BinaryAddress tmp;
            std::string pathname;
            std::string address;
            size_t firstElement = line.find('@', curPos);
            size_t endSecond = line.find(',', firstElement);
            pathname = line.substr(curPos, firstElement - curPos);
            if (endSecond == std::string::npos){
                address = line.substr(firstElement+1);
            } else {
                address = line.substr(firstElement+1,endSecond - firstElement - 1);
            }
            //std::cerr << "PATHNAME=" << pathname << "    " << "ADDRESS="<< address << std::endl;
            tmp.binaryName.reset(strdup(pathname.c_str()));
            if (pathname.find("VIRTADDR") == std::string::npos){
                tmp.libraryOffset = std::stoull(address,0,16);
            } else {
                tmp.processAddress = std::stoull(address,0,16);
            }
            curPos = endSecond+1;
            e.push_back(tmp);
            
            if (endSecond == std::string::npos)
                break;
        }
        ret[id] = e;
        // std::string idNum;
        // std::string stackList;
        // uint64_t id = 0;
        // assert(std::getline(ss, idNum,'$'));
        // assert(std::getline(ss, stackList,'$'));
        // id = std::stoull(idNum);
        // std::istringstream ss2(stackList);
        // std::string pair;
        // while (std::getline(ss2, pair,',')) {
        //     std::string libname = pair.substr(0,pair.find('@'));
        //     std::string offset  = pair.substr(pair.find('@')+1);
        //     DiogenesCommon::BinaryAddress tmp;
        //     tmp.binaryName.reset(strdup(libname.c_str()));
        //     if (libname.find("VIRTADDR") != std::string::npos) 
        //         tmp.processAddress =  std::stoull(offset,0,16);
        //     else
        //         tmp.libraryOffset =   std::stoull(offset,0,16);
        //     e.push_back(tmp);
        // }
//        ret[id]= e;

    }
    return ret;
    // FILE * in = fopen(infile,"rb");
    // fseek(in, 0, SEEK_END);
    // uint64_t size = ftell(in);
    // fseek(in, 0, SEEK_SET);
    // std::shared_ptr<char> tmp(new char[size+1]);

    // uint64_t readPos = 0;
    // do {
    //     char * myString = tmp.get();
    //     readPos += fread(&myString[readPos], 1, size - readPos, in);
    // } while (readPos != size);
    // tmp.get()[size] = '\000';
    // char * token = strtok(tmp.get(), "\n");
    // while (token != NULL) {
    //     uint64_t hash = 0;
    //     std::string tmpToken = std::string(token);
    //     std::istringstream ifstring(tmpToken);
    //     std::string line;
    //     while (getline(ifstring, line, '$')) {
    //         if (line.find("@") == std::string::npos){
    //             hash = std::stoull(line);
    //             //std::cerr << "My hash - " << hash << std::endl;
    //             ret[hash] = std::vector<DiogenesCommon::BinaryAddress>();
    //         } else {
    //             DiogenesCommon::BinaryAddress sp;
    //             sp.binaryName = std::shared_ptr<char>(strdup(line.substr(0, line.find('@')).c_str()));	
    //             if (strcmp(sp.binaryName.get(), "VIRTADDR") == 0) {
    //                 sp.processAddress = std::stoull(line.substr(line.find('@')+1), 0, 16); 
    //             } else {
    //                 sp.libraryOffset = std::stoull(line.substr(line.find('@')+1), 0, 16); 
    //             }	
    //             std::cerr <<  hash << "," << sp.binaryName << "," << std::hex << sp.processAddress << "," << sp.libraryOffset << std::endl;
    //             ret[hash].push_back(sp);
    //         }
    //     }
    //     token = strtok(NULL,"\n");
    // }
    return ret;
        // uint64_t hash = std::stoull(tmpToken.substr(0, tmpToken.find('$')));
        // uint64_t start = tmpToken.find('$') + 1;
        
        // while(start < tmpToken.size()){
        // 	StackPoint sp;
        // 	std::string r;
        // 	if (tmpToken.find('$', start) != std::string::npos){
        // 		r = tmpToken.substr(start, tmpToken.find('$', start));
        // 		start = tmpToken.find('$', start) + 1;
        // 	}	else{
        // 		r = tmpToken.substr(start);
        // 		start = tmpToken.size();
        // 	}
        // 	std::cerr << r << std::endl;
        // 	sp.libname = r.substr(0, r.find('@'));	
        // 	sp.libOffset = std::stoull(r.substr(r.find('@')));
        // 	std::cerr <<  hash << "," << sp.libname << "," << sp.libOffset << std::endl;
        // 	ret[hash].push_back(sp);
        // }


//     std::map<std::string, std::shared_ptr<char>> nameMap;
//     std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> ret;
//     uint64_t fileSize = 0;
//     char * fileData = FileIO_ReadFile(malloc, infile, &fileSize);
//     if (fileData == NULL){
//         std::cerr << "ERROR COULD NOT OPEN STACKFILE - " << infile << std::endl;
//         exit(-1);
//     }
//     //std::cerr << "Building hashmap from file with filesize " << fileSize << std::endl;
//     HashMap * map = ReadStackKeys(malloc,free, fileData, fileSize);

//     fileSize = 0;
//     uint64_t * keys = ReadStackKeys_GetIds(map, &fileSize);
//     if (keys == NULL){
//         std::cerr << "[ReadStacksFromMutatee] Could not read keys from file - " << infile << std::endl;
//         return ret;
//     }
//     for(uint64_t i = 0; i < fileSize; i++) {
//         uint64_t stackCount = 0;
//         std::vector<DiogenesCommon::BinaryAddress> addrs;
//         StackKeyEntry * entries = ReadStackKeys_GetElementAt(map, keys[i], &stackCount);
//         if (entries == NULL) {
//             std::cerr << "[ReadStacksFromMutatee] Could not read stack keys for entry - " << i << std::endl;
//             continue;
//         }
//         for (uint64_t j = 0; j < stackCount; j++) {
//             DiogenesCommon::BinaryAddress t;
//             //std::cerr << "Entry Value: Addr = " << entries[j].addr << " libname = " << entries[j].libname << std::endl;
//             t.libraryOffset = 0; t.processAddress = 0; t.binaryName = NULL;
//             std::string entryName = std::string(entries[j].libname);
//             if (entryName.find("VIRTADDR") != std::string::npos)
//                 t.processAddress = entries[j].addr;
//             else {
//                 if (nameMap.find(entryName) == nameMap.end())
//                     nameMap[entryName] = std::shared_ptr<char>(strdup(entryName.c_str()));
//                 t.binaryName = nameMap[entryName];
//                 t.libraryOffset = entries[j].addr;
//             } 
//             addrs.push_back(t);
//         }
//         ret[keys[i]] = addrs;
//     }
}
