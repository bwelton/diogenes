#include <cuda_runtime_api.h>
#include <cuda.h>

#define CheckGPUResult(ans) { gpuAssertion((ans), __FILE__, __LINE__); }
inline void gpuAssertion(cudaError_t ecode, const char *file, int line)
{
   if (ecode != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(ecode), file, line);
      exit(ecode);
   }
}


// Create random data
void * GenRandom(size_t bcount) {
	void * d = (void *) malloc(bcount);
	for (int x = 0; x < bcount; x++){
		((char *)d)[x] = (char)(rand() % 256);
	}
	return d;
}

int main() {
	size_t bsize = 4096 * sizeof(double);
	double * tmp = (double *)GenRandom(bsize);
	double * dev;
	cudaStream_t _stream;

	CheckGPUResult(cudaMalloc((void **)&dev, bsize));
	CheckGPUResult(cudaStreamCreate(&_stream));

	// Each of these copies shows a DeviceToHost Transfer followed by a HostToDevice Transfer
	CheckGPUResult(cudaMemcpyAsync(dev, tmp, bsize, cudaMemcpyHostToDevice, _stream));
	CheckGPUResult(cudaMemcpyAsync(dev, tmp, bsize, cudaMemcpyHostToDevice, _stream));
	CheckGPUResult(cudaMemcpyAsync(dev, tmp, bsize, cudaMemcpyHostToDevice, _stream));
	CheckGPUResult(cudaMemcpyAsync(dev, tmp, bsize, cudaMemcpyHostToDevice, _stream));

	CheckGPUResult(cudaDeviceSynchronize());
	CheckGPUResult(cudaFree(dev));
	free(tmp);
}