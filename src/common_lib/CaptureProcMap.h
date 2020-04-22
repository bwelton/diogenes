#ifndef COMMON_CAPTUREPROCMAP
#define COMMON_CAPTUREPROCMAP 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sendfile.h>
#ifdef __cplusplus
extern "C" {
#endif
void CaptureProcMap_Write(char * filename);
#ifdef __cplusplus
}
#endif
#endif