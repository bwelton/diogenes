#ifndef COMMON_STACKWALK_HEADER
#define COMMON_STACKWALK_HEADER 1
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <unistd.h>
#include "StackTrie.h"
#include "CVector.h"
#include "CPPStackTree.h"
typedef struct StackwalkInst{
	void * (*sw_malloc_wrapper)(size_t);
	void (*sw_free_wrapper)(void *);
	uint64_t globalID;
	CPPStackTrie tree;
} StackwalkInst; 
#ifdef __cplusplus
extern "C" {
#endif
StackwalkInst * Stackwalk_Init(void * (*allocator_fun)(size_t), void (*free_fun)(void *));
uint64_t Stackwalk_GetStackID(StackwalkInst * swalk);
uint64_t Stackwalk_GetStackIDLibunwind(StackwalkInst * inst);
char * Stackwalk_PrintStack(StackwalkInst * inst, size_t * size);
uint64_t Stackwalk_GetStackIDLockUnwind(StackwalkInst * inst);
#ifdef __cplusplus
}
#endif
#endif