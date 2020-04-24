#include "SyncDetector.h"
#include "CPPLocker.h"
/* Gotcha wrapper declarations */ 
gotcha_wrappee_handle_t syncdetect_cuMemAllocHost_v2_handle;
gotcha_wrappee_handle_t syncdetect_malloc_handle;
gotcha_wrappee_handle_t syncdetect_cuMemAllocManaged_handle;
gotcha_wrappee_handle_t syncdetect_cudaMallocManaged_handle;
gotcha_wrappee_handle_t syncdetect_cuMemcpyHtoD_v2_handle;
gotcha_wrappee_handle_t syncdetect_cuMemcpyDtoH_v2_handle;
gotcha_wrappee_handle_t syncdetect_free_handle;
gotcha_wrappee_handle_t syncdetect_cuMemcpyDtoHAsync_v2_handle;
gotcha_wrappee_handle_t syncdetect_cuMemcpyHtoDAsync_v2_handle;
gotcha_wrappee_handle_t syncdetect_cuMemcpyAsync_handle;
gotcha_wrappee_handle_t syncdetect_cuMemFreeHost_handle;

typeof(&syncdetect_cuMemAllocHost_v2) syncdetect_cuMemAllocHost_v2_wrapper;
typeof(&syncdetect_malloc) syncdetect_malloc_wrapper;
typeof(&syncdetect_cuMemAllocManaged) syncdetect_cuMemAllocManaged_wrapper;
typeof(&syncdetect_cuMemcpyHtoD_v2) syncdetect_cuMemcpyHtoD_v2_wrapper;
typeof(&syncdetect_cuMemcpyDtoH_v2) syncdetect_cuMemcpyDtoH_v2_wrapper;
typeof(&syncdetect_free) syncdetect_free_wrapper;
typeof(&syncdetect_cuMemcpyDtoHAsync_v2) syncdetect_cuMemcpyDtoHAsync_v2_wrapper;
typeof(&syncdetect_cuMemcpyHtoDAsync_v2) syncdetect_cuMemcpyHtoDAsync_v2_wrapper;
typeof(&syncdetect_cuMemcpyAsync) syncdetect_cuMemcpyAsync_wrapper;
typeof(&syncdetect_cuMemFreeHost) syncdetect_cuMemFreeHost_wrapper;

/*end*/

struct gotcha_binding_t gotcha_Binder_Funcs[] = {{"cuMemHostAlloc",(void*)syncdetect_cuMemAllocHost_v2,&syncdetect_cuMemAllocHost_v2_handle},
												 {"malloc",(void*)syncdetect_malloc,&syncdetect_malloc_handle},
												 {"free",(void*)syncdetect_free,&syncdetect_free_handle},
												 {"cuMemAllocManaged",(void*)syncdetect_cuMemAllocManaged,&syncdetect_cuMemAllocManaged_handle},
												 {"cuMemcpyHtoD_v2",(void*)syncdetect_cuMemcpyHtoD_v2,&syncdetect_cuMemcpyHtoD_v2_handle},
												 {"cuMemcpyDtoH_v2",(void*)syncdetect_cuMemcpyDtoH_v2,&syncdetect_cuMemcpyDtoH_v2_handle},
												 {"cuMemcpyDtoHAsync_v2",(void*)syncdetect_cuMemcpyDtoHAsync_v2,&syncdetect_cuMemcpyDtoHAsync_v2_handle},
												 {"cuMemcpyHtoDAsync_v2",(void*)syncdetect_cuMemcpyHtoDAsync_v2,&syncdetect_cuMemcpyHtoDAsync_v2_handle},
												 {"cuMemcpyAsync",(void*)syncdetect_cuMemcpyAsync,&syncdetect_cuMemcpyAsync_handle},
												 {"cuMemHostFree",(void*)syncdetect_cuMemFreeHost,&syncdetect_cuMemFreeHost_handle},
												};
