#include "Stackwalk.h"

StackwalkInst * Stackwalk_Init(void * (*allocator_fun)(size_t), void (*free_fun)(void *)) {
	StackwalkInst * ret = (StackwalkInst*)allocator_fun(sizeof(StackwalkInst));
	ret->sw_malloc_wrapper = allocator_fun;
	ret->sw_free_wrapper = free_fun;
	ret->globalID = 1;
	ret->tree = StackTrie_Initalize(0, NULL, allocator_fun, free_fun);
	return ret;
}

uint64_t Stackwalk_GetStackID(StackwalkInst * inst) {
	void * local_stack[100];
	uint64_t data[100];
	int ret = backtrace(local_stack, 100);
	StackTrie * tree = inst->tree;
	
	if (StackTrie_LookupStack(tree, (uint64_t*)local_stack, (void **)data, ret) == true)
		return ((uint64_t*)data)[ret - 1];
	{
		uint64_t insert[100];
		insert[ret-1] = inst->globalID;
		inst->globalID++;
		StackTrie_InsertStack(tree, (uint64_t*)local_stack, (void **)insert, ret);
		return insert[ret-1];
	}

}

char * Stackwalk_PrintStack(StackwalkInst * inst, size_t * size) {
	CVector * resultVec = CVector_Init(inst->sw_malloc_wrapper, inst->sw_free_wrapper, 100000);
	StackTrie_ConvertTreeToStackKey(inst->tree, resultVec, NULL);
	*size = 0;
	char * ptr = (char *)CVector_GetData(resultVec,size);
	ptr[*size] = '\000';
	return ptr;
}
