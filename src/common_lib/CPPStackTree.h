#ifndef DIOGENESCOMMON_CPPSTACKTREE
#define DIOGENESCOMMON_CPPSTACKTREE 1
#include <inttypes.h>
#include <stdbool.h> 
#include <pthread.h>
typedef void * CPPStackTrie;
#ifdef __cplusplus
extern "C" {
#endif
CPPStackTrie CPPStackTrie_Initalize();
void CPPStackTrie_ConvertTreeToStackKey(CPPStackTrie tree, char ** ret, uint64_t * size);
bool CPPStackTrie_InsertStack(CPPStackTrie tree, uint64_t * elementArray, uint64_t * dataArray, uint64_t count);
bool CPPStackTrie_LookupStack(CPPStackTrie tree,  uint64_t * elementArray, uint64_t * returnData, uint64_t count);
#ifdef __cplusplus
}
#endif
#endif