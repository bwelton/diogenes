#include "Autocorrect.h"
#include "ReadStacks.h"

FastStackTree * autocorr_GlobalStacktree;
/* Gotcha wrapper declarations */ 
gotcha_wrappee_handle_t autocorr_cuMemAllocHost_v2_handle;
gotcha_wrappee_handle_t autocorr_cuMemcpyHtoD_v2_handle;
gotcha_wrappee_handle_t autocorr_cuMemcpyDtoH_v222_handle;
gotcha_wrappee_handle_t autocorr_cuMemcpyDtoHAsync_v2_handle;
gotcha_wrappee_handle_t autocorr_cuMemcpyHtoDAsync_v2_handle;
gotcha_wrappee_handle_t autocorr_cuMemFreeHost_handle;
gotcha_wrappee_handle_t autocorr_cuMemAlloc_handle;
gotcha_wrappee_handle_t autocorr_cuMemFree22_handle;


typeof(&autocorr_cuMemAllocHost_v2) autocorr_cuMemAllocHost_v2_wrapper;
typeof(&autocorr_cuMemcpyHtoD_v2) autocorr_cuMemcpyHtoD_v2_wrapper;
typeof(&autocorr_cuMemcpyDtoH_v222) autocorr_cuMemcpyDtoH_v222_wrapper;
typeof(&autocorr_cuMemcpyDtoHAsync_v2) autocorr_cuMemcpyDtoHAsync_v2_wrapper;
typeof(&autocorr_cuMemcpyHtoDAsync_v2) autocorr_cuMemcpyHtoDAsync_v2_wrapper;
typeof(&autocorr_cuMemFreeHost) autocorr_cuMemFreeHost_wrapper;
typeof(&autocorr_cuMemAlloc) autocorr_cuMemAlloc_wrapper;
typeof(&autocorr_cuMemFree22) autocorr_cuMemFree22_wrapper;

typeof(&autocorr_synchronize_device) autocorr_ctxSynchronize_wrapper;
/*end*/
// int autocorr_cuMemcpyDtoH_v222(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount)
struct gotcha_binding_t gotcha_Binder_Funcs22[] = {{"cuMemHostAlloc",(void*)autocorr_cuMemAllocHost_v2,&autocorr_cuMemAllocHost_v2_handle},
												 {"cuMemcpyHtoD_v2",(void*)autocorr_cuMemcpyHtoD_v2,&autocorr_cuMemcpyHtoD_v2_handle},
												 {"cuMemcpyDtoH_v2",(void*)autocorr_cuMemcpyDtoH_v222,&autocorr_cuMemcpyDtoH_v222_handle},
												 {"cuMemFree_v2",(void*)autocorr_cuMemFree22,&autocorr_cuMemFree22_handle},
												 {"cuMemcpyDtoHAsync_v2",(void*)autocorr_cuMemcpyDtoHAsync_v2,&autocorr_cuMemcpyDtoHAsync_v2_handle},
												 {"cuMemcpyHtoDAsync_v2",(void*)autocorr_cuMemcpyHtoDAsync_v2,&autocorr_cuMemcpyHtoDAsync_v2_handle},
												 {"cuMemHostFree",(void*)autocorr_cuMemFreeHost,&autocorr_cuMemFreeHost_handle},
												 {"cuMemAlloc_v2",(void*)autocorr_cuMemAlloc,&autocorr_cuMemAlloc_handle},};
// {"cuMemFree_v2",(void*)autocorr_cuMemFree,&autocorr_cuMemFree_handle}
std::unordered_map<std::string, uint64_t> autocorr_GMap = {{"cuMemcpyDtoHAsync_v2",AUTOCORR_CALLID_cuMemcpyDtoHAsync_v2},
														   {"cuMemcpyHtoDAsync_v2",AUTOCORR_CALLID_cuMemcpyHtoDAsync_v2},
														   {"cuMemcpyHtoD_v2",AUTOCORR_CALLID_cuMemcpyHtoD_v2},
														   {"cuMemcpyDtoH_v2",AUTOCORR_CALLID_cuMemcpyDtoH_v2},
														   {"cuMemFree_v2",AUTOCORR_CALLID_cuMemFree_v2}};
extern "C" {
void ac_mutatee_init() {
	//PageLocker_Initalize()
	char autocorrFilename[] = AUTOCORRECT_BINARYFILE;
	autocorr_GlobalStacktree = new FastStackTree(0);
	std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> stackEntries =ReadStacksFromMutatee(autocorrFilename);
	for(auto & i : stackEntries) {
		assert(i.second[0].binaryName != NULL);
		i.second[0].libraryOffset = autocorr_GMap[std::string(i.second[0].binaryName.get())];
		autocorr_GlobalStacktree->InsertStack(i.second);
	}
	
	atexit(autocorr_mutatee_exit_handler);
	dlopen("libcudart.so",RTLD_NOW);
	fprintf(stderr,"in Warpper AC Main\n");
    void * lib_libcuda = dlopen("libcuda.so", RTLD_NOW);
	((void(*)(void))dlsym(lib_libcuda, "cuInit"))();
	autocorr_cuMemAllocHost_v2_wrapper = (typeof(&autocorr_cuMemAllocHost_v2))dlsym(lib_libcuda,"cuMemHostAlloc");
    void * lib_libc = dlopen("libc.so", RTLD_NOW);
    autocorr_cuMemcpyHtoD_v2_wrapper = (typeof(&autocorr_cuMemcpyHtoD_v2))dlsym(lib_libcuda,"cuMemcpyHtoD_v2");
    autocorr_cuMemcpyDtoH_v222_wrapper = (typeof(&autocorr_cuMemcpyDtoH_v222))dlsym(lib_libcuda,"cuMemcpyDtoH_v2");
	assert(autocorr_cuMemcpyDtoH_v222_wrapper != NULL);
	autocorr_cuMemcpyDtoHAsync_v2_wrapper = (typeof(&autocorr_cuMemcpyDtoHAsync_v2))dlsym(lib_libcuda,"cuMemcpyDtoHAsync_v2");
	autocorr_cuMemcpyHtoDAsync_v2_wrapper = (typeof(&autocorr_cuMemcpyHtoDAsync_v2))dlsym(lib_libcuda,"cuMemcpyHtoDAsync_v2");
	autocorr_cuMemFreeHost_wrapper = (typeof(&autocorr_cuMemFreeHost))dlsym(lib_libcuda,"cuMemFreeHost");
	autocorr_cuMemAlloc_wrapper = (typeof(&autocorr_cuMemAlloc))dlsym(lib_libcuda,"cuMemAlloc_v2");
	autocorr_cuMemFree22_wrapper = (typeof(&autocorr_cuMemFree22))dlsym(lib_libcuda,"cuMemFree_v2");

	autocorr_ctxSynchronize_wrapper = (typeof(&autocorr_synchronize_device))dlsym(lib_libcuda,"cuCtxSynchronize");
	assert(autocorr_cuMemFree_wrapper != NULL);
	gotcha_wrap(gotcha_Binder_Funcs22, sizeof(gotcha_Binder_Funcs22)/sizeof(struct gotcha_binding_t), "binderWrappers2");
}
}

