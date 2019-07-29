#include "TransferTimeline.h"
std::shared_ptr<TransferTimeline> Worker;
int exited = 0;
thread_local pid_t my_thread_id = -1; 
thread_local int my_process_id = -1;

void TransferTimeline::SetThreadLocals() {
	if (my_thread_id == -1) 
		my_thread_id = (pid_t) syscall(__NR_gettid);
	if (my_process_id == -1)
		my_process_id = (int) getpid();	
}


PluginReturn TransferTimeline::Precall(std::shared_ptr<Parameters> params) {
	MemoryTransfer * mem = params.get()->GetMemtrans();
	if (mem->IsTransfer() == false)
		return NO_ACTION;
	mem->PreTransfer();
	//std::cerr << "[TRANSFER-TIMELINE] - Captured transfer: " << params.get()->GetName() << std::endl;
	return NO_ACTION;
}

PluginReturn TransferTimeline::Postcall(std::shared_ptr<Parameters> params) {
	SetThreadLocals();
	MemoryTransfer * mem = params.get()->GetMemtrans();	
	if (mem->IsTransfer() == false)
		return NO_ACTION;
	mem->PostTransfer();
	WriteLog(params);
	return NO_ACTION;
}

// Fmt: id,procid,threadid,name,size,orighash,desthash,duplicateid

void TransferTimeline::WriteLog(std::shared_ptr<Parameters> params) {
	std::stringstream ss;
	Parameters * p = params.get();

	MemoryTransfer * mem = params.get()->GetMemtrans();
	int64_t duplicateid = -1;
	size_t globalId = p->GetInstID();

	if (!mem->IsSupportedTransfer()){
		ss << globalId << "," << my_process_id << "," << my_thread_id << "," << p->GetName() << ",-1,0,0,-1" << std::endl;
	} else {
		uint32_t originData = mem->GetOriginHash();
		uint32_t transferHash = mem->GetTransferHash();

		if (originData == transferHash)
			duplicateid = globalId;
		else {
			boost::recursive_mutex::scoped_lock lock(_timeline_mtx);
			if(_pastTransfers.find(transferHash) != _pastTransfers.end())
				duplicateid = _pastTransfers[transferHash];
			else
				_pastTransfers[transferHash] = globalId;
		}
		ss << globalId << "," << my_process_id << "," << my_thread_id << "," << p->GetName() << "," << mem->GetSize() << ",0x"
		   << std::hex <<  originData << std::dec << ",0x" << std::hex << transferHash << std::dec << "," << duplicateid << std::endl;
	}
	std::string tmp = ss.str();
	_tt_log.get()->Write(tmp);
}


TransferTimeline::TransferTimeline(std::vector<std::string> & cmd_list) {
	exited = 0;
	_tt_log.reset(new LogInfo(fopen("transfer_timeline.out", "w")));
}

TransferTimeline::~TransferTimeline() {
	exited = 1;
	_tt_log.get()->Flush();
	_tt_log.reset();
}


extern "C"{

void init(std::vector<std::string> & cmd_list) {
	PLUG_BUILD_FACTORY(cmd_list)
}

PluginReturn Precall(std::shared_ptr<Parameters> params){
	if (exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Precall(params);
}

PluginReturn Postcall(std::shared_ptr<Parameters> params) {
	if (exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Postcall(params);
}

}