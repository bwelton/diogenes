#include "CPPStackTree.h"
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

namespace DiogenesCommon {
    class CPPStackTrie{
        public:
            CPPStackTrie(uint64_t key, uint64_t value);
            void ConvertTreeToStackKey(std::vector<uint64_t> & eleStack, std::stringstream & outStr);
            bool InsertStack(uint64_t * elementArray, uint64_t * dataArray, uint64_t count);
            bool LookupStack(uint64_t * elementArray, uint64_t * returnData, uint64_t count);
            uint64_t _key;
            uint64_t _value;
            std::unordered_map<uint64_t, CPPStackTrie *> _children;
    };
};


DiogenesCommon::CPPStackTrie::CPPStackTrie(uint64_t key, uint64_t value) : _key(key), _value(value) {


};


bool DiogenesCommon::CPPStackTrie::InsertStack(uint64_t * elementArray, uint64_t * dataArray, uint64_t count) {
    DiogenesCommon::CPPStackTrie * cur = this;
    uint64_t pos = 0;
    bool newEntry = false;
    while (pos < count) {
        DiogenesCommon::CPPStackTrie * n;
        auto it = cur->_children.find(elementArray[pos]);
        if (it == cur->_children.end()) {
            n = new DiogenesCommon::CPPStackTrie(elementArray[pos],0);
            cur->_children[elementArray[pos]] = n;
        } else {
            n = it->second;
        }
        pos++;
        if (pos == count){
            if (n->_value == 0) {
                n->_value = *dataArray;
                //std::cerr << "Inserted new stack with value - " << n->_value << std::endl;
                newEntry = true;
            }
        } 
        cur = n;
    }
    return newEntry;
}

bool DiogenesCommon::CPPStackTrie::LookupStack(uint64_t * elementArray, uint64_t * returnData, uint64_t count) {
    DiogenesCommon::CPPStackTrie * cur = this;
    uint64_t pos = 0;
    bool newEntry = false;
    while (pos < count) {
        DiogenesCommon::CPPStackTrie * n;
        auto it = cur->_children.find(elementArray[pos]);
        if (it == cur->_children.end()) {
           return false;
        } else {
            n = it->second;
        }
        pos++;
        if (pos == count){
            if (n->_value != 0) {
                //std::cerr << "Found stack with ID - " << n->_value << std::endl;
                *returnData = n->_value;
                return true;
            } else {
                return false;
            }
        }
        cur = n;
    }
    return false;
}

void DiogenesCommon::CPPStackTrie::ConvertTreeToStackKey(std::vector<uint64_t> & eleStack, std::stringstream & outStr) {
    eleStack.push_back(_key);
    if (_value != 0) {
        outStr << std::dec << _value << "$";
        for (int i = 0; i < eleStack.size(); i++){
            outStr << "VIRTADDR@" << std::hex << eleStack[i];
            if (i < eleStack.size() - 1) {
                outStr << ",";
            }
        }
        outStr << "\n";
    }
    for(auto i : _children) {
        i.second->ConvertTreeToStackKey(eleStack, outStr);
    }
    eleStack.pop_back();
}

thread_local DiogenesCommon::CPPStackTrie * stacktree_LocalThread = NULL;
extern "C" {
    CPPStackTrie CPPStackTrie_Initalize() {
        return (CPPStackTrie) new DiogenesCommon::CPPStackTrie(0,0);
    }
    void CPPStackTrie_ConvertTreeToStackKey(CPPStackTrie  tree, char ** ret, uint64_t * size) {
        std::stringstream tmp;
        *size = 0;
        if (tree == NULL)
            return;
        for(auto i : ((DiogenesCommon::CPPStackTrie *)tree)->_children){
            std::vector<uint64_t> stack;
            i.second->ConvertTreeToStackKey(stack,tmp);
        }
        std::string rt =tmp.str();
        if (rt.size() == 0)
            return;
        *ret = strdup(rt.c_str());
        *size = rt.size();
        return;
    }
    bool CPPStackTrie_InsertStack(CPPStackTrie  tree, uint64_t * elementArray, uint64_t * dataArray, uint64_t count) {
        if (tree == NULL)
            return false;
        //std::cerr << "Inserting Stack!" << std::endl;
        return (( DiogenesCommon::CPPStackTrie *)tree)->InsertStack(elementArray, dataArray, count);
    }

    bool CPPStackTrie_LookupStack(CPPStackTrie  tree,  uint64_t * elementArray, uint64_t * returnData, uint64_t count) {
        if (tree == NULL)
            return false;
        return (( DiogenesCommon::CPPStackTrie *)tree)->LookupStack(elementArray, returnData, count);
    }
};

