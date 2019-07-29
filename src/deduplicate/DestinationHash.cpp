#include "DestinationHash.h"
DestinationHash::DestinationHash(bool enabled, FILE * file) {
	_enabled =  enabled;
	_identCount = 0;
	if (_enabled == true) 
		_log.reset(new LogInfo(file));
}

int DestinationHash::PerformAction(TransferPtr t) {
	if (_enabled == false)
		return 0;

	std::shared_ptr<void> origData = t.get()->GetDestinationData();
	uint32_t origHash = t.get()->HashPtr(origData, t.get()->GetSize());
	uint32_t curHash = t.get()->GetHash();
	uint64_t id = t.get()->GetID();

	std::stringstream ss;
	ss << id << "," << (origHash == curHash ? "same" : "diff") << "," << origHash << "," << curHash << "," << t.get()->GetSize() << std::endl;
	std::string out = ss.str();	
	_log.get()->Write(out);

	{
		boost::recursive_mutex::scoped_lock lock(_mtx);
		if (origHash == curHash)
			_identCount++;
	}
	return 0;
}

int DestinationHash::PostTransfer(TransferPtr t){
	return 0;
}

DestinationHash::~DestinationHash() {
	std::stringstream ss;
	ss << "Found " << _identCount << " Transfers replacing identical data at the destination" << std::endl;
	std::string out = ss.str();	
	_log.get()->Write(out);
}