#include "HashMap.h"
#include <assert.h>

int main() {
	HashMap * map = HashMap_Initalize(malloc, free);
	for (uint64_t i = 1; i < 100000; i++) {
		HashMap_InsertElement(map, i, (void*)(i + 9000000));
	}
	for (uint64_t i = 1; i < 100000; i++) {
		void * data = NULL;
		bool found = HashMap_FindValue(map, i, &data);
		assert(data == (void*)(i+9000000) && found == true);
	}
	HashMap_DebugPrintBuckets(map);
	for (uint64_t i = 1; i < 100000; i++) {
		void * data = NULL;
		HashMap_EraseElement(map, i);
		assert(HashMap_FindValue(map, i,&data) == false);
	}
	fprintf(stderr, "%s\n", "AT END" );
	HashMap_DebugPrintBuckets(map);
}