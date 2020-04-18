#ifndef FPSTACKWALKER 
#define FPSTACKWALKER 1
#include <unistd.h>
#include <stdint.h>
void FPStackWalker_Init();
size_t FPStackWalker_GetStackFP(uint64_t * items, size_t maxSize);
#endif