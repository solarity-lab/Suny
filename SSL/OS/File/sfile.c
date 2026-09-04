#include "sfile.h"

static int sfile_buffer_grow(struct SFile *f, size_t need) {
    if (f->buf_len + need <= f->buf_cap)
        return 0;

    size_t new_cap = f->buf_cap == 0 ? 64 : f->buf_cap;
    while (new_cap < f->buf_len + need)
        new_cap *= 2;

    char *new_buf = (char *)realloc(f->buffer, new_cap);
    if (!new_buf)
        return -1;

    f->buffer  = new_buf;
    f->buf_cap = new_cap;
    return 0;
}

int sfile_write(struct SFile *file, const char *data, size_t len) {
    if (!file || file->closed)
        return -1;

    if (sfile_buffer_grow(file, len) != 0)
        return -1;

    memcpy(file->buffer + file->buf_len, data, len);
    file->buf_len += len;

    return (int)len;
}

struct SFile *sfile_open(const char *path, const char *mode) {
    if (!path || !mode) return NULL;

    FILE *fp = fopen(path, mode);
    if (!fp)
        return NULL;

    struct SFile *f = (struct SFile *)calloc(1, sizeof(struct SFile));
    if (!f) {
        fclose(fp);
        return NULL;
    }

    f->fp   = fp;
    f->path = strdup(path);
    f->mode = strdup(mode);

    f->buffer   = NULL;
    f->buf_len  = 0;
    f->buf_cap  = 0;
    f->closed   = 0;

    return f;
}

int sfile_close(struct SFile *file) {
    if (!file || file->closed)
        return -1;

    if (file->buf_len > 0) {
        size_t written = fwrite(
            file->buffer,
            1,
            file->buf_len,
            file->fp
        );

        if (written != file->buf_len) {
            return -1;
        }
    }

    fclose(file->fp);

    free(file->buffer);
    free(file->path);
    free(file->mode);

    file->closed = 1;
    free(file);

    return 0;
}


int sfile_print(struct SFile *file) {
    if (!file) {
        printf("<File NULL>\n");
        return -1;
    }

    printf("<File Object at %p>\n", (void *)file);

    printf("  path      : \"%s\"\n", file->path ? file->path : "(null)");
    printf("  mode      : \"%s\"\n", file->mode ? file->mode : "(null)");
    printf("  state     : %s\n", file->closed ? "CLOSED" : "OPEN");
    printf("  FILE*     : %p (object belong to CAPI)\n", (void *)file->fp);

    printf("\n  buffer:\n");
    printf("    address : %p\n", (void *)file->buffer);
    printf("    length  : %zu bytes\n", file->buf_len);
    printf("    capacity: %zu bytes\n", file->buf_cap);

    if (file->buffer && file->buf_len > 0) {
        size_t preview_len = file->buf_len > 32 ? 32 : file->buf_len;
        printf("    preview : \"");
        fwrite(file->buffer, 1, preview_len, stdout);
        if (preview_len < file->buf_len)
            printf("...");
        printf("\"\n");
    } else {
        printf("    preview : (empty)\n");
    }

    printf("\n  flags:\n");
    printf("    closed  : %s\n", file->closed ? "true" : "false");

    return 0;
}

int sfile_clear(struct SFile *file) {
    if (!file || file->closed)
        return -1;

    file->buf_len = 0;
    return 0;
}

int sfile_delete(struct SFile *file) {
    if (!file)
        return -1;

    char *path = NULL;
    if (file->path)
        path = strdup(file->path);

    if (!file->closed) {
        if (sfile_close(file) != 0) {
            free(path);
            return -1;
        }
    }

    if (path) {
        int r = remove(path);
        free(path);
        return r == 0 ? 0 : -1;
    }

    return -1;
}