#include "scall.h"
#include "smem.h"
#include "sdebug.h"

/*
    Hàm trợ giúp store tham số vô thanh ảo
*/

int store_args(struct Sobj** temp, struct Sframe *frame, int args_size) {
    for (int i = 0; i < args_size; i++) {
        struct Sobj* back = Sframe_back(frame);
        if (back) {
            struct Sobj* value = Sframe_pop(frame);
            temp[i] = value;
        } else temp[i] = true_obj;
    }
    return 0;
}

/*
    Hàm trợ giúp store tham số vô hàm
*/

int store_args_to_func(struct Sframe *f_frame, struct Sfunc *func, struct Sobj** temp) {
    for (int i = 0; i < func->args_size; i++) {
        if (!temp[i]) continue;
        Sframe_store_local(f_frame, func->args_address[i], temp[i], LOCAL_OBJ);
    }
    return 0;
}

int store_args_function_class(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj, struct Sobj* class_f) {
    struct Sframe *f_frame = context->frame;
    struct Sfunc *func = f_obj->f_type->f_func;
    
    struct Sobj** temp = Smem_Calloc(func->args_size, sizeof(struct Sobj*));

    store_args(temp, frame, func->args_size - 1);
    
    /*
        tham số đầu tiên luôn là chính bản thân hàm
        khi call a.foo(x, y, z)
        thực chất là call a.foo(self, x, y, z)
    */

    if (func->args_size > 0) temp[func->args_size - 1] = class_f;
    
    Sreverse((void **) temp, func->args_size);
    
    store_args_to_func(f_frame, func, temp);
    
    Smem_Free(temp);
    return 0;
}

int store_args_function(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) {
    struct Sfunc *func = f_obj->f_type->f_func;
    struct Sframe *f_frame = context->frame;

    struct Sobj** temp = Smem_Calloc(func->args_size, sizeof(struct Sobj*));    
    
    /*
        Store tham số vô danh sách object tạm thời
    */

    store_args(temp, frame, func->args_size);
    
    /*
        Đảo ngược nó
    */

    Sreverse((void **) temp, f_obj->f_type->f_func->args_size);
    
    /*
        Store vô trong frame của hàm
    */

    store_args_to_func(f_frame, func, temp);
    
    Smem_Free(temp);
    
    return 0;
}

int store_closure_args(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) {
    struct Sfunc *func = f_obj->f_type->f_func;
    struct Senvi* envi = func->envi;
    struct Sframe *f_frame = context->frame;

    /*
        Sao chép biến môi trường được capture của hàm cha vô hàm con
    */
    
    Sarray_copy((void**) f_frame->f_locals, (void**) envi->envi, envi->size);
    /*
        lấy size và index của envi hàm cha
    */
    f_frame->f_locals_size = envi->size;
    f_frame->f_locals_index = envi->size;
    
    struct Sobj** temp = Smem_Calloc(func->args_size, sizeof(struct Sobj*));    
    
    store_args(temp, frame, func->args_size);
    
    Sreverse((void **) temp, func->args_size);
    
    store_args_to_func(f_frame, func, temp);
    
    Smem_Free(temp);
    return 0;
}

struct Scall_context *
Scall_context_new(void) {
    SDEBUG("[sfunc.c] struct Scall_context *Scall_context_new(void) (building...)\n");
    struct Scall_context *context = Smem_Malloc(sizeof(struct Scall_context));
    
    context->frame = Sframe_new();

    context->args_index = 0;
    context->code_index = 0;
    context->local_index = 0;
    context->stack_index = 0;
    
    SDEBUG("[sfunc.c] struct Scall_context *Scall_context_new(void) (done)\n");
    return context;
}

