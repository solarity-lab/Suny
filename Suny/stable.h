#ifndef STABLE_H
#define STABLE_H

#include "score.h"
// #define NOT_FOUND -1

struct Stable;
struct Ssymbol;

#define not_found(x) (x == -1)
#define creat_label(compiler) (++compiler->label_counter)
#define creat_address(compiler) (++compiler->address_counter)

struct loop_stack {
    int continue_label;
    int break_label;
};

struct ScompilerUnit {
    int address_counter;
    int label_counter;
    int args_address_counter;

    int is_in_function;
    int is_in_block;
    int is_in_loop;
    int is_in_class;

    struct loop_stack loop_stack[1024];  
    int loop_index;

    struct Sframe *frame;

    struct Stable *prev_table;
};

struct Ssymbol {
    char *name;
    int address;
    int args_size;
    int func_tag; // if this global is in a function
    char* func_name; // same but its its function

    struct Stable* table;

    int is_closure;
    int is_attribute;
    int is_argument;

    int is_global;
    int is_local;
};

struct Stable {
    struct Ssymbol **symbols;
    int symbol_count;
    int symbol_capacity;

    struct Stable *prev; // for nested scopes
    struct Stable *global;

    int is_function_table; // flag to indicate if this table is for function scope 

    char* function_name;

    int is_closure;
    int is_global;
};

struct Stable* 
Stable_new(void);

struct Ssymbol *
Symbol_new
(char *name, int address, int args_size, int func_tag, char* func_name);

struct Ssymbol *
Ssymbol_load
(struct Stable *table, char *name);

struct Ssymbol *
Ssymbol_store
(struct Stable *table, char* name, int address);

struct Ssymbol *
Ssymbol_add
(struct Stable *table, char* name, int address);

struct ScompilerUnit *
ScompilerUnit_new(void);

struct loop_stack 
ScompilerUnit_get_loop
(struct ScompilerUnit *compiler);

int ScompilerUnit_reset
(struct ScompilerUnit *compiler);

int ScompilerUnit_add_loop
(struct ScompilerUnit *compiler, int continue_label, int break_label);

int ScompilerUnit_pop_loop
(struct ScompilerUnit *compiler);

#endif // STABLE_H