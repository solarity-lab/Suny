#include "sfunc.h"
#include "smem.h"

struct Sfunc *
Sfunc_new(void) {
    struct Sfunc *func = Smem_Malloc(sizeof(struct Sfunc));

    func->params = Smem_Calloc(MAX_ARGS_SIZE, sizeof(struct Sobj*));

    func->parent = NULL;

    func->args_index = 0;
    func->code_index = 0;

    func->code_size = 0;
    func->args_size = 0;

    func->cf = NULL;
    func->code = NULL;
    func->f_frame = NULL;
    func->envi = NULL;

    func->is_closure = 0;

    func->args_address[0] = 0;
    memset(func->args_address, 0, sizeof(func->args_address));

    return func;
}

int 
Sfunc_free
(struct Sfunc *func) {
    #ifdef DEBUG
    printf("[sfunc.c] int Sfunc_free(struct Sfunc *func) (building...)\n");
    #endif

    if (func->code) {
        Scode_free(func->code);
    }

    if (func->is_closure && func->envi) {
        Senvi_free(func->envi);
    }
    
    Smem_Free(func->params);
    Smem_Free(func);

#ifdef DEBUG
    printf("[sfunc.c] int Sfunc_free(struct Sfunc *func) (done)\n");
#endif

    return 0;
}

struct Sobj*
Sobj_set_func
(struct Sfunc *func) {
    struct Sobj *obj = Sobj_new();
    obj->f_type = Stype_new();
    obj->type = FUNC_OBJ;
    obj->f_type->f_func = func;
    return obj;
}

struct Sobj*
Sobj_set_closure
(struct Sfunc *func) {
    struct Sobj *obj = Sobj_new();
    obj->f_type = Stype_new();
    obj->type = CLOSURE_OBJ;
    obj->f_type->f_func = func;
    return obj;
}

struct Sfunc*
Sfunc_set_code
(struct Sfunc *func, 
    struct Scode *code) {
    func->code = code;
    func->code_size = code->size;
    return func;   
}

struct Sfunc*
Sfunc_insert_code
(struct Sfunc *func, 
    struct Scode *code) {
    struct Scode *old_code = func->code;
    
    Scode_push(old_code, code);
    
    func->code = old_code;
    func->code_size = old_code->size;
    
    return func;
}

struct Sframe*
Sfunc_call_c_api_func
(struct Sframe *frame, 
    struct Sobj* f_obj) {
    if (f_obj->type == BUILTIN_OBJ) {
        Sframe_call_c_api_func(frame, get_c_api_func(f_obj));
        return frame;
    } else {
        Sframe_push_null(frame);
    }

    return frame;
}