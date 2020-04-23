#include "SymbolInfo.h"
std::string DiogenesCommon::LineMap::Print(uint64_t spaceLength) {
    std::stringstream ss;
    ss << std::string(spaceLength, ' ') << filename << "(ln:"<< std::dec <<  line << ",col:"<< column << ")" << std::endl;
    return ss.str();
}

std::string DiogenesCommon::SymbolInfo::Print(uint64_t spaceLength) {
    std::string header = "Function Names: ";
    std::stringstream ss;

    ss << std::string(spaceLength, ' ') << header;
                                           
    for (int i = 0; i < funcName.size(); i++) {
        if (i == 0) {
            ss << funcName[i] << std::endl;
        } else {
            ss << std::string(spaceLength+header.size(), ' ') << funcName[i] << std::endl;
        }
    }
    if (funcName.size() == 0)
        ss << std::endl;
    header = std::string("Source Files: ");
    ss << std::string(spaceLength, ' ') << header;  
    int count = 0;
    for (auto i : lineinfo) {
        if (count == 0)
            ss << i.Print(0); 
        else
            ss << i.Print(spaceLength+header.size()); 
    }
    if (lineinfo.size() == 0)
        ss << std::endl;
    return ss.str();
}