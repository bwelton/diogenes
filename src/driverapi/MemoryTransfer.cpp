#include "MemoryTransfer.h"
#include <cassert>

#ifdef DEBUG
#define WRITE_DEBUG(msg) \
	std::cerr << "[MemoryTransferDebug] - " << msg << std::endl;
#else
#define WRITE_DEBUG(msg)
#endif

bool CallIsTransfer(CallID call) {
	if (TransferCallIDs == NULL)
		return false;
	if (TransferCallIDs->find(call) != TransferCallIDs->end())
		return true;
	return false;	
}


bool MemoryTransfer::IsTransfer() {
	return _transfer;
}

bool MemoryTransfer::IsSupportedTransfer() {
	if (_supported == 0)
		return false;
	else if (_supported == 1)
		return true;

	CallID callIdent = _params->GetID();
	if (StandardCopies->find(callIdent) != StandardCopies->end() ||
		ArrayCopies->find(callIdent) != ArrayCopies->end()){
		_supported = 1;
		if (ArrayCopies->find(callIdent) != ArrayCopies->end())
			_arrayTransfer = true;
		return true;
	}

	_supported = 0;
	if (UnsupportedCopies->find(callIdent) != UnsupportedCopies->end()){
		return false;
	}

	std::cerr << "WARNING: We have an unknown transfer call with ID: " << _params->GetID() << " with name " << _params->GetName() << std::endl;
	return false;
}

MemoryTransfer::MemoryTransfer(Parameters *params) :
	_params(params), _arrayTransfer(false), _supported(-1), _transfer(false), _origData(0), _transferedData(0), _checkHash(true) {
	if (CallIsTransfer(params->GetID()))
		_transfer = true;
	else
		_transfer = false;
	if (_transfer == true)
		IsSupportedTransfer();
}

bool MemoryTransfer::IsCPUDest() {
	std::vector<CallID> DestIsCPU = STANDARD_COPIES_DEST_CPU
	CallID thiscall = _params->GetID();
	if (std::find(DestIsCPU.begin(), DestIsCPU.end(), thiscall) != DestIsCPU.end())
		return true;
	return false;
}

bool MemoryTransfer::IsCPUSource() {
	std::vector<CallID> GPUDest = STANDARD_COPIES_DEST_GPU
	CallID thiscall = _params->GetID();
	if (std::find(GPUDest.begin(), GPUDest.end(), thiscall) != GPUDest.end())
		return true;
	return false;
}

uint64_t MemoryTransfer::GetCPUAddress() {
	CUdeviceptr cpuAddr;
	if (_transfer == false)
		return 0;
	if (IsCPUDest()){
		_dstType = CU_MEMORYTYPE_HOST;
		_srcType = CU_MEMORYTYPE_DEVICE;
		cpuAddr = (CUdeviceptr)*((void**)_params->GetParameter(0));
	} else if (IsCPUSource()) {
		_dstType = CU_MEMORYTYPE_DEVICE;
		_srcType = CU_MEMORYTYPE_HOST;		
		cpuAddr = (CUdeviceptr)*((void**)_params->GetParameter(1));
	} else {
		return 0;
	}
	_transferSize = ((size_t*)_params->GetParameter(2))[0];
	return (uint64_t)((void*)cpuAddr);
}

uint32_t MemoryTransfer::GetOriginHash() {
	return _origData;
}

uint32_t MemoryTransfer::GetTransferHash() {
	return _transferedData;
}

size_t MemoryTransfer::GetSize() {
	return _transferSize;
}

uint32_t MemoryTransfer::GetHash(void * ptr, size_t size){
	return XXHash32::hash(ptr, size, 0);
}

