#include "HashData.h"
HashData::HashData(TransferCall call, void * CPUPtr, void * GPUPtr, size_t size, 
		CALL_TYPE sync, cudaMemcpyKind kind, cudaStream_t stream) : TransferBase(call, CPUPtr, GPUPtr, size, sync, kind, stream) {
	_hash = 0;
}

HashData::~HashData() {

}

uint32_t HashData::HashPtr(std::shared_ptr<void> ptr, size_t size) {
	return XXHash32::hash(ptr.get(), size, 0);
}

uint32_t HashData::HashPtr(void * ptr, size_t size) {
	return XXHash32::hash(ptr, size, 0);
}
uint32_t HashData::Hash() {
	if (_hash == 0)
		_hash = XXHash32::hash(_CPUPtr, _size, 0);
	return _hash;
}