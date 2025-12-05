#include "stable.h"
#include "smem.h"

struct ScompilerUnit *ScompilerUnit_new(void) {
    struct ScompilerUnit *compiler = Smem_Malloc(sizeof(struct ScompilerUnit));
    compiler->address_counter = 120;
    compiler->label_counter = 0;
    compiler->is_in_function = 0;
    compiler->is_in_block = 0;
    compiler->is_in_loop = 0;
    compiler->is_in_class = 0;
    compiler->loop_index = 0;
    compiler->frame = NULL;
    return compiler;
}

struct Stable* Stable_new(void) {
    struct Stable *table = Smem_Malloc(sizeof(struct Stable));
    table->symbol_count = 0;
    table->symbol_capacity = 1024;
    table->prev = NULL;
    table->is_function_table = 0;
    table->is_closure = 0;
    table->symbols = Smem_Malloc(sizeof(struct Ssymbol) * table->symbol_capacity);
    table->function_name = NULL;
    return table;
}

struct Ssymbol *Symbol_new(char *name, int address, int args_size, int func_tag, char* func_name) {
    struct Ssymbol *symbol = Smem_Malloc(sizeof(struct Ssymbol));
    symbol->name = name;
    symbol->address = address;
    symbol->is_closure = 0;
    symbol->args_size = args_size;
    symbol->func_tag = func_tag;
    symbol->func_name = func_name;
    return symbol;
}

int Stable_add_symbol(struct Stable *table, char *name, int address, int args_size, int func_tag, char* func_name) {
    struct Ssymbol *symbol = Symbol_new(name, address, args_size, func_tag, func_name);
    
    if (table->symbol_count >= table->symbol_capacity) {
        table->symbol_capacity *= 2;
        table->symbols = Smem_Realloc(table->symbols, sizeof(struct Ssymbol) * table->symbol_capacity);
    }

    for (int i = 0; i < table->symbol_count; i++) {
        if (strcmp(table->symbols[i]->name, name) == 0) {
            return table->symbols[i]->address;
        }
    }

    table->symbols[table->symbol_count++] = symbol;
    return address;
}

int Stable_find_symbol(struct Stable *table, char *name) {
    for (int i = 0; i < table->symbol_count; i++) {
        if (strcmp(table->symbols[i]->name, name) == 0) {
            return table->symbols[i]->address;
        }
    }
    return -1;
}

int Stable_remove_symbol(struct Stable *table, char *name) {
    for (int i = 0; i < table->symbol_count; i++) {
        if (strcmp(table->symbols[i]->name, name) == 0) {
            table->symbols[i]->name = NULL;
            table->symbols[i]->address = 0;
            return 0;
        }
    }
    return -1;
}

int Stable_remove_symbol_address(struct Stable *table, int address) {
    for (int i = 0; i < table->symbol_count; i++) {
        if (table->symbols[i]->address == address) {
            table->symbols[i]->name = NULL;
            table->symbols[i]->address = 0;
            return 0;
        }
    }
    return -1;
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

struct Ssymbol *Symbol_find_symbol(struct Stable *table, char *name) {
    for (int i = 0; i < table->symbol_count; i++) {
        if (strcmp(table->symbols[i]->name, name) == 0) {
            return table->symbols[i];
        }
    }

    if (table->global) {
        for (int i = 0; i < table->global->symbol_count; i++) {
            if (strcmp(table->global->symbols[i]->name, name) == 0) {
                return table->global->symbols[i];
            }
        }
    }

    if (table->prev) {
        struct Ssymbol *symbol = Symbol_find_symbol(table->prev, name);
        if (!symbol) return NULL;

        symbol->is_closure = 1;
        return symbol;
    }

    return NULL;
}

struct Ssymbol *Symbol_store(struct Stable *table, char* name, int address) {

    if (table->symbol_count == table->symbol_capacity) {
        table->symbol_capacity *= 2;
        table->symbols = Smem_Realloc(table->symbols, sizeof(struct Ssymbol) * table->symbol_capacity);
    }

    for (int i = 0; i < table->symbol_count; i++) {
        if (strcmp(table->symbols[i]->name, name) == 0) {
            return table->symbols[i];
        }
    }

    for (int i = 0; i < table->global->symbol_count; i++) {
        if (strcmp(table->global->symbols[i]->name, name) == 0) {
            return table->global->symbols[i];
        }
    }

    if (table->prev && !table->prev->is_global) {
        struct Ssymbol *symbol = Symbol_store(table->prev, name, address);
        symbol->is_closure = 1;
        return symbol;
    }

    struct Ssymbol *symbol = Symbol_new(name, address, 0, 0, NULL);
    table->symbols[table->symbol_count++] = symbol;
    return symbol;
}