int MemoryTransfer::GetStream() {
	if (_transfer == false)
		return 0;
	CallID thiscall = _params->GetID();

	switch(thiscall) {
		// Synch Transfers on Default Stream, Return 0. Double check this is a complete list.
		case ID_cuMemcpyDtoH:
		case ID_cuMemcpyHtoD:
		case ID_cuMemcpyDtoH_v2:
		case ID_cuMemcpyHtoD_v2:
		case ID_cuMemcpyDtoD:
		case ID_cuMemcpyDtoD_v2:
		case ID_cuMemcpyAtoD_v2:
		case ID_cuMemcpyAtoD:
		case ID_cuMemcpyAtoH_v2:
		case ID_cuMemcpyHtoA_v2:
		case ID_cuMemcpyAtoH:
		case ID_cuMemcpyHtoA:
		case ID_cuMemcpy:
			WRITE_DEBUG(_params->GetName() << " triggered default stream.")
			return 0;
		// Return the fourth parameter with the stream id.
		case ID_cuMemcpyHtoDAsync:
		case ID_cuMemcpyDtoHAsync:
		case ID_cuMemcpyDtoDAsync:
		case ID_cuMemcpyHtoDAsync_v2:
		case ID_cuMemcpyDtoHAsync_v2:
		case ID_cuMemcpyDtoDAsync_v2:		
			WRITE_DEBUG(_params->GetName() << " async call DtoH or HtoD with id " << ((size_t*)_params->GetParameter(3))[0])
			return int(((size_t*)_params->GetParameter(3))[0]);
		// If an array transfer is detected, retrun the 5th parameter 
		case ID_cuMemcpyAtoHAsync_v2:
		case ID_cuMemcpyHtoAAsync_v2:
		case ID_cuMemcpyAtoHAsync:
		case ID_cuMemcpyHtoAAsync:
			WRITE_DEBUG(_params->GetName() << " async call AtoH or HtoA with id " << ((size_t*)_params->GetParameter(4))[0])
			return int(((size_t*)_params->GetParameter(4))[0]);
		default:
			WRITE_DEBUG(_params->GetName() << " - Could not find any stream for this call!")
			return -1;
	}
}

uint32_t MemoryTransfer::GetHashAtLocation(void * dstPtr, size_t tSize, CUmemorytype location) {
	void * data;
	bool mallocd = false;
	if (location == CU_MEMORYTYPE_HOST) {
		data = dstPtr;
	} else if (location == CU_MEMORYTYPE_DEVICE) {
		data = malloc(tSize);
		if(Bound_cuMemcpyDtoH_v2(data,(CUdeviceptr)dstPtr, tSize) != 0){
			std::cerr << "Could not copy data off GPU for destination hash checking, exiting now" << std::endl;
			exit(-1);
		}		
		mallocd = true;
	} else {
		std::cerr << "[MemoryTransfer - GetHashAtLocation] Unknown type supplied - " << location << std::endl;
		assert(1 == 0);
	}
	uint32_t hashv = GetHash(data, tSize);
	if (mallocd == true)
		free(data);
	return hashv;
}

uint32_t MemoryTransfer::GetSourceDataArray( void * dstPtr, size_t tSize, size_t offset) {
	void * data = malloc(tSize);
	if (_dstType == CU_MEMORYTYPE_ARRAY) {
		if(Bound_cuMemcpyAtoH_v2(data, (CUarray)dstPtr, offset, tSize) != 0) {
			std::cerr << "Could not copy array data for destination hash checking, exiting now" << std::endl;
			exit(-1);			
		}
	} else {
	 	std::cerr << "Wrong processing routine has been used for this transfer with ID: " << _params->GetID() <<  " and name " << _params->GetName() << std::endl;
	 	exit(-1);		
	}
	uint32_t hashv = GetHash(data, tSize);
	free(data);
	return hashv;
}

void MemoryTransfer::SetHashGeneration(bool s) {
	_checkHash = s;

}

void MemoryTransfer::Synchronize() {
	if (Bound_cuCtxSynchronize() != 0)
		std::cerr << "[MemoryTransfer.cpp] Could Not synchronize the device!" << std::endl;
}

