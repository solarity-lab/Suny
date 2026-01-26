#ifndef SMEM_H
#define SMEM_H

#include "score.h"  
#include "serror.h"

#define Smem_Malloc(size) Smem_RawMalloc(size, __FILE__, __LINE__)
#define Smem_Free(ptr) Smem_RawFree(ptr, __FILE__, __LINE__)
#define Smem_Realloc(ptr, size) Smem_RawRealloc(ptr, size, __FILE__, __LINE__)
#define Smem_Calloc(nmemb, size) Smem_RawCalloc(nmemb, size, __FILE__, __LINE__)

struct AllocInfo {
    void* ptr;
    size_t size;

    const char* file;
    int line;
    int freed;

    struct AllocInfo* next;
};

static struct AllocInfo* alloc_list = NULL;

struct AllocInfo* AllocInfoNew(void);

void* Smem_RawMalloc(size_t size, const char* file, int line);

void* Smem_RawRealloc(void* ptr, size_t size, const char* file, int line);

void* Smem_RawCalloc(size_t nmemb, size_t size, const char* file, int line);

int Smem_RawFree(void* ptr, const char* file, int line);

#endif // SMEM_H