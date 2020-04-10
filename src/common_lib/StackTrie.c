#include "StackTrie.h"

void StackTrie_ConvertTreeToStackKey(StackTrie * tree, CVector * ret, CVector * stack) {
	bool first = false;
	if (stack == NULL) {
		stack =  CVector_Init(tree->stacktrie_allocator, tree->stacktrie_free, 19*100);
		first = true;
	}
	size_t elementWritten = 0;
	if (first == false){
		char tmp[100];
		elementWritten = snprintf(tmp, 100, "VIRTADDR@%llx,", tree->key);
		CVector_Append(stack, tmp, elementWritten);
	}
	if (tree->data != NULL){
		char tmp[100];
		size_t written = snprintf(tmp, 100, "%llu$", (uint64_t)tree->data);
		CVector_Append(ret, tmp, written);
		void * data = CVector_GetData(stack, &written);
		CVector_Append(ret, data, written-1);
		tmp[0] = '\000';
		written = snprintf(tmp, 100, "\n");
		CVector_Append(ret, tmp, written);
	}
	size_t childSize = 0;
	KeyValuePair ** children = HashMap_DumpElements(tree->children, &childSize);
	for (size_t i = 0; i < childSize; i++) {
		StackTrie_ConvertTreeToStackKey((StackTrie*)KeyValuePair_GetData(children[i]),ret,stack);
	}
	CVector_EraseElement(stack, elementWritten);
}


StackTrie * StackTrie_Initalize(uint64_t key, void * data, void * (*allocator_fun)(size_t), void (*free_fun)(void *)) {
	StackTrie * ret = (StackTrie*)allocator_fun(sizeof(StackTrie));
	ret->stacktrie_allocator = allocator_fun;
	ret->stacktrie_free = free_fun;
	ret->key = key;
	ret->data = data;
	ret->children = HashMap_Initalize(allocator_fun,free_fun);
	return ret;
}

bool StackTrie_InsertStack(StackTrie * tree, uint64_t * elementArray, void ** dataArray, uint64_t count) {
	StackTrie * cur = tree;
	bool allocatedLast = false;
	uint64_t pos = 0;
	while(pos < count){
		void * dataElement = NULL;
		void * value = NULL;
		allocatedLast = false;
		
		if (dataArray != NULL)
			if (dataArray[pos] != NULL)
				dataElement = dataArray[pos];

		if (HashMap_FindValue(cur->children, elementArray[pos], &value) == false) {
			StackTrie * newElement = StackTrie_Initalize(elementArray[pos], dataElement, cur->stacktrie_allocator, cur->stacktrie_free);
			HashMap_InsertElement(cur->children, elementArray[pos], (void*)newElement);
			value = (void*)newElement;
			allocatedLast = true;
		}
		cur = (StackTrie*)value;
		if(dataElement != NULL && cur->data == NULL){
			cur->data = dataElement;
		}
		pos++;
	}
	return allocatedLast;
}

bool StackTrie_LookupStack(StackTrie * tree, uint64_t * elementArray, void ** returnData, uint64_t count) {
	StackTrie * cur = tree;
	uint64_t pos = 0;
	while (pos < count) {
		void * value = NULL;
		if (HashMap_FindValue(cur->children, elementArray[pos], &value) == false){
			return false;
		}
		cur = (StackTrie*)value;
		if (returnData != NULL) {
			returnData[pos] = cur->data;
		}
		pos++;
	}
	return true;
}

// For thread specific support

static pthread_key_t StackTrie_key;
static pthread_once_t StackTrie_key_once = PTHREAD_ONCE_INIT;

static void StackTrie_make_key() {
    (void) pthread_key_create(&StackTrie_key, NULL);
}
StackTrie * StackTrie_GetThreadSpecific(void * (*allocator_fun)(size_t), void (*free_fun)(void *)) {
	pthread_once(&StackTrie_key_once, StackTrie_make_key);
	void *ptr;
	if ((ptr = pthread_getspecific(StackTrie_key)) == NULL) {
		ptr = (void*)StackTrie_Initalize(0,NULL, allocator_fun, free_fun);
		//SkipListHandle_Initalize((StackTrie_**)&ptr);
		pthread_setspecific(StackTrie_key, ptr);
	}
	return (StackTrie *)ptr;
}