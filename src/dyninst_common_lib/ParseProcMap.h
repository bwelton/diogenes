#ifndef DIOGENES_PARSEPROCMAP
#define DIOGENES_PARSEPROCMAP 1
#include "BinaryAddress.h"
#include <memory>
#include <set>
#include <string>
#include <map>
#include "pmparser.h"
namespace DiogenesCommon{
    class ParseProcMap {
        public:
            ParseProcMap(char * procfile);
            bool GetLibraryAndOffset(BinaryAddress & in);
        private:
            std::map<uint64_t, procmaps_struct> _maps;
            std::set<std::string> _beforeHeap;
            std::map<std::string, std::shared_ptr<char>> _pathnameMap;
    };
}
#endif