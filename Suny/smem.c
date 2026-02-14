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
    if (nmemb == 0 || size == 0) return NULL;
    if (nmemb > SIZE_MAX / size) {
        __ERROR("Smem_RawCalloc: size overflow (%zu * %zu)\n", nmemb, size);
        return NULL;
    }

    void* p = calloc(nmemb, size);
    if (!p) {
        __ERROR("Smem_RawCalloc: OOM\n");
        return NULL;
    }

    return p;
}

void* Smem_RawRealloc(void* ptr, size_t size) {
    if (!ptr) return Smem_RawMalloc(size);
    if (size == 0) {
        Smem_RawFree(ptr);
        return NULL;
    }

    void* p = realloc(ptr, size);
    if (!p) {
        __ERROR("Smem_RawRealloc: OOM\n");
        return NULL;
    }

    return p;
}

int Smem_RawFree(void* ptr) {
    if (!ptr) return 0;

    free(ptr);

    return -1;
}