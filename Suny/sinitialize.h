#ifndef SINITIALIZE_H
#define SINITIALIZE_H

#include "score.h"
#include "sobj.h"
#include "stable.h"

int Sinitialize_c_api_func
(
    struct Sframe *frame,
    struct Stable *table,
    int address,
    int args_size,
    char* name,
    void* func
);

int Sinitialize_variables
(
    struct Sframe *frame,
    struct Stable *table,
    char* name,
    int address,
    struct Sobj* value
);

int SinitializeEnvironment
(
    struct Sframe* frame, 
    struct ScompilerUnit* compiler, 
    struct Stable* table
);

#endif