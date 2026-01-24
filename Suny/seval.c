#include "seval.h"
#include "smem.h"
#include "sdebug.h"

#define __add 0
#define __sub 1
#define __mul 2
#define __div 3
#define __gt 4
#define __lt 5
#define __eq 6
#define __ne 7
#define __ge 8
#define __le 9
#define __and 10
#define __or 11
#define __not 12
#define __mod 13

int eval_class(struct Sframe *frame, struct Sobj *class_obj, struct Sobj *obj, void* meta_f) {
    if (meta_f) {
        struct Sobj** args = (struct Sobj**)Smem_Malloc(sizeof(struct Sobj*) * 2);
        args[0] = class_obj;
        args[1] = obj;

        struct Scall_context *context = Scall_context_new();

        Scall_context_set_frame_with_args(context, frame, meta_f, args);
        Svm_run_call_context(context);
        Scall_context_free(context);

        Smem_Free(args);

        MOVETOGC(class_obj, frame->gc_pool);
        MOVETOGC(obj, frame->gc_pool);

        return 0;
    }

    return -1;
}

struct Sobj *eval_string(struct Sobj *obj1, struct Sobj *obj2, int op) {
    if (op == __add) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        struct Sstr *str = Sstr_add(str1, str2);
        
        struct Sobj *sobj = Sobj_make_string_object(str);
        
        return sobj;
    } else if (op == __mul) {
        struct Sstr *str = NULL;
        int size = 0;
        if (obj1->type == STRING_OBJ) {
            str = obj1->f_type->f_str;
            size = obj2->value->value;
        } else if (obj2->type == STRING_OBJ) {
            str = obj2->f_type->f_str;
            size = obj1->value->value;
        } else {
            __ERROR("attempt to perform arithmetic on a null value\n");
        }
        
        struct Sstr *sstr = Sstr_mul(str, size);
        
        struct Sobj *sobj = Sobj_make_string_object(sstr);
        
        return sobj;
    } else if (op == __mod) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        struct Sstr *str = Sstr_mod(str1, str2);
        
        struct Sobj *sobj = Sobj_make_string_object(str);
        
        return sobj;
    } else if (op == __eq) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        int result = Scharcmp_equal(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ne) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        int result = !Scharcmp_equal(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __gt) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        int result = Scharcmp_bigger(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __lt) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        int result = Scharcmp_smaller(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ge) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        int result = Scharcmp_bigger(str1->string, str2->string, str1->size, str2->size);
        int result2 = Scharcmp_equal(str1->string, str2->string, str1->size, str2->size);
        
        if (result || result2) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __le) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;
        
        int result = Scharcmp_smaller(str1->string, str2->string, str1->size, str2->size);
        int result2 = Scharcmp_equal(str1->string, str2->string, str1->size, str2->size);
        
        if (result || result2) {
            return true_obj;
        } else {
            return false_obj;
        }
    }

    return NULL;
}

