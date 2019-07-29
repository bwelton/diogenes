#pragma once
#include <map>
#include <set>
#include <string>
class DetectDuplicateStackpoints {
public:
	DetectDuplicateStackpoints() {};

	// Returns TRUE if entry does not exist and was inserted.
	bool CheckAndInsert(std::string & filename, uint64_t offsetAddress){
		bool ret = true;
		auto it = _imap.find(filename);
		if (it == _imap.end()) {
			_imap[filename] = std::set<uint64_t>();
			it = _imap.find(filename);
		}
		auto it2 = it->second.find(offsetAddress);
		if (it2 != it->second.end())
			ret = false;
		else 
			it->second.insert(offsetAddress);
		return ret;
	};

private:
	std::map<std::string, std::set<uint64_t> > _imap;
	
};