void mutatee_init() {
	//PageLocker_Initalize()
	atexit(mutatee_exit_handler);

	struct sigaction act;
	memset (&act, '\0', sizeof(act));
 
	act.sa_sigaction = &syncdetect_catchSegFault;
	act.sa_flags = SA_SIGINFO;
 
	if (sigaction(SIGSEGV, &act, NULL) < 0) {
		fprintf(stderr, "%s\n", "Could not catch signal!!! exiting");
		exit(-1);
	}

	CPPLocker local_locker = CPPPageLocker_GetThreadSpecific();
	dlopen("libcudart.so",RTLD_NOW);
	fprintf(stderr,"in Warpper Main\n");
    void * lib_libcuda = dlopen("libcuda.so", RTLD_NOW);
	((void(*)(void))dlsym(lib_libcuda, "cuInit"))();
	syncdetect_cuMemAllocHost_v2_wrapper = (typeof(&syncdetect_cuMemAllocHost_v2))dlsym(lib_libcuda,"cuMemHostAlloc");
    void * lib_libc = dlopen("libc.so", RTLD_NOW);
	syncdetect_malloc_wrapper = (typeof(&syncdetect_malloc))dlsym(lib_libc,"malloc");
	syncdetect_free_wrapper = (typeof(&syncdetect_free))dlsym(lib_libc,"free");
	syncdetect_cuMemAllocManaged_wrapper = (typeof(&syncdetect_cuMemAllocManaged))dlsym(lib_libcuda,"cuMemAllocManaged");
    syncdetect_cuMemcpyHtoD_v2_wrapper = (typeof(&syncdetect_cuMemcpyHtoD_v2))dlsym(lib_libcuda,"cuMemcpyHtoD_v2");
    syncdetect_cuMemcpyDtoH_v2_wrapper = (typeof(&syncdetect_cuMemcpyDtoH_v2))dlsym(lib_libcuda,"cuMemcpyDtoH_v2");
	syncdetect_cuMemcpyDtoHAsync_v2_wrapper = (typeof(&syncdetect_cuMemcpyDtoHAsync_v2))dlsym(lib_libcuda,"cuMemcpyDtoHAsync_v2");
	syncdetect_cuMemcpyHtoDAsync_v2_wrapper = (typeof(&syncdetect_cuMemcpyHtoDAsync_v2))dlsym(lib_libcuda,"cuMemcpyHtoDAsync_v2");
	syncdetect_cuMemcpyAsync_wrapper = (typeof(&syncdetect_cuMemcpyAsync))dlsym(lib_libcuda,"cuMemcpyAsync");
	syncdetect_cuMemFreeHost_wrapper = (typeof(&syncdetect_cuMemFreeHost))dlsym(lib_libcuda,"cuMemFreeHost");
	gotcha_wrap(gotcha_Binder_Funcs, sizeof(gotcha_Binder_Funcs)/sizeof(struct gotcha_binding_t), "binderWrappers");
}


// gotcha_wrappee_handle_t syncdetect_cuMemAllocHost_v2_handle;
// gotcha_wrappee_handle_t syncdetect_malloc_handle;

// extern "C" int syncdetect_cuMemAllocHost_v2(void ** ptr, size_t size);
// extern "C" void * syncdetect_malloc(size_t size);
// //int(*syncdetect_cuMemAllocHost_v2_wrapper)(void**,size_t);
// //void*(*syncdetect_malloc_wrapper)(size_t);
// typeof(&syncdetect_cuMemAllocHost_v2) syncdetect_cuMemAllocHost_v2_wrapper;
// typeof(&syncdetect_malloc) syncdetect_malloc_wrapper;

// int syncdetect_cuMemAllocHost_v2(void ** ptr, size_t size) {
//     //return syncdetect_cuMemAllocHost_v2_wrapper;
// }
// void * syncdetect_malloc(size_t size) {
//     //return syncdetect_malloc_wrapper;
// }

// struct gotcha_binding_t gotcha_Binder_Funcs[] = {{"cuMemAllocHost_v2",(void*)syncdetect_cuMemAllocHost_v2,&syncdetect_cuMemAllocHost_v2_handle},
// 												 {"malloc",(void*)syncdetect_malloc,&syncdetect_malloc_handle}};

