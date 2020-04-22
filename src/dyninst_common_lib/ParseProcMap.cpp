#include "ParseProcMap.h"
namespace DiogenesCommon{

ParseProcMap::ParseProcMap(char * procfile){
    procmaps_iterator* parse = pmparser_parse(procfile);
	procmaps_struct* tmp=NULL;
	while( (tmp = pmparser_next(parse)) != NULL){
        if (tmp->is_x == false)
            continue;
        _maps[(uint64_t)tmp->addr_start] = *tmp;
        if (strlen(tmp->pathname) > 0) {
            std::string tmps = std::string(tmp->pathname);
            if (_pathnameMap.find(tmps) == _pathnameMap.end())
                _pathnameMap[tmps] = std::shared_ptr<char>(strdup(tmps.c_str()));
        }
	}
    pmparser_free(parse);
}
bool ParseProcMap::GetLibraryAndOffset(BinaryAddress & in) {
    auto it = _maps.lower_bound(in.processAddress);
    if (it == _maps.end())
        return false;
    procmaps_struct s = it->second;
    if ((uint64_t)s.addr_start <= in.processAddress && (uint64_t)s.addr_end >= in.processAddress){
        in.libraryOffset = (in.processAddress - (uint64_t)s.addr_start) + s.offset;
        if (strlen(s.pathname) > 0)
            in.binaryName = _pathnameMap[std::string(s.pathname)];
        else
            in.binaryName = NULL;
        return true;
    }
    return false;
}
};