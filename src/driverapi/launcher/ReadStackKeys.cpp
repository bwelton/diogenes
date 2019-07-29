#include "ReadStackKeys.h"

ReadStackKeys::ReadStackKeys(std::string key, std::string bin) : _key(key), _bin(bin) {}

void ReadStackKeys::ExtractLineInfo(StackRecMap & rec) {
	SymbolMap symbolInfo;
	// Open all symtabs for functions found on the stack. 
	for (auto i : rec) {
		std::vector<std::string> lnames = i.second.GetLibNames();
		for (auto z : lnames){
			if (symbolInfo.find(z) == symbolInfo.end())
				symbolInfo[z] = std::shared_ptr<SymbolLookup>(new SymbolLookup(z));
		}	
	}
	for (auto & i : rec)
		i.second.GetStackSymbols(symbolInfo);
}

void ReadStackKeys::GetStackRecords(StackRecMap & ret, std::function<void(StackRecMap &)> parsingFunc) {
	std::cerr << "[ReadStackKeys::GetStackRecords] Reading stack file: " << _bin << std::endl;
	std::cerr << "[ReadStackKeys::GetStackRecords] Reading key file: " << _key << std::endl;


	FILE * keyFile = fopen(_key.c_str(), "rb");
	assert(keyFile != NULL);

	StackKeyReader reader(keyFile);
	std::map<uint64_t, std::vector<StackPoint> > readStacks = reader.ReadStacks();

	for (auto & i : readStacks)
		ret[i.first] = StackRecord(i.first, i.second);

	ExtractLineInfo(ret);

	std::vector<StackPoint> e;
	// Insert an empty element at 0 for unidentified synchronizations.
	ret[0] = StackRecord(0, e);

	parsingFunc(ret);

}

// Individual parsing functions for different data file types
void ReadStackKeys::ProcessStacktraceSynch(StackRecMap & ret) {
	FILE * binfile = fopen(_bin.c_str(), "rb");
	assert(binfile != NULL);

	uint64_t totalSyncs = 0;
	uint64_t hash = 0;
	uint64_t pos = 0;
	while (fread(&hash, 1, sizeof(uint64_t), binfile) > 0) {
		if (ret.find(hash) == ret.end()){
			std::cerr << "[ReadStackKeys::ProcessStacktraceSynch] Could not find stack associated with - " << hash << std::endl;
		} else {
			ret[hash].AddOccurance(pos);
		}
		pos++;
		if (feof(binfile))
			break;
	}
	fclose(binfile);

}

void ReadStackKeys::ProcessTFTimingData(StackRecMap & ret) {
	TFReaderWriter rw;
	TF_Record rec;
	uint64_t total = 0;
	uint64_t pos = 0;
	assert(rw.Open(_bin.c_str(), TF_READ) == true);
	while (rw.Read(rec)) {
		if (rec.type == TF_SYNCRECORD) {
			if (rec.s.dynId == 0)
				continue;
			assert(ret.find(rec.s.stackId) != ret.end());
			ret[rec.s.stackId]._timing.push_back(rec);
			total += rec.s.count;
			pos++;
		}
	}

	std::cerr << "[ReadStackKeys::ProcessTFTimingData] Processed " << pos << " synchronization records" << std::endl;
	std::cerr << "[ReadStackKeys::ProcessTFTimingData] Captured " << total << " calls to low level synchronization" << std::endl;

}