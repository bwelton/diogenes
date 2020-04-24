#include "SyncDetector.h"

/* Gotcha wrapper declarations */ 
gotcha_wrappee_handle_t memgraph_cuMemAllocHost_v2_handle;
gotcha_wrappee_handle_t memgraph_malloc_handle;
gotcha_wrappee_handle_t memgraph_cuMemAllocManaged_handle;
gotcha_wrappee_handle_t memgraph_cudaMallocManaged_handle;
gotcha_wrappee_handle_t memgraph_cuMemcpyHtoD_v2_handle;
gotcha_wrappee_handle_t memgraph_cuMemcpyDtoH_v2_handle;
gotcha_wrappee_handle_t memgraph_free_handle;
gotcha_wrappee_handle_t memgraph_cuMemcpyDtoHAsync_v2_handle;
gotcha_wrappee_handle_t memgraph_cuMemcpyHtoDAsync_v2_handle;
gotcha_wrappee_handle_t memgraph_cuMemcpyAsync_handle;
gotcha_wrappee_handle_t memgraph_cuMemFreeHost_handle;

typeof(&memgraph_cuMemAllocHost_v2) memgraph_cuMemAllocHost_v2_wrapper;
typeof(&memgraph_malloc) memgraph_malloc_wrapper;
typeof(&memgraph_cuMemAllocManaged) memgraph_cuMemAllocManaged_wrapper;
typeof(&memgraph_cuMemcpyHtoD_v2) memgraph_cuMemcpyHtoD_v2_wrapper;
typeof(&memgraph_cuMemcpyDtoH_v2) memgraph_cuMemcpyDtoH_v2_wrapper;
typeof(&memgraph_free) memgraph_free_wrapper;
typeof(&memgraph_cuMemcpyDtoHAsync_v2) memgraph_cuMemcpyDtoHAsync_v2_wrapper;
typeof(&memgraph_cuMemcpyHtoDAsync_v2) memgraph_cuMemcpyHtoDAsync_v2_wrapper;
typeof(&memgraph_cuMemcpyAsync) memgraph_cuMemcpyAsync_wrapper;
typeof(&memgraph_cuMemFreeHost) memgraph_cuMemFreeHost_wrapper;

/*end*/

struct gotcha_binding_t gotcha_Binder_Funcs[] = {{"cuMemHostAlloc",(void*)memgraph_cuMemAllocHost_v2,&memgraph_cuMemAllocHost_v2_handle},
												 {"malloc",(void*)memgraph_malloc,&memgraph_malloc_handle},
												 {"free",(void*)memgraph_free,&memgraph_free_handle},
												 {"cuMemAllocManaged",(void*)memgraph_cuMemAllocManaged,&memgraph_cuMemAllocManaged_handle},
												 {"cuMemcpyHtoD_v2",(void*)memgraph_cuMemcpyHtoD_v2,&memgraph_cuMemcpyHtoD_v2_handle},
												 {"cuMemcpyDtoH_v2",(void*)memgraph_cuMemcpyDtoH_v2,&memgraph_cuMemcpyDtoH_v2_handle},
												 {"cuMemcpyDtoHAsync_v2",(void*)memgraph_cuMemcpyDtoHAsync_v2,&memgraph_cuMemcpyDtoHAsync_v2_handle},
												 {"cuMemcpyHtoDAsync_v2",(void*)memgraph_cuMemcpyHtoDAsync_v2,&memgraph_cuMemcpyHtoDAsync_v2_handle},
												 {"cuMemcpyAsync",(void*)memgraph_cuMemcpyAsync,&memgraph_cuMemcpyAsync_handle},
												 {"cuMemHostFree",(void*)memgraph_cuMemFreeHost,&memgraph_cuMemFreeHost_handle},
												};
void mutatee_init() {
	//PageLocker_Initalize()
	atexit(mutatee_exit_handler);
	dlopen("libcudart.so",RTLD_NOW);
	fprintf(stderr,"in Warpper Main\n");
    void * lib_libcuda = dlopen("libcuda.so", RTLD_NOW);
	((void(*)(void))dlsym(lib_libcuda, "cuInit"))();
	memgraph_cuMemAllocHost_v2_wrapper = (typeof(&memgraph_cuMemAllocHost_v2))dlsym(lib_libcuda,"cuMemHostAlloc");
    void * lib_libc = dlopen("libc.so", RTLD_NOW);
	memgraph_malloc_wrapper = (typeof(&memgraph_malloc))dlsym(lib_libc,"malloc");
	memgraph_free_wrapper = (typeof(&memgraph_free))dlsym(lib_libc,"free");
	memgraph_cuMemAllocManaged_wrapper = (typeof(&memgraph_cuMemAllocManaged))dlsym(lib_libcuda,"cuMemAllocManaged");
    memgraph_cuMemcpyHtoD_v2_wrapper = (typeof(&memgraph_cuMemcpyHtoD_v2))dlsym(lib_libcuda,"cuMemcpyHtoD_v2");
    memgraph_cuMemcpyDtoH_v2_wrapper = (typeof(&memgraph_cuMemcpyDtoH_v2))dlsym(lib_libcuda,"cuMemcpyDtoH_v2");
	memgraph_cuMemcpyDtoHAsync_v2_wrapper = (typeof(&memgraph_cuMemcpyDtoHAsync_v2))dlsym(lib_libcuda,"cuMemcpyDtoHAsync_v2");
	memgraph_cuMemcpyHtoDAsync_v2_wrapper = (typeof(&memgraph_cuMemcpyHtoDAsync_v2))dlsym(lib_libcuda,"cuMemcpyHtoDAsync_v2");
	memgraph_cuMemcpyAsync_wrapper = (typeof(&memgraph_cuMemcpyAsync))dlsym(lib_libcuda,"cuMemcpyAsync");
	memgraph_cuMemFreeHost_wrapper = (typeof(&memgraph_cuMemFreeHost))dlsym(lib_libcuda,"cuMemFreeHost");
	gotcha_wrap(gotcha_Binder_Funcs, sizeof(gotcha_Binder_Funcs)/sizeof(struct gotcha_binding_t), "binderWrappers");
}
