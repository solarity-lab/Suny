#include "sc_api_func.h"
#include "smem.h"

struct Sc_api_func* 
Sc_api_func_set
(void* func, char* name, int address, int args_size) {
    struct Sc_api_func* api_func = Smem_Malloc(sizeof(struct Sc_api_func));
    api_func->func = func;
    api_func->name = name;
    api_func->address = address;
    api_func->args_size = args_size;
    return api_func;
}