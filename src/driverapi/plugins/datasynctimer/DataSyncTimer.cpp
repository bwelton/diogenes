#include "DataSyncTimer.h"
std::shared_ptr<DataSyncTimer> Worker;
thread_local int _DST_exited;

DataSyncTimer::DataSyncTimer(std::vector<std::string> & cmd_list) {
	_DST_exited = 0;
	_transCount = 0;
	_outFile.reset(new TFReaderWriter());
	_outFile->Open("DSTIME_trace.bin", TF_WRITE);
	_cpuOverhead.reset(new TFReaderWriter());
	_cpuOverhead->Open("DCPUTIME_trace.bin", TF_WRITE);
	_totalTime.reset(new TFReaderWriter());
	_totalTime->Open("DTOTIME_trace.bin", TF_WRITE);
	//_out.reset(new CollisionOut(std::string("DT_collisions.txt")));
	_keyFile.reset(new StackKeyWriter(fopen("DSTIME_stacks.bin","w")));
}

DataSyncTimer::~DataSyncTimer() {
	_DST_exited = 1;
}

PluginReturn DataSyncTimer::Precall(std::shared_ptr<Parameters> params) {
	MemoryTransfer * mem = params->GetMemtrans();
	if (mem->IsTransfer() == true){
		//mem->SetHashGeneration(true);
		//mem->PreTransfer();	
		auto start = std::chrono::high_resolution_clock::now();
		mem->Synchronize();
		auto stop = std::chrono::high_resolution_clock::now();
		// mem->SetHashGeneration(true);
		// mem->PostTransfer();		
		std::vector<StackPoint> points;
		bool ret = GET_FP_STACKWALK(points);
		uint64_t pos = 0;
		if (ret == false) {
			std::cout << "unknown timing stack, discarding time" << std::endl;
			return NO_ACTION;
		} else {
			pos = _keyFile->InsertStack(points);
		}
		TF_Record rec;
		rec.type = TF_TRANSFERREC;
		rec.r.dynId = pos;
		rec.r.stackId = pos;
		std::chrono::duration<double> diff = stop-start;
		rec.r.time = diff.count();
		_outFile->Write(rec);
		_transCount++;
		params->SetInstID(_transCount);
		// May be too slow for timing. fix later
		_prevTransfers[_transCount] = std::make_pair(std::chrono::high_resolution_clock::now(), pos);	
	}
	return NO_ACTION;
}

PluginReturn DataSyncTimer::Postcall(std::shared_ptr<Parameters> params) {
	MemoryTransfer * mem = params->GetMemtrans();
	if (mem->IsTransfer() == true){
		auto stop_cpu = std::chrono::high_resolution_clock::now();
		mem->Synchronize();
		auto stop_sync = std::chrono::high_resolution_clock::now();
		size_t id = params->GetInstID();
		if (_prevTransfers.find(id) == _prevTransfers.end()){
			std::cerr << "[DataSyncTimer] Could not look up ID: " << id << std::endl;
			return NO_ACTION;
		}
		TF_Record rec;
		rec.type = TF_TRANSFERREC;
		rec.r.dynId = _prevTransfers[id].second;
		rec.r.stackId = _prevTransfers[id].second;
		std::chrono::duration<double> diff = stop_cpu - _prevTransfers[id].first;
		rec.r.time = diff.count();
		_cpuOverhead->Write(rec);
		diff = stop_sync - _prevTransfers[id].first;
		rec.r.time = diff.count();
		_totalTime->Write(rec);	
		_prevTransfers.erase(id);
		

		// uint64_t originData = mem->GetOriginHash();
		// uint64_t transferedHash = mem->GetTransferHash();
		// if (transferedHash == 0) {
		// 	std::cerr << "[DataSyncTimer] Error getting hash of transfered data at stack id " << pos << std::endl;
		// 	return NO_ACTION;
		// }
		// bool duplicate = false;
		// bool overwrite = false;
		// bool prevTransfer = false;
		// uint64_t previousID = 0;

		// if (originData == transferedHash && originData != 0) {
		// 	duplicate = true;
		// 	overwrite = true;
		// }
		// if (_prevTransfers.find(transferedHash) != _prevTransfers.end()) {
		// 	duplicate =  true;
		// 	prevTransfer = true;
		// 	previousID = _prevTransfers[transferedHash];
		// } else {
		// 	_prevTransfers[transferedHash] = pos;
		// }
		// if (duplicate == true) {
		// 	_out->Write(pos, (overwrite ? uint64_t(1) : uint64_t(0)), (prevTransfer ? uint64_t(1) : uint64_t(0)), previousID);
		// }
	}
	return NO_ACTION;
}

extern "C"{
void init(std::vector<std::string> & cmd_list) {
	PLUG_BUILD_FACTORY(cmd_list)
}

PluginReturn Precall(std::shared_ptr<Parameters> params){
	if (_DST_exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Precall(params);
}

PluginReturn Postcall(std::shared_ptr<Parameters> params) {
	if (_DST_exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Postcall(params);
}

}