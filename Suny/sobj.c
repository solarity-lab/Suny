#include "sobj.h"
#include "smem.h"
#include "sdebug.h"

struct Svalue* 
Svalue_new(void) {
    struct Svalue *value = Smem_Malloc(sizeof(struct Svalue));
    value->value = 0;
    return value;
}

struct Sobj* 
Sobj_new(void) {
    struct Sobj *obj = Smem_Malloc(sizeof(struct Sobj));

    obj->gc = Sgc_new();

    obj->dname = NULL;
    obj->ddoc = NULL;
    obj->next = NULL;
    obj->prev = NULL;
    obj->is_free = 0;

    obj->type = NULL_OBJ;
    obj->value = Svalue_new();
    obj->size = 0;
    obj->address = 0;

    obj->f_value = NULL;
    obj->f_type = NULL;

    obj->global_address = 0;
    obj->local_address = 0;

    obj->is_variable = 0;

    obj->is_global = 0;
    obj->is_local = 0;
    obj->is_calle = 0;
    obj->is_closure = 0;
    obj->is_argument = 0;
    obj->is_return = 0;
    obj->is_belong_class = 0;
    obj->is_super_class_member = 0;

    obj->c_api_func = NULL;

    obj->meta = NULL;

    return obj;
}

int Sobj_free_variable(struct Sobj* obj) {
    if (obj->is_free) {
        return 0;
    }

    obj->is_free = 1;

    Smem_Free(obj->value);
    Smem_Free(obj->gc);
    Smem_Free(obj);
    return 1;
}

int 
Sobj_free
(struct Sobj* obj) {
    SDEBUG("[sobj.c] Sobj_free(struct Sobj* obj) free %p\n", obj);
    if (!obj) {
        __ERROR("Error sobj.c: obj is null\n");
    }

    if (obj->is_free) {
        return 0; 
    }

    obj->is_free = 1;

    if (obj->type == STRING_OBJ) {
        Sstr_free(obj->f_type->f_str);
        Smem_Free(obj->f_type);
    }

    else if (obj->type == LIST_OBJ) {
        Slist_free(obj->f_type->f_list);
        Smem_Free(obj->f_type);
    }

    else if (obj->type == FUNC_OBJ) {
        Sfunc_free(obj->f_type->f_func);
        Smem_Free(obj->f_type);
    }

    else if (obj->type == CLOSURE_OBJ) {
        Senvi_free(obj->f_type->f_func->envi);
        Sfunc_free(obj->f_type->f_func);
        Smem_Free(obj->f_type);
    }

    else if (obj->type == CLASS_OBJ) {
        Sclass_free(obj->f_type->f_class);
        Smem_Free(obj->meta);
        Smem_Free(obj->f_type);
    }

    else if (obj->type == USER_DATA_OBJ) {
        if (obj->meta) {
            if (obj->meta->mm_free) {
                obj->meta->mm_free(obj);
            } else {
                __ERROR("Error: LIKE, DAMN MAN? CODING IN C WITH NO FREE FUNCTION?");
            }

            Smem_Free(obj->meta);
        }
        
        Suserdata_free(obj->f_type->f_userdata);
        
        Stype_free(obj->f_type);
    }

    Smem_Free(obj->value);
    Smem_Free(obj->gc);
    Smem_Free(obj);

    obj = NULL;

    return 0;
}

struct Sobj*
Sobj_make_number
(float value) {
    struct Sobj *obj = Sobj_new();
    obj->type = NUMBER_OBJ;
    obj->value->value = value;
    return obj;
}

int
Sobj_free_objs
(struct Sobj** objs, int size) {
    if (size <= 0) {
        Smem_Free(objs);
        return 0;
    }

    for (int i = 0; i < size; i++) {
        Sobj_free(objs[i]);
    }
    
    Smem_Free(objs);

    return 0;
}

void*
Sobj_get_obj
(struct Sobj* obj, enum Sobj_t type) {
    if (type == FUNC_OBJ) {
        return obj->f_type->f_func;
    } else if (type == USER_DATA_OBJ) {
        return obj->f_type->f_userdata;
    } else if (type == LIST_OBJ) {
        return obj->f_type->f_list;
    } else if (type == STRING_OBJ) {
        return obj->f_type->f_str;
    } else if (type == CLASS_OBJ) {
        return obj->f_type->f_class;
    } else if (type == GLOBAL_OBJ || type == LOCAL_OBJ) {
        return obj->f_value;
    }
    return obj->value;
}

struct Sobj*
Sobj_make_null(void) {
    struct Sobj *obj = Sobj_new();
    obj->type = NULL_OBJ;
    return obj;
}

struct Sobj*
Sobj_make_type(enum Sobj_t type) {
    struct Sobj *obj = Sobj_new();
    obj->type = type;
    return obj;
}