void MemoryTransfer::PrecallHandleStandard() {
	// This function is designed to get data from the 
	if (_origData != 0)
		return;
	WRITE_DEBUG ( "In Precall Handler for Standard Copies")

	std::vector<CallID> DestIsCPU = STANDARD_COPIES_DEST_CPU
	std::vector<CallID> DestIsGPU = STANDARD_COPIES_DEST_GPU
	std::vector<CallID> DeviceToDevice = STANDARD_COPIES_HOSTDEST_DEVICE
	std::vector<CallID> UnknownIDs = STANDARD_COPIES_UNKNOWN_PTRS
	CallID thiscall = _params->GetID();

	// setup source and destination types
	if (std::find(DestIsCPU.begin(), DestIsCPU.end(), thiscall) != DestIsCPU.end()) {
		WRITE_DEBUG(_params->GetName() << " is a GPU -> CPU transfer")
		_dstType = CU_MEMORYTYPE_HOST;
		_srcType = CU_MEMORYTYPE_DEVICE;  
	} else if (std::find(DestIsGPU.begin(), DestIsGPU.end(), thiscall) != DestIsGPU.end()){
		WRITE_DEBUG(_params->GetName() << " is a CPU -> GPU transfer")
		_dstType = CU_MEMORYTYPE_DEVICE;
		_srcType = CU_MEMORYTYPE_HOST;
	} else if (std::find(DeviceToDevice.begin(), DeviceToDevice.end(), thiscall) != DeviceToDevice.end()) {
		WRITE_DEBUG(_params->GetName() << " is a GPU -> GPU transfer")
		_dstType = CU_MEMORYTYPE_DEVICE;
		_srcType = CU_MEMORYTYPE_DEVICE;
	} else if (std::find(UnknownIDs.begin(), UnknownIDs.end(), thiscall) != UnknownIDs.end()) {
		WRITE_DEBUG(_params->GetName() << " is a Unified Memory Address transfer")
		if (Bound_cuPointerGetAttribute((void*)&_srcType, CU_POINTER_ATTRIBUTE_MEMORY_TYPE, (CUdeviceptr)*((void**)_params->GetParameter(1))) != 0){
			std::cerr << "Could not get source pointer attrs for call - " << _params->GetName() << std::endl;
			assert (1 == 0);
		}
		if (Bound_cuPointerGetAttribute((void*)&_dstType, CU_POINTER_ATTRIBUTE_MEMORY_TYPE, (CUdeviceptr)*((void**)_params->GetParameter(0))) != 0) {
			std::cerr << "Could not get destination pointer attrs for call - " << _params->GetName() << std::endl;
			assert (1 == 0);
		}
	} else {
		std::cerr << "[MEMORY_COPY - PRECALL STANDARD] Unknown transfer ID has been supplied of " << _params->GetName() << std::endl;
		assert(1 == 0);
	}
	_transferSize = ((size_t*)_params->GetParameter(2))[0];
	if (_checkHash == true)
		_origData = GetHashAtLocation(*((void**)_params->GetParameter(0)), _transferSize, _dstType);
	else 
		_origData = 0; 	
	WRITE_DEBUG(_params->GetName() << " has a size of " << _transferSize << " and is overwriting data with a hash of " << std::hex << _origData << std::dec )
}

