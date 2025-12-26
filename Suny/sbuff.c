#include "sbuff.h"
#include "smem.h"
#include "sdebug.h"
#include <dirent.h>

struct SZIO*
Sbuff_new(void) {
    struct SZIO *zio = Smem_Malloc(sizeof(struct SZIO));

    zio->file = "stdin";
    zio->buffer = NULL;
    zio->size = 0;

    zio->nf = 0;
    zio->maxf = 100;
    zio->folders = Smem_Calloc(zio->maxf, sizeof(char*));

    return zio;    
}

int 
Sbuff_free
(struct SZIO *zio) {
    if (!zio) return 0;
    if (zio->buffer) Smem_Free(zio->buffer);
    Smem_Free(zio->folders);
    Smem_Free(zio);
    return 1;
}

struct SZIO*
Sbuff_read_file
(char* file) {
    if (!file) return NULL;

    FILE *fp = fopen(file, "rb");
    if (!fp) {
        __ERROR("Error: Failed to open file '%s'\n", file);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    struct SZIO *zio = Sbuff_new();
    if (!zio) {
        fclose(fp);
        return NULL;
    }

    zio->file = file;
    zio->buffer = (char*)Smem_Malloc(fsize + 1);
    if (!zio->buffer) {
        Smem_Free(zio);
        fclose(fp);
        return NULL;
    }

    size_t nread = fread(zio->buffer, 1, fsize, fp);
    fclose(fp);

    zio->buffer[nread] = '\0';
    zio->size = (int)nread;

    return zio;
}

struct SZIO* 
Sbuff_write_bytecode_file(unsigned char* content, size_t size, const char* filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return NULL;

    size_t written = fwrite(content, 1, size, fp);
    fclose(fp);

    if (written != size) {
        return NULL; 
    }

    struct SZIO *zio = Smem_Malloc(sizeof(struct SZIO));
    if (!zio) return NULL;

    zio->file = strdup(filename); 
    zio->buffer = Smem_Malloc(size);
    if (!zio->buffer) {
        Smem_Free(zio->file);
        Smem_Free(zio);
        return NULL;
    }

    memcpy(zio->buffer, content, size);
    zio->size = size;

    return zio;
}

struct SZIO*
Sbuff_read_folder_name(char* path) {
    if (!path) return NULL;

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return NULL;
    }

    struct dirent *entry;
    struct SZIO *zio = Sbuff_new();

    zio->file = strdup(path);
    zio->folders = NULL;
    zio->nf = 0;
    zio->maxf = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (zio->nf >= zio->maxf) {
            zio->maxf = (zio->maxf == 0) ? 8 : zio->maxf * 2;
            zio->folders = Smem_Realloc(zio->folders, zio->maxf * sizeof(char*));
        }

        zio->folders[zio->nf] = strdup(entry->d_name); // lưu tên file/folder
        zio->nf++;
    }

    closedir(dir);
    return zio;
}

struct SZIO*
Sbuff_find_file_in(char* filename, char* path) {
    if (!filename || !path) return NULL;

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return NULL;
    }

    struct dirent *entry;
    struct SZIO *zio = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (strcmp(entry->d_name, filename) == 0) {
            zio = Sbuff_new();
            zio->file = strdup(entry->d_name);

            size_t len = strlen(path) + strlen(entry->d_name) + 2;
            char *fullpath = Smem_Malloc(len);
            snprintf(fullpath, len, "%s/%s", path, entry->d_name);
            zio->buffer = strdup(fullpath);
            zio->size = 0;

            Smem_Free(fullpath);
            break;
        }
    }

    closedir(dir);
    return zio;
}

struct SZIO* 
Sbuff_read_bytecode_file
(const char* filename) {
    if (!filename) return NULL;

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        __ERROR("Cannot open bytecode file: %s\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    if (size <= 0) {
        fclose(fp);
        return NULL;
    }

    struct SZIO *zio = Sbuff_new();
    if (!zio) {
        fclose(fp);
        return NULL;
    }

    zio->file = strdup(filename);
    zio->buffer = Smem_Malloc(size);
    zio->size = size;
    zio->bytecode_size = size;
    zio->bytecode = zio->buffer;

    if (!zio->buffer) {
        Sbuff_free(zio);
        fclose(fp);
        return NULL;
    }

    fread(zio->buffer, 1, size, fp);
    fclose(fp);

    return zio;
}