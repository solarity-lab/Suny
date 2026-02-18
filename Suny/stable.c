#include "stable.h"
#include "sdebug.h"
#include "smem.h"

struct ScompilerUnit *ScompilerUnit_new(void) {
    struct ScompilerUnit *compiler = Smem_Malloc(sizeof(struct ScompilerUnit));
    compiler->address_counter = 100;
    compiler->label_counter = 0;
    compiler->args_address_counter = 0;
    compiler->is_in_function = 0;
    compiler->is_in_block = 0;
    compiler->is_in_loop = 0;
    compiler->is_in_class = 0;
    compiler->loop_index = 0;
    compiler->prev_table = NULL;
    compiler->frame = NULL;
    return compiler;
}

struct Stable* Stable_new(void) {
    struct Stable *table = Smem_Malloc(sizeof(struct Stable));
    table->symbol_count = 0;
    table->symbol_capacity = 1024;
    table->prev = NULL;
    table->is_function_table = 0;
    table->is_class_table = 0;
    table->is_closure = 0;
    table->is_global = 0;
    table->symbols = Smem_Malloc(sizeof(struct Ssymbol) * table->symbol_capacity);
    table->symbols[0] = NULL;
    table->function_name = NULL;
    return table;
}

struct Ssymbol *Symbol_new(char *name, int address, int args_size, int func_tag, char* func_name) {
    struct Ssymbol *symbol = Smem_Malloc(sizeof(struct Ssymbol));
    symbol->name = strdup(name);
    symbol->address = address;

    symbol->is_closure = 0;
    symbol->is_global = 0;
    symbol->is_local = 0;

    symbol->is_class = 0;
    symbol->is_function = 0;
    symbol->is_argument = 0;
    symbol->is_attribute = 0;

    symbol->args_size = args_size;
    symbol->func_tag = func_tag;
    symbol->func_name = func_name;

    return symbol;
}

int ScompilerUnit_reset(struct ScompilerUnit *compiler) {
    compiler->address_counter = 0;
    compiler->label_counter = 0;
    compiler->is_in_function = 0;
    compiler->is_in_block = 0;
    compiler->is_in_loop = 0;
    compiler->is_in_class = 0;
    compiler->loop_index = 0;
    return 0;
}

int ScompilerUnit_add_loop(struct ScompilerUnit *compiler, int continue_label, int break_label) {
    if (compiler->loop_index < 1024) {
        compiler->loop_stack[compiler->loop_index].continue_label = continue_label;
        compiler->loop_stack[compiler->loop_index].break_label = break_label;
        compiler->loop_index++;
        return 0;
    }
    return -1;
}

int ScompilerUnit_pop_loop(struct ScompilerUnit *compiler) {
    if (compiler->loop_index > 0) {
        compiler->loop_index--;
        return 0;
    }
    return -1;    
}

struct loop_stack ScompilerUnit_get_loop(struct ScompilerUnit *compiler) {
    if (compiler->loop_index > 0) {
        return compiler->loop_stack[compiler->loop_index - 1];
    }
    struct loop_stack empty_loop = {0, 0};
    return empty_loop;
}

struct Ssymbol *Ssymbol_load(struct Stable *table, char *name) {
    for (int i = 0; i < table->symbol_count; i++) {
        struct Ssymbol *symbol = table->symbols[i];

        if (!symbol) continue;

        if (strcmp(symbol->name, name) == 0) {
            return symbol;
        }
    }

    if (table->prev && table->is_local_table && !table->prev->is_class_table) {
        return Ssymbol_load(table->prev, name);
    }

    if (table->prev && !table->prev->is_global && !table->prev->is_class_table && table->is_function_table) {
        struct Stable *prev = table->prev;
        while (prev) {
            struct Ssymbol *found = NULL;

            for (int i = 0; i < prev->symbol_count; i++) {
                if (strcmp(prev->symbols[i]->name, name) == 0) {
                    found = prev->symbols[i];
                    break;
                }
            }

            if (found) {
                found->is_closure = 1;
                return found;
            }
            prev = prev->prev;
        }
    }

    if (table->global) {
        for (int i = 0; i < table->global->symbol_count; i++) {
            struct Ssymbol *symbol = table->global->symbols[i];

            if (!symbol) continue;

            if (strcmp(symbol->name, name) == 0) {
                return symbol;
            }
        }
    }

    return NULL;
}

struct Ssymbol *Ssymbol_store(struct Stable *table, char* name, int address) {
    struct Ssymbol *symbol = Ssymbol_load(table, name);
    
    if (symbol) return symbol;
    
    if (table->symbol_count == table->symbol_capacity) {
        table->symbol_capacity *= 2;
        table->symbols = Smem_Realloc(table->symbols, sizeof(struct Ssymbol) * table->symbol_capacity);
    }

    symbol = Symbol_new(name, address, 0, 0, NULL);
    
    table->symbols[table->symbol_count++] = symbol;

    if (table->is_function_table) {
        symbol->is_local = 1;
    } else if (table->is_global) {
        symbol->is_global = 1;
    } 

    return symbol;
}

struct Ssymbol *
Ssymbol_add
(struct Stable *table, char* name, int address) {
    if (table->symbol_count == table->symbol_capacity) {
        table->symbol_capacity *= 2;
        table->symbols = Smem_Realloc(table->symbols, sizeof(struct Ssymbol) * table->symbol_capacity);
    }

    struct Ssymbol *symbol = Symbol_new(name, address, 0, 0, NULL);
    symbol->is_global = 1;                                              // SET TO GLOBAL

    table->symbols[table->symbol_count++] = symbol;
    return symbol;
}