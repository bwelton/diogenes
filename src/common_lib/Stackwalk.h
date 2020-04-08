#pragma ONCE
#include "StackTrie.h"
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <unistd.h>
typedef struct StackwalkInst{
	void * (*sw_mallow_wrapper)(size_t);
	void (*sw_free_wrapper)(void *);
	uint64_t globalID;
	StackTrie * tree;
} StackwalkInst; 

StackwalkInst * Stackwalk_Init(void * (*allocator_fun)(size_t), void (*free_fun)(void *));
uint64_t Stackwalk_GetStackID(StackwalkInst * swalk);

