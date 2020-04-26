#include "AddressSymbolizer.h"

DiogenesCommon::SymbolStorage::SymbolStorage(std::string libname) : _bedit(new DiogenesCommon::DyninstBinaryEdit(libname,false, std::string())) {

    if(_bedit->GetImage() == NULL)
        return;
    std::vector<BPatch_function *> funcs;
    _bedit->GetImage()->getProcedures(funcs, true);
    for (auto i : funcs) {
        BPatch_flowGraph * cfg = i->getCFG();
        std::set<BPatch_basicBlock*> blocks;
        cfg->getAllBasicBlocks(blocks);
        for (auto b : blocks){
            if(_blockMap.find(b->getStartAddress()) == _blockMap.end())
                _blockMap[b->getStartAddress()] = std::set<BPatch_function*>();
            _blockMap[b->getStartAddress()].insert(i);
            //std::cerr << "[SymbolStorage::SymbolStorage] " << libname << " Inserting function " << i->getMangledName() << " with start address " << std::hex << b->getStartAddress() << std::endl;
        }

        // Get Statements for the function.
        std::vector<BPatch_statement> statements;
        if(i->getModule() != NULL){
            i->getModule()->getStatements(statements);
        } 
        for (auto s : statements) {
            bool copy = true;
            uint64_t stateAddr = (uint64_t)s.startAddr();
            if(_statementMap.find(stateAddr) == _statementMap.end())
                _statementMap[stateAddr] = std::vector<BPatch_statement>();
            for (auto e : _statementMap[stateAddr]) {
                if(strcmp(e.fileName(), s.fileName()) == 0 && e.startAddr() == s.startAddr() &&
                   e.endAddr() == s.endAddr() && s.lineNumber() == e.lineNumber() && s.lineOffset() == e.lineOffset()) {
                       copy = false;
                       break;
                }
            }
            if (copy) {
                _statementMap[stateAddr].push_back(s);
            }
        }
    }
/*    if (sobj == NULL)
        return;
    std::vector<Symbol *> symDump;
    _sobj->getAllSymbols(symDump);
    for (auto x : symDump) {
        if (_funcSymbols.find((uint64_t)x->getOffset()) != _funcSymbols.end())
            std::cerr << "[DiogenesCommon::SymbolStorage] Warning: symbol with same offset address identified! - "<< std::hex
                      << (uint64_t)x->getOffset() << std::dec<< std::endl;
        
        _funcSymbols[(uint64_t)x->getOffset()] = x;
    }
    std::vector<Module*> mods;
    _sobj->getAllModules(mods);
    for(auto n : mods) {
        std::vector<Dyninst::SymtabAPI::Statement::Ptr> statements;
        n->getStatements(statements);
        for (auto t : statements) {
            if (_lineStatements.find((uint64_t)t->startAddr()) != _lineStatements.end())
                std::cerr << "[DiogenesCommon::SymbolStorage] Warning: line info with same start offset address identified! - "<< std::hex
                          << (uint64_t)t->startAddr() << std::dec << std::endl;
        //    std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] STATEMENTDUMP FOR " << _sobj->file() << " " << std::hex << (uint64_t)t->startAddr() << ".." << std::hex << (uint64_t)t->endAddr() << "  " << std::dec << t->getFile() << " at line " << t->getLine() << std::endl;
            _lineStatements[(uint64_t)t->startAddr()] = t;
        }
    }

    for (auto i : _funcSymbols) {
        auto x = i.second;
        std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] SYMDUMP FOR " << _sobj->file() << " " << x->getMangledName() << " " << std::hex << (uint64_t)x->getOffset() << std::dec <<"..."<< x->getSize() <<  std::endl;
    }
    for (auto i : _lineStatements) {
        auto t = i.second;
        std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] STATEMENTDUMP FOR " << _sobj->file() << " " << std::hex << (uint64_t)t->startAddr() << ".." << std::hex << (uint64_t)t->endAddr() << "  " << std::dec << t->getFile() << " at line " << t->getLine() << std::endl;        
    }*/
}

std::vector<DiogenesCommon::LineMap> DiogenesCommon::SymbolStorage::GetLineInfoAtOffset(uint64_t offset) {
    std::vector<DiogenesCommon::LineMap> ret;
    auto it = _statementMap.lower_bound(offset);
    if (it == _statementMap.end()){
        //std::cerr << "FOUND NOTHING "<< std::endl;
        return ret;
    }
    if (it != _statementMap.begin() && it->first != offset)
        std::advance(it,-1);    

    for (auto x : it->second){
        if ((uint64_t)x.startAddr() <= offset && offset <= (uint64_t)x.endAddr()) {
            DiogenesCommon::LineMap tmp;
            tmp.line = (x.lineNumber() == -1 ? 0 : x.lineNumber());
            tmp.column = (x.lineOffset() == -1 ? 0 : x.lineOffset());
            tmp.filename = std::string(x.fileName());
            ret.push_back(tmp);
        }
    }
    return ret;
    // auto it = _lineStatements.lower_bound(offset);
    // if (it == _lineStatements.end())
    //     return ret;

    // auto fwd = it;
    // while(fwd != _lineStatements.end()) {
    //     if (offset >= fwd->first && offset <= (uint64_t)fwd->second->endAddr()){
    //         DiogenesCommon::LineMap tmp;
    //         tmp.line = fwd->second->getLine();
    //         tmp.column = fwd->second->getColumn();
    //         tmp.filename = fwd->second->getFile();
    //         ret.push_back(tmp);
    //     } else{
    //         break;
    //     }
    //     std::advance(fwd,1);
    // }

    // while (it != _lineStatements.begin()) {
    //     std::advance(it, -1);
    //     if (offset >= it->first && offset <= (uint64_t)it->second->endAddr()){
    //         DiogenesCommon::LineMap tmp;
    //         tmp.line = it->second->getLine();
    //         tmp.column = it->second->getColumn();
    //         tmp.filename = it->second->getFile();
    //         ret.push_back(tmp);
    //     } else{
    //         break;
    //     }
    // }
    // return ret;
 }

