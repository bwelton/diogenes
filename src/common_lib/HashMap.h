#pragma ONCE
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
#include "CVector.h"
typedef struct KeyValuePair KeyValuePair;
typedef struct HashMap HashMap;
struct KeyValuePair {
	uint64_t key;
	void * data;
	KeyValuePair * next;
};

struct HashMap {
	uint64_t bucketCount;
	uint64_t used;
	KeyValuePair ** buckets;
	KeyValuePair ** emptyPool;
	uint64_t emptyPos;
	uint64_t emptyMaxSize;

	KeyValuePair ** allocedBuckets;
	uint64_t allocedBucketCount;
	uint64_t allocedBucketSize;

	void * (*hashmap_allocator)(size_t);
	void (*hashmap_free)(void *);

	uint64_t elementCount;

};


HashMap* HashMap_Initalize(void * (*allocator_fun)(size_t), void (*free_fun)(void *));
bool HashMap_FindValue(HashMap * map, uint64_t key, void ** data);
void HashMap_EraseElement(HashMap * map, uint64_t key);
void HashMap_InsertElement(HashMap * map, uint64_t key, void * data);
void HashMap_DebugPrintBuckets(HashMap * map);
KeyValuePair ** HashMap_DumpElements(HashMap * map, size_t * size);
void * KeyValuePair_GetData(KeyValuePair * pair);
uint64_t KeyValuePair_GetKey(KeyValuePair * pair);