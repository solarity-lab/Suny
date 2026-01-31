#ifndef SVM_H
#define SVM_H

#include "senvi.h"
#include "scompiler.h"
#include "sframe.h"
#include "score.h"
#include "sobj.h"
#include "sinitialize.h"
#include "sfunc.h"
#include "sstr.h"
#include "scopy.h"
#include "seval.h"
#include "scompare.h"
#include "sclass.h"
#include "sbool.h"
#include "scall.h"

#include <math.h>

byte_t
get_next_code
(struct Sframe *frame);

byte_t
jump_to(struct Sframe *frame, int address);

SUNY_API struct Sframe*
Svm_run_program
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_PUSH_FLOAT
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_GLOBAL
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_STORE_GLOBAL
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_MAKE_FUNCTION
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_FUNCTION_CALL
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_PRINT
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_COPY_TOP
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_PUSH_STRING
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_POP_JUMP_IF_FALSE
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_JUMP_TO
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_BUILD_LIST
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_ITEM
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_STORE_ITEM
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LEN_OF
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_TRUE
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_FALSE
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_LOCAL
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_STORE_LOCAL
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_CLASS_BEGIN
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_MAKE_BLOCK
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_IS_LOG
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_NOT_LOG
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_AND_LOG
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_OR_LOG
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_ATTR
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_STORE_ATTR
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_NULL
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_POP
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_EXIT_PROGRAM   
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOOP_PREP   
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOOP_STEP   
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_STOP_PROGRAM
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_STORE_CLOSURE
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_CLOSURE
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_MAKE_CLOSURE
(struct Sframe *frame);

SUNY_API struct Sframe*
Svm_evaluate_LOAD_MEMBER
(struct Sframe *frame, struct Sclass *sclass);

SUNY_API struct Sframe*
Svm_evaluate_STORE_MEMBER
(struct Sframe *frame, struct Sclass *sclass);

SUNY_API struct Sframe*
Svm_evaluate_SET_SUPER_CLASS
(struct Sframe *frame, struct Sclass *sclass);

SUNY_API struct Sframe*
Svm_evaluate_SET_SHARED_CLASS
(struct Sframe *frame, struct Sclass *sclass);

SUNY_API struct Sframe*
Svm_evaluate_RETURN_TOP
(struct Sframe *frame, struct Scall_context *context);

SUNY_API struct Sframe*
Svm_evalutate_BINARY_OPER
(struct Sframe *frame, byte_t op);

SUNY_API struct Sframe*
Svm_run_call_context
(struct Scall_context *context);

SUNY_API struct Sframe*
Svm_call_function(struct Sframe *frame, struct Sobj *f_obj);

SUNY_API struct Sframe*
Svm_call_class_function(struct Sframe *frame, struct Sobj *f_obj);

SUNY_API struct Sframe*
Svm_call_closure(struct Sframe *frame, struct Sobj *f_obj);

#endif // SVM_H