std::vector<std::string> DiogenesCommon::SymbolStorage::GetFunctionAtOffset(uint64_t offset) {

    // std::vector<BPatch_function*> tmp;
    std::vector<std::string> ret;
    // _bedit->GetImage()->findFunction(offset, tmp);
    // for (auto i : tmp)
    //     ret.push_back(i->getMangledName());
    //std::cerr << "Looking for lower bound of address " << std::hex << offset << std::endl;
    auto it = _blockMap.lower_bound(offset);
    if (it == _blockMap.end()){
        std::cerr << "FOUND NOTHING "<< std::endl;
        return ret;
    }
    if (it != _blockMap.begin() && it->first != offset)
        std::advance(it,-1);    


    //if (offset >= it->first && offset <= it->first + it->second->getSize())
    //     return it->second->getMangledName();
    for (auto i : it->second) {
        //std::cerr<< "Adding name.... " << i->getDemangledName() << std::endl;
        ret.push_back(i->getDemangledName());
    }
    // if (offset >= it->first && offset <= it->first + it->second->getSize())
    //     return it->second->getMangledName();
    return ret;
}

void DiogenesCommon::AddressSymbolizer::GetSymbolAtAddress(BinaryAddress & addr) {
    if (addr.binaryName == NULL)
        return;
    std::string libname = std::string(addr.binaryName.get());
    if (libname.find("VIRTADDR") != std::string::npos || libname.find("[vdso]") != std::string::npos || libname.find("[heap]") != std::string::npos)
        return;
    if(_symbolMap.find(libname) == _symbolMap.end()) {
        // Symtab * tmpObj;
        // Symtab::openFile(tmpObj, libname);
        std::shared_ptr<DiogenesCommon::SymbolStorage> tmpStore(new DiogenesCommon::SymbolStorage(libname));
        // if (FileIO_CheckDebug()){
        //     std::vector<Symbol *> symDump;
        //     tmpObj->getAllSymbols(symDump);
        //     for (auto n : symDump) {
        //         std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] SYMDUMP FOR " << libname << " " << n->getMangledName() << " " << std::hex << (uint64_t)n->getOffset() << std::dec <<"..."<< n->getSize() <<  std::endl;
        //     }
        //     std::vector<Module*> mods;
        //     tmpObj->getAllModules(mods);
        //     for(auto n : mods) {
        //         std::vector<Dyninst::SymtabAPI::Statement::Ptr> statements;
        //         n->getStatements(statements);
        //         for (auto t : statements) {
        //             std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] STATEMENTDUMP FOR " << libname << " " << std::hex << (uint64_t)t->startAddr() << ".." << std::hex << (uint64_t)t->endAddr() << "  " << std::dec << t->getFile() << " at line " << t->getLine() << std::endl;
        //         }
        //     }
        // }
        _symbolMap[libname] = tmpStore;
    }
    std::shared_ptr<DiogenesCommon::SymbolStorage> obj = _symbolMap[libname];
    if (obj == NULL){
        std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] Could not open object - " << libname << " with symtab, no symbol information present" << std::endl;
        return;
    }
    addr.symbolInfo.funcName = obj->GetFunctionAtOffset(addr.libraryOffset);
    addr.symbolInfo.lineinfo = obj->GetLineInfoAtOffset(addr.libraryOffset);
    // for (auto i : addr.symbolInfo.funcName)
    //     std::cerr << "written funcname = " << i << std::endl;
	// std::vector<Symbol *> ret = obj->findSymbolByOffset(addr.libraryOffset);
    // if (FileIO_CheckDebug())
    //     std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] Found - " << std::dec << ret.size() << " number of symbols for " 
    //               << libname << "@" << std::hex << addr.libraryOffset << std::dec << std::endl;

    // for (auto i : ret) {
    //     addr.symbolInfo.funcName.push_back(i->getMangledName());
    // }

	// std::vector<Dyninst::SymtabAPI::Statement::Ptr> lineNumbers;
	// obj->getSourceLines(lineNumbers, addr.libraryOffset);
    // if (FileIO_CheckDebug())
    //     std::cerr << "[AddressSymbolizer::GetSymbolAtAddress] Found - " << std::dec << lineNumbers.size() << " number of line info structs for " 
    //               << libname << "@" << std::hex << addr.libraryOffset << std::dec << std::endl;    
    // for (auto i : lineNumbers) {
    //     DiogenesCommon::LineMap m;
    //     m.line = i->getLine();
    //     m.column = i->getColumn();
    //     m.filename = i->getFile();
    //     addr.symbolInfo.lineinfo.push_back(m);
    // }
	/*SymtabAPI::Function * func = NULL;
	_obj->getContainingFunction(offset, func);*/
}
