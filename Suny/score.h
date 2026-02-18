#ifndef SCORE_H
#define SCORE_H

#define MAX_STACK_SIZE 100
#define MAX_IDENTIFIER_SIZE 1024
#define MAX_NUMBER_LEN 1024
#define DEFAULT_MAX 1024
#define MAX_LABEL_SIZE 1024

struct Sobj;
struct Sframe;
struct Stable;

typedef unsigned char byte_t;

typedef int address_t;

typedef struct Sobj* (*builtin_func)(struct Sframe*);

typedef struct Sframe* (*main_func)(struct Sframe*, struct Stable*);

#define set_true(a) (a) = 1
#define set_false(a) (a) = 0

#define STRUE 1
#define SFALSE 0

#define SUNY_VERSION "1.1"
#define SUNY_COPYRIGHT "Copyright (c) 2025-present dinhsonhai132"
#define AUTHOR "dinhsonhai132"

#ifdef _WIN32
    #include <windows.h>
    typedef HMODULE SUNY_MODULE;
    typedef FARPROC SUNY_FUNCTION;
#else
    #include <stdio.h>
    #include <dlfcn.h>
    typedef void* SUNY_MODULE;
    typedef void* SUNY_FUNCTION;
#endif

#ifdef _WIN32
    #define SUNY_API   __declspec(dllexport)
    #define SUNY_LOCAL __declspec(dllimport)
#else
    #define SUNY_API   __attribute__((visibility("default")))
    #define SUNY_LOCAL __attribute__((visibility("hidden")))
#endif

#define SUNY_BREAK_POINT while(1)
#define SUNY_EXIT exit(0)

#include "opcode.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

int                 __SDEBUG                          (const char* format, ...);
SUNY_MODULE         SUNY_OPEN_LIB                   (char* lib_name);
SUNY_FUNCTION       SUNY_GET_FUNCTION_FROM          (SUNY_MODULE handle, char* func_name);
void                SUNY_CLOSE_LIB                  (SUNY_MODULE handle);


#ifndef DEBUG
    #define SDEBUG __SDEBUG
#else
    #define SDEBUG printf
#endif

#endif // SCORE_H
