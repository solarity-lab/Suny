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
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
        struct Sstr *str = Sstr_add(str1, str2);
        
        struct Sobj *sobj = Sobj_make_string_object(str);
        
        return sobj;
    } else if (op == __mul) {
        struct Sstr *str = NULL;
        int size = 0;
        if (TypeOf(obj1) == STRING_OBJ) {
            str = tget_str(obj1);
            size = ValueOf(obj2);
        } else if (TypeOf(obj2) == STRING_OBJ) {
            str = tget_str(obj2);
            size = ValueOf(obj1);
        } else {
            __ERROR("attempt to perform arithmetic on a null value\n");
        }
        
        struct Sstr *sstr = Sstr_mul(str, size);
        
        struct Sobj *sobj = Sobj_make_string_object(sstr);
        
        return sobj;
    } else if (op == __mod) {
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
        struct Sstr *str = Sstr_mod(str1, str2);
        
        struct Sobj *sobj = Sobj_make_string_object(str);
        
        return sobj;
    } else if (op == __eq) {
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
        int result = Scharcmp_equal(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ne) {
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
        int result = !Scharcmp_equal(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __gt) {
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
        int result = Scharcmp_bigger(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __lt) {
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
        int result = Scharcmp_smaller(str1->string, str2->string, str1->size, str2->size);
        
        if (result) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ge) {
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
        int result = Scharcmp_bigger(str1->string, str2->string, str1->size, str2->size);
        int result2 = Scharcmp_equal(str1->string, str2->string, str1->size, str2->size);
        
        if (result || result2) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __le) {
        struct Sstr *str1 = tget_str(obj1);
        struct Sstr *str2 = tget_str(obj2);
        
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
        struct Slist *list1 = tget_list(obj1);
        struct Slist *list2 = tget_list(obj2);
        
        struct Slist *list = Slist_append(list1, list2);
        
        struct Sobj *sobj = Sobj_make_list(list);
        
        return sobj;
    } else if (op == __mul) {
        struct Slist *list = NULL;
        int size = 0;
        if (TypeOf(obj1) == LIST_OBJ) {
            list = tget_list(obj1);
            size = ValueOf(obj2);
        } else if (TypeOf(obj2) == LIST_OBJ) {
            list = tget_list(obj2);
            size = ValueOf(obj1);
        } else {
            __ERROR("attempt to perform arithmetic on a null value\n");
        }
        
        struct Slist *slist = Slist_mul(list, size);
        
        struct Sobj *sobj = Sobj_make_list(slist);
        
        return sobj;
    } else if (op == __eq) {
        struct Slist *list1 = tget_list(obj1);
        struct Slist *list2 = tget_list(obj2);
        
        int result = Slist_cmp(list1, list2);
        
        if (result == 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ne) {
        struct Slist *list1 = tget_list(obj1);
        struct Slist *list2 = tget_list(obj2);
        
        int result = Slist_cmp(list1, list2);
        
        if (result != 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __lt) {
        struct Slist *list1 = tget_list(obj1);
        struct Slist *list2 = tget_list(obj2);
        
        int result = Slist_cmp(list1, list2);
        
        if (result < 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __gt) {
        struct Slist *list1 = tget_list(obj1);
        struct Slist *list2 = tget_list(obj2);
        
        int result = Slist_cmp(list1, list2);
        
        if (result > 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __le) {
        struct Slist *list1 = tget_list(obj1);
        struct Slist *list2 = tget_list(obj2);
        
        int result = Slist_cmp(list1, list2);
        
        if (result <= 0) {
            return true_obj;
        } else {
            return false_obj;
        }
    } else if (op == __ge) {
        struct Slist *list1 = tget_list(obj1);
        struct Slist *list2 = tget_list(obj2);
        
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
        if (MetaOf(obj1) && MetaOf(obj1)->mm_add) {
            return MetaOf(obj1)->mm_add(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_add) {
            return MetaOf(obj2)->mm_add(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __sub) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_sub) {
            return MetaOf(obj1)->mm_sub(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_sub) {
            return MetaOf(obj2)->mm_sub(obj1, obj2);
        } 
        else {
            return null_obj;
        }
    } else if (op == __mul) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_mul) {
            return MetaOf(obj1)->mm_mul(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_mul) {
            return MetaOf(obj2)->mm_mul(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __div) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_div) {
            return MetaOf(obj1)->mm_div(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_div) {
            return MetaOf(obj2)->mm_div(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __eq) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_eq) {
            return MetaOf(obj1)->mm_eq(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_eq) {
            return MetaOf(obj2)->mm_eq(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __ne) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_ne) {
            return MetaOf(obj1)->mm_ne(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_ne) {
            return MetaOf(obj2)->mm_ne(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __lt) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_lt) {
            return MetaOf(obj1)->mm_lt(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_lt) {
            return MetaOf(obj2)->mm_lt(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __gt) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_gt) {
            return MetaOf(obj1)->mm_gt(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_gt) {
            return MetaOf(obj2)->mm_gt(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __le) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_le) {
            return MetaOf(obj1)->mm_le(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_le) {
            return MetaOf(obj2)->mm_le(obj1, obj2);
        } else {
            return null_obj;
        }
    } else if (op == __ge) {
        if (MetaOf(obj1) && MetaOf(obj1)->mm_ge) {
            return MetaOf(obj1)->mm_ge(obj1, obj2);
        } else if (MetaOf(obj2) && MetaOf(obj2)->mm_ge) {
            return MetaOf(obj2)->mm_ge(obj1, obj2);
        } else {
            return null_obj;
        }
    } 
 
    return NULL;
}

struct Sobj* Seval_mod(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __mod);
    return Svalue((int) ValueOf(obj1) % (int) ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_add
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if  (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __add);
    else if (TypeOf(obj1) == LIST_OBJ && TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __add);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __add);

    return Svalue(ValueOf(obj1) + ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_sub
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __sub);
    return Svalue(ValueOf(obj1) - ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_mul
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (TypeOf(obj1) == STRING_OBJ || TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __mul);
    else if (TypeOf(obj1) == LIST_OBJ || TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __mul);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __mul);
    return Svalue(ValueOf(obj1) * ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_div
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __div);
    return Svalue(ValueOf(obj1) / ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_bigger
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __gt);
    else if (TypeOf(obj1) == LIST_OBJ && TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __gt);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __gt);
    return Sobj_make_bool(ValueOf(obj1) > ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_smaller
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) __ERROR("attempt to perform arithmetic on a null value\n");
    if (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __lt);
    else if (TypeOf(obj1) == LIST_OBJ && TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __lt);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __lt);
    return Sobj_make_bool(ValueOf(obj1) < ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) {
        if (TypeOf(obj1) == NULL_OBJ && TypeOf(obj2) == NULL_OBJ) {
            return true_obj;
        } else {
            return false_obj;
        }
    }

    if (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __eq);
    else if (TypeOf(obj1) == LIST_OBJ && TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __eq);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __eq);
    return Sobj_make_bool(ValueOf(obj1) == ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_not_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) {
        if (TypeOf(obj1) == NULL_OBJ && TypeOf(obj2) == NULL_OBJ) {
            return false_obj;
        } else {
            return true_obj;
        }
    }

    if (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __ne);
    else if (TypeOf(obj1) == LIST_OBJ && TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __ne);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __ne);

    return Sobj_make_bool(ValueOf(obj1) != ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_bigger_and_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) {
        if (TypeOf(obj1) == NULL_OBJ && TypeOf(obj2) == NULL_OBJ) {
            return true_obj;
        } else {
            return false_obj;
        }
    }

    if (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __ge);
    else if (TypeOf(obj1) == LIST_OBJ && TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __ge);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __ge);
    return Sobj_make_bool(ValueOf(obj1) >= ValueOf(obj2));
}

SUNY_API struct Sobj*
Seval_smaller_and_equal
(struct Sobj *obj1, struct Sobj *obj2) {
    if (TypeOf(obj1) == NULL_OBJ || TypeOf(obj2) == NULL_OBJ) {
        if (TypeOf(obj1) == NULL_OBJ && TypeOf(obj2) == NULL_OBJ) {
            return true_obj;
        } else {
            return false_obj;
        }
    }

    if (TypeOf(obj1) == STRING_OBJ && TypeOf(obj2) == STRING_OBJ) return eval_string(obj1, obj2, __le);
    else if (TypeOf(obj1) == LIST_OBJ && TypeOf(obj2) == LIST_OBJ) return eval_list(obj1, obj2, __le);
    else if (TypeOf(obj1) == USER_DATA_OBJ || TypeOf(obj2) == USER_DATA_OBJ) return eval_userdata(obj1, obj2, __le);
    return Sobj_make_bool(ValueOf(obj1) <= ValueOf(obj2));
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

    if (TypeOf(obj1) == CLASS_OBJ) {
        if (MetaOf(obj1)->meta_f_add) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_add);
    } 
    
    else if (TypeOf(obj2) == CLASS_OBJ) {
        if (MetaOf(obj2)->meta_f_add) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_add);
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

    if (TypeOf(obj1) == CLASS_OBJ) {
        if (MetaOf(obj1)->meta_f_sub) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_sub);
    }
    
    else if (TypeOf(obj2) == CLASS_OBJ) {
        if (MetaOf(obj2)->meta_f_sub) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_sub);
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

    if (TypeOf(obj1) == CLASS_OBJ) {
        if (MetaOf(obj1)->meta_f_mul) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_mul);
    }

    else if (TypeOf(obj2) == CLASS_OBJ) {
        if (MetaOf(obj2)->meta_f_mul) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_mul);
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

    if (TypeOf(obj1) == CLASS_OBJ) {
        if (MetaOf(obj1)->meta_f_div) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_div);
    } 
    
    else if (TypeOf(obj2) == CLASS_OBJ) {
        if (MetaOf(obj2)->meta_f_div) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_div);
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

    if (TypeOf(obj1) == CLASS_OBJ) if (MetaOf(obj1)->meta_f_ge) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_ge);
    if (TypeOf(obj2) == CLASS_OBJ) if (MetaOf(obj2)->meta_f_ge) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_ge);

    struct Sobj *sobj = Seval_bigger(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);

    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_smaller(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (TypeOf(obj1) == CLASS_OBJ) if (MetaOf(obj1)->meta_f_le) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_le);
    if (TypeOf(obj2) == CLASS_OBJ) if (MetaOf(obj2)->meta_f_le) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_le);

    struct Sobj *sobj = Seval_smaller(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}   

SUNY_API int Seval_evaluate_equal(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (TypeOf(obj1) == CLASS_OBJ) if (MetaOf(obj1)->meta_f_eq) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_eq);
    if (TypeOf(obj2) == CLASS_OBJ) if (MetaOf(obj2)->meta_f_eq) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_eq);

    struct Sobj *sobj = Seval_equal(obj1, obj2);
    
    MOVETOGC(obj1, frame->gc_pool);
    MOVETOGC(obj2, frame->gc_pool);
    
    Sframe_push(frame, sobj);

    return 0;
}

SUNY_API int Seval_evaluate_not_equal(struct Sframe *frame) {
    struct Sobj *obj2 = Sframe_pop(frame);
    struct Sobj *obj1 = Sframe_pop(frame);

    if (TypeOf(obj1) == CLASS_OBJ) if (MetaOf(obj1)->meta_f_ne) return eval_class(frame, obj1, obj2, (void*) MetaOf(obj1)->meta_f_ne);
    if (TypeOf(obj2) == CLASS_OBJ) if (MetaOf(obj2)->meta_f_ne) return eval_class(frame, obj2, obj1, (void*) MetaOf(obj2)->meta_f_ne);

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