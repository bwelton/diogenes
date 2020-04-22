#ifndef DIOGENES_PARSEPROCMAP
#define DIOGENES_PARSEPROCMAP 1
#include "BinaryAddress.h"
#include "pmparser.h"
#include <memory>
#include <string>
#include <map>

namespace DiogenesCommon{
    class ParseProcMap {
        public:
            ParseProcMap(char * procfile);
            bool GetLibraryAndOffset(BinaryAddress & in);
        private:
            std::map<uint64_t, procmaps_struct> _maps;
            std::map<std::string, std::shared_ptr<char>> _pathnameMap;
    };
}
#endif