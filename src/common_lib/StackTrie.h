#pragma ONCE
#include <pthread.h>
#include "HashMap.h"

typedef struct StackTrie StackTrie;

struct StackTrie {
	uint64_t key;
	void * data;
	HashMap * children;
	void * (*stacktrie_allocator)(size_t);
	void (*stacktrie_free)(void *);
};

StackTrie * StackTrie_Initalize(uint64_t key, void * data, void * (*allocator_fun)(size_t), void (*free_fun)(void *));
bool StackTrie_InsertStack(StackTrie * tree, uint64_t * elementArray, void ** dataArray, uint64_t count);
bool StackTrie_LookupStack(StackTrie * tree, uint64_t * elementArray, void ** returnData, uint64_t count);
StackTrie * StackTrie_GetThreadSpecific(void * (*allocator_fun)(size_t), void (*free_fun)(void *));