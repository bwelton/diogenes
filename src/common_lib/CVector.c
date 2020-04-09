#include "CVector.h"

CVector * CVector_Init(void * (*allocator_fun)(size_t), void (*free_fun)(void *),size_t resizeSize) {
	CVector * ret =  (CVector *)allocator_fun(sizeof(CVector));
	ret->allocator = allocator_fun;
	ret->free = free_fun;
	ret->maxSize = 0;
	ret->curSize = 0;
	ret->resizeSize = resizeSize;
	ret->data = NULL;
	CVector_Reallocate(ret, resizeSize);
	return ret;
}

void * CVector_GetData(CVector * vec, size_t * size) {
	*size = vec->curSize;
	return vec->data;	
}

void CVector_Destroy(CVector * vec) {
	vec->free(vec->data);
	vec->free(vec);
}

size_t CVector_GetSize(CVector * vec){
	return vec->curSize;
}
void CVector_EraseElement(CVector * vec, size_t eraseSize) {
	if (vec->curSize > eraseSize)
		vec->curSize = vec->curSize - eraseSize;
	else 
		vec->curSize = 0;
}

void CVector_Reallocate(CVector * vec, size_t addBytes) {
	void * tmp = vec->allocator(sizeof(char) * (vec->maxSize +addBytes));
	if (vec->data != NULL)
		memcpy(tmp, vec->data, vec->curSize);
	vec->free(vec->data);
	vec->data = tmp;
	vec->maxSize = vec->maxSize+addBytes;
}

void CVector_Append(CVector * vec, void * data, size_t bytes) {
	size_t curSize =  vec->curSize;
	if (vec->curSize + bytes >= vec->maxSize)
		CVector_Reallocate(vec,bytes + vec->resizeSize);

	memcpy(&(((char*)vec->data)[curSize]), data, bytes);
	vec->curSize += bytes;
}

void CVector_SetResize(CVector * vec, size_t bytes) {
	vec->resizeSize = bytes;
}
