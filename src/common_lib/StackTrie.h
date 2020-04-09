#ifndef COMMON_STACKTRIE_HEADER
#define COMMON_STACKTRIE_HEADER 1
#include <pthread.h>
#include "CVector.h"
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
void StackTrie_ConvertTreeToStackKey(StackTrie * tree, CVector * ret, CVector * stack);
bool StackTrie_InsertStack(StackTrie * tree, uint64_t * elementArray, void ** dataArray, uint64_t count);
bool StackTrie_LookupStack(StackTrie * tree, uint64_t * elementArray, void ** returnData, uint64_t count);
StackTrie * StackTrie_GetThreadSpecific(void * (*allocator_fun)(size_t), void (*free_fun)(void *));
#endif