struct Sobj* eval_list(struct Sobj *obj1, struct Sobj *obj2, int op) {
    if (op == __add) {
        struct Slist *list1 = obj1->f_type->f_list;
        struct Slist *list2 = obj2->f_type->f_list;
        
        struct Slist *list = Slist_append(list1, list2);
        
        struct Sobj *sobj = Sobj_make_list(list);
        
        return sobj;
    } else if (op == __mul) {
        struct Slist *list = NULL;
        int size = 0;
        if (obj1->type == LIST_OBJ) {
            list = obj1->f_type->f_list;
            size = obj2->value->value;
        } else if (obj2->type == LIST_OBJ) {
            list = obj2->f_type->f_list;
            size = obj1->value->value;
        } else {
            __ERROR("attempt to perform arithmetic on a null value\n");
        }
        
        struct Slist *slist = Slist_mul(list, size);
        
        struct Sobj *sobj = Sobj_make_list(slist);
        
        return sobj;
    } else if (op == __eq) {
        struct Slist *list1 = obj1->f_type->f_list;
        struct Slist *list2 = obj2->f_type->f_list;
        
        int result = Slist_cmp(list1, list2);
        
        if (result == 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ne) {
        struct Slist *list1 = obj1->f_type->f_list;
        struct Slist *list2 = obj2->f_type->f_list;
        
        int result = Slist_cmp(list1, list2);
        
        if (result != 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __lt) {
        struct Slist *list1 = obj1->f_type->f_list;
        struct Slist *list2 = obj2->f_type->f_list;
        
        int result = Slist_cmp(list1, list2);
        
        if (result < 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __gt) {
        struct Slist *list1 = obj1->f_type->f_list;
        struct Slist *list2 = obj2->f_type->f_list;
        
        int result = Slist_cmp(list1, list2);
        
        if (result > 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __le) {
        struct Slist *list1 = obj1->f_type->f_list;
        struct Slist *list2 = obj2->f_type->f_list;
        
        int result = Slist_cmp(list1, list2);
        
        if (result <= 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ge) {
        struct Slist *list1 = obj1->f_type->f_list;
        struct Slist *list2 = obj2->f_type->f_list;
        
        int result = Slist_cmp(list1, list2);
        
        if (result >= 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else {
        __ERROR("unknown operator\n");
    }

    return NULL;
}

struct Sobj *eval_userdata(struct Sobj *obj1, struct Sobj *obj2, int op) {
    if (op == __add) {
        if (obj1->meta && obj1->meta->mm_add) {
            return obj1->meta->mm_add(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_add) {
            return obj2->meta->mm_add(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __sub) {
        if (obj1->meta && obj1->meta->mm_sub) {
            return obj1->meta->mm_sub(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_sub) {
            return obj2->meta->mm_sub(obj1, obj2);
        } 
        else {
            return null_obj;
        }
    } else if (op == __mul) {
        if (obj1->meta && obj1->meta->mm_mul) {
            return obj1->meta->mm_mul(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_mul) {
            return obj2->meta->mm_mul(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __div) {
        if (obj1->meta && obj1->meta->mm_div) {
            return obj1->meta->mm_div(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_div) {
            return obj2->meta->mm_div(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __eq) {
        if (obj1->meta && obj1->meta->mm_eq) {
            return obj1->meta->mm_eq(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_eq) {
            return obj2->meta->mm_eq(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __ne) {
        if (obj1->meta && obj1->meta->mm_ne) {
            return obj1->meta->mm_ne(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_ne) {
            return obj2->meta->mm_ne(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __lt) {
        if (obj1->meta && obj1->meta->mm_lt) {
            return obj1->meta->mm_lt(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_lt) {
            return obj2->meta->mm_lt(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __gt) {
        if (obj1->meta && obj1->meta->mm_gt) {
            return obj1->meta->mm_gt(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_gt) {
            return obj2->meta->mm_gt(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __le) {
        if (obj1->meta && obj1->meta->mm_le) {
            return obj1->meta->mm_le(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_le) {
            return obj2->meta->mm_le(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __ge) {
        if (obj1->meta && obj1->meta->mm_ge) {
            return obj1->meta->mm_ge(obj1, obj2);
        } else if (obj2->meta && obj2->meta->mm_ge) {
            return obj2->meta->mm_ge(obj1, obj2);
        } else {
            return null_obj;
        }
    } 
 
    return NULL;
}

struct Sobj* Seval_mod(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __mod);
    return Svalue((int) obj1->value->value % (int) obj2->value->value);
}

SUNY_API struct Sobj*
Seval_add
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __add);
    else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __add);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __add);

    return Svalue(obj1->value->value + obj2->value->value);
}

SUNY_API struct Sobj*
Seval_sub
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __sub);
    return Svalue(obj1->value->value - obj2->value->value);
}

SUNY_API struct Sobj*
Seval_mul
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (obj1->type == STRING_OBJ || obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __mul);
    else if (obj1->type == LIST_OBJ || obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __mul);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __mul);
    return Svalue(obj1->value->value * obj2->value->value);
}

SUNY_API struct Sobj*
Seval_div
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __div);
    return Svalue(obj1->value->value / obj2->value->value);
}

SUNY_API struct Sobj*
Seval_bigger
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __gt);
    else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __gt);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __gt);
    return Sobj_make_bool(obj1->value->value > obj2->value->value);
}

SUNY_API struct Sobj*
Seval_smaller
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __lt);
    else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __lt);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __lt);
    return Sobj_make_bool(obj1->value->value < obj2->value->value);
}

SUNY_API struct Sobj*
Seval_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) {
        if (obj1->type == NULL_OBJ && obj2->type == NULL_OBJ) {
            return true_obj;
        } else {
            return false_obj;
        }
    }

    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __eq);
    else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __eq);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __eq);
    return Sobj_make_bool(obj1->value->value == obj2->value->value);
}

SUNY_API struct Sobj*
Seval_not_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) {
        if (obj1->type == NULL_OBJ && obj2->type == NULL_OBJ) {
            return false_obj;
        } else {
            return true_obj;
        }
    }

    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __ne);
    else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __ne);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __ne);

    return Sobj_make_bool(obj1->value->value != obj2->value->value);
}

SUNY_API struct Sobj*
Seval_bigger_and_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) {
        if (obj1->type == NULL_OBJ && obj2->type == NULL_OBJ) {
            return true_obj;
        } else {
            return false_obj;
        }
    }

    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __ge);
    else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __ge);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __ge);
    return Sobj_make_bool(obj1->value->value >= obj2->value->value);
}

