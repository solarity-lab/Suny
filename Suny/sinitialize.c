#include "sinitialize.h"

int Sinitialize_loadlib_to_frame
(
    struct Sframe *frame,
    int address,
    int args_size,
    char* name,
    void* func
)

{
    struct Sc_api_func* api_func = Sc_api_func_set(func, name, address, args_size);
    Sframe_load_c_api_func(frame, api_func->func, api_func->address, api_func->name, api_func->args_size);
    return 0;
}

int Sinitialize_c_api_func
(
    struct Sframe *frame,
    struct Stable *table,
    int address,
    int args_size,
    char* name,
    void* func
)

{
    SDEBUG("Initializing C API function: %s at address %d with %d args\n", name, address, args_size);
    Sinitialize_loadlib_to_frame(frame, address, args_size, name, func);
    Ssymbol_add(table, name, address);
    SDEBUG("C API function %s initialized successfully.\n", name);
    return 0;
}

int Sinitialize_variables
(
    struct Sframe *frame,
    struct Stable *table,
    char* name,
    int address,
    struct Sobj* value
)

{
    Sframe_store_global(frame, address, value, GLOBAL_OBJ);
    Ssymbol_add(table, name, address);
    return 0;
}

int SinitializeEnvironment
(
    struct Sframe* frame, 
    struct ScompilerUnit* compiler, 
    struct Stable* table
) 

{
    frame->compiler = compiler;
    frame->table = table;
    compiler->frame = frame;
    return 0;
}