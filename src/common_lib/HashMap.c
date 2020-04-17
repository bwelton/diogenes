#include "HashMap.h"
#define EMPTY_POOL_SIZE 64
#define BUCKET_COUNT 32

uint64_t HashMap_KeyGenerator(uint64_t key) {
	key ^= key >> 33;
	key *= 0xff51afd7ed558ccd;
	key ^= key >> 33;
	key *= 0xc4ceb9fe1a85ec53;
	key ^= key >> 33;
	return key;
}

uint64_t KeyValuePair_GetKey(KeyValuePair * pair){
	if (pair == NULL)
		return 0;
	return pair->key;
}


void * KeyValuePair_GetData(KeyValuePair * pair){
	if (pair == NULL)
		return NULL;
	return pair->data;
}




KeyValuePair ** HashMap_DumpElements(HashMap * map, size_t * size) {
	KeyValuePair ** ret = (KeyValuePair**)map->hashmap_allocator(map->elementCount * sizeof(KeyValuePair*));
	size_t curPos = 0;
	for (size_t i = 0; i < map->bucketCount; i++) {
		if(map->buckets[i] != NULL) {
			KeyValuePair * next = map->buckets[i];
			while (next != NULL) {
				if (curPos >= map->elementCount) {
					assert(1==0);
				}
				ret[curPos] = next;
				next = next->next;
				curPos++;
			}
		}
	}
	*size  = curPos;
	return ret;
}

void * HashMap_AllocElement(HashMap * map, size_t size) {
	return map->hashmap_allocator(size);
}
void HashMap_FreeElement(HashMap * map, void * data) {
	map->hashmap_free(data);
}

void HashMap_ResizeKeyPairArray(HashMap * map,KeyValuePair *** inputArray, size_t count, size_t newMax) {
	KeyValuePair ** newArray = (KeyValuePair **)map->hashmap_allocator(sizeof(KeyValuePair*) *newMax);
	for(size_t i = 0; i < newMax; i++) {
		if (i < count)
			newArray[i] = (*inputArray)[i];
		else
			newArray[i] = NULL;
	}
	if (count != 0 && *inputArray != NULL)
		map->hashmap_free((void*)(*inputArray));
	(*inputArray) = newArray;
}

void HashMap_ReallocateEmpty(HashMap * map) {
	if(map->emptyPos != 0) {
		return;
	}
	KeyValuePair * newElements = (KeyValuePair *)map->hashmap_allocator(sizeof(KeyValuePair) * EMPTY_POOL_SIZE);
	for(int i = 0; i < EMPTY_POOL_SIZE; i++) {
		newElements[i].key = 0;
		newElements[i].data = NULL;
		newElements[i].next = NULL;
		map->emptyPool[i] = &(newElements[i]);
	}
	if (map->allocedBucketSize <= map->allocedBucketCount){
		HashMap_ResizeKeyPairArray(map, &(map->allocedBuckets), map->allocedBucketCount, map->allocedBucketSize+32);
		map->allocedBucketSize += 32;
	}
	map->allocedBuckets[map->allocedBucketCount] = newElements;
	map->allocedBucketCount++;
	map->emptyPos = EMPTY_POOL_SIZE;
}	

uint64_t HashMap_CalculateOffset(HashMap * map, uint64_t key) {
	uint64_t kv = HashMap_KeyGenerator(key);
	return kv % map->bucketCount;
}

bool HashMap_AddElement(HashMap * map, KeyValuePair * cur) {
	uint64_t offset = HashMap_CalculateOffset(map, cur->key);
	KeyValuePair * m = map->buckets[offset];
	if (m == NULL){
		map->buckets[offset] = cur;
	} else {
		while(m->next != NULL){
			if (m->key == cur->key){
				fprintf(stderr, "%s\n", "Duplicate entries not supported!");
				return false;
			}
			m = m->next;
		}
		m->next = cur;
	}
	cur->next = NULL;
	return true;
}


void HashMap_ReclaimKey(HashMap * map, KeyValuePair * key) {
	if (map->emptyMaxSize < map->emptyPos+1) {
		HashMap_ResizeKeyPairArray(map, &(map->emptyPool), map->emptyPos, map->emptyMaxSize*2);
		map->emptyMaxSize = map->emptyMaxSize * 2;
	}
	key->key = 0;
	key->data = NULL;
	key->next = NULL;
	map->emptyPool[map->emptyPos] = key;
	map->emptyPos++;
}

