#include "FastStackTree.h"
FastStackTree::FastStackTree(uint64_t sAddress) : _sAddr(sAddress), ending(false) {}

void FastStackTree::InsertStack(std::vector<DiogenesCommon::BinaryAddress> & addrs) {
	FastStackTree * cur = this;
	for(int i = 0; i < addrs.size(); i++) {
		auto it = cur->_children.find(addrs[i].libraryOffset);
		if (it != cur->_children.end())
			cur = it->second;
		else {
			cur->_children[addrs[i].libraryOffset] = new FastStackTree(addrs[i].libraryOffset);
			cur = cur->_children[addrs[i].libraryOffset];
		} 
	}
	cur->ending = true;
}
bool FastStackTree::Lookup(uint64_t * s, uint64_t size) {
	FastStackTree * cur = this;
	for(uint64_t i = 0; i < size; i++) {
		auto it = cur->_children.find(s[i]);
		if (it == cur->_children.end())
			return false;
		cur = it->second;
	}
	return cur->ending;
}	