int 
Scall_context_free_frame
(struct Scall_context *context) {
    SDEBUG("[sfunc.c] int Scall_context_free_frame(struct Scall_context *context) (building...)\n");
    struct Sframe *frame = context->frame;

    for (int i = 0; i < frame->f_locals_size; ++i) {
        struct Sobj *local = frame->f_locals[i];
        struct Sobj *value = local->f_value;

        if (!value) __ERROR("Error: value is null\n");

        /*
            Nếu là CLOSURE thì SKIP ví dụ
            func a {
                x = 1
                func b {
                    x = 2 // KHÔNG ĐƯỢC FREE
                }
            }
        */

        if (local->is_closure) continue; 

        if (value->is_return) {

            /*
                Nếu là RETURN thì hủy slot chưa giá trị return đó
            */

            _SUNYDECREF(value); // hủy tức phải dec ref
            Sobj_free(local); // hủy slot
            continue;
        }


        /*
            Hủy slot các biến trong hàm
        */

        _SUNYDECREF(value);
        MOVETOGC(value, frame->gc_pool);

        Sobj_free(local); // hủy slot
    }

    /*
        Free các thứ còn lại không dùng đến như stack, consts, label_map, ...
    */

    Sobj_free_objs(frame->f_stack, frame->f_stack_index);

    Sobj_free_objs(frame->f_consts, frame->f_const_index);

    Slabel_map_free(frame->f_label_map);

    Smem_Free(frame->f_locals); 
    Smem_Free(frame->f_globals);
    Smem_Free(frame);

    SDEBUG("[sfunc.c] int Scall_context_free_frame(struct Scall_context *context) (done)\n");
    return 0;   
}

int 
Scall_context_free
(struct Scall_context *context) {
    SDEBUG("[sfunc.c] int Scall_context_free(struct Scall_context *context) (building...)\n");
    if (context->frame) Scall_context_free_frame(context);

    Smem_Free(context);
    
    SDEBUG("[sfunc.c] int Scall_context_free(struct Scall_context *context) (done)\n");
    return 0;
}

struct Scall_context*
Scall_context_set
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context->main_frame = frame;
    
    struct Sframe *f_frame = context->frame;
    
    struct Scode *f_code = f_obj->f_type->f_func->code;

    /*
        Copy biến toàn cục các frame dùng chung bảng toàn cục
    */
    
    Sarray_copy((void**) f_frame->f_globals, (void**) frame->f_globals, frame->f_globals_size);
    
    f_frame->f_globals_size = frame->f_globals_size;
    f_frame->f_globals_index = frame->f_globals_index;
        
    /*
        Các Sframe dùng chung một HEAP KHÔNG DÙNG RIÊNG
    */

    f_frame->f_heaps = frame->f_heaps;
    f_frame->f_heap_size = frame->f_heap_size;
    f_frame->f_heap_index = frame->f_heap_index;

    /*
        Các Sframe dùng chung một GC KHÔNG DÙNG RIÊNG 
    */

    f_frame->gc_pool = frame->gc_pool;

    /*
        khởi tạo thứ còn lại
    */

    f_frame->f_code = f_code;
    f_frame->f_label_map = Slabel_map_set_program(f_code);
    
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context*
Scall_context_set_class_function
(struct Scall_context *context, 
struct Sframe *frame, 
struct Sobj* f_obj) {
    struct Sobj* class_f = f_obj->prev;
    if (!class_f) return Scall_context_set_frame(context, frame, f_obj);
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_class_function(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context = Scall_context_set(context, frame, f_obj);
    store_args_function_class(context, frame, f_obj, class_f);
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_class_function(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context*
Scall_context_set_frame
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {
    context = Scall_context_set(context, frame, f_obj);
    store_args_function(context, frame, f_obj);
    return context;
}

struct Scall_context*
Scall_context_set_closure
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_closure(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context = Scall_context_set(context, frame, f_obj);
    store_closure_args(context, frame, f_obj);
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_closure(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context*
Scall_context_set_frame_with_args
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj, 
    struct Sobj** args) {
    context->main_frame = frame;
    struct Sframe *f_frame = context->frame;
    struct Sfunc* func = f_obj->f_type->f_func;
    context = Scall_context_set(context, frame, f_obj);
    store_args_to_func(f_frame, func, args);
    return context;
}
