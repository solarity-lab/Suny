#include "smem.h"
#include "sdebug.h"

struct AllocInfo* AllocInfoNew(void) {
    struct AllocInfo* p = malloc(sizeof(struct AllocInfo));
    if (!p) return NULL;
    memset(p, 0, sizeof(struct AllocInfo));
    return p;
}

static struct AllocInfo* find_alloc(void* ptr) {
    struct AllocInfo* cur = alloc_list;
    while (cur) {
        if (cur->ptr == ptr) return cur;
        cur = cur->next;
    }
    return NULL;
}

void* Smem_RawMalloc(size_t size, const char* file, int line) {
    if (size == 0) {
        return NULL;
    }

    void* p = malloc(size);
    if (!p) {
        __ERROR("Smem_RawMalloc: cannot allocate %zu bytes\n", size);
        return NULL;
    }

    memset(p, 0, size);

    struct AllocInfo* info = AllocInfoNew();
    info->ptr = p;
    info->size = size;
    info->file = file;
    info->line = line;

    info->next = alloc_list;
    alloc_list = info;

    info->freed = 0;

    return p;
}


void* Smem_RawCalloc(size_t nmemb, size_t size, const char* file, int line) {
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

    struct AllocInfo* info = AllocInfoNew();
    info->ptr = p;
    info->size = nmemb * size;
    info->file = file;
    info->line = line;
    info->freed = 0;

    info->next = alloc_list;
    alloc_list = info;

    return p;
}

void* Smem_RawRealloc(void* ptr, size_t size, const char* file, int line) {
    if (!ptr) return Smem_RawMalloc(size, file, line);
    if (size == 0) {
        Smem_RawFree(ptr, file, line);
        return NULL;
    }

    struct AllocInfo* info = find_alloc(ptr);
    if (!info) {
        __ERROR("Realloc unknown ptr at %s:%d\n", file, line);
        return NULL;
    }

    void* p = realloc(ptr, size);
    if (!p) {
        __ERROR("Smem_RawRealloc: OOM\n");
        return NULL;
    }

    info->ptr = p;
    info->size = size;
    info->file = file;
    info->line = line;

    return p;
}

int Smem_RawFree(void* ptr, const char* file, int line) {
    if (!ptr) return 0;

    struct AllocInfo* info = find_alloc(ptr);
    if (!info) {
        __ERROR("Invalid free at %s:%d\n", file, line);
        return -1;
    }

    if (info->freed) {
        __ERROR("Double free at %s:%d (allocated at %s:%d)\n",
                file, line, info->file, info->line);
        return -1;
    }

    free(ptr);
    info->freed = 1;
    return 0;
}