#include "DataTransfer.h"
static std::shared_ptr<DataTransfer> Worker;
static int exited;

DataTransfer::DataTransfer(std::vector<std::string> & cmd_list) {
	exited = 0;
	_out.reset(new CollisionOut(std::string("DT_collisions.txt")));
	_keyFile.reset(new StackKeyWriter(fopen("DT_stacks.bin","w")));
}

DataTransfer::~DataTransfer() {
	exited = 1;
}

PluginReturn DataTransfer::Precall(std::shared_ptr<Parameters> params) {
	MemoryTransfer * mem = params->GetMemtrans();
	if (mem->IsTransfer() == true){
		mem->SetHashGeneration(true);
		mem->PreTransfer();		
	}
	return NO_ACTION;
}

PluginReturn DataTransfer::Postcall(std::shared_ptr<Parameters> params) {
	MemoryTransfer * mem = params->GetMemtrans();
	if (mem->IsTransfer() == true){
		mem->SetHashGeneration(true);
		mem->PostTransfer();		
		std::vector<StackPoint> points;
		bool ret = GET_FP_STACKWALK(points);
		uint64_t pos = 0;
		if (ret == false) {
			std::cout << "unknown timing stack, discarding time" << std::endl;
			return NO_ACTION;
		} else {
			pos = _keyFile->InsertStack(points);
		}			
		uint64_t originData = mem->GetOriginHash();
		uint64_t transferedHash = mem->GetTransferHash();
		if (transferedHash == 0) {
			std::cerr << "[DataTransfer] Error getting hash of transfered data at stack id " << pos << std::endl;
			return NO_ACTION;
		}
		bool duplicate = false;
		bool overwrite = false;
		bool prevTransfer = false;
		uint64_t previousID = 0;

		if (originData == transferedHash && originData != 0) {
			duplicate = true;
			overwrite = true;
		}
		if (_prevTransfers.find(transferedHash) != _prevTransfers.end()) {
			duplicate =  true;
			prevTransfer = true;
			previousID = _prevTransfers[transferedHash];
		} else {
			_prevTransfers[transferedHash] = pos;
		}
		//if (duplicate == true) {
		_out->Write(pos, (overwrite ? uint64_t(1) : uint64_t(0)), (prevTransfer ? uint64_t(1) : uint64_t(0)), previousID);
		//}
	}
	return NO_ACTION;
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