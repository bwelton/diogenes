#include "DuplicateTracker.h"

DuplicateTracker::~DuplicateTracker() {
	// Always squak this to screen
  	fprintf(stderr,"[DUPLICATE DETECTION] - Collisions: %llu, Size: %lld\n", _collisionCount, _collisionSize); 
  	fprintf(stderr,"[DUPLICATE DETECTION] - Total Transfer: %llu, Total Size: %lld\n", _totalCount, _totalSize); 
	std::stringstream ss;
	ss << "[DUPLICATE DETECTION] - Collisions: " << _collisionCount << ", Size: " << _collisionSize << std::endl;
	ss << "[DUPLICATE DETECTION] - Total Transfer: " << _totalCount << ", Total Size: " << _totalSize << std::endl;
	std::string out = ss.str();	
	_log.get()->Write(out);
}
DuplicateTracker::DuplicateTracker(bool enabled, FILE * file) {
	_collisionSize = 0;
	_collisionCount = 0;
	_totalSize = 0;
	_totalCount = 0;
	_enabled =  enabled;
	if (_enabled == true) 
		_log.reset(new LogInfo(file));
}

bool DuplicateTracker::DetectDuplicate(TransferPtr t) {
	bool duplicate = false;
	size_t transSize = t.get()->GetSize();
	if (transSize == 0)
		return false;

	uint32_t hash = t.get()->GetHash();

	boost::recursive_mutex::scoped_lock lock(_mtx);

	if (_prevTransfers.find(hash) != _prevTransfers.end()) {
		_collisionCount++;
		_collisionSize += transSize;
		duplicate = true;
	} else {
		_prevTransfers[hash] = transSize;
	}
	_totalSize += transSize;
	_totalCount += 1;
	return duplicate;
}


int DuplicateTracker::PerformAction(TransferPtr t) {
	if (_enabled == true) {
		bool ret = DetectDuplicate(t);
		std::stringstream ss;
		// std::cerr << "[DUPLICATE DETECTION] - Transfer " << t.get()->GetStringTransferKind() << " is " << (ret ? "a": "NOT a") << " duplicate" << std::endl;
		// std::cerr << "My Size: " << t.get()->GetSize() << std::endl;		
		ss << "[DUPLICATE DETECTION] - Transfer " << t.get()->GetStringTransferKind() << " is " << (ret ? "a": "NOT a") << " duplicate" << std::endl;
		if (ret == true)
			std::cerr << "[DUPLICATE DETECTION] - Transfer " << t.get()->GetStringTransferKind() << " is " << (ret ? "a": "NOT a") << " duplicate" << std::endl;
		std::string out = ss.str();
		_log.get()->Write(out);
	}
	return 0;
}

int DuplicateTracker::PostTransfer(TransferPtr t) {
	return 0;
}