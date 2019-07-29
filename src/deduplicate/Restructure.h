#include <cuda_runtime_api.h>
#include <cuda.h>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

typedef boost::function<void(void *, void *, size_t, cudaMemcpyKind, cudaStream_t)> TransferCall;
typedef std::shared_ptr<TransferBase> TransferPtr;
enum CALL_TYPE {
	SYNC = 0,
	ASYNC = 1
};

class TransferBase {
protected:
	void * _CPUPtr;
	void * _GPUPtr;
	size_t _size;
	cudaStream_t _stream;
	cudaMemcpyKind _kind;
	CALL_TYPE _sync;
	std::string _callName;
	TransferCall _call;
	int _ret; 
public:
	TransferBase(TransferCall call, void * CPUPtr, void * GPUPtr, size_t size, 
		CALL_TYPE sync, cudaMemcpyKind kind, cudaStream_t _stream = 0);
	int PerformTransfer();
	virtual uint32_t Hash() = 0;
	size_t GetSize();
	uint32_t GetHash();
};

class HashData : public TransferBase {
protected:
	uint32_t _hash;
public: 
	uint32_t Hash();
};


class InstrumentBase {
public: 
	virtual int PerformAction(TransferPtr t) = 0;
};

class TransferTimeline : public InstrumentBase {
public:
	TransferTimeline(bool enabled = false, FILE * file = stderr);
	int PerformAction(TransferPtr t);
}

class DuplicateTracker : public InstrumentBase {
public:
	DuplicateTracker(bool enabled = false, FILE * file = stderr);
	int PerformAction(TransferPtr t);
}


class InstrimentFactory {


};