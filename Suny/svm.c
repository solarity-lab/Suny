#include "svm.h"
#include "smem.h"
#include "sdebug.h"

struct Sframe* Svm_evaluate(struct Sframe *frame, byte_t op) {
    switch (op) {

        case AND_LOG:
            frame = Svm_evaluate_AND_LOG(frame);
            break;

        case ADD_LABEL:
            op = get_next_code(frame);
            break;

        case OR_LOG:
            frame = Svm_evaluate_OR_LOG(frame);
            break;

        case NOT_LOG:
            frame = Svm_evaluate_NOT_LOG(frame);
            break;

        case PUSH_FLOAT:
            frame = Svm_evaluate_PUSH_FLOAT(frame);
            break;

        case LOAD_CLOSURE:
            frame = Svm_evaluate_LOAD_CLOSURE(frame);
            break;

        case STORE_CLOSURE:
            frame = Svm_evaluate_STORE_CLOSURE(frame);
            break;

        case MAKE_CLOSURE:
            frame = Svm_evaluate_MAKE_CLOSURE(frame);
            break;

        case LOAD_GLOBAL:
            frame = Svm_evaluate_LOAD_GLOBAL(frame);
            break;

        case STORE_GLOBAL:
            frame = Svm_evaluate_STORE_GLOBAL(frame);
            break;

        case LOAD_LOCAL:
            frame = Svm_evaluate_LOAD_LOCAL(frame);
            break;

        case STORE_LOCAL:
            frame = Svm_evaluate_STORE_LOCAL(frame);
            break;

        case PUSH_STRING:
            frame = Svm_evaluate_PUSH_STRING(frame);
            break;

        case BUILD_LIST:
            frame = Svm_evaluate_BUILD_LIST(frame);
            break;

        case LEN_OF:
            frame = Svm_evaluate_LEN_OF(frame);
            break;

        case POP_JUMP_IF_FALSE:
            frame = Svm_evaluate_POP_JUMP_IF_FALSE(frame);
            break;

        case MAKE_FUNCTION:
            frame = Svm_evaluate_MAKE_FUNCTION(frame);
            break;

        case FUNCTION_CALL:
            frame = Svm_evaluate_FUNCTION_CALL(frame);
            break;

        case LOAD_NULL:
            frame = Svm_evaluate_LOAD_NULL(frame);
            break;

        case JUMP_TO:
            frame = Svm_evaluate_JUMP_TO(frame);
            break;

        case LOAD_ATTR:
            frame = Svm_evaluate_LOAD_ATTR(frame);
            break;

        case STORE_ATTR:
            frame = Svm_evaluate_STORE_ATTR(frame);
            break;

        case CLASS_BEGIN:
            frame = Svm_evaluate_CLASS_BEGIN(frame);
            break;

        case POP_TOP:
            frame = Svm_evaluate_POP(frame);
            break;

        case LOAD_ITEM:
            frame = Svm_evaluate_LOAD_ITEM(frame);
            break;

        case STORE_ITEM:
            frame = Svm_evaluate_STORE_ITEM(frame);
            break;

        case IS_LOG:
            frame = Svm_evaluate_IS_LOG(frame);
            break;

        case PRINT:
            frame = Svm_evaluate_PRINT(frame);
            break;

        case LOAD_TRUE:
            frame = Svm_evaluate_LOAD_TRUE(frame);
            break;

        case LOAD_FALSE:
            frame = Svm_evaluate_LOAD_FALSE(frame);
            break;

        case EXIT_PROGRAM:
            frame = Svm_evaluate_EXIT_PROGRAM(frame);
            break;

        case STOP_PROGRAM:
            frame = Svm_evaluate_STOP_PROGRAM(frame);
            break;

        case LOOP_PREP:
            frame = Svm_evaluate_LOOP_PREP(frame);
            break;

        case LOOP_STEP:
            frame = Svm_evaluate_LOOP_STEP(frame);
            break;

        default:
            if (IS_BINARY_OPER(op)) {
                frame = Svm_evalutate_BINARY_OPER(frame, op);
            } else {
                __ERROR("Error: unknown opcode %s\n", print_op(op));
                exit(1);
            }
            break;
    }

