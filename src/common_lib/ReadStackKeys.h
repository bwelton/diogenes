#ifndef COMMON_READSTACKKEYS 
#define COMMON_READSTACKKEYS 1
#include "CVector.h"
#include "HashMap.h"
#include <string.h>
typedef struct StackKeyEntry {
	char * libname;
	uint64_t addr;
} StackKeyEntry;
#ifdef __cplusplus
extern "C" {
#endif
HashMap * ReadStackKeys(void * (*allocator_fun)(size_t), void (*free_fun)(void *), char * inputData, size_t size);
uint64_t * ReadStackKeys_GetIds(HashMap * map, uint64_t * size);
StackKeyEntry * ReadStackKeys_GetElementAt(HashMap * map, uint64_t i, uint64_t * size);
#ifdef __cplusplus
}
#endif
#endif