SUNY_API struct Sobj*
Seval_smaller_and_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (obj1->type == NULL_OBJ || obj2->type == NULL_OBJ) {
        if (obj1->type == NULL_OBJ && obj2->type == NULL_OBJ) {
            return true_obj;
        } else {
            return false_obj;
        }
    }

    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) return eval_string(obj1, obj2, __le);
    else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) return eval_list(obj1, obj2, __le);
    else if (obj1->type == USER_DATA_OBJ || obj2->type == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __le);
    return Sobj_make_bool(obj1->value->value <= obj2->value->value);
}

SUNY_API int Seval_evaluate_mod(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    struct Sobj *sobj = Seval_mod(obj1, obj2);

    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);

    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_add(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_add) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_add);
    } 
    
    else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_add) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_add);
    }

    struct Sobj *sobj = Seval_add(obj1, obj2);

    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);

    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_sub(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_sub) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_sub);
    }
    
    else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_sub) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_sub);
    }

    struct Sobj *sobj = Seval_sub(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_mul(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_mul) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_mul);
    }

    else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_mul) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_mul);
    }
    
    struct Sobj *sobj = Seval_mul(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_div(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) {
        if (obj1->meta->meta_f_div) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_div);
    } 
    
    else if (obj2->type == CLASS_OBJ) {
        if (obj2->meta->meta_f_div) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_div);
    }

    struct Sobj *sobj = Seval_div(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_bigger(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) if (obj1->meta->meta_f_ge) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_ge);
    if (obj2->type == CLASS_OBJ) if (obj2->meta->meta_f_ge) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_ge);

    struct Sobj *sobj = Seval_bigger(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);

    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_smaller(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) if (obj1->meta->meta_f_le) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_le);
    if (obj2->type == CLASS_OBJ) if (obj2->meta->meta_f_le) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_le);

    struct Sobj *sobj = Seval_smaller(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}   

SUNY_API int Seval_evaluate_equal(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) if (obj1->meta->meta_f_eq) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_eq);
    if (obj2->type == CLASS_OBJ) if (obj2->meta->meta_f_eq) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_eq);

    struct Sobj *sobj = Seval_equal(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_not_equal(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (obj1->type == CLASS_OBJ) if (obj1->meta->meta_f_ne) return eval_class(frame, obj1, obj2, (void*) obj1->meta->meta_f_ne);
    if (obj2->type == CLASS_OBJ) if (obj2->meta->meta_f_ne) return eval_class(frame, obj2, obj1, (void*) obj2->meta->meta_f_ne);

    struct Sobj *sobj = Seval_not_equal(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_bigger_and_equal(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    struct Sobj *sobj = Seval_bigger_and_equal(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_smaller_and_equal(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    struct Sobj *sobj = Seval_smaller_and_equal(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}