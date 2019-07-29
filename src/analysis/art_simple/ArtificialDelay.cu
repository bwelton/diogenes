#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define ALLOC_SIZE 1024000
#define CUDA_SUCCESS cudaSuccess 

int GetCudaFrequency() {
  cudaDeviceProp prop;
  cudaGetDeviceProperties(&prop, 0);
  return prop.clockRate;
}

__global__ void SpinForever() {
  int i = 0;
  while (i == 0) {}
}

// Spin in the GPU for a certain delay, siAAmulates load to induce synchronization delay.
__global__ void LaunchSpinForTime(uint64_t delay) {
    uint64_t start_clock = clock64();
    uint64_t clock_offset = 0;
    while (clock_offset < delay)
    {
        clock_offset = clock64() - start_clock;
        //printf("Clock Offset: %llu\n", clock_offset);
    }
}


int ExperimentNoUse(uint64_t cpuDelay) {
  // 1.18 is a constant factor to delay to the exact amount of CPU Delay
  uint64_t gpuDelay = ((uint64_t(cpuDelay) * uint64_t(GetCudaFrequency() * 1000)) * 1.18) * 1.20;
  fprintf(stderr, "GPU will delay for - %llu clock cycles (%llu seconds)\n",gpuDelay, uint64_t(cpuDelay * 1.20));
  //uint64_t *x, *d_x, *d_y;
  // Launch GPU Kernel
  LaunchSpinForTime<<<1,1>>>(gpuDelay);
  //LaunchSpinOnMem<<<1,1>>>(flags);
  // Immediately sleep to simulate CPU workload
  sleep(cpuDelay);
  fprintf(stderr, "Left CPU Work, waiting on GPU synchronization\n");

  // Unnnecessary Synchronization that should take between 10-20% of execution time
  struct timeval t0,t1;
  gettimeofday(&t0, 0);
  cudaDeviceSynchronize();
  gettimeofday(&t1, 0);
  long long unsigned elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
  fprintf(stderr, "GPU synchronization complete - Actual Delay %llu\n", elapsed/1000000);
  sleep(15);
  return 0;
}

int myexperiment() {
  uint64_t *d_x;
  if (CUDA_SUCCESS != cudaMalloc(&d_x, 100))
   fprintf(stderr, "ERROR\n"); 
  for (int i = 0; i < 2; i++)
    ExperimentNoUse(uint64_t(60));
  return 0;
}

int main() {
    return myexperiment();
}