    return frame;
}

byte_t
get_next_code
(struct Sframe *frame) {
    if (frame->f_code_index > frame->f_code->size) {
        __ERROR("Error: program ended at opcode 0x%02X\n", frame->f_code->code[frame->f_code->size - 1]);
    }
    return frame->f_code->code[frame->f_code_index++];
}

byte_t
jump_to(struct Sframe *frame, int address) {
    struct Spos pos = Slabel_map_get(frame->f_label_map, address);
    int index = pos.indexof;
    frame->f_code_index = index - 1;

    return frame->f_code->code[frame->f_code_index];
}

SUNY_API struct Sframe*
Svm_run_program(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_run_program(struct Sframe *frame) (building...)\n");

    if (!frame->f_heaps) {
        printf("Error: no heap\n");
    }

    if (!frame->gc_pool) {
        printf("Error: no garbage collector\n");
    }

    Sgc_activate(frame);

    byte_t op = get_next_code(frame);
    
    if (op == PROGRAM_START) {
        op = get_next_code(frame);
        
        while (1) {
            if (op == PROGRAM_END) {
                break;
            }

            frame = Svm_evaluate(frame, op);
             
            op = get_next_code(frame);
            
            if (frame->gc_pool->pool_index > POOL_SIZE_LIMIT) {
                Sgc_collect(frame->gc_pool);
            }
        }
    } else {
        __ERROR("Invalid program head start %s\n", print_op(op));
    }

    Sgc_deactivate(frame);   

    SDEBUG("[svm.c] Svm_run_program(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_run_call_context(struct Scall_context *context) {
    SDEBUG("[svm.c] Svm_run_call_context(struct Scall_context *context) (building...)\n");

    struct Sframe *f_frame = context->frame;

    if (!f_frame->f_heaps) {
        __ERROR("Error: no heap\n");
    }

    if (!f_frame->gc_pool) {
        __ERROR("Error: no garbage pool\n");
    }

    byte_t op = get_next_code(f_frame);

    context->ret_obj = null_obj;
    context->ret_obj->is_return = 1;

    while (1) {
        if (op == END_FUNCTION || op == END_CLOSURE) {
            break;  
        }
        
        if (op == RETURN_TOP) {        // return top
            f_frame = Svm_evaluate_RETURN_TOP(f_frame, context);
            break;
        }
        
        else {
            f_frame = Svm_evaluate(f_frame, op);
        }

        if (f_frame->gc_pool->pool_index > POOL_SIZE_LIMIT) {
            Sgc_collect(f_frame->gc_pool);
        }

        op = get_next_code(f_frame);
    }

    Sframe_push(context->main_frame, context->ret_obj);

    SDEBUG("[svm.c] Svm_run_call_context(struct Scall_context *context) (done)\n");

    return context->main_frame;
}

SUNY_API struct Sframe*
Svm_evaluate_FUNCTION_CALL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_FUNCTION_CALL(struct Sframe *frame) (building...)\n");

    struct Sobj *f_obj = Sframe_pop(frame);
    
    if (f_obj->type == BUILTIN_OBJ) Sframe_call_c_api_func(frame, get_c_api_func(f_obj));
    else if (f_obj->type == FUNC_OBJ && f_obj->prev) Svm_call_class_function(frame, f_obj);
    else if (f_obj->type == CLASS_OBJ) Sclass_call(frame, f_obj);
    else if (f_obj->type == CLOSURE_OBJ) Svm_call_closure(frame, f_obj);
    else if (f_obj->type == FUNC_OBJ) Svm_call_function(frame, f_obj);
    else Sframe_push_null(frame);

    MOVETOGC(f_obj, frame->gc_pool);
    
    SDEBUG("[svm.c] Svm_evaluate_FUNCTION_CALL(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_PUSH_FLOAT
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_PUSH_FLOAT(struct Sframe *frame) (building...)\n");

    byte_t b1 = get_next_code(frame); // 0x00
    byte_t b2 = get_next_code(frame); // 0x00
    byte_t b3 = get_next_code(frame); // 0x80
    byte_t b4 = get_next_code(frame); // 0x3F

    uint32_t i = (uint32_t)b1           |
                ((uint32_t)b2 << 8)     |
                ((uint32_t)b3 << 16)    |
                ((uint32_t)b4 << 24);

    float value;
    memcpy(&value, &i, sizeof(value));

    Sframe_push_number(frame, value);

    SDEBUG("[svm.c] Svm_evaluate_PUSH_FLOAT(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_GLOBAL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_GLOBAL(struct Sframe *frame) (building...)\n");

    int address = get_next_code(frame);
    struct Sobj *obj = Sframe_load_global(frame, address);

    Sframe_push(frame, obj->f_value);
    
    SDEBUG("[svm.c] Svm_evaluate_LOAD_GLOBAL(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_STORE_GLOBAL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_STORE_GLOBAL(struct Sframe *frame) (building...)\n");

    struct Sobj *obj = Sframe_pop(frame);

    int address = get_next_code(frame);

    obj->is_global = 1;

    Sframe_store_global(frame, address, obj, GLOBAL_OBJ);

    SDEBUG("[svm.c] Svm_evaluate_STORE_GLOBAL(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evalutate_BINARY_OPER
(struct Sframe *frame, byte_t op) {
    SDEBUG("[svm.c] Svm_evalutate_BINARY_OPER(struct Sframe *frame, byte_t op) (building...)\n");

    switch (op) {
        case BINARY_ADD: {
            Seval_evaluate_add(frame);
            break;
        } case BINARY_SUB: {
            Seval_evaluate_sub(frame);
            break;
        } case BINARY_MUL: {
            Seval_evaluate_mul(frame);
            break;
        } case BINARY_DIV: {
            Seval_evaluate_div(frame);
            break;
        } case BINARY_BIGGER: {
            Seval_evaluate_bigger(frame);
            break;
        } case BINARY_SMALLER: {
            Seval_evaluate_smaller(frame);
            break;
        } case BINARY_EQUAL: {
            Seval_evaluate_equal(frame);
            break;
        } case BINARY_NOT_EQUAL: {
            Seval_evaluate_not_equal(frame);
            break;
        } case BINARY_BIGGER_EQUAL: {
            Seval_evaluate_bigger_and_equal(frame);
            break;
        } case BINARY_SMALLER_EQUAL: {
            Seval_evaluate_smaller_and_equal(frame);
            break;
        } case BINARY_MOD: {
            Seval_evaluate_mod(frame);
            break;
        } default: {
            Sframe_push_null(frame);
            break;
        }
    }

    SDEBUG("[svm.c] Svm_evalutate_BINARY_OPER(struct Sframe *frame, byte_t op) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_PRINT
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_PRINT(struct Sframe *frame) (building...)\n");

    struct Sobj *obj = POP_OBJ();
    float val = obj->value->value;

    printf("%g\n", val);
    
    MOVETOGC(obj, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_PRINT(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_PUSH_STRING
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_PUSH_STRING(struct Sframe *frame) (building...)\n");

    int size = get_next_code(frame);

    char* buff = Smem_Malloc(size + 1);

    for (int i = 0; i < size; ++i) {
        buff[i] = get_next_code(frame);
    }

    buff[size] = '\0';

    Sframe_push_string(frame, buff, size);

    Smem_Free(buff);

    SDEBUG("[svm.c] Svm_evaluate_PUSH_STRING(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_POP_JUMP_IF_FALSE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_POP_JUMP_IF_FALSE(struct Sframe *frame) (building...)\n");

    int address = get_next_code(frame);

    struct Sobj *obj = Sframe_pop(frame);

    if (!obj->value->value) {
        jump_to(frame, address);
    }

    MOVETOGC(obj, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_POP_JUMP_IF_FALSE(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_POP_JUMP_IF_TRUE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_POP_JUMP_IF_TRUE(struct Sframe *frame) (building...)\n");

    int address = get_next_code(frame);

    struct Sobj *obj = Sframe_pop(frame);

    if (obj->value->value) {
        jump_to(frame, address);
    }

    MOVETOGC(obj, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_POP_JUMP_IF_TRUE(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_JUMP_TO
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_JUMP_TO(struct Sframe *frame) (building...)\n");

    int address = get_next_code(frame);
    jump_to(frame, address);

    SDEBUG("[svm.c] Svm_evaluate_JUMP_TO(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_BUILD_LIST
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_BUILD_LIST(struct Sframe *frame) (building...)\n");

    int size = get_next_code(frame);

    struct Slist *list = Slist_new();

    for (int i = 0; i < size; ++i) {
        struct Sobj *item = Sframe_pop(frame);
        _SUNYINCREF(item);
        
        Slist_add(list, item);
    };

    struct Sobj *obj = Sobj_make_list(list);

    Sframe_push(frame, obj);

    SDEBUG("[svm.c] Svm_evaluate_BUILD_LIST(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_ITEM
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_ITEM(struct Sframe *frame) (building...)\n");

    struct Sobj *index = Sframe_pop(frame);
    struct Sobj *list = Sframe_pop(frame);

    if (index->value->value < 0) {
        __ERROR("Error frame.c: index under of range: %f\n", index->value->value);
        return frame;
    }

    if (list->type == LIST_OBJ) {
        if (index->value->value >= list->f_type->f_list->count) {
            __ERROR("Error frame.c: index out of range: %d\n", index->value->value);
            return frame;
        };

        struct Sobj *item = Slist_get(list->f_type->f_list, index->value->value);

        if (!item) {
            Sframe_push(frame, null_obj);
        }

        Sframe_push(frame, item);
    } else if (list->type == STRING_OBJ) {
        if (index->value->value > list->f_type->f_str->size) {
            Sframe_push(frame, null_obj);
            return frame;
        };

        int index_value = index->value->value;
        char c = list->f_type->f_str->string[index_value];
        struct Sobj *obj = Sobj_make_char(c);

        if (!obj) {
            Sframe_push(frame, null_obj);
        }

        Sframe_push(frame, obj);
    } else if (list->type == USER_DATA_OBJ) {
        struct Sobj *ret = NULL;
        if (list->meta && list->meta->mm_index) {
            ret = list->meta->mm_index(list, index);
            Sframe_push(frame, ret);
        } else {
            Sframe_push(frame, null_obj);
        }
    } else {
        Sframe_push(frame, null_obj);
    }

    MOVETOGC(list, frame->gc_pool);
    MOVETOGC(index, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_LOAD_ITEM(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_STORE_ITEM
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_STORE_ITEM(struct Sframe *frame) (building...)\n");

    struct Sobj *value = Sframe_pop(frame);
    struct Sobj *index = Sframe_pop(frame);
    struct Sobj *list = Sframe_pop(frame);

    if (list->type == USER_DATA_OBJ) {
        if (list->meta && list->meta->mm_store_item) {
            list->meta->mm_store_item(list, index, value);
        }

        MOVETOGC(index, frame->gc_pool);
        MOVETOGC(list, frame->gc_pool);
        MOVETOGC(value, frame->gc_pool);

        return frame;
    }

    if (list->type == LIST_OBJ) {
        struct Slist *slist = list->f_type->f_list;
        int index_value = index->value->value;

        if (index_value < 0) {
            index_value = 0 - index_value + 1;
        }

        if (index_value >= slist->capacity) {
            slist->capacity *= 2;
            slist->array = (struct Sobj**)Smem_Realloc(slist->array, sizeof(struct Sobj*) * slist->capacity);
        }

        if (index_value >= slist->count) {
            for (int i = slist->count; i <= index_value; ++i) {
                Slist_add(slist, null_obj);
            }
        }
        
        struct Sobj *pre_item = slist->array[index_value];
        slist->array[index_value] = value;
        
        _SUNYDECREF(pre_item);
        MOVETOGC(pre_item, frame->gc_pool);
    }

    _SUNYINCREF(value);

    MOVETOGC(value, frame->gc_pool);
    MOVETOGC(index, frame->gc_pool);
    MOVETOGC(list, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_STORE_ITEM(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LEN_OF
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LEN_OF(struct Sframe *frame) (building...)\n");

    struct Sobj *list = Sframe_pop(frame);

    if (list->type == LIST_OBJ) {
        struct Sobj *obj = Svalue(list->f_type->f_list->count);
        Sframe_push(frame, obj);
    } else if (list->type == STRING_OBJ) {
        struct Sobj *obj = Svalue(list->f_type->f_str->size);
        Sframe_push(frame, obj);
    } else {
        Sframe_push(frame, null_obj);
    }

    MOVETOGC(list, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_LEN_OF(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_TRUE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_TRUE(struct Sframe *frame) (building...)\n");
    
    Sframe_push_bool(frame, 1);

    SDEBUG("[svm.c] Svm_evaluate_LOAD_TRUE(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_FALSE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_FALSE(struct Sframe *frame) (building...)\n");
    
    Sframe_push_bool(frame, 0);

    SDEBUG("[svm.c] Svm_evaluate_LOAD_FALSE(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_LOCAL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_LOCAL(struct Sframe *frame) (building...)\n");
    int address = get_next_code(frame);
    struct Sobj *obj = Sframe_load_local(frame, address);

    PUSH_OBJ(obj->f_value);

    SDEBUG("[svm.c] Svm_evaluate_LOAD_LOCAL(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_STORE_LOCAL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_STORE_LOCAL(struct Sframe *frame) (building...)\n");
    struct Sobj *obj = Sframe_pop(frame);

    int address = get_next_code(frame);

    Sframe_store_local(frame, address, obj, LOCAL_OBJ);
    SDEBUG("[svm.c] Svm_evaluate_STORE_LOCAL(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_SET_SHARED_CLASS
(struct Sframe *frame, struct Sclass *sclass) {
    SDEBUG("[svm.c] Svm_evaluate_SET_SHARED_CLASS(struct Sframe *frame) (building...)\n");

    int address = get_next_code(frame);

    struct Sobj* super_class = Sframe_load_global(frame, address)->f_value;

    if (super_class->type != CLASS_OBJ) {
        return frame;
    }

    sclass = Sclass_shared_class(sclass, super_class->f_type->f_class);

    SDEBUG("[svm.c] Svm_evaluate_SET_SHARED_CLASS(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_SET_SUPER_CLASS
(struct Sframe *frame, struct Sclass *sclass) {
    SDEBUG("[svm.c] Svm_evaluate_SET_SUPER_CLASS(struct Sframe *frame) (building...)\n");
    
    int address = get_next_code(frame);

    struct Sobj* super_class = Sframe_load_global(frame, address)->f_value;

    if (super_class->type != CLASS_OBJ) {
        return frame;
    }

    sclass = Sclass_extends_class(sclass, super_class->f_type->f_class);
    
    SDEBUG("[svm.c] Svm_evaluate_SET_SUPER_CLASS(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_MEMBER
(struct Sframe *frame, struct Sclass *sclass) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_MEMBER(struct Sframe *frame) (building...)\n");
    int address = get_next_code(frame);
    struct Sobj *obj = Sclass_get_object(sclass, address);

    if (obj == NULL) {
        __ERROR("Error: attribute not found\n");
    }

    Sframe_push(frame, obj);
    SDEBUG("[svm.c] Svm_evaluate_LOAD_MEMBER(struct Sframe *frame) (done)\n");
    return frame;
};

SUNY_API struct Sframe*
Svm_evaluate_STORE_MEMBER
(struct Sframe *frame, struct Sclass *sclass) {
    SDEBUG("[svm.c] Svm_evaluate_STORE_MEMBER(struct Sframe *frame) (building...)\n");
    int address = get_next_code(frame);
    sclass = Sclass_store_object(sclass, frame, address);
    
    SDEBUG("[svm.c] Svm_evaluate_STORE_MEMBER(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_CLASS_BEGIN
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_CLASS_BEGIN(struct Sframe *frame) (building...)\n");
    byte_t op = get_next_code(frame);

    struct Sclass *sclass = Sclass_new();

    struct Sobj *obj = Sobj_make_class(sclass);

    while (1) {
        if (op == CLASS_END) {
            break;
        }

        else if (op == STORE_MEMBER) {
            frame = Svm_evaluate_STORE_MEMBER(frame, sclass);
        }

        else if (op == LOAD_MEMBER) {
            frame = Svm_evaluate_LOAD_MEMBER(frame, sclass);
        }

        else if (op == SET_SUPER_CLASS) {
            frame = Svm_evaluate_SET_SUPER_CLASS(frame, sclass);
        }

        else if (op == SET_SHARED_CLASS) {
            frame = Svm_evaluate_SET_SHARED_CLASS(frame ,sclass);
        }

        else {
            frame = Svm_evaluate(frame, op);
        }

        if (frame->gc_pool->pool_index > POOL_SIZE_LIMIT) {
            Sgc_collect(frame->gc_pool);
        }

        op = get_next_code(frame);
    }

    SDEBUG("[svm.c] Svm_evaluate_CLASS_BEGIN(struct Sframe *frame) (done)\n");

    Sframe_push(frame, obj);

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_NOT_LOG
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_NOT_LOG(struct Sframe *frame) (building...)\n");

    struct Sobj *obj = Sframe_pop(frame);

    int value = !obj->value->value;

    Sframe_push_bool(frame, value);

    MOVETOGC(obj, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_NOT_LOG(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_AND_LOG
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_AND_LOG(struct Sframe *frame) (building...)\n");

    struct Sobj *obj1 = Sframe_pop(frame);
    struct Sobj *obj2 = Sframe_pop(frame);

    int value = obj1->value->value && obj2->value->value;

    Sframe_push_bool(frame, value);

    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    SDEBUG("[svm.c] Svm_evaluate_AND_LOG(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_OR_LOG
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_OR_LOG(struct Sframe *frame) (building...)\n");
    struct Sobj *obj1 = Sframe_pop(frame);
    struct Sobj *obj2 = Sframe_pop(frame);

    int value = obj1->value->value || obj2->value->value;
    
    Sframe_push_bool(frame, value);

    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_OR_LOG(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_ATTR
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_ATTR(struct Sframe *frame) (building...)\n");
    int address = get_next_code(frame);

    struct Sobj* class = Sframe_pop(frame);

    if (class->type == CLASS_OBJ) {
        struct Sclass *sclass = class->f_type->f_class;

        struct Sobj* value = Sclass_get_object(sclass, address);

        if (value) {
            value->f_value->prev = class;
            Sframe_push(frame, value->f_value);
        } else {
            Sframe_push_null(frame);
        }
    } else {
        Sframe_push_null(frame);
    }

    MOVETOGC(class, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_LOAD_ATTR(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_STORE_ATTR
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_STORE_ATTR(struct Sframe *frame) (building...)\n");
    struct Sobj* class = Sframe_pop(frame);
    struct Sobj* value = Sframe_pop(frame);

    int address = get_next_code(frame);

    if (class->type == CLASS_OBJ) {
        struct Sclass *sclass = class->f_type->f_class;
        Sclass_store_local_obj(sclass, frame, value, address);
    }

    MOVETOGC(class, frame->gc_pool);
    MOVETOGC(value, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_STORE_ATTR(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_NULL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_NULL(struct Sframe *frame) (building...)\n");
    Sframe_push(frame, null_obj);
    SDEBUG("[svm.c] Svm_evaluate_LOAD_NULL(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_EXIT_PROGRAM
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_EXIT_PROGRAM(struct Sframe *frame) (building...)\n");
    
    SUNY_EXIT;

    SDEBUG("[svm.c] Svm_evaluate_EXIT_PROGRAM(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_STOP_PROGRAM
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_STOP_PROGRAM(struct Sframe *frame) (building...)\n");
    
    SUNY_BREAK_POINT;

    SDEBUG("[svm.c] Svm_evaluate_STOP_PROGRAM(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_POP(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_POP(struct Sframe *frame) (building...)\n");
    struct Sobj *obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    SDEBUG("[svm.c] Svm_evaluate_POP(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_RETURN_TOP(struct Sframe *frame, struct Scall_context *context) {
    SDEBUG("[svm.c] Svm_evaluate_RETURN_TOP(struct Sframe *frame) (building...)\n");
    
    struct Sobj *obj = Sframe_pop(frame);

    obj->is_return = 1;

    Sobj_free(context->ret_obj);

    context->ret_obj = obj; 

    SDEBUG("[svm.c] Svm_evaluate_RETURN_TOP(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_call_function(struct Sframe *frame, struct Sobj *f_obj) {
    SDEBUG("[svm.c] Svm_call_function(struct Sframe *frame, struct Sobj *f_obj) (building...)\n");

    struct Scall_context *context = Scall_context_new();

    Scall_context_set_frame(context, frame, f_obj);
    Svm_run_call_context(context);
    Scall_context_free(context);

    SDEBUG("[svm.c] Svm_call_function(struct Sframe *frame, struct Sobj *f_obj) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_call_closure(struct Sframe *frame, struct Sobj *f_obj) {
    SDEBUG("[svm.c] Svm_call_closure(struct Sframe *frame, struct Sobj *f_obj) (building...)\n");

    struct Scall_context *context = Scall_context_new();

    Scall_context_set_closure(context, frame, f_obj);
    Svm_run_call_context(context);
    Scall_context_free(context);

    SDEBUG("[svm.c] Svm_call_closure(struct Sframe *frame, struct Sobj *f_obj) (done)\n");
    return frame;
} 

SUNY_API struct Sframe*
Svm_evaluate_STORE_CLOSURE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_STORE_CLOSURE(struct Sframe *frame) (building...)\n");
    int address = get_next_code(frame);

    struct Sobj *obj = Sframe_pop(frame);

    Sframe_store_closure(frame, address, obj, LOCAL_OBJ);

    SDEBUG("[svm.c] Svm_evaluate_STORE_CLOSURE(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_IS_LOG
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_IS_LOG(struct Sframe *frame) (building...)\n");

    struct Sobj *obj1 = Sframe_pop(frame);
    struct Sobj *obj2 = Sframe_pop(frame);

    Sframe_push_bool(frame, obj1 == obj2);

    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);

    SDEBUG("[svm.c] Svm_evaluate_IS_LOG(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_CLOSURE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_CLOSURE(struct Sframe *frame) (building...)\n");
    int address = get_next_code(frame);

    struct Sobj *obj = Sframe_load_closure(frame, address);

    Sframe_push(frame, obj->f_value);

    SDEBUG("[svm.c] Svm_evaluate_LOAD_CLOSURE(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOOP_PREP   
(struct Sframe *frame) {
    int address = get_next_code(frame);
    int label = get_next_code(frame);

    struct Sobj* loop_counter = Sframe_load_global(frame, address);

    if (loop_counter->f_value->value->value <= 0) {
        jump_to(frame, label);
    }

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOOP_STEP   
(struct Sframe *frame) {
    int address = get_next_code(frame);
    int label = get_next_code(frame);

    struct Sobj* loop_counter = Sframe_load_global(frame, address);

    loop_counter->f_value->value->value--;

    if (loop_counter->f_value->value->value != 0) {
        jump_to(frame, label);
    }

    return frame;
}

SUNY_API struct Sframe*
Svm_call_class_function(struct Sframe *frame, struct Sobj *f_obj) {
    SDEBUG("[svm.c] Svm_call_function(struct Sframe *frame, struct Sobj *f_obj) (building...)\n");

    struct Scall_context *context = Scall_context_new();

    Scall_context_set_class_function(context, frame, f_obj);
    Svm_run_call_context(context);
    Scall_context_free(context);

    SDEBUG("[svm.c] Svm_call_function(struct Sframe *frame, struct Sobj *f_obj) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_MAKE_FUNCTION
(struct Sframe *frame) {
    SDEBUG("[svm.c 401] Svm_evaluate_MAKE_FUNCTION(struct Sframe *frame) (building...)\n");

    byte_t op = get_next_code(frame);
    int args_count = op;

    int code_size = 0;

    struct Scode *code = Scode_new();
    struct Sfunc *func = Sfunc_new();

    op = get_next_code(frame);

    while (op != START_FUNCTION) {
        if (op == MAKE_ARGS) {
            op = get_next_code(frame);
            func->args_address[func->args_index++] = op;
            func->args_size++;
        }

        op = get_next_code(frame);
    }

    op = get_next_code(frame);
     
    while (op != END_FUNCTION) {
        PUSH(code, op);

        ++code_size;

        op = get_next_code(frame);

        if (op == END_FUNCTION) break;
    }

    PUSH(code, END_FUNCTION);

    if (args_count != func->args_size) {
        __ERROR("Invalid number of arguments");
        return NULL;
    }

    func->code = code;
    func->code_size = code_size + 1;

    struct Sobj *f_obj = Sobj_set_func(func);

    Sframe_push(frame, f_obj);

    SDEBUG("[svm.c] Svm_evaluate_MAKE_FUNCTION(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_MAKE_CLOSURE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_MAKE_CLOSURE(struct Sframe *frame, struct Scall_context *context) (building...)\n");

    int address_box[1024];
    int address_box_size = 0;

    struct Sfunc *func = Sfunc_new();

    byte_t op = get_next_code(frame);

    struct Senvi *f_envi = Senvi_new();

    int code_size = 0;

    struct Scode *code = Scode_new();

    op = get_next_code(frame); 

    while (op != START_FUNCTION) {
        if (op == MAKE_ARGS) {
            op = get_next_code(frame);
            func->args_address[func->args_index++] = op;
            func->args_size++;
        }

        op = get_next_code(frame);
    }

    op = get_next_code(frame);

    int func_level = 1;

    SDEBUG("[svm.c] Svm_evaluate_MAKE_CLOSURE(struct Sframe *frame, struct Scall_context *context) (building function...)\n");
    while (1) {
        if (op == LOAD_CLOSURE || op == STORE_CLOSURE) {
            int address = get_next_code(frame);
            PUSH(code, op);
            PUSH(code, address);

            address_box[address_box_size++] = address;

            code_size += 2;

            op = get_next_code(frame);
            continue;   
        }
        
        if (op == END_CLOSURE) {
            func_level--;
        } else if (op == MAKE_CLOSURE) {
            func_level++;
        }

        if (op == END_CLOSURE && func_level == 0) break;

        PUSH(code, op);

        ++code_size;

        op = get_next_code(frame);
    }

    SDEBUG("[svm.c] Svm_evaluate_MAKE_CLOSURE(struct Sframe *frame, struct Scall_context *context) (done function)\n");

    for (int i = 0; i < address_box_size; i++) {
        for (int j = 0; j < frame->f_locals_size; j++) {
            if (frame->f_locals[j]->address == address_box[i]) {
                Senvi_store(f_envi, frame->f_locals[j]);
            }
        }
    }

    PUSH(code, END_CLOSURE);

    func->code = code;
    func->code_size = code_size + 1;

    func->envi = f_envi;

    struct Sobj *f_obj = Sobj_set_closure(func);

    Sframe_push(frame, f_obj);

    SDEBUG("[svm.c] Svm_evaluate_MAKE_CLOSURE(struct Sframe *frame, struct Scall_context *context) (done)\n");
    return frame;   
}