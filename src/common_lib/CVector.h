#ifndef COMMON_CVECTOR_HEADER
#define COMMON_CVECTOR_HEADER 1
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdint.h> 
#include <stdbool.h>

typedef struct CVector {
	size_t maxSize;
	size_t curSize;
	size_t resizeSize;
	void * data;
	void * (*allocator)(size_t);
	void (*free)(void *);
} CVector;

CVector * CVector_Init(void * (*allocator_fun)(size_t), void (*free_fun)(void *),size_t resizeSize);
void * CVector_GetData(CVector * vec, size_t * size);
void CVector_Destroy(CVector * vec);
size_t CVector_GetSize(CVector * vec);
void CVector_Reallocate(CVector * vec, size_t addBytes);
void CVector_Append(CVector * vec, void * data, size_t bytes);
void CVector_SetResize(CVector * vec, size_t bytes);
void CVector_EraseElement(CVector * vec, size_t eraseSize);
#endif