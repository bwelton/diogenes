#include "TransferBase.h"
TransferBase::TransferBase(TransferCall call, void * CPUPtr, void * GPUPtr, size_t size, 
		CALL_TYPE sync, cudaMemcpyKind kind, cudaStream_t stream) {
	_call = call;
	_CPUPtr = CPUPtr;
	_GPUPtr = GPUPtr;
	_size = size;
	_sync = sync;
	_kind = kind;
	_stream = stream;
	_transferPerformed = false;
	_syncnd = false;
	_globalId = 0;
	// _synchronizationDelay = 0.0;
}

uint64_t TransferBase::GetID() {
	return _globalId;
}

void TransferBase::SetID(uint64_t i) {
	_globalId = i;
}

cudaStream_t TransferBase::GetStream(){
	return _stream;
}

void TransferBase::PerformTransfer() {
	if(_transferPerformed == false){
		_ret = _call(_GPUPtr, _CPUPtr, (void *) _size, (void *) _kind, (void *) _stream);
		std::cerr << "Performed Transfer"<<std::endl;
		_transferPerformed = true;
	}
}
uint32_t TransferBase::GetHash() {
	if (_kind == cudaMemcpyDeviceToHost) {
		if (_transferPerformed == false)
			PerformTransfer();
		if (_sync == ASYNC && _syncnd == false) { 
			CheckGPUResult(cudaDeviceSynchronize());
			_syncnd = true;
		}
	}
	if (_kind == cudaMemcpyDeviceToDevice || _kind == cudaMemcpyHostToHost)
		return 0;
	return Hash();
}

size_t TransferBase::GetSize() {
	return _size;
}

int TransferBase::GetReturn() {
	if (_transferPerformed == false) {
		PerformTransfer();
	}
	return _ret;
}

std::string & TransferBase::GetStringTransferKind() {
	return _kindMapping[_kind];
}

TransferBase::~TransferBase() {
	if (_transferPerformed == false) {
		PerformTransfer();
		_transferPerformed = true;
	}
}

void * TransferBase::GetCPUPtr() {
	return _CPUPtr;
}

std::shared_ptr<void> TransferBase::GetDestinationData() {
	std::shared_ptr<void> ret(malloc(_size), free);
	if (_kind == cudaMemcpyDeviceToHost || _kind == cudaMemcpyHostToHost){
		std::memcpy(ret.get(), _CPUPtr, _size);
		return ret;
	}
	if (_kind == cudaMemcpyHostToDevice){
		// Pull data from GPU destintation
		CheckGPUResult((cudaError_t)_transferCalls[_kindMapping[cudaMemcpyDeviceToHost]](_GPUPtr, ret.get(), (void *) _size, (void *) _kind, (void *) _stream));
		return ret;
	}
	assert("WE SHOULD NEVER BE HERE" == 0);
	return NULL;
}