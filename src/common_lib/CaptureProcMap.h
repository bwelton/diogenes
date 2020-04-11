#ifndef COMMON_CAPTUREPROCMAP
#define COMMON_CAPTUREPROCMAP 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sendfile.h>
void CaptureProcMap_Write(char * filename);

#endif