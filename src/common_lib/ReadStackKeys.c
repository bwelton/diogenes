#include "ReadStackKeys.h"
HashMap * ReadStackKeys(void * (*allocator_fun)(size_t), void (*free_fun)(void *), char * inputData, size_t size) {
	char * ptr, *save1 = NULL;
	HashMap * ret = HashMap_Initalize(allocator_fun, free_fun);
	ptr = strtok_r(inputData, "\n", &save1);
	while (ptr != NULL) {
		char * curLocation, *save2 = NULL;
		CVector * data = CVector_Init(allocator_fun, free_fun,5000);
		uint64_t ident = 1;
		curLocation = strtok_r(ptr, "$", &save2);
		if (curLocation == NULL)
			break;
		if (sscanf(curLocation, "%"PRIu64"", &ident) != 1){
			fprintf(stderr, "%s\n", "FATAL ERROR IN SSCANF!!!");
			assert(1==0);
		}
		//fprintf(stderr,"Dump of stack id - %"PRIu64"\n", ident);
		curLocation = strtok_r(NULL, "$", &save2);
		char * save3 = NULL;
		char * element = strtok_r(curLocation, ",", &save3);
		while (element != NULL) {
			char * save4 = NULL;
			StackKeyEntry e;
			char * strN = strtok_r(element, "@", &save4);
			if(strN != NULL) {
				e.libname = (char*)allocator_fun(strlen(strN));
				strncpy(e.libname, strN,strlen(strN));
				e.addr = 0;
				save4 =  strtok_r(NULL, "@", &save4);
				assert(save4 != NULL);
				sscanf(save4, "%"PRIx64"", &(e.addr));
				//fprintf(stderr,"Address= %"PRIu64"\n", e.addr);
				CVector_Append(data, &e, sizeof(StackKeyEntry));
			}
			element = strtok_r(NULL, ",", &save3);
		}
		ptr = strtok_r(NULL, "\n", &save1);
		HashMap_InsertElement(ret, ident, (void*) data);
	}
	return ret;
}

uint64_t * ReadStackKeys_GetIds(HashMap * map, uint64_t * size) {
	size_t size_ret = 0;
	KeyValuePair ** eleDump  = HashMap_DumpElements(map, &size_ret);
	uint64_t * ret = (uint64_t*)map->hashmap_allocator(sizeof(uint64_t) * (size_ret));
	for (size_t i = 0; i < size_ret; i++) {
		ret[i] = KeyValuePair_GetKey(eleDump[i]);
	}
	*size = size_ret;
	return ret;
}

StackKeyEntry * ReadStackKeys_GetElementAt(HashMap * map, uint64_t i, uint64_t * size) {
	CVector * data;
	if (HashMap_FindValue(map,i, (void**)&data) == false)
		return NULL;
	StackKeyEntry * ret = (StackKeyEntry*)CVector_GetData(data, size);
	*size = *size / sizeof(StackKeyEntry);
	return ret;
}
