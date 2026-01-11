#include "smem.h"
#include "sdebug.h"

void* Smem_RawMalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    void* p = malloc(size);
    if (!p) {
        __ERROR("Smem_RawMalloc: cannot allocate %zu bytes\n", size);
        return NULL;
    }

    memset(p, 0, size);
    return p;
}


void* Smem_RawCalloc(size_t nmemb, size_t size) {
    if (nmemb == 0 || size == 0) {
        return NULL;
    }

    if (nmemb > SIZE_MAX / size) {
        __ERROR("Smem_RawCalloc: size overflow (%zu * %zu)\n", nmemb, size);
        return NULL;
    }

    void* p = calloc(nmemb, size);
    if (!p) {
        __ERROR("Smem_RawCalloc: cannot allocate %zu elements of size %zu\n",
                nmemb, size);
        return NULL;
    }

    return p;
}

void* Smem_RawRealloc(void* ptr, size_t size) {
    void* p = realloc(ptr, size);
    if (!p) {
        __ERROR("Smem_RawRealloc: cannot reallocate %zu bytes\n", size);
        return p;
    }
    return p;
}

int Smem_RawFree(void* ptr) {
    free(ptr);
    ptr = NULL;
    return 0;
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
    if (!ptr) return 0;
    Smem_RawFree(ptr);
    ptr = NULL;
    return 0;
}