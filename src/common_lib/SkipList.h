#ifndef COMMON_SKIPLIST_HEADER
#define COMMON_SKIPLIST_HEADER 1
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdint.h> 
#include <stdbool.h>
typedef struct SkipListNode SkipListNode;
typedef struct SkipListHandle SkipListHandle;
struct SkipListNode {
	uint64_t value;
	size_t size;
	SkipListNode * next;
	SkipListNode * levelDown;
};

struct SkipListHandle {
	SkipListNode * head;
	SkipListNode ** empty;
	size_t empty_count;
};

SkipListHandle * SkipListHandle_GetThreadSpecific();
uint64_t SkipListHandle_FindNearestElement(uint64_t item, size_t * size);
void SkipListHandle_AddElement(uint64_t item, size_t size);
void SkipListHandle_RemoveElement(uint64_t item);
void SkipListHandle_DebugPrint();
#endif