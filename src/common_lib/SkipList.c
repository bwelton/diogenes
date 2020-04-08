#include "SkipList.h"

static pthread_key_t skiplist_key;
static pthread_once_t skiplist_key_once = PTHREAD_ONCE_INIT;

#define SKIPLIST_DEPTH 5
volatile int SkipListHandle_GenerateRandom = 0;

SkipListNode ** SkipListNode_GenerateEmpty(size_t count) {
	SkipListNode ** empty_list = (SkipListNode**)valloc(sizeof(SkipListNode*) *count);
	SkipListNode * empty = (SkipListNode*)valloc(sizeof(SkipListNode) *count);
	for (int i = 0; i < count; i++){
		empty[i].value = 0;
		empty[i].next = NULL;
		empty[i].levelDown = NULL;
		empty[i].size = 0;
		empty_list[i] = &(empty[i]);
	}	
	return empty_list;
}

void SkipListHandle_Initalize(SkipListHandle ** ptr) {
	(*ptr) = (SkipListHandle*)valloc(sizeof(SkipListHandle));
	if(*ptr == NULL)
		exit(0);
	(*ptr)->head = NULL;
	(*ptr)->empty = SkipListNode_GenerateEmpty(4096);
	(*ptr)->empty_count = 4096;
	SkipListNode * prevLevel = NULL;
	for (int i = 0; i < SKIPLIST_DEPTH; i++) {
		if ((*ptr)->head == NULL) {
			(*ptr)->head = (*ptr)->empty[(*ptr)->empty_count-1];
		} else {
			prevLevel->levelDown = (*ptr)->empty[(*ptr)->empty_count-1];
		}
		prevLevel = (*ptr)->empty[(*ptr)->empty_count-1];
		(*ptr)->empty_count = (*ptr)->empty_count-1;
	}
}

static void SkipListHandle_make_key() {
    (void) pthread_key_create(&skiplist_key, NULL);
}

SkipListHandle * SkipListHandle_GetThreadSpecific() {
	pthread_once(&skiplist_key_once, SkipListHandle_make_key);
	void *ptr;
	if ((ptr = pthread_getspecific(skiplist_key)) == NULL) {
		SkipListHandle_Initalize((SkipListHandle**)&ptr);
		pthread_setspecific(skiplist_key, ptr);
		// if (SkipListHandle_GenerateRandom == 0)
		// 	srand(time(NULL));
		SkipListHandle_GenerateRandom = 1;
	}
	return (SkipListHandle *)ptr;
}

