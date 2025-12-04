#include "svm.h"
#include "smem.h"
#include "sdebug.h"

SUNY_API struct Sobj *Seval_binary_add(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_add(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame); // args 2
    struct Sobj *obj1 = Sframe_pop(frame); // args 1

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_add) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_add, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;

            Smem_Free(args);
     
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_add) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_add, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;

            Smem_Free(args);
     
            return value;
        }
    }

    struct Sobj *sobj = Seval_add(obj1, obj2);

    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_sub(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_sub(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_sub) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_sub, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;

            Smem_Free(args);
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_sub) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_sub, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;

            Smem_Free(args);
            return value;
        }
    }

    struct Sobj *sobj = Seval_sub(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_mul(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_mul(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_mul) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_mul, args);
            Svm_run_call_context(context);
            struct Sobj* value = context->ret_obj;

            Smem_Free(args);
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_mul) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_mul, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;

            Smem_Free(args);
            return value;
        }
    }

    struct Sobj *sobj = Seval_mul(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_div(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_div(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_div) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_div, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_div) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_div, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
            
            Smem_Free(args);
            return value;
        }
    }

    struct Sobj *sobj = Seval_div(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_bigger(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_bigger(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_ge) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_ge, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_ge) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_ge, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    }

    struct Sobj *sobj = Seval_bigger(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_smaller(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_smaller(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_le) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_le, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_le) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_le, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    }

    struct Sobj *sobj = Seval_smaller(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}   

