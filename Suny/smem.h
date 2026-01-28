#ifndef SMEM_H
#define SMEM_H

#include "score.h"  
#include "serror.h"

#define Smem_Malloc(size)           Smem_RawMalloc(size)
#define Smem_Free(ptr)              Smem_RawFree(ptr)
#define Smem_Realloc(ptr, size)     Smem_RawRealloc(ptr, size)
#define Smem_Calloc(nmemb, size)    Smem_RawCalloc(nmemb, size)

void* Smem_RawMalloc(size_t size);

void* Smem_RawRealloc(void* ptr, size_t size);

void* Smem_RawCalloc(size_t nmemb, size_t size);

int Smem_RawFree(void* ptr);

#endif // SMEM_H