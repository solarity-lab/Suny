#ifndef SBUFF_H
#define SBUFF_H

#include "score.h"

/*
    This file defines buffered I/O utilities used by the Suny runtime
    and compiler.

    The SZIO structure represents an abstraction over file input/output,
    directory scanning, and bytecode loading/storing. It is responsible
    for reading source files, traversing folders, and handling bytecode
    serialization for the virtual machine.

    This module acts as the low-level bridge between the filesystem
    and the compiler / VM pipeline.
*/

struct SZIO {
    char *file;                 /* file name */
    char *buffer;               /* data of file */
    int size;                   /* size of file */

    char** folders;
    int nf;
    int maxf;

    unsigned char* bytecode;    /* data of this file if this file is <file>.o or <file>.csuny */
    int bytecode_size;          /* size of bytecode */
};

struct SZIO*
Sbuff_new(void);

/*
    file a file in path return it as an SZIO object
*/
struct SZIO*
Sbuff_find_file_in(char* filename, char* path);

struct SZIO*
Sbuff_read_folder_name(char* path);

int
Sbuff_free
(struct SZIO *zio);

/*
    return a SZIO object of file
*/

struct SZIO*
Sbuff_read_file
(char* file);

/*
    Write bytecode data to a file.
*/

struct SZIO* 
Sbuff_write_bytecode_file
(unsigned char* content, size_t size, const char* filename);

/*
    Read bytecode data from a file and return it as an SZIO object.
*/

struct SZIO* 
Sbuff_read_bytecode_file
(const char* filename);

#endif // SBUFF_H