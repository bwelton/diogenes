#include <string>
#include <dlfcn.h>
#include <stdio.h>
extern "C" {
void * __real_dlopen(const char *filename, int flags);
void *__wrap_dlopen(const char *filename, int flags) {
	if (filename != NULL) {
		void * ret;
		//std::string tmp = std::string((char *)filename);
		ret =  __real_dlopen(filename, flags);
		fprintf(stderr, "DLOPEN: %s HANDLE: %p\n", filename, ret);
		return ret;
	} else {
		return __real_dlopen(filename, flags);
	}
}
void * __real_dlsym(void * handle, const char * symbol);
void * __wrap_dlsym(void * handle, const char * symbol) {
	if (symbol != NULL) {
		fprintf(stderr, "DLSYMBOL: %s, HANDLE: %p\n", symbol, handle);
	}
	return __real_dlsym(handle, symbol);
}
}