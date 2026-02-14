#ifndef SC_API_FUNC_H
#define SC_API_FUNC_H

#include "score.h"
#include "stool.h"
#include "slist.h"
#include "sio.h"
#include "scopy.h"

/*
    this file define c_api_func object 
    c_api_func object is an object that has a pointer point to a Suny C API function
*/

struct Sframe;
struct ScompilerUnit;
struct Stable;
struct Scall_context;

/*
    call dicrectly the function 
*/

#define call_func(func, frame) ((struct Sobj* (*)(struct Sframe*)) (func))((frame));

#define get_c_api_func(f_obj) (f_obj)->c_api_func->func

struct Sc_api_func {
    void* func;         /* pointer that point to a Suny C API function*/
    char* name;         /* raw C name of the function */
    int address;
    int args_size;
};

struct Sc_api_func* 
Sc_api_func_set
(void* func, char* name, int address, int args_size);

#endif // SC_API_FUNC_H