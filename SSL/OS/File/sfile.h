#ifndef SFILE_H
#define SFILE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct SFile {
    FILE *fp;
    char *path;
    char *mode;

    char *buffer;
    size_t buf_len;
    size_t buf_cap;

    int closed;
};

struct SFile *sfile_open(const char *path, const char *mode);

int sfile_close(struct SFile *file);
int sfile_write(struct SFile *file, const char *data, size_t len);
int sfile_print(struct SFile *file);
int sfile_clear(struct SFile *file);
int sfile_delete(struct SFile *file);

#endif // SFILE_H