void HashMap_EraseKey(HashMap * map, KeyValuePair * cur) {
	uint64_t offset = HashMap_CalculateOffset(map, cur->key);
	KeyValuePair * m = map->buckets[offset];
	KeyValuePair * prev = NULL;
	// if (m != NULL) {
	while(m != NULL){
		if(m->key == cur->key) {
			if(prev == NULL){
				map->buckets[offset] = m->next;
			} else {
				prev->next = m->next;
			}
			HashMap_ReclaimKey(map,m);
			map->elementCount--;
			return;
		}
		prev = m;
		m = m->next;
	}
	// }
	fprintf(stderr, "%s: %"PRIu64"\n", "Could not find key!", cur->key);
}

void HashMap_EraseElement(HashMap * map, uint64_t key) {
	KeyValuePair tmp;
	tmp.key = key;
	HashMap_EraseKey(map, &tmp);
}

void HashMap_ExtendBuckets(HashMap * map, uint64_t newSize) {
	HashMap_ResizeKeyPairArray(map, &(map->buckets), map->bucketCount, newSize);
	size_t existingSize = map->bucketCount;
	map->bucketCount = newSize;
	for (size_t i = 0; i < existingSize; i++) {
		if(map->buckets[i] != NULL) {
			KeyValuePair * cur = map->buckets[i];
			KeyValuePair * prev = NULL;
			while (cur != NULL) {
				uint64_t offset = HashMap_CalculateOffset(map, cur->key);
				KeyValuePair * cnext = cur->next;
				if (offset != i) {
					if (prev == NULL)
						map->buckets[i] = cur->next;
					else 
						prev->next = cur->next;

					HashMap_AddElement(map, cur);
				} else {
					prev = cur;
				}
				cur = cnext;
			}
		}
	}
	//map->bucketCount = newSize;
}

HashMap* HashMap_Initalize(void * (*allocator_fun)(size_t), void (*free_fun)(void *)) {
	HashMap * ret = (HashMap*)allocator_fun(sizeof(HashMap));
	ret->hashmap_allocator = allocator_fun;
	ret->hashmap_free = free_fun;
	ret->buckets = NULL;
	ret->bucketCount = 0;
	ret->used = 0;
	ret->emptyPos = 0;
	ret->allocedBucketCount = 0;
	ret->allocedBucketSize = 0;
	ret->emptyPool = (KeyValuePair **)allocator_fun(sizeof(KeyValuePair *) * EMPTY_POOL_SIZE);
	ret->emptyMaxSize = EMPTY_POOL_SIZE;
	ret->elementCount = 0;
	HashMap_ExtendBuckets(ret, BUCKET_COUNT);
	HashMap_ReallocateEmpty(ret);
	return ret;
}

KeyValuePair * HashMap_GetEmptyElement(HashMap * map) {
	if(map->emptyPos == 0)
		HashMap_ReallocateEmpty(map);
	KeyValuePair * ret = map->emptyPool[map->emptyPos - 1];
	map->emptyPos = map->emptyPos - 1;
	ret->key = 0;
	ret->data = NULL;
	ret->next = NULL;
	return ret;
}

void HashMap_InsertElement(HashMap * map, uint64_t key, void * data) {
	KeyValuePair * empty = HashMap_GetEmptyElement(map);
	empty->key =  key;
	empty->data = data;
	if(HashMap_AddElement(map,empty) == false) {
		HashMap_ReclaimKey(map,empty);
	} else{
		map->elementCount++;
		if (map->elementCount > map->bucketCount * 4)
			HashMap_ExtendBuckets(map, map->bucketCount * 2);
	}

}

bool HashMap_FindValue(HashMap * map, uint64_t key, void ** data) {
	uint64_t offset = HashMap_CalculateOffset(map, key);
	KeyValuePair * pair = map->buckets[offset];
	while (pair != NULL) {
		if (pair->key == key){
			*data = pair->data;
			return true;
		}
		pair = pair->next;
	}
	return false;
}

void HashMap_DebugPrintBuckets(HashMap * map) {
	for (size_t i = 0; i < map->bucketCount; i++) {
		KeyValuePair * bucket = map->buckets[i];
		fprintf(stderr, "Bucket ID: %"PRIu64" Elements = ", i);
		while(bucket != NULL) {
			fprintf(stderr, "(%"PRIu64",%p),", bucket->key, bucket->data);
			bucket =  bucket->next;
		}
		fprintf(stderr, "\n");
	}
}