// void binding_initalizer(){
//     void * lib_libcuda = dlopen("libcuda.so", RTLD_LAZY);
// 	syncdetect_cuMemAllocHost_v2_wrapper = (typeof(&syncdetect_cuMemAllocHost_v2)) dlsym(lib_libcuda,"cuMemAllocHost_v2");
//     void * lib_libc = dlopen("libc.so", RTLD_LAZY);
// 	syncdetect_malloc_wrapper = (typeof(&syncdetect_malloc))dlsym(lib_libc,"malloc");
// 	gotcha_wrap(gotcha_Binder_Funcs, sizeof(gotcha_Binder_Funcs)/sizeof(struct gotcha_binding_t), "binderWrappers");
// };



// // #include <cuda.h>
// // #include <driver_types.h>
// void do_init();
// // void * malloc(size_t size) {
// // 	do_init();
// // 	puts("In malloc wrapper\n");
// // 	return valloc(size);
// // }

// int cuMemAllocHost_v2(void ** ptr, size_t size);


// gotcha_wrappee_handle_t DIOGENES_cudaFree_handle;
// gotcha_wrappee_handle_t DIOGENES_cudaMemcpy_handle;
// gotcha_wrappee_handle_t DIOGENES_libcmalloc_handle;
// gotcha_wrappee_handle_t DIOGENES_libcfree_handle;
// gotcha_wrappee_handle_t DIOGENES_cudaMemcpyAsync_handle;

// typeof(&DIOGENES_cudaMemcpy) DIOGENES_cudaMemcpy_wrapper;
// typeof(&DIOGENES_cudaFree) DIOGENES_cudaFree_wrapper;
// typeof(&DIOGENES_REC_GLIBMALLOC) DIOGENES_libcmalloc_wrapper;
// typeof(&DIOGENES_REC_GLIBFREE) DIOGENES_libcfree_wrapper;
// typeof(&DIOGENES_cudaMemcpyAsync) DIOGENES_cudaMemcpyAsync_wrapper;



// struct gotcha_binding_t gotfuncs_tbl[] = {{"cuMemAllocHost_v2", (void*)DIOGENES_cudaFree,&DIOGENES_cudaFree_handle},
// 											   {"cudaMemcpyAsync", (void*)DIOGENES_cudaMemcpyAsync,&DIOGENES_cudaMemcpyAsync_handle},
// 											   {"cudaMemcpy", (void*)DIOGENES_cudaMemcpy,&DIOGENES_cudaMemcpy_handle},
// 											   {"malloc", (void*)DIOGENES_REC_GLIBMALLOC,&DIOGENES_libcmalloc_handle},
// 											   {"free", (void *)DIOGENES_REC_GLIBFREE, &DIOGENES_libcfree_handle}};


// void *libcuda = NULL;
// //void *(*real_mmap)(void *, size_t, int, int, int, off_t);


// int cuMemHostRegister(void * ptr, size_t size, unsigned int flags);


// typeof(&cuMemHostRegister) real_cuMemHostRegister;

// // typeof(&cuMemAllocHost_v2) real_cuMemAllocHost_v2;

// //int (*real_cuMemAllocHost_v2)(void **, size_t);


// int cuMemHostRegister(void * ptr, size_t size, unsigned int flags) {
// 	if (!real_cuMemHostRegister) 
// 		do_init();
// 	return real_cuMemHostRegister(ptr, size, flags);
// }

// size_t GetPageRound(size_t size) {
// 	return size + (size % 4096);
// }

// int cuMemAllocHost_v2(void ** ptr, size_t size) {
// 	fprintf(stderr, "In cuMemAllocWrapper\n");
// 	*ptr = malloc(size);
// 	return cuMemHostRegister(ptr, size, 0);
// }


// void do_init()
// {
//   if (real_cuMemHostRegister)
//   	return;
//   real_cuMemHostRegister = (typeof(&cuMemHostRegister))0x1;
//   libcuda = dlopen("libcuda.so", RTLD_LAZY | RTLD_GLOBAL);
//   assert(libcuda != NULL);
//   real_cuMemHostRegister =(typeof(&cuMemHostRegister))dlsym(libcuda, "cuMemHostRegister");
// }
