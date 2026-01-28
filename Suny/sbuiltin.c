#include "sbuiltin.h"
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

SUNY_API struct Sobj* Sisdigit_builtin(struct Sframe *frame) {
    struct Sobj *obj = Sframe_pop(frame);

    char* str = obj->f_type->f_str->string;

    if (Sisstrdigit(str)) {
        MOVETOGC(obj, frame->gc_pool);
        return Svalue(1);
    }

    MOVETOGC(obj, frame->gc_pool);
    return Svalue(0);
}

SUNY_API struct Sobj* Sputs(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);

    Sio_write(obj);

    MOVETOGC(obj, frame->gc_pool);

    return null_obj;
}

SUNY_API struct Sobj* Sprint(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);

    if (obj->type == CLASS_OBJ) {
        if (obj->meta->meta_f_tostring) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 1);
            args[0] = obj;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj->meta->meta_f_tostring, args);
            Svm_run_call_context(context);
            Scall_context_free(context);
            
            Smem_Free(args);

            struct Sobj *value = Sframe_pop(frame);
            Sio_write(value);
            MOVETOGC(value, frame->gc_pool);

            return null_obj;
        }
    }

    Sio_write(obj);

    printf("\n");

    MOVETOGC(obj, frame->gc_pool);

    return null_obj;
}

SUNY_API struct Sobj* Sread(struct Sframe* frame) {
    struct Sobj *write = Sframe_pop(frame);

    Sio_write(write);

    char buffer[1024];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        struct Sobj *empty = Sobj_make_string("", 0);
        return empty;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }

    struct Sobj *result = Sobj_make_string(buffer, len);

    MOVETOGC(result, frame->gc_pool);

    return result;
}


SUNY_API struct Sobj* Sexit(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);

    if (obj->value->value != 0) {
        SUNY_EXIT;
    }

    MOVETOGC(obj, frame->gc_pool);

    return null_obj;
}

SUNY_API struct Sobj* Sload_dll(struct Sframe *frame) {
    return null_obj;
}

SUNY_API struct Sobj* Ssize(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);

    if (obj->type == LIST_OBJ) {
        MOVETOGC(obj, frame->gc_pool);
        return Svalue(obj->f_type->f_list->count);
    } else if (obj->type == STRING_OBJ) {
        MOVETOGC(obj, frame->gc_pool);
        return Svalue(obj->f_type->f_str->size);
    }

    return Svalue(0);
}

SUNY_API struct Sobj* Spush(struct Sframe* frame) {
    struct Sobj *value = Sframe_pop(frame);
    struct Sobj *list = Sframe_pop(frame);

    Slist_add(list->f_type->f_list, value);

    _SUNYINCREF(value);
    MOVETOGC(list, frame->gc_pool);

    return list;
}

SUNY_API struct Sobj* Spop(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);

    struct Slist* list = obj->f_type->f_list;

    struct Sobj* last = list->array[list->count - 1];
    list->count -= 1;

    _SUNYDECREF(last);
    MOVETOGC(last, frame->gc_pool);

    return obj;
}

SUNY_API struct Sobj* Srange(struct Sframe *frame) {
    struct Sobj *end = Sframe_pop(frame);
    struct Sobj *start = Sframe_pop(frame);

    int start_value = start->value->value;
    int end_value = end->value->value;

    struct Slist *list = Slist_range(start_value, end_value);
    struct Sobj *obj = Sobj_make_list(list);

    MOVETOGC(start, frame->gc_pool);
    MOVETOGC(end, frame->gc_pool);

    return obj;
}

SUNY_API struct Sobj* Slist_cast(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);
    
    if (obj->type == STRING_OBJ) {
        char* str = obj->f_type->f_str->string;
        struct Slist *list = Slist_from_string_chars(str);
        struct Sobj *result = Sobj_make_list(list);

        MOVETOGC(obj, frame->gc_pool);
        return result;
    }

    MOVETOGC(obj, frame->gc_pool);
    return obj;
}

SUNY_API struct Sobj* Sstring_cast(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);
    char* str = Sio_sprintf(obj);

    struct Sobj *result = Sobj_make_string(str, strlen(str));

    Smem_Free(str);

    MOVETOGC(obj, frame->gc_pool);
    return result;
}

SUNY_API struct Sobj* Sint_cast(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);
    if (obj->type == STRING_OBJ) {
        char* str = obj->f_type->f_str->string;
        int int_value = Satof(str);
        return Svalue(int_value);
    }

    int value = obj->value->value;

    MOVETOGC(obj, frame->gc_pool);
    return Svalue(value);;
}

SUNY_API struct Sobj* Sfloat_cast(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Svalue(obj->value->value);
}

SUNY_API struct Sobj* Sbool_cast(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Sobj_make_bool(obj->value->value);
}

SUNY_API struct Sobj* Stype(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);
    struct Sobj *cast = NULL;

    if (obj->type == STRING_OBJ) {
        cast = Sobj_make_string("string", 6);
    } else if (obj->type == NUMBER_OBJ) {
        cast = Sobj_make_string("number", 6);
    } else if (obj->type == TRUE_OBJ || obj->type == FALSE_OBJ) {
        cast = Sobj_make_string("bool", 4);
    } else if (obj->type == LIST_OBJ) {
        cast = Sobj_make_string("list", 4);
    } else if (obj->type == NULL_OBJ) {
        cast = Sobj_make_string("null", 4);
    } else if (obj->type == CLASS_OBJ) {
        cast = Sobj_make_string("class", 5);
    } else if (obj->type == FUNC_OBJ) {
        cast = Sobj_make_string("function", 8);
    } else if (obj->type == USER_DATA_OBJ) {
        cast = Sobj_make_string("userdata", 8);
    } else {
        cast = Sobj_make_string("unknown", 7);
    }

    MOVETOGC(obj, frame->gc_pool);
    return cast;
}

SUNY_API struct Sobj* Scopy(struct Sframe *frame) {
    struct Sobj *obj = Sframe_pop(frame);
    struct Sobj *copy = Sobj_deep_copy(obj);
    MOVETOGC(obj, frame->gc_pool);
    return copy;
}

SUNY_API struct Sobj* Ssystem(struct Sframe* frame) {
    struct Sobj *obj = Sframe_pop(frame);
    char* str = obj->f_type->f_str->string;
    system(str);
    MOVETOGC(obj, frame->gc_pool);
    return null_obj;
}