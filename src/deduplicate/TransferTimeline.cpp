#include "TransferTimeline.h"

int TransferTimeline::PerformAction(TransferPtr t) {
	if (_enabled == true)
		AddTransfer(t.get()->GetStringTransferKind(), t.get()->GetHash(), t.get()->GetSize(), t.get()->GetID());
	return 0;
}

void TransferTimeline::AddTransfer(std::string & type, uint32_t id, size_t size, uint64_t globalId) {
	uint64_t origTransfer = 0;
	uint64_t count = 0;
	{
		boost::recursive_mutex::scoped_lock lock(_timeline_mtx);
		if (_pastTransfers.find(id) != _pastTransfers.end())
			origTransfer = _pastTransfers[id];
		else
			_pastTransfers[id] = globalId;
		count = _count;
		_count++;
	}
	WriteLogEntry(type, id, size, count, origTransfer, globalId);
}

void TransferTimeline::WriteLogEntry(std::string & type, uint32_t id, size_t size, uint64_t count, uint64_t origTransfer, uint64_t globalId) {
	std::stringstream s;
	s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	Write(s.str());
}

int TransferTimeline::PostTransfer(TransferPtr t){
	return 0;
}

void TransferTimeline::Write(std::string a){
	_timeline_log.get()->Write(a);
}

TransferTimeline::TransferTimeline(bool enabled, FILE * file) {
	_enabled = enabled;
	if (enabled == true) {
		_timeline_log.reset(new LogInfo(file));
	}
	_count = 1;
}

TransferTimeline::~TransferTimeline() {
	
}


