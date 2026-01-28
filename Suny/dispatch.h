#ifndef DISPATCH_H
#define DISPATCH_H

#include "opcode.h"

#define SET_DISPATCH(name) \ 
    static void* name[256] = { \
        [PUSH_FLOAT]        = &&OP_PUSH_FLOAT, \
        [PUSH_STRING]       = &&OP_PUSH_STRING \
        [PUSH_TRUE]         = &&OP_PUSH_TRUE, \
        [PUSH_FALSE]        = &&OP_PUSH_FALSE, \
        [LOAD_NULL]         = &&OP_LOAD_NULL, \
        [BINARY_ADD]        = &&OP_BINARY_ADD, \
        [BINARY_SUB]        = &&OP_BINARY_SUB, \
        [BINARY_MUL]        = &&OP_BINARY_MUL, \
        [BINARY_DIV]        = &&OP_BINARY_DIV, \
        [BINARY_MOD]        = &&OP_BINARY_MOD, \
        [BINARY_BIGGER]     = &&OP_BINARY_BIGGER, \
        [BINARY_SMALLER]    = &&OP_BINARY_SMALLER, \
        [BINARY_EQUAL]      = &&OP_BINARY_EQUAL, \
        [BINARY_NOT_EQUAL]  = &&OP_BINARY_NOT_EQUAL, \
        [BINARY_BIGGER_EQUAL]  = &&OP_BINARY_BIGGER_EQUAL, \
        [BINARY_SMALLER_EQUAL] = &&OP_BINARY_SMALLER_EQUAL \
        [AND_LOG]           = &&OP_AND_LOG \
        [OR_LOG]            = &&OP_OR_LOG, \
        [NOT_LOG]           = &&OP_NOT_LOG \
        [POP_TOP]           = &&OP_POP_TOP \
        [PRINT]             = &&OP_PRINT, \
        [LEN_OF]            = &&OP_LEN_OF, \
        [BUILD_LIST]        = &&OP_BUILD_LIST, \
        [LOAD_GLOBAL]       = &&OP_LOAD_GLOBAL \
        [STORE_GLOBAL]      = &&OP_STORE_GLOBAL, \
        [LOAD_LOCAL]        = &&OP_LOAD_LOCAL, \
        [STORE_LOCAL]       = &&OP_STORE_LOCAL \
        [LOAD_ITEM]         = &&OP_LOAD_ITEM, \
        [STORE_ITEM]        = &&OP_STORE_ITEM, \
        [LOAD_ATTR]         = &&OP_LOAD_ATTR, \
        [STORE_ATTR]        = &&OP_STORE_ATTR, \
        [LOAD_MEMBER]       = &&OP_LOAD_MEMBER \
        [STORE_MEMBER]      = &&OP_STORE_MEMBER, \
        [LOAD_METHOD]       = &&OP_LOAD_METHOD \
        [MAKE_FUNCTION]     = &&OP_MAKE_FUNCTION, \
        [START_FUNCTION]    = &&OP_START_FUNCTION, \
        [END_FUNCTION]      = &&OP_END_FUNCTION, \
        [MAKE_ARGS]         = &&OP_MAKE_ARGS, \
        [FUNCTION_CALL]     = &&OP_FUNCTION_CALL, \
        [FUNCTION_CALL_VARARGS] = &&OP_FUNCTION_CALL_VARARGS, \
        [RETURN_TOP]        = &&OP_RETURN_TOP, \
        [MAKE_CLOSURE]      = &&OP_MAKE_CLOSURE, \
        [END_CLOSURE]       = &&OP_END_CLOSURE \
        [LOAD_CLOSURE]      = &&OP_LOAD_CLOSURE, \
        [STORE_CLOSURE]     = &&OP_STORE_CLOSURE, \
        [JUMP]              = &&OP_JUMP, \
        [JUMP_FORWARD]      = &&OP_JUMP_FORWARD, \
        [JUMP_BACKWARD]     = &&OP_JUMP_BACKWARD, \
        [JUMP_IF_TRUE]      = &&OP_JUMP_IF_TRUE, \
        [JUMP_IF_FALSE]     = &&OP_JUMP_IF_FALSE, \
        [POP_JUMP_IF_TRUE]  = &&OP_POP_JUMP_IF_TRUE, \
        [POP_JUMP_IF_FALSE] = &&OP_POP_JUMP_IF_FALSE, \
        [JUMP_TO]           = &&OP_JUMP_TO \
        [JUMP_TO_N_TIMES]   = &&OP_JUMP_TO_N_TIMES \
        [LOOP_PREP]         = &&OP_LOOP_PREP, \
        [LOOP_STEP]         = &&OP_LOOP_STEP, \
        [CLASS_BEGIN]       = &&OP_CLASS_BEGIN \
        [CLASS_END]         = &&OP_CLASS_END, \
        [SET_SUPER_CLASS]   = &&OP_SET_SUPER_CLASS \
        [START_BLOCK]       = &&OP_START_BLOCK \
        [END_BLOCK]         = &&OP_END_BLOCK, \
        [PROGRAM_START]     = &&OP_PROGRAM_START, \
        [PROGRAM_END]       = &&OP_PROGRAM_END \
        [EXIT_PROGRAM]      = &&OP_EXIT_PROGRAM, \
        [STOP_PROGRAM]      = &&OP_STOP_PROGRAM, \
        [SKIP]              = &&OP_SKIP, \
    };

#endif // DISPATCH_H