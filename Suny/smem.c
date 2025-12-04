#include "smem.h"
#include "sdebug.h"

void* Smem_RawMalloc(size_t size) {
    void* p = malloc(size);
    if (!p) {
        __ERROR("Smem_RawMalloc: cannot allocate %zu bytes\n", size);
        return NULL;
    }
    memset(p, 0, size);
    return p;
}

void* Smem_RawRealloc(void* ptr, size_t size) {
    void* p = realloc(ptr, size);
    if (!p && size != 0) {
        __ERROR("Smem_RawRealloc: cannot reallocate %zu bytes\n", size);
        return NULL;
    }
    memset(p, 0, size);
    return p;
}

void* Smem_RawCalloc(size_t nmemb, size_t size) {
    void* p = calloc(nmemb, size);
    if (!p) {
        __ERROR("Smem_RawCalloc: cannot allocate %zu elements of size %zu\n", nmemb, size);
        return NULL;
    }
    memset(p, 0, nmemb * size);
    return p;
}

void* Smem_Malloc(size_t size) {
    void* p = Smem_RawMalloc(size);
    if (!p) return NULL;
    return p;
}

void* Smem_Realloc(void* ptr, size_t size) {
    void* p = Smem_RawRealloc(ptr, size);
    return p;
}

void* Smem_Calloc(size_t nmemb, size_t size) {
    void* p = Smem_RawCalloc(nmemb, size);
    return p;
}

int Smem_Free(void* ptr) {
    Smem_RawFree(ptr);
    return 0;
}

int Smem_RawFree(void* ptr) {
    free(ptr);
    ptr = NULL;
    return 0;
}