SkipListNode * SkipListHandle_TraverseLevelFind(SkipListNode * ptr, uint64_t item, bool getPrevEle) {
	// Returns the element <= item, if it exists, otherwise end of list
	SkipListNode * prev = ptr;
	while(ptr != NULL) {
		if (ptr->value >= item){
			return prev;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return prev;

	// while(ptr != NULL) {
	// 	if (getPrevEle && ptr->value >= item) {
	// 		return prev;
	// 	} else if (ptr->value > item) {
	// 		return ptr;
	// 	}
	// 	prev = ptr;
	// 	ptr = ptr->next;
	// }
	
	// while(ptr->next != NULL){
	// 	if (getPrevEle) {
	// 		if (ptr->value >= item){
	// 			return prev;
	// 		}
	// 	} else {
	// 		if (ptr->value > item){
	// 			return prev;
	// 		}			
	// 	}
	// 	prev = ptr;
	// 	ptr = ptr->next;
	// }

	//return prev;
}
unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned fastpsudorand()
{
bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
return lfsr =  (lfsr >> 1) | (bit << 15);
}
bool SkipListHandle_flip( )
{
    int i = fastpsudorand() % 2;
    if (i == 0)
        return false;          
    else
		return true;              
}

SkipListNode * SkipListHandle_GetEmpty(SkipListHandle * handle) {
	if (handle->empty_count == 0) {
		handle->empty = SkipListNode_GenerateEmpty(4096);
		handle->empty_count = 4096;
	}
	handle->empty_count = handle->empty_count - 1;
	handle->empty[handle->empty_count]->value = 0;
	handle->empty[handle->empty_count]->next = NULL;
	handle->empty[handle->empty_count]->levelDown = NULL;
	handle->empty[handle->empty_count]->size = 0;
	return handle->empty[handle->empty_count];
}

uint64_t SkipListHandle_FindNearestElement(uint64_t item, size_t * size) {
	SkipListHandle * handle = SkipListHandle_GetThreadSpecific();
	SkipListNode * cur = handle->head;
	for (int i = 0; i < SKIPLIST_DEPTH; i++){
		cur = SkipListHandle_TraverseLevelFind(cur, item, false);
		if (cur->value <= item && cur->value + cur->size > item){
			*size = cur->size;
			return cur->value;
		}
		if (cur->next != NULL) {
			if (cur->next->value == item){
				*size = cur->next->size;
				return item;
			}
		}
		if (cur->levelDown != NULL){
			cur = cur->levelDown;
		} else {
			break;
		}
	}
	// if (cur == NULL)
	// 	return 0;
	// if (cur->value <= item && cur->value + cur->size > item)
	// 	return cur->value;
	return 0;
}

void SkipListHandle_AddElement(uint64_t item, size_t size){
	if(item == 0 || size == 0)
		return;
	SkipListNode * depth[SKIPLIST_DEPTH];
	SkipListHandle * handle = SkipListHandle_GetThreadSpecific();
	SkipListNode * cur = handle->head;	
	for (int i = 0; i < SKIPLIST_DEPTH; i++){
		cur = SkipListHandle_TraverseLevelFind(cur, item, false);
		depth[i] = cur;
		if (cur->value == item){
			fprintf(stderr, "%s\n", "DUPLICATE ENTRY, WE SHOULD NOT BE HERE!");
			return;
		}
		cur = cur->levelDown;
	}
	SkipListNode * newNode = SkipListHandle_GetEmpty(handle);
	newNode->value = item;
	newNode->size = size;
	for (int i = SKIPLIST_DEPTH - 1; i >= 0; i--){
		newNode->next = depth[i]->next;
		depth[i]->next = newNode;
		//(newNode->next, depth[i]->next);
		if (SkipListHandle_flip()) {
			SkipListNode * tmpNode = SkipListHandle_GetEmpty(handle);
			tmpNode->value = newNode->value;
			tmpNode->size = newNode->size;
			tmpNode->levelDown = newNode;
			newNode = tmpNode;
		} else {
			break;
		}
	}
}

void SkipListHandle_DeleteElement(SkipListHandle * handle, SkipListNode * node) {
	if (handle->empty_count >= 4096)
		return;
	handle->empty[handle->empty_count] = node;
	handle->empty_count++;
}

void SkipListHandle_RemoveElement(uint64_t item) {
	SkipListHandle * handle = SkipListHandle_GetThreadSpecific();
	SkipListNode * cur = handle->head;
	while(cur != NULL) {
		cur = SkipListHandle_TraverseLevelFind(cur, item, true);
		if (cur->next != NULL) {
			if (cur->next->value == item){
				SkipListNode * nextNode = cur->next->next;
				SkipListHandle_DeleteElement(handle, cur->next);
				cur->next = nextNode;
			}
		}
		cur = cur->levelDown;
	}
}

void SkipListHandle_DebugPrint() {
	SkipListHandle * handle = SkipListHandle_GetThreadSpecific();
	SkipListNode * cur = handle->head;
	while(cur != NULL) {
		SkipListNode * next = cur;
		while(next != NULL) {
			fprintf(stdout, "(%llu,%llu,%p,%p),",next->value, next->size, next->levelDown, next);
			next = next->next;
		}
		cur = cur->levelDown;
		fprintf(stdout, "\n");
	}
};