SUNY_API struct Sobj *Seval_binary_equal(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_equal(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_eq) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_eq, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_eq) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_eq, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    }

    struct Sobj *sobj = Seval_equal(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_not_equal(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_not_equal(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_ne) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj1;
            args[1] = obj1;
            args[2] = obj2;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj1->meta->meta_f_ne, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    } else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_ne) {
            struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 3);
            args[0] = obj2;
            args[1] = obj2;
            args[2] = obj1;

            struct Scall_context *context = Scall_context_new();

            Scall_context_set_frame_with_args(context, frame, obj2->meta->meta_f_ne, args);
            Svm_run_call_context(context);

            struct Sobj* value = context->ret_obj;
     
            Smem_Free(args);
            return value;
        }
    }

    struct Sobj *sobj = Seval_not_equal(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_bigger_and_equal(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_bigger_and_equal(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    struct Sobj *sobj = Seval_bigger_and_equal(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

SUNY_API struct Sobj *Seval_binary_smaller_and_equal(struct Sframe *frame) {
    SDEBUG("[svm.c] Seval_binary_smaller_and_equal(struct Sframe *frame)\n");
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    struct Sobj *sobj = Seval_smaller_and_equal(obj1, obj2);
    
    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
    return sobj;
}

byte_t
get_next_code
(struct Sframe *frame) {
    if (frame->f_code_index > frame->f_code->size) {
        __ERROR("Error: program ended\n");
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
    int done = 0;
    
    if (op == PROGRAM_START) {
        op = get_next_code(frame);
        
        while (!done) {
            if (op == PROGRAM_END) {
                done = 1;
            }

            else if (op == AND_LOG) {
                frame = Svm_evaluate_AND_LOG(frame);
            }

            else if (op == OR_LOG) {
                frame = Svm_evaluate_OR_LOG(frame);
            }

            else if (op == NOT_LOG) {
                frame = Svm_evaluate_NOT_LOG(frame);
            }

            else if (op == ADD_LABEL) {
                op = get_next_code(frame);
            }

            else if (op == PUSH_FLOAT) {
                frame = Svm_evalutate_PUSH_FLOAT(frame);
            }

            else if (op == LOAD_GLOBAL) {
                frame = Svm_evalutate_LOAD_GLOBAL(frame);
            }

            else if (op == STORE_GLOBAL) {
                frame = Svm_evalutate_STORE_GLOBAL(frame);
            }

            else if (op == PUSH_STRING) {
                frame = Svm_evaluate_PUSH_STRING(frame);
            }

            else if (op == BUILD_LIST) {
                frame = Svm_evaluate_BUILD_LIST(frame);
            }

            else if (op == LEN_OF) {
                frame = Svm_evaluate_LEN_OF(frame);
            }

            else if (op == POP_JUMP_IF_FALSE) {
                frame = Svm_evaluate_POP_JUMP_IF_FALSE(frame);
            }

            else if (op == LOAD_ATTR) {
                frame = Svm_evaluate_LOAD_ATTR(frame);
            }

            else if (op == MAKE_FUNCTION) {
                frame = Svm_evaluate_MAKE_FUNCTION(frame);
            }
            
            else if (op == FUNCTION_CALL) {
                frame = Svm_evaluate_FUNCTION_CALL(frame);
            }

            else if (op == LOAD_NULL) {
                frame = Svm_evaluate_LOAD_NULL(frame);
            }

            else if (op == JUMP_TO) {
                frame = Svm_evaluate_JUMP_TO(frame);
            }

            else if (IS_BINARY_OPER(op)) {
                frame = Svm_evalutate_BINARY_OPER(frame, op);
            }

            else if (op == STORE_ATTR) {
                frame = Svm_evaluate_STORE_ATTR(frame);
            }

            else if (op == CLASS_BEGIN) {
                frame = Svm_evaluate_CLASS_BEGIN(frame);
            }

            else if (op == POP_TOP) {
                frame = Svm_evaluate_POP(frame);
            }

            else if (op == LOAD_ITEM) {
                frame = Svm_evaluate_LOAD_ITEM(frame);
            } 

            else if (op == STORE_ITEM) {
                frame = Svm_evaluate_STORE_ITEM(frame);
            }

            else if (op == PRINT) {
                frame = Svm_evaluate_PRINT(frame);
            } 

            else if (op == LOAD_TRUE) {
                frame = Svm_evaluate_LOAD_TRUE(frame);
            }

            else if (op == LOAD_FALSE) {
                frame = Svm_evaluate_LOAD_FALSE(frame);
            } 

            else {
                __ERROR("Invalid op %s\n", print_op(op));
                SUNY_EXIT;
            }
             
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

    int done = 0;

    while (!done) {
        if (op == END_FUNCTION || op == END_CLOSURE) {
            done = 1;
        }
        
        else if (op == ADD_LABEL) {
            op = get_next_code(f_frame);
        }

        else if (op == POP_TOP) {
            f_frame = Svm_evaluate_POP(f_frame);
        }

        else if (op == AND_LOG) {
            f_frame = Svm_evaluate_AND_LOG(f_frame);
        }

        else if (op == OR_LOG) {
            f_frame = Svm_evaluate_OR_LOG(f_frame);
        }

        else if (op == NOT_LOG) {
            f_frame = Svm_evaluate_NOT_LOG(f_frame);
        }

        else if (IS_BINARY_OPER(op)) {
            f_frame = Svm_evalutate_BINARY_OPER(f_frame, op);
        }

        else if (op == PRINT) {
            f_frame = Svm_evaluate_PRINT(f_frame);
        }

        else if (op == RETURN_TOP) {
            f_frame = Svm_evaluate_RETURN_TOP(f_frame, context);
            break;
        }

        else if (op == PUSH_FLOAT) {
            f_frame = Svm_evalutate_PUSH_FLOAT(f_frame);
        }

        else if (op == LOAD_ATTR) {
            f_frame = Svm_evaluate_LOAD_ATTR(f_frame);
        }

        else if (op == STORE_ATTR) {
            f_frame = Svm_evaluate_STORE_ATTR(f_frame);
        }

        else if (op == CLASS_BEGIN) {
            f_frame = Svm_evaluate_CLASS_BEGIN(f_frame);
        }

        else if (op == FUNCTION_CALL) {
            f_frame = Svm_evaluate_FUNCTION_CALL(f_frame);
        }

        else if (op == PUSH_STRING) {
            f_frame = Svm_evaluate_PUSH_STRING(f_frame);
        }

        else if (op == LOAD_GLOBAL) {
            f_frame = Svm_evaluate_LOAD_LOCAL(f_frame);
        }

        else if (op == LOAD_CLOSURE) {
            f_frame = Svm_evaluate_LOAD_CLOSURE(f_frame);
        }

        else if (op == STORE_CLOSURE) {
            f_frame = Svm_evaluate_STORE_CLOSURE(f_frame);
        }

        else if (op == MAKE_CLOSURE) {
            f_frame = Svm_evaluate_MAKE_CLOSURE(f_frame);
        }
        
        else if (op == STORE_GLOBAL) {
            f_frame = Svm_evaluate_STORE_LOCAL(f_frame);
        }

        else if (op == JUMP_TO) {
            f_frame = Svm_evaluate_JUMP_TO(f_frame);
        }

        else if (op == POP_JUMP_IF_FALSE) {
            f_frame = Svm_evaluate_POP_JUMP_IF_FALSE(f_frame);
        }

        else if (op == LOAD_NULL) {
            f_frame = Svm_evaluate_LOAD_NULL(f_frame);
        }

        else if (op == BUILD_LIST) {
            f_frame = Svm_evaluate_BUILD_LIST(f_frame);
        }

        else if (op == LEN_OF) {
            f_frame = Svm_evaluate_LEN_OF(f_frame);
        }

        else if (op == LOAD_ITEM) {
            f_frame = Svm_evaluate_LOAD_ITEM(f_frame);
        }

        else if (op == STORE_ITEM) {
            f_frame = Svm_evaluate_STORE_ITEM(f_frame);
        }

        else if (op == LOAD_TRUE) {
            f_frame = Svm_evaluate_LOAD_TRUE(f_frame);
        }

        else if (op == LOAD_FALSE) {
            f_frame = Svm_evaluate_LOAD_FALSE(f_frame);
        }

        else {
            __ERROR("Error: unknown opcode in function: %s\n", print_op(op));
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
Svm_evaluate_MAKE_FUNCTION
(struct Sframe *frame) {
    SDEBUG("[svm.c 401] Svm_evaluate_MAKE_FUNCTION(struct Sframe *frame) (building...)\n");

    byte_t args_count = get_next_code(frame);

    int code_size = 0;

    struct Scode *code = Scode_new();

    int func_level = 1;

    byte_t op = get_next_code(frame);

    while (1) {
        if (op == END_FUNCTION) {
            func_level--;
        } else if (op == MAKE_FUNCTION) {
            func_level++;
        }

        if (op == END_FUNCTION && func_level == 0) break;

        PUSH(code, op);

        ++code_size;

        op = get_next_code(frame);
    }

    PUSH(code, END_FUNCTION);

    struct Sfunc *func = Sfunc_set(code, args_count, code_size);

    struct Sobj *f_obj = Sobj_set_func(func);

    Sframe_push(frame, f_obj);

    SDEBUG("[svm.c] Svm_evaluate_MAKE_FUNCTION(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_FUNCTION_CALL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_FUNCTION_CALL(struct Sframe *frame) (building...)\n");

    struct Sobj *f_obj = Sframe_pop(frame);
    
    if (f_obj->type == BUILTIN_OBJ) Sfunc_call_c_api_func(frame, f_obj);
    else if (f_obj->type == CLASS_OBJ) Sclass_call(frame, f_obj);
    else if (f_obj->type == CLOSURE_OBJ) Svm_call_closure(frame, f_obj);
    else if (f_obj->type == FUNC_OBJ) Svm_call_func(frame, f_obj);

    SUNYDECREF(f_obj, frame->gc_pool);
    
    SDEBUG("[svm.c] Svm_evaluate_FUNCTION_CALL(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evalutate_PUSH_FLOAT
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evalutate_PUSH_FLOAT(struct Sframe *frame) (building...)\n");

    byte_t b1 = get_next_code(frame);
    byte_t b2 = get_next_code(frame);
    byte_t b3 = get_next_code(frame);
    byte_t b4 = get_next_code(frame);

    uint32_t i = (uint32_t)b1 |
                ((uint32_t)b2 << 8) |
                ((uint32_t)b3 << 16) |
                ((uint32_t)b4 << 24);

    float value;
    memcpy(&value, &i, sizeof(value));

    Sframe_push_number(frame, value);

    SDEBUG("[svm.c] Svm_evalutate_PUSH_FLOAT(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evalutate_LOAD_GLOBAL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evalutate_LOAD_GLOBAL(struct Sframe *frame) (building...)\n");

    int address = get_next_code(frame);
    struct Sobj *obj = Sframe_load_global(frame, address);

    Sframe_push(frame, obj->f_value);
    
    SDEBUG("[svm.c] Svm_evalutate_LOAD_GLOBAL(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evalutate_STORE_GLOBAL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evalutate_STORE_GLOBAL(struct Sframe *frame) (building...)\n");

    struct Sobj *obj = Sframe_pop(frame);

    int address = get_next_code(frame);

    obj->global_address = address;
    obj->is_global = 1;

    Sframe_store_global(frame, address, obj, GLOBAL_OBJ);

    SDEBUG("[svm.c] Svm_evalutate_STORE_GLOBAL(struct Sframe *frame) (done)\n");

    return frame;
}

SUNY_API struct Sframe*
Svm_evalutate_BINARY_OPER
(struct Sframe *frame, byte_t op) {
    SDEBUG("[svm.c] Svm_evalutate_BINARY_OPER(struct Sframe *frame, byte_t op) (building...)\n");

    struct Sobj *obj = NULL;

    switch (op) {
        case BINARY_ADD: {
            obj = Seval_binary_add(frame);
            break;
        } case BINARY_SUB: {
            obj = Seval_binary_sub(frame);
            break;
        } case BINARY_MUL: {
            obj = Seval_binary_mul(frame);
            break;
        } case BINARY_DIV: {
            obj = Seval_binary_div(frame);
            break;
        } case BINARY_BIGGER : {
            obj = Seval_binary_bigger(frame);
            break;
        } case BINARY_SMALLER : {
            obj = Seval_binary_smaller(frame);
            break;
        } case BINARY_EQUAL : {
            obj = Seval_binary_equal(frame);
            break;
        } case BINARY_NOT_EQUAL : {
            obj = Seval_binary_not_equal(frame);
            break;
        } case BINARY_BIGGER_EQUAL : {
            obj = Seval_binary_bigger_and_equal(frame);
            break;
        } case BINARY_SMALLER_EQUAL : {
            obj = Seval_binary_smaller_and_equal(frame);
            break;
        } default: {
            __ERROR("Error: unknown binary operator\n");
            break;
        }
    }

    Sframe_push(frame, obj);

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
    
    SUNYDECREF(obj, frame->gc_pool);

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

    SUNYDECREF(obj, frame->gc_pool);

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

    SUNYDECREF(obj, frame->gc_pool);

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
        __ERROR("Error: index below zero, index: %f\n", index->value->value);
        return frame;
    }

    if (list->type == LIST_OBJ) {
        if (index->value->value >= list->f_type->f_list->count) {
            __ERROR("Error: index out of range, index: %f, size: %d\n", index->value->value, list->f_type->f_list->count);
            return frame;
        };

        struct Sobj *item = Slist_get(list->f_type->f_list, index->value->value);

        if (!item) {
            __ERROR("Error: item is null\n");
        }

        Sframe_push(frame, item);
    } else if (list->type == STRING_OBJ) {
        if (index->value->value > list->f_type->f_str->size) {
            __ERROR("Error: index out of range\n");
            return frame;
        };

        int index_value = index->value->value;
        char c = list->f_type->f_str->string[index_value];
        struct Sobj *obj = Sobj_make_char(c);

        if (!obj) {
            __ERROR("Error: index out of range\n");
        }

        Sframe_push(frame, obj);
    } else if (list->type == USER_DATA_OBJ) {
        struct Sobj *ret = NULL;
        if (list->meta && list->meta->mm_index) {
            ret = list->meta->mm_index(list, index);
            Sframe_push(frame, ret);
        } else {
            __ERROR("Error: user data index not supported\n");
        }
    } else {
        Sframe_push(frame, Svalue(0));
    }

    SUNYDECREF(list, frame->gc_pool);
    SUNYDECREF(index, frame->gc_pool);

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
        } else {
            __ERROR("Error: user data store item not supported\n");
        }

        SUNYDECREF(index, frame->gc_pool);

        return frame;
    }

    int index_value = index->value->value;

    struct Sobj *pre_item = list->f_type->f_list->array[index_value];

    list->f_type->f_list->array[index_value] = value;

    _SUNYINCREF(value);
    _SUNYDECREF(pre_item);

    SUNYDECREF(pre_item, frame->gc_pool);
    SUNYDECREF(index, frame->gc_pool);

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
        Sframe_push_null(frame);
    }

    SUNYDECREF(list, frame->gc_pool);

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
Svm_evaluate_CLASS_BEGIN
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_CLASS_BEGIN(struct Sframe *frame) (building...)\n");
    byte_t op = get_next_code(frame);

    struct Sclass *sclass = Sclass_new();

    struct Sobj *obj = Sobj_make_class(sclass);

    while (op != CLASS_END) {
        if (op == ADD_LABEL) {
            op = get_next_code(frame);
        }

        else if (op == LOAD_ATTR) {
            frame = Svm_evaluate_LOAD_ATTR(frame);
        }

        else if (op == STORE_ATTR) {
            frame = Svm_evaluate_STORE_ATTR(frame);
        }

        else if (op == AND_LOG) {
            frame = Svm_evaluate_AND_LOG(frame);
        }

        else if (op == OR_LOG) {
            frame = Svm_evaluate_OR_LOG(frame);
        }

        else if (op == NOT_LOG) {
            frame = Svm_evaluate_NOT_LOG(frame);
        }

        else if (op == PUSH_FLOAT) {
            frame = Svm_evalutate_PUSH_FLOAT(frame);
        }

        else if (op == LOAD_GLOBAL) {
            frame = Svm_evalutate_LOAD_GLOBAL(frame);
        }

        else if (op == STORE_GLOBAL) {
            int address = get_next_code(frame);
            sclass = Sclass_store_object(sclass, frame, address);
        }

        else if (op == CLASS_BEGIN) {
            frame = Svm_evaluate_CLASS_BEGIN(frame);
        }

        else if (op == PUSH_STRING) {
            frame = Svm_evaluate_PUSH_STRING(frame);
        }

        else if (op == BUILD_LIST) {
            frame = Svm_evaluate_BUILD_LIST(frame);
        }

        else if (op == LOAD_NULL) {
            frame = Svm_evaluate_LOAD_NULL(frame);
        }

        else if (op == LEN_OF) {
            frame = Svm_evaluate_LEN_OF(frame);
        }

        else if (op == POP_JUMP_IF_FALSE) {
            frame = Svm_evaluate_POP_JUMP_IF_FALSE(frame);
        }

        else if (op == MAKE_FUNCTION) {
            frame = Svm_evaluate_MAKE_FUNCTION(frame);
        }
        
        else if (op == FUNCTION_CALL) {
            frame = Svm_evaluate_FUNCTION_CALL(frame);
        }

        else if (op == JUMP_TO) {
            frame = Svm_evaluate_JUMP_TO(frame);
        }

        else if (IS_BINARY_OPER(op)) {
            frame = Svm_evalutate_BINARY_OPER(frame, op);
        }

        else if (op == POP_TOP) {
            frame = Svm_evaluate_POP(frame);
        }

        else if (op == LOAD_ITEM) {
            frame = Svm_evaluate_LOAD_ITEM(frame);
        }

        else if (op == STORE_ITEM) {
            frame = Svm_evaluate_STORE_ITEM(frame);
        }

        else if (op == PRINT) {
            frame = Svm_evaluate_PRINT(frame);
        } 

        else if (op == LOAD_TRUE) {
            frame = Svm_evaluate_LOAD_TRUE(frame);
        }

        else if (op == LOAD_FALSE) {
            frame = Svm_evaluate_LOAD_FALSE(frame);
        }
        
        else {
            __ERROR("Invalid program in class %s\n", print_op(op));
            exit(1);
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

    SUNYDECREF(obj, frame->gc_pool);

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

    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);
    
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

    SUNYDECREF(obj1, frame->gc_pool);
    SUNYDECREF(obj2, frame->gc_pool);

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

        value->f_value->prev = class;

        if (value != NULL) {
            Sframe_push(frame, value->f_value);
        } else {
            Sframe_push(frame, null_obj);
        }
    } else {
        Sframe_push(frame, null_obj);
    }

    SUNYDECREF(class, frame->gc_pool);

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

    SDEBUG("[svm.c] Svm_evaluate_STORE_ATTR(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_NULL
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_NULL(struct Sframe *frame) (building...)\n");
    Sframe_push_null(frame);
    SDEBUG("[svm.c] Svm_evaluate_LOAD_NULL(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_EXIT_PROGRAM
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_EXIT_PROGRAM(struct Sframe *frame) (building...)\n");
    exit(0);
    SDEBUG("[svm.c] Svm_evaluate_EXIT_PROGRAM(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_STOP_PROGRAM
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_STOP_PROGRAM(struct Sframe *frame) (building...)\n");
    exit(1);
    SDEBUG("[svm.c] Svm_evaluate_STOP_PROGRAM(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_POP(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_POP(struct Sframe *frame) (building...)\n");
    struct Sobj *obj = Sframe_pop(frame);
    SUNYDECREF(obj, frame->gc_pool);
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
Svm_call_func(struct Sframe *frame, struct Sobj *f_obj) {
    SDEBUG("[svm.c] Svm_call_func(struct Sframe *frame, struct Sobj *f_obj) (building...)\n");

    struct Scall_context *context = Scall_context_new();

    Scall_context_set_frame(context, frame, f_obj);
    Svm_run_call_context(context);
    Scall_context_free(context);

    SDEBUG("[svm.c] Svm_call_func(struct Sframe *frame, struct Sobj *f_obj) (done)\n");
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

    Sframe_store_local(frame, address, obj, LOCAL_OBJ);

    SDEBUG("[svm.c] Svm_evaluate_STORE_CLOSURE(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_LOAD_CLOSURE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_LOAD_CLOSURE(struct Sframe *frame) (building...)\n");
    int address = get_next_code(frame);

    struct Sobj *obj = Sframe_load_local(frame, address);

    Sframe_push(frame, obj->f_value);

    SDEBUG("[svm.c] Svm_evaluate_LOAD_CLOSURE(struct Sframe *frame) (done)\n");
    return frame;
}

SUNY_API struct Sframe*
Svm_evaluate_MAKE_CLOSURE
(struct Sframe *frame) {
    SDEBUG("[svm.c] Svm_evaluate_MAKE_CLOSURE(struct Sframe *frame, struct Scall_context *context) (building...)\n");

    int address_box[1024];
    int address_box_size = 0;

    byte_t fargs_count = get_next_code(frame);

    struct Senvi *f_envi = Senvi_new();

    int code_size = 0;

    struct Scode *code = Scode_new();

    byte_t op = get_next_code(frame); 

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

    struct Sfunc *func = Sfunc_set(code, fargs_count, code_size);

    func->envi = f_envi;

    struct Sobj *f_obj = Sobj_set_closure(func);

    Sframe_push(frame, f_obj);
    
    SDEBUG("[svm.c] Svm_evaluate_MAKE_CLOSURE(struct Sframe *frame, struct Scall_context *context) (done)\n");
    return frame;   
}