void MemoryTransfer::PrecallHandleArray() {
	if (_origData != 0)
		return;
	std::vector<CallID> singlePtrToArray = ARRAY_COPIES_SINGLEPTR_TO_ARRAY
	std::vector<CallID> arrayToSinglePtr = ARRAY_COPIES_ARRAY_TO_SINGLEPTR
	CallID thiscall = _params->GetID();

	if (std::find(singlePtrToArray.begin(), singlePtrToArray.end(), thiscall) != singlePtrToArray.end()) {
		switch (thiscall) {
			case ID_cuMemcpyDtoA_v2:
			case ID_cuMemcpyDtoA:
					_srcType = CU_MEMORYTYPE_DEVICE;
					_dstType = CU_MEMORYTYPE_ARRAY;
					break;
			case ID_cuMemcpyHtoA_v2:
			case ID_cuMemcpyHtoA:
			case ID_cuMemcpyHtoAAsync:
			case ID_cuMemcpyHtoAAsync_v2:
					_srcType = CU_MEMORYTYPE_HOST;
					_dstType = CU_MEMORYTYPE_ARRAY;
					break;
			default:
					std::cerr << "[MemoryTransfer - Precall Array] UNKNOWN TYPE OF TRANSFER SINGLE PTR ARRAY\n";
					assert(1 == 0);
		}
	} else if (std::find(arrayToSinglePtr.begin(), arrayToSinglePtr.end(), thiscall) != arrayToSinglePtr.end()) {
		switch(thiscall) {
			case ID_cuMemcpyAtoH:
			case ID_cuMemcpyAtoHAsync_v2:
			case ID_cuMemcpyAtoH_v2:
			case ID_cuMemcpyAtoHAsync:
				_srcType = CU_MEMORYTYPE_ARRAY;
				_dstType = CU_MEMORYTYPE_HOST;
				break;
			case ID_cuMemcpyAtoD_v2:
			case ID_cuMemcpyAtoD:
				_srcType = CU_MEMORYTYPE_ARRAY;
				_dstType = CU_MEMORYTYPE_DEVICE;
				break;
			default:
				std::cerr << "[MemoryTransfer - Precall Array] UNKNOWN TYPE OF TRANSFER ARRAY TO SINGLE PTR\n";
				assert(1 == 0);
		}
	} else {
		std::cerr << "UNKNOWN TYPE OF ARRAY COPY" << std::endl;
		assert (1 == 0);
	}
	if (_dstType == CU_MEMORYTYPE_HOST || _dstType == CU_MEMORYTYPE_DEVICE) {
		_transferSize = ((size_t*)_params->GetParameter(3))[0];
		_origData = GetHashAtLocation(*((void**)_params->GetParameter(0)), _transferSize, _dstType);
	} else if (_dstType == CU_MEMORYTYPE_ARRAY) {
		_transferSize = ((size_t*)_params->GetParameter(3))[0];
		_origData = GetSourceDataArray(*((void**)_params->GetParameter(0)), _transferSize, ((size_t*)_params->GetParameter(1))[0]);
	}
}

void MemoryTransfer::PostcallHandleArray() {
	if (_transferedData != 0)
		return;
	if (_srcType == CU_MEMORYTYPE_HOST || _srcType == CU_MEMORYTYPE_DEVICE) {
		_transferSize = ((size_t*)_params->GetParameter(3))[0];
		_transferedData = GetHashAtLocation(*((void**)_params->GetParameter(2)), _transferSize, _srcType);
	} else if (_srcType == CU_MEMORYTYPE_ARRAY) {
		_transferSize = ((size_t*)_params->GetParameter(3))[0];
		// Grab it at the destination instead of the source....
		_transferedData = GetHashAtLocation(*((void**)_params->GetParameter(0)), _transferSize, _dstType);
	}	
}

void MemoryTransfer::PostcallHandleStandard() {
	if (_transferedData != 0)
		return;
	if(_srcType == CU_MEMORYTYPE_DEVICE){
		_transferedData = GetHashAtLocation(*((void**)_params->GetParameter(0)), _transferSize, _dstType);
	}
	else{
		_transferedData = GetHashAtLocation(*((void**)_params->GetParameter(1)), _transferSize, _srcType);
	}

	WRITE_DEBUG("Hash of the data being transferred was" << std::hex << _transferedData << std::dec)
}

// Perform the pretransfer operations to get hash of dest/source.
void MemoryTransfer::PreTransfer() {
	if (_supported == 0)
		return;

	WRITE_DEBUG("Precall transfer for call " << _params->GetName())

	if (_arrayTransfer == true){
		PrecallHandleArray();
	}
	else if (_arrayTransfer == false){
		PrecallHandleStandard();
	}
}

void MemoryTransfer::PostTransfer() {
	if (_supported == 0)
		return;
	Bound_cuCtxSynchronize();
	if (_arrayTransfer == true)
		PostcallHandleArray();
	else if (_arrayTransfer == false)
		PostcallHandleStandard();
}
