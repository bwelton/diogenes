#include "StackTrie.h"
#include "CVector.h"
#include <assert.h>


// StackTrie * StackTrie_Initalize(uint64_t key, void * data, void * (*allocator_fun)(size_t), void (*free_fun)(void *));
// bool StackTrie_InsertStack(StackTrie * tree, uint64_t * elementArray, void ** dataArray, uint64_t count);
// bool StackTrie_LookupStack(StackTrie * tree, uint64_t * elementArray, void ** returnData, uint64_t count);
int main() {
	StackTrie * tree = StackTrie_Initalize(0, NULL, malloc, free);
	uint64_t testSet1[] = {5,4,7,6,5};
	uint64_t testData1[] = {0,0,0,0,1};
	StackTrie_InsertStack(tree, testSet1, (void**)testData1, 5);

	uint64_t testSet2[] = {5,4,7};
	uint64_t testData2[] = {0,0,4};
	StackTrie_InsertStack(tree, testSet2, (void**)testData2, 3);

	uint64_t testSet3[] = {3,4,7};
	uint64_t testData3[] = {0,0,8};
	StackTrie_InsertStack(tree, testSet3, (void**)testData3, 3);

	uint64_t testSet4[] = {2,8,102};
	uint64_t testData4[] = {0,0,45};
	StackTrie_InsertStack(tree, testSet4, (void**)testData4, 3);

	uint64_t testSet5[] = {5,4,7,9,5};
	uint64_t testData5[] = {0,0,0,0,7};
	StackTrie_InsertStack(tree, testSet5, (void**)testData5, 5);
	CVector * resultVec = CVector_Init(malloc, free, 20);
	StackTrie_ConvertTreeToStackKey(tree, resultVec, NULL);
	size_t size = 0;
	char * ptr = (char *)CVector_GetData(resultVec,&size);
	ptr[size] = '\000';
	fprintf(stderr, "OUTPUT:\n%s\n",ptr);



	testData1[4] = 0;
	assert(StackTrie_LookupStack(tree, testSet1, (void**)testData1, 5) == true);
	assert(testData1[4] == 1);

	testData2[2] = 0;
	assert(StackTrie_LookupStack(tree, testSet2, (void**)testData2, 3) == true);
	assert(testData2[2] == 4);

	testData3[2] = 0;
	assert(StackTrie_LookupStack(tree, testSet3, (void**)testData3, 3) == true);
	assert(testData3[2] == 8);

	testData4[2] = 0;
	assert(StackTrie_LookupStack(tree, testSet4, (void**)testData4, 3) == true);
	assert(testData4[2] == 45);

	testData5[4] = 0;
	assert(StackTrie_LookupStack(tree, testSet5, (void**)testData5, 5) == true);
	assert(testData5[4] == 7);
	// HashMap * map = HashMap_Initalize(malloc, free);
	// for (uint64_t i = 1; i < 100000; i++) {
	// 	HashMap_InsertElement(map, i, (void*)(i + 9000000));
	// }
	// for (uint64_t i = 1; i < 100000; i++) {
	// 	void * data = NULL;
	// 	bool found = HashMap_FindValue(map, i, &data);
	// 	assert(data == (void*)(i+9000000) && found == true);
	// }
	// HashMap_DebugPrintBuckets(map);
	// for (uint64_t i = 1; i < 100000; i++) {
	// 	void * data = NULL;
	// 	HashMap_EraseElement(map, i);
	// 	assert(HashMap_FindValue(map, i,&data) == false);
	// }
	// fprintf(stderr, "%s\n", "AT END" );
	// HashMap_DebugPrintBuckets(map);
}