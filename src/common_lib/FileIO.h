#ifndef COMMON_FILEIO
#define COMMON_FILEIO 1
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#ifdef __cplusplus
extern "C" {
#endif
char * FileIO_ReadFile(void * (*allocator_fun)(size_t),char * filename, uint64_t * size);
int FileIO_CheckDebug();
#ifdef __cplusplus
}
#endif
#endif