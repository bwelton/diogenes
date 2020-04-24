#include "Stackwalk.h"
#include "FPStackWalker.h"

pthread_mutex_t lock; 

uint64_t Stackwalk_GetStackID_GNUBtrace(StackwalkInst * inst) {
	// void * local_stack[100];
	// uint64_t data[100];
	// int ret = backtrace(local_stack, 100);
	// StackTrie * tree = inst->tree;
	
	// if (StackTrie_LookupStack(tree, (uint64_t*)local_stack, (void **)data, ret) == true)
	// 	return ((uint64_t*)data)[ret - 1];
	// {
	// 	uint64_t insert[100];
	// 	insert[ret-1] = inst->globalID;
	// 	inst->globalID++;
	// 	StackTrie_InsertStack(tree, (uint64_t*)local_stack, (void **)insert, ret);
	// 	return insert[ret-1];
	// }

}

StackwalkInst * Stackwalk_Init(void * (*allocator_fun)(size_t), void (*free_fun)(void *)) {
	StackwalkInst * ret = (StackwalkInst*)allocator_fun(sizeof(StackwalkInst));
	ret->sw_malloc_wrapper = allocator_fun;
	ret->sw_free_wrapper = free_fun;
	ret->globalID = 1;
	ret->tree = CPPStackTrie_Initalize();//StackTrie_Initalize(0, NULL, allocator_fun, free_fun);
	return ret;
}

uint64_t Stackwalk_GetStackID_FPStackWalker(StackwalkInst * inst) {
	uint64_t instPointerAddr[100];
	uint64_t data = 0;
	//puts("Getting Stackwalk...");
	size_t ret = FPStackWalker_GetStackFP(instPointerAddr, 100);
	//puts("Got Stackwalk...");
	if (ret == 0){
		fprintf(stderr, "No stack obtained\n");
		return ret;
	}
	// for (size_t i = 0; i < ret; i++) {
	// 	fprintf(stderr,"RA = %"PRIx64"\n", instPointerAddr[i]);
	// }
	//fprintf(stderr,"EndStack\n");
	CPPStackTrie tree = inst->tree;
	if (CPPStackTrie_LookupStack(tree, instPointerAddr, &data, ret) == true){
		//fprintf(stderr, "Found previous match!\n");
		return data;
	}
	else
	{
		data = inst->globalID;
		inst->globalID++;
		CPPStackTrie_InsertStack(tree, instPointerAddr, &data, ret);
		return data;	  
	}
}


uint64_t Stackwalk_GetStackID_libunwind(StackwalkInst * inst) {
  unw_cursor_t cursor; unw_context_t uc;
  unw_word_t ip, sp;

  uint64_t instPointerAddr[100];
  uint64_t data;
  size_t pos = 0;
  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);
  while (unw_step(&cursor) > 0) {
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
	if (ip == 0)
		continue;
	instPointerAddr[pos] = (uint64_t)ip;
	pos++;
	if (pos >= 100)
		assert(1 == 0);
    //fprintf(stdout,"[Stackwalk_GetStackID_libunwind] ip = %lx\n", (long) ip);
  }

  StackTrie * tree = inst->tree;
  if (CPPStackTrie_LookupStack(tree, instPointerAddr, &data, pos) == true){
	return data;
  }
  else
  {
		data = inst->globalID;
		inst->globalID++;
		CPPStackTrie_InsertStack(tree, instPointerAddr, &data, pos);
		return data;	  
  }
}
uint64_t Stackwalk_GetStackIDLockUnwind(StackwalkInst * inst) {
	pthread_mutex_lock(&lock); 
	uint64_t ret =  Stackwalk_GetStackID_libunwind(inst);
	pthread_mutex_unlock(&lock); 
	return ret;
}
uint64_t Stackwalk_GetStackID(StackwalkInst * inst) {
	pthread_mutex_lock(&lock); 
#ifdef USE_GNU_BACKTRACE
	return Stackwalk_GetStackID_GNUBtrace(inst);
#endif
	//uint64_t ret =  Stackwalk_GetStackID_libunwind(inst);
	uint64_t ret =  Stackwalk_GetStackID_FPStackWalker(inst);
	pthread_mutex_unlock(&lock); 
	return ret;
	//return Stackwalk_GetStackID_libunwind(inst);

}
uint64_t Stackwalk_GetStackIDLibunwind(StackwalkInst * inst) {
	return Stackwalk_GetStackID_libunwind(inst);
}


char * Stackwalk_PrintStack(StackwalkInst * inst, size_t * size) {
	char * ptr;
	CPPStackTrie_ConvertTreeToStackKey(inst->tree, &ptr, size);
	// CVector * resultVec = CVector_Init(inst->sw_malloc_wrapper, inst->sw_free_wrapper, 100000);
	// StackTrie_ConvertTreeToStackKey(inst->tree, resultVec, NULL);
	// *size = 0;
	// char * ptr = (char *)CVector_GetData(resultVec,size);
	// ptr[*size] = '\000';
	return ptr;
}
