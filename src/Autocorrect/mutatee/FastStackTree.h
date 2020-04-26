#ifndef AUTOCORR_FASTSTACK
#define AUTOCORR_FASTSTACK 1
#include <unordered_map>
#include <vector>
#include <inttypes.h>
#include "BinaryAddress.h"

class FastStackTree {
	public: 
		FastStackTree(uint64_t sAddress);
		void InsertStack(std::vector<DiogenesCommon::BinaryAddress> & addrs);
		bool Lookup(uint64_t * s, uint64_t size);
		std::unordered_map<uint64_t, FastStackTree *> _children;
		bool ending;
		uint64_t _sAddr;
};

#endif