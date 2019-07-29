#include "StacktraceGen.h"
#include <execinfo.h>
#define BT_BUF_SIZE 100

StackTraceGen::StackTraceGen(bool enabled, FILE * file) {
	_enabled = enabled;
	if (enabled == false)
		return;
	_st_log.reset(new LogInfo(file));
}
StackTraceGen::~StackTraceGen() {
	WriteStackMap();
}

std::string StackTraceGen::GenStackTrace() {
	std::stringstream ret; 	
	std::vector<Frame> stackwalk;
	std::string s;
	void * sym; 
	Walker * walker = Walker::newWalker();
	walker->walkStack(stackwalk);
	Dyninst::Offset offset;
	for (int i = 0; i < stackwalk.size(); i++) {
		stackwalk[i].getName(s);
		ret << s << " - ";
		if(stackwalk[i].getLibOffset(s, offset, sym) == false){
		 	continue;
		}
		Symtab * curSym = static_cast<Symtab *>(sym);
		if (sym == NULL) {
			ret << "\n";
			continue;
		}
		std::vector<Statement *> lines;
		if((curSym)->getSourceLines(lines,offset) == false) {
			ret << "\n";
			continue;
		}
		for (int q = 0; q < lines.size(); q++) {
			ret << lines[q]->getFile() << ":" << std::to_string(lines[q]->getLine()) << " ";
		}
		ret << "\n";
	}
	ret << "\n";
	return ret.str();
}

void StackTraceGen::WriteStackMap() {
	std::vector<std::pair<int, uint32_t> > hashes;
	for (auto x : _stackMap) {
		hashes.push_back(std::make_pair(x.second.second, x.first));
	}
	std::sort(hashes.begin(), hashes.end());
	for (auto x : hashes) {
		char tmp[4096];
		snprintf(tmp, 4096, "Hash,%u,Count,%d\n",x.second, x.first);
		std::string output = std::string(tmp);
		_st_log.get()->Write(output);
		_st_log.get()->Write(_stackMap[x.second].first);
	}
	boost::recursive_mutex::scoped_lock lock(_st_mtx);
	_stackMap.clear();
	_transferHash.clear();
}

int StackTraceGen::PerformAction(TransferPtr t) {
	if (_enabled == false)
		return 0;
	
	std::string stack = GenStackTrace();
	uint32_t transferHash = t.get()->GetHash();
	uint32_t hash = t.get()->HashPtr((void *)stack.c_str(), stack.size());
	{
		boost::recursive_mutex::scoped_lock lock(_st_mtx);
		if (_transferHash.find(transferHash) != _transferHash.end()) {
			if (_stackMap.find(hash) != _stackMap.end())
				_stackMap[hash].second += 1;
			else
				_stackMap[hash] = std::make_pair(stack, 1);
		} else {
			_transferHash.insert(transferHash);
		}
	}
}

int StackTraceGen::PostTransfer(TransferPtr t) {
	boost::recursive_mutex::scoped_lock lock(_st_mtx);
	if (_stackMap.size() > 3000) {
		WriteStackMap();
	}
}