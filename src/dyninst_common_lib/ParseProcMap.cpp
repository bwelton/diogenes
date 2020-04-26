#include "ParseProcMap.h"
#include <iostream>
#include <dlfcn.h>
namespace DiogenesCommon{

ParseProcMap::ParseProcMap(char * procfile){
    procmaps_iterator* parse = pmparser_parse(procfile);
	procmaps_struct* tmp=NULL;
    bool foundHeap = false;
	while( (tmp = pmparser_next(parse)) != NULL){
        if (tmp->is_x == false)
            continue;
        _maps[(uint64_t)tmp->addr_start] = *tmp;
        if (strlen(tmp->pathname) > 0) {
            std::string tmps = std::string(tmp->pathname);
            if (_pathnameMap.find(tmps) == _pathnameMap.end()){
                _pathnameMap[tmps] = std::shared_ptr<char>(strdup(tmps.c_str()));
                std::cerr << "Checking if file " << tmps << " is PIC/PIE" << std::endl;
                if(!dlopen(tmps.c_str(),RTLD_NOLOAD)) {
                    void * handle = dlopen(tmps.c_str(),RTLD_LAZY);
                    if (handle != NULL){
                        _isPIC[tmps] = true;
                        std::cerr << tmps << " is a PIC/PIE" << std::endl;
                        dlclose(handle);
                    } else {
                        std::cerr << tmps << " is NOT PIC/PIE" << std::endl;
                        _isPIC[tmps] = false;
                    }
                } else {
                    std::cerr << tmps << " is a PIC/PIE" << std::endl;
                    _isPIC[tmps] = true;
                }
            }
        }
	}
    // for (auto i : _maps) {
    //     std::cerr << "PROCMAP DUMP - " << std::hex << i.first <<  "..." << (uint64_t)i.second.addr_end << " - " << i.second.pathname << std::endl;
    // }
    pmparser_free(parse);
}


bool ParseProcMap::GetLibraryAndOffset(BinaryAddress & in) {
    auto it = _maps.upper_bound(in.processAddress);
    if (it == _maps.end())
        return false;
    if (it != _maps.begin())
        std::advance(it,-1);
    procmaps_struct s = it->second;
    // std::cerr << "PROCADDR = " << std::hex<< in.processAddress << " MATCHED = " << std::hex<< (uint64_t)s.addr_start << std::endl;
    if ((uint64_t)s.addr_start <= in.processAddress && (uint64_t)s.addr_end >= in.processAddress){
        std::string checkForPic = std::string(s.pathname);
        if(_isPIC[checkForPic] == false)
            in.libraryOffset = (in.processAddress);
        else
            in.libraryOffset = (in.processAddress - (uint64_t)s.addr_start) + s.offset;

// #ifndef __POWERPC__
//         in.libraryOffset = (in.processAddress - (uint64_t)s.addr_start) + s.offset;
// #else
//         // Whack check on powerpc due to the weird way application binaries are
//         // compiled. This may be a ray only thing though...
//         std::string checkForBeforeHeap = std::string(s.pathname);
//         if (_beforeHeap.find(checkForBeforeHeap) != _beforeHeap.end())
//             in.libraryOffset = (in.processAddress - (uint64_t)s.addr_start) + s.offset;
//         else
//             in.libraryOffset = (in.processAddress);
// #endif
        if (strlen(s.pathname) > 0)
            in.binaryName = _pathnameMap[std::string(s.pathname)];
        else {
            in.binaryName = NULL;
            return false;
        }
        return true;
    }
    return false;
}
};