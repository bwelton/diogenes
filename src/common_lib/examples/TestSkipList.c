#include "SkipList.h"

// uint64_t SkipListHandle_FindNearestElement(uint64_t item);
// void SkipListHandle_AddElement(uint64_t item, size_t size);
// void SkipListHandle_RemoveElement(uint64_t item);
int main(void) {
	uint64_t elements[] = {9,44,12,5,8812,9999999,412,62,100,100000,97};
	for(int i = 0; i < sizeof(elements)/sizeof(uint64_t); i++){
		fprintf(stderr, "%s: %llu\n", "Adding element", elements[i]);
		SkipListHandle_DebugPrint();
		SkipListHandle_AddElement(elements[i],2);
	}
	SkipListHandle_DebugPrint();
	for(int i = 0; i < sizeof(elements)/sizeof(uint64_t); i++){
		size_t size = 0;
		if (elements[i] != SkipListHandle_FindNearestElement(elements[i], &size))
			fprintf(stderr, "%s: %llu %s %llu \n", "Error could not find nearest element should return ", elements[i], " but returns ", SkipListHandle_FindNearestElement(elements[i], &size));
		else 
			fprintf(stderr, "Found element - %llu\n", elements[i]);

		if (elements[i] != SkipListHandle_FindNearestElement(elements[i]+1, &size))
			fprintf(stderr, "%s: %llu %s %llu \n", "Error could not find nearest element should return ", elements[i], " but returns ", SkipListHandle_FindNearestElement(elements[i]+1, &size));
		else 
			fprintf(stderr, "Found element - %llu as %llu\n", elements[i]+1,SkipListHandle_FindNearestElement(elements[i]+1, &size));
	}

	for (uint64_t i = 1; i < 10000005; i++) {
		size_t size = 0;
		uint64_t nearest = SkipListHandle_FindNearestElement(i,&size);
		for(int t = 0; t < sizeof(elements)/sizeof(uint64_t); t++) {
			if (i == elements[t]){
				if (nearest != elements[t]){
					fprintf(stderr, "Error searching for element - %llu but found %llu\n", i,nearest);
				}
			} else if (i - 1 == elements[t]){
				if (nearest != elements[t])
					fprintf(stderr, "Error searching for element - %llu but found %llu\n", i,nearest);
			}
		}
	}
	for(int i = 0; i < sizeof(elements)/sizeof(uint64_t); i++){
		SkipListHandle_RemoveElement(elements[i]);
		fprintf(stderr, "%s: %llu\n", "Removed - ", elements[i]);
		SkipListHandle_DebugPrint();
	}
	for(int i = 0; i < sizeof(elements)/sizeof(uint64_t); i++){
		SkipListHandle_RemoveElement(elements[i]);
	}

	//for (int i = 0; i < sizeof(elements)/sizeof(uint64_t); i++)
}