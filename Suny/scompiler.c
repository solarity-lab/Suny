#include "scompiler.h"

SUNY_API struct Scode* 
Scompiler_compile
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    if (!ast) {
        return NULL_CODE_PTR;
    }

    switch (ast->type) {
        case AST_PROGRAM:
            return Scompiler_compile_ast_program(compiler, ast, table);
        case AST_BINARY_EXPRESSION:
            return Scompiler_compile_ast_binary_expression(compiler, ast, table);
        case AST_LITERAL:
            return Scompiler_compile_ast_literal(compiler, ast, table);
        case AST_IDENTIFIER:
            return Scompiler_compile_ast_identifier(compiler, ast, table);
        case AST_ASSIGNMENT:
            return Scompiler_compile_ast_assignment(compiler, ast, table);
        case AST_FUNCTION_CALL_EXPRESSION:
            return Scompiler_compile_ast_function_call(compiler, ast, table);
        case AST_RETURN_STATEMENT:
            return Scompiler_compile_ast_return(compiler, ast, table);
        case AST_COMPARE_EXPRESSION:
            return Scompiler_compile_ast_comparison(compiler, ast, table);
        case AST_IF:
            return Scompiler_compile_ast_if(compiler, ast, table);
        case AST_FOR:
            return Scompiler_compile_ast_for(compiler, ast, table);        
        case AST_WHILE:
            return Scompiler_compile_ast_while(compiler, ast, table);
        case AST_FUNCTION_STATEMENT:
            return Scompiler_compile_ast_function(compiler, ast, table);
        case AST_STRING_EXPRESSION:
            return Scompiler_compile_ast_string(compiler, ast, table);
        case AST_BREAK:
            return Scompiler_compile_ast_break(compiler, ast, table);
        case AST_CONTINUE:
            return Scompiler_compile_ast_continue(compiler, ast, table);
        case AST_LIST:
            return Scompiler_compile_ast_list(compiler, ast, table);
        case AST_CLASS:
            return Scompiler_compile_ast_class(compiler, ast, table);
        case AST_OR_EXPRESSION:
            return Scompiler_compile_ast_or(compiler, ast, table);
        case AST_AND_EXPRESSION:
            return Scompiler_compile_ast_and(compiler, ast, table);
        case AST_NOT_EXPRESSION:
            return Scompiler_compile_ast_not(compiler, ast, table);
        case AST_ANONYMOUS_FUNCTION:
            return Scompiler_compile_ast_anonymous_function(compiler, ast, table);
        case AST_NULL:
            return Scompiler_compile_ast_null_expression(compiler, ast, table);
        case AST_ATTRIBUTE_EXPRESSION:
            return Scompiler_compile_ast_attribute(compiler, ast, table);
        case AST_STORE_ATTRIBUTE:
            return Scompiler_compile_ast_store_attribute(compiler, ast, table);
        case AST_FUNCTION_CALL_PRIMARY_EXPRESSION:
            return Scompiler_compile_ast_function_call_primary(compiler, ast, table);
        case AST_STORE_INDEX:
            return Scompiler_compile_ast_store_index(compiler, ast, table);
        case AST_EXTRACT:
            return Scompiler_compile_ast_extract_expression(compiler, ast, table);
        case AST_IMPORT:
            return Scompiler_compile_ast_import(compiler, ast, table);
        case AST_INCLUDE:
            return Scompiler_compile_ast_include(compiler, ast, table);
        case AST_LOOP:
            return Scompiler_compile_ast_loop(compiler, ast, table);
        case AST_NULL_EXPRESSION: {
            PUSH(code, LOAD_NULL);
            return code;
        }
        case AST_TRUE: {
            PUSH(code, LOAD_TRUE);
            return code;
        }
        case AST_FALSE: {
            PUSH(code, LOAD_FALSE);
            return code;
        }
        default: {
            char* message = string("Unsupported AST node type %d", ast->type);
            struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
            Serror_syntax_error(error);
            return NULL_CODE_PTR;
        }
    }

    return NULL_CODE_PTR;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_program
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    table->global = table;
    table->is_global = 1;
    table->global->is_global = 1;

    if (!ast || ast->type != AST_PROGRAM) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Invalid AST node for program compilation", ast->lexer);
        Serror_syntax_error(error);
        return NULL_CODE_PTR;
    }

    struct Scode *code = NULL_CODE_PTR;

    PUSH(code, PROGRAM_START);

    for (int i = 0; i < ast->child_count; i++) {
        struct Sast *child = ast->children[i];
        struct Scode *child_code = Scompiler_compile(compiler, child, table);
        INSERT(code, child_code);

        if (is_expr(ast->children[i])) {
            PUSH(code, POP_TOP);
        }
    }

    PUSH(code, PROGRAM_END);
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_binary_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    struct Scode *left = Scompiler_compile(compiler, ast->left, table);
    struct Scode *right = Scompiler_compile(compiler, ast->right, table);

    INSERT(code, left);
    INSERT(code, right);

    switch(ast->op) {
        case ADD:
            PUSH(code, BINARY_ADD);
            break;
        case SUB:
            PUSH(code, BINARY_SUB);
            break;
        case MUL:
            PUSH(code, BINARY_MUL);
            break;
        case DIV:
            PUSH(code, BINARY_DIV);
            break;
        default:
            struct Serror *error = Serror_set("COMPILER_ERROR", "Unknown operator", ast->lexer);
            Serror_syntax_error(error);
            break;
    }

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_literal
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    float value = ast->value;

    struct Scode *code = NULL_CODE_PTR;

    PUSH(code, PUSH_FLOAT);

    unsigned char float_code[4];
    memcpy(float_code, &value, sizeof(float));

    PUSH(code, float_code[0]);
    PUSH(code, float_code[1]);
    PUSH(code, float_code[2]);
    PUSH(code, float_code[3]);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_identifier
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    struct Ssymbol* symbol = Ssymbol_load(table, ast->lexeme);

    if (!symbol) {
        char* message = string("Undefined variable '%s'", ast->lexeme);
        struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
        Serror_syntax_error(error);
    }

    if (symbol->is_closure) {
        PUSH(code, LOAD_CLOSURE);
    } else {
        PUSH(code, LOAD_GLOBAL);
    }

    PUSH(code, symbol->address);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_assignment
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    struct Ssymbol* symbol = Ssymbol_store(table, ast->var_name, ++compiler->address_counter);

    if (compiler->is_in_class) {
        symbol->is_attribute = 1;
    }

    if (!symbol) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Global scope have not intialized yet", ast->lexer);
        Serror_syntax_error(error);
    }

    struct Scode *value_code = Scompiler_compile(compiler, ast->var_value, table);
    INSERT(code, value_code);

    if (symbol->is_closure) {
        PUSH(code, STORE_CLOSURE);
    } else {
        PUSH(code, STORE_GLOBAL);
    }

    PUSH(code, symbol->address);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_print
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    struct Scode *print_value_code = Scompiler_compile(compiler, ast->print_value, table);
    INSERT(code, print_value_code);

    PUSH(code, PRINT);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_function_call
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    struct Ssymbol* symbol = Ssymbol_load(table, ast->lexeme);

    if (!symbol) {
        char* message = string("Undefined function '%s'", ast->lexeme);
        struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
        Serror_syntax_error(error);
    }

    byte_t address = symbol->address;

    struct Scode *param_code = Scompiler_compile_ast_block(compiler, ast->params, table, ast->param_count);
    INSERT(code, param_code);

    PUSH(code, LOAD_GLOBAL);
    PUSH(code, address);

    PUSH(code, FUNCTION_CALL);
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_return
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    if (!table->is_function_table) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Return statement outside of function", ast->lexer);
        Serror_syntax_error(error);
    }

    struct Scode *ret_code = Scompiler_compile(compiler, ast->ret_val, table);
    INSERT(code, ret_code);

    PUSH(code, RETURN_TOP);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_comparison
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    struct Scode *left = Scompiler_compile(compiler, ast->left, table);
    struct Scode *right = Scompiler_compile(compiler, ast->right, table);

    byte_t opcode;

    switch (ast->op) {
        case BIGGER:
            opcode = BINARY_BIGGER;
            break;
        case SMALLER:
            opcode = BINARY_SMALLER;
            break;
        case EQUALS:
            opcode = BINARY_EQUAL;
            break;
        case BIGGER_EQUALS:
            opcode = BINARY_BIGGER_EQUAL;
            break;
        case SMALLER_EQUALS:
            opcode = BINARY_SMALLER_EQUAL;
            break;
        case NOT_EQUALS:
            opcode = BINARY_NOT_EQUAL;
            break;
        default:
            struct Serror *error = Serror_set("COMPILER_ERROR", "Unknown operator", ast->lexer);
            Serror_syntax_error(error);
            return NULL;
    }

    INSERT(code, left);
    INSERT(code, right);
    PUSH(code, opcode);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_if
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *conditon = Scompiler_compile(compiler, ast->condition, table);
    struct Scode *if_body = Scompiler_compile_ast_body(compiler, ast->if_body, table, ast->if_body_size);
    struct Scode *else_body = Scompiler_compile_ast_body(compiler, ast->else_body, table, ast->else_body_size);

    struct Scode *code = NULL_CODE_PTR;

    int end_address = ++compiler->label_counter;
    int else_address = ++compiler->label_counter;

    INSERT(code, conditon);

    PUSH(code, POP_JUMP_IF_FALSE);
    PUSH(code, end_address);

    INSERT(code, if_body);

    PUSH(code, JUMP_TO);
    PUSH(code, else_address);

    PUSH(code, ADD_LABEL);
    PUSH(code, end_address);

    INSERT(code, else_body);

    PUSH(code, ADD_LABEL);
    PUSH(code, else_address);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_while
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    compiler->is_in_loop = 1;

    int while_start = ++compiler->label_counter;
    int while_end = ++compiler->label_counter;

    ScompilerUnit_add_loop(compiler, while_start, while_end);

    struct Scode *conditon = Scompiler_compile(compiler, ast->condition, table);
    struct Scode *while_body = Scompiler_compile_ast_body(compiler, ast->body, table, ast->body_size);

    ScompilerUnit_pop_loop(compiler);

    compiler->is_in_loop = 0;

    struct Scode *code = NULL_CODE_PTR;

    PUSH(code, ADD_LABEL);
    PUSH(code, while_start);

    INSERT(code, conditon);

    PUSH(code, POP_JUMP_IF_FALSE);
    PUSH(code, while_end);

    INSERT(code, while_body);

    PUSH(code, JUMP_TO);
    PUSH(code, while_start);

    PUSH(code, ADD_LABEL);
    PUSH(code, while_end);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_function
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Ssymbol* symbol = Ssymbol_store(table, ast->lexeme, ++compiler->address_counter);

    if (compiler->is_in_class) {
        symbol->is_attribute = 1;
    }

    byte_t faddress = symbol->address;
    byte_t fargs_count = ast->args_count;
    
    table->function_name = ast->lexeme;

    struct Scode *code = NULL_CODE_PTR;

    if (ast->is_lambda) {
        struct Scode *body_expr = Scompiler_compile_ast_function_body_expression(compiler, ast->expr, table, ast->args_count, ast->param_names);

        if (table->is_function_table) {
            PUSH(code, MAKE_CLOSURE);
        } else {
            PUSH(code, MAKE_FUNCTION);
        }

        PUSH(code, fargs_count);

        INSERT(code, body_expr);
        PUSH(code, RETURN_TOP);

        if (table->is_function_table) {
            PUSH(code, END_CLOSURE);
        } else {
            PUSH(code, END_FUNCTION);
        }

        PUSH(code, STORE_GLOBAL);
        PUSH(code, faddress);

        return code;
    }

    struct Scode *body = Scompiler_compile_ast_function_body(compiler, ast->body, table, ast->body_size, ast->param_names, ast->args_count);

    if (table->is_function_table) {
        PUSH(code, MAKE_CLOSURE);
    } else {
        PUSH(code, MAKE_FUNCTION);
    }

    PUSH(code, fargs_count);

    INSERT(code, body);

    if (table->is_function_table) {
        PUSH(code, END_CLOSURE);
    } else {
        PUSH(code, END_FUNCTION);
    }

    PUSH(code, STORE_GLOBAL);
    PUSH(code, faddress);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_string
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;

    char* string = ast->lexeme;

    int size = 0;

    unsigned char cstring[MAX_IDENTIFIER_SIZE];

    for (int i = 0; string[i] != '\0'; i++) {
        cstring[i] = string[i];
        ++size;
    }

    cstring[size] = '\0';

    PUSH(code, PUSH_STRING);

    PUSH(code, size);

    for (int i = 0; i < size; i++) {
        PUSH(code, cstring[i]);
    }

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_break
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    if (!compiler->is_in_loop) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Break statement outside of loop", ast->lexer);
        Serror_syntax_error(error);
    }

    struct Scode *code = NULL_CODE_PTR;
    struct loop_stack loop = ScompilerUnit_get_loop(compiler);
    int address = loop.break_label;
    PUSH(code, JUMP_TO);
    PUSH(code, address);
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_continue
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    if (!compiler->is_in_loop) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Continue statement outside of loop", ast->lexer);
        Serror_syntax_error(error);
    }

    struct Scode *code = NULL_CODE_PTR;
    struct loop_stack loop = ScompilerUnit_get_loop(compiler);
    int address = loop.continue_label;
    PUSH(code, JUMP_TO);
    PUSH(code, address);
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_list
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;
    Sreverse((void **) ast->list, ast->list_count);

    for (int i = 0; i < ast->list_count; i++) {
        struct Scode *item_code = Scompiler_compile(compiler, ast->list[i], table);
        INSERT(code, item_code);
    }

    PUSH(code, BUILD_LIST);
    PUSH(code, ast->list_count);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_for
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    int iden = compiler->address_counter++;
    int loop_start = compiler->label_counter++;
    int loop_end = compiler->label_counter++;

    compiler->is_in_loop = 1;

    ScompilerUnit_add_loop(compiler, loop_start, loop_end);

    Ssymbol_store(table, ast->lexeme, iden);

    struct Scode *for_body = Scompiler_compile_ast_body(compiler, ast->block, table, ast->block_size);
    struct Scode *iter = Scompiler_compile(compiler, ast->expr, table);

    compiler->is_in_loop = 0;

    ScompilerUnit_pop_loop(compiler);

    struct Scode *code = NULL_CODE_PTR;

    INSERT(code, iter);

    PUSH(code, STORE_GLOBAL);
    PUSH(code, __iter__a);

    PUSH(code, PUSH_FLOAT);
    PUSH(code, '\x00');
    PUSH(code, '\x00');
    PUSH(code, '\x00');
    PUSH(code, '\x00');

    PUSH(code, STORE_GLOBAL);
    PUSH(code, __i__a);

    PUSH(code, PUSH_FLOAT);
    PUSH(code, '\x00');
    PUSH(code, '\x00');
    PUSH(code, '\x00');
    PUSH(code, '\x00');

    PUSH(code, STORE_GLOBAL);
    PUSH(code, iden);

    PUSH(code, ADD_LABEL);
    PUSH(code, loop_start);

    PUSH(code, LOAD_GLOBAL);
    PUSH(code, __i__a);

    PUSH(code, LOAD_GLOBAL);
    PUSH(code, __iter__a);

    PUSH(code, LEN_OF);

    PUSH(code, BINARY_SMALLER);

    PUSH(code, POP_JUMP_IF_FALSE);
    PUSH(code, loop_end);

    PUSH(code, LOAD_GLOBAL);
    PUSH(code, __iter__a);

    PUSH(code, LOAD_GLOBAL);
    PUSH(code, __i__a);

    PUSH(code, LOAD_ITEM);

    PUSH(code, STORE_GLOBAL);
    PUSH(code, iden);

    INSERT(code, for_body);

    PUSH(code, LOAD_GLOBAL);
    PUSH(code, __i__a);

    PUSH(code, PUSH_FLOAT);
    PUSH(code, '\x00');
    PUSH(code, '\x00');
    PUSH(code, '\x80');
    PUSH(code, '\x3F');

    PUSH(code, BINARY_ADD);

    PUSH(code, STORE_GLOBAL);
    PUSH(code, __i__a);

    PUSH(code, JUMP_TO);
    PUSH(code, loop_start);

    PUSH(code, ADD_LABEL);
    PUSH(code, loop_end);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_class
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    if (compiler->is_in_function) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Class can't be inside function", ast->lexer);
        Serror_syntax_error(error);
    }

    if (compiler->is_in_loop) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Class can't be inside loop", ast->lexer);
        Serror_syntax_error(error);
    }
    
    struct Ssymbol* symbol = Ssymbol_store(table, ast->lexeme, ++compiler->address_counter);

    Ssymbol_store(table, "__init__", __INIT__ADDRESS);
    Ssymbol_store(table, "__add__", __ADD__ADDRESS);
    Ssymbol_store(table, "__sub__", __SUB__ADDRESS);
    Ssymbol_store(table, "__mul__", __MUL__ADDRESS);
    Ssymbol_store(table, "__div__", __DIV__ADDRESS);
    Ssymbol_store(table, "__tostring__", __TO_STR__ADDRESS);

    compiler->is_in_class = 1;
    struct Scode *class_body = Scompiler_compile_ast_body(compiler, ast->block, table, ast->block_size);
    compiler->is_in_class = 0;

    struct Scode *code = NULL_CODE_PTR;

    PUSH(code, CLASS_BEGIN);

    INSERT(code, class_body);

    PUSH(code, CLASS_END);

    PUSH(code, STORE_GLOBAL);

    PUSH(code, symbol->address);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_or
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *left = Scompiler_compile(compiler, ast->left, table);
    struct Scode *right = Scompiler_compile(compiler, ast->right, table);

    struct Scode *code = NULL_CODE_PTR;

    INSERT(code, left);
    INSERT(code, right);

    PUSH(code, OR_LOG);
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_and
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *left = Scompiler_compile(compiler, ast->left, table);
    struct Scode *right = Scompiler_compile(compiler, ast->right, table);   

    struct Scode *code = NULL_CODE_PTR;

    INSERT(code, left);
    INSERT(code, right);

    PUSH(code, AND_LOG);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_not
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *expr = Scompiler_compile(compiler, ast->expr, table);

    struct Scode *code = NULL_CODE_PTR;

    INSERT(code, expr);

    PUSH(code, NOT_LOG);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_anonymous_function
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *block = Scompiler_compile_ast_function_body(compiler, ast->body, table, ast->body_size, ast->param_names, ast->args_count);
    
    struct Scode *code = NULL_CODE_PTR;

    int args_count = ast->args_count;

    if (table->is_function_table) {
        PUSH(code, MAKE_CLOSURE);
    } else {
        PUSH(code, MAKE_FUNCTION);
    }
    
    PUSH(code, args_count);

    INSERT(code, block);

    if (table->is_function_table) {
        PUSH(code, END_CLOSURE);
    } else {
        PUSH(code, END_FUNCTION);
    }
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_function_call_primary
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *params = Scompiler_compile_ast_block(compiler, ast->params, table, ast->param_count);
    struct Scode *expr = Scompiler_compile(compiler, ast->expr, table);

    struct Scode *code = NULL_CODE_PTR;

    INSERT(code, params);
    INSERT(code, expr);

    PUSH(code, FUNCTION_CALL);
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_include
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    if (compiler->is_in_function) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Include statement inside function is not allowed", ast->lexer);
        Serror_syntax_error(error);
    }

    if (compiler->is_in_loop) {
        struct Serror *error = Serror_set("COMPILER_ERROR", "Include statement inside loop is not allowed", ast->lexer);
        Serror_syntax_error(error);
    }

    if (if_file_exists(ast->lexeme)) {
        return Scode_get_code_from_file(ast->lexeme, compiler, table);
    }

    if (if_folder_exists(ast->lexeme)) {
        char* file = string("%s/main.suny", ast->lexeme);

        if (if_file_exists(file)) {
            return Scode_get_code_from_file(file, compiler, table);
        } else {
            char* message = string("Cannot find file '%s' make sure main.suny exists in the folder", file);
            struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
            Serror_syntax_error(error);
            return NULL_CODE_PTR;
        }
    }

    if (if_file_exists_in(ast->lexeme, "C:\\Suny\\libs\\")) {
        char* file = string("C:\\Suny\\libs\\%s", ast->lexeme);
        return Scode_get_code_from_file(file, compiler, table);
    }

    if (if_folder_exists_in(ast->lexeme, "C:\\Suny\\libs\\")) {
        char* file = string("C:\\Suny\\libs\\%s\\main.suny", ast->lexeme);

        if (if_file_exists(file)) {
            return Scode_get_code_from_file(file, compiler, table);
        } else {
            char* message = string("Cannot find folder '%s' make sure main.suny exists in Library folder (C:\\Suny\\libs)", ast->lexeme);
            struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
            Serror_syntax_error(error);
            return NULL_CODE_PTR;
        }
    }

    char* message = string("Cannot find file '%s'", ast->lexeme);
    struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
    Serror_syntax_error(error);
    return NULL_CODE_PTR;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_import
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    char* file = string("%s.dll", ast->lexeme);

    Sdll_func dll_func = dll_get_func("Smain", file);

    main_func f = (main_func) dll_func;

    f(compiler->frame, table);

    return NULL_CODE_PTR;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_attribute
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;
    struct Scode *target = Scompiler_compile(compiler, ast->target, table);
    char* name = ast->attr_name;

    struct Ssymbol* symbol = Ssymbol_load(table, name);

    if (!symbol && table->global != NULL && table->is_function_table) {
        symbol = Ssymbol_load(table->global, name);

        if (!symbol) {
            char* message = string("Undefined attribute '%s'", name);
            struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
            Serror_syntax_error(error);
            return NULL_CODE_PTR;
        }
    }

    if (!symbol) {
        char* message = string("Undefined attribute '%s'", name);
        struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
        Serror_syntax_error(error);
        return NULL_CODE_PTR;
    }

    INSERT(code, target);

    PUSH(code, LOAD_ATTR);
    PUSH(code, symbol->address);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_store_attribute
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Sast* assign = ast->expr;
    struct Sast* value = ast->attribute;

    if (assign->type == AST_ATTRIBUTE_EXPRESSION) {
        char* name = assign->attr_name;
        struct Ssymbol* symbol = Ssymbol_load(table, name);

        if (!symbol) {
            char* message = string("Undefined attribute '%s'", name);
            struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
            Serror_syntax_error(error);
            return NULL_CODE_PTR;
        }

        struct Scode *code = NULL_CODE_PTR;
        struct Scode *target = Scompiler_compile(compiler, assign->target, table);
        struct Scode *value_code = Scompiler_compile(compiler, value, table);

        INSERT(code, value_code);
        INSERT(code, target);

        PUSH(code, STORE_ATTR);
        PUSH(code, symbol->address);

        return code;
    }

    return NULL_CODE_PTR;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_null_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;
    PUSH(code, LOAD_NULL);
    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_block
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size) {
    struct Scode *code = NULL_CODE_PTR;

    for (int i = 0; i < block_size; i++) {
        struct Scode *child = Scompiler_compile(compiler, block[i], table);
        INSERT(code, child);
    }

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_body
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size) {
    struct Scode *code = NULL_CODE_PTR;

    for (int i = 0; i < block_size; i++) {
        struct Scode *child = Scompiler_compile(compiler, block[i], table);
        INSERT(code, child);

        if (is_expr(block[i])) {
            PUSH(code, POP_TOP);
        }
    }

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_local_body
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size) {
    struct Scode *code = NULL_CODE_PTR;

    struct Stable *local_table = Stable_new();
    local_table->global = table->global;
    local_table->global->is_global = 1;
    local_table->prev = table;

    for (int i = 0; i < block_size; i++) {
        struct Scode *child = Scompiler_compile(compiler, block[i], local_table);
        INSERT(code, child);

        if (is_expr(block[i])) {
            PUSH(code, POP_TOP);
        }
    }

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_function_body
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size, char **args, int args_size) {
    struct Scode *code = NULL_CODE_PTR;
 
    struct Stable *local_table = Stable_new();
    
    local_table->is_function_table = 1;
    local_table->global = table->global;
    
    local_table->prev = table;

    compiler->is_in_function = 1;
    
    for (int i = 0; i < args_size; i++) {
        struct Ssymbol* symbol = Ssymbol_store(local_table, args[i], ++compiler->address_counter);
        symbol->is_argument = 1;
        PUSH(code, MAKE_ARGS);
        PUSH(code, symbol->address);
    }

    PUSH(code, START_FUNCTION);

    for (int i = 0; i < block_size; i++) {
        struct Scode *child = Scompiler_compile(compiler, block[i], local_table);

        INSERT(code, child);

        if (is_expr(block[i])) {
            PUSH(code, POP_TOP);
        }
    }

    local_table->is_function_table = 1;
    compiler->is_in_function = 0;

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_function_body_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table, int args_count, char **param_names) {
    compiler->is_in_function = 1;
    struct Scode *code = NULL_CODE_PTR;

    struct Stable *local_table = Stable_new();
    
    local_table->is_function_table = 1;
    local_table->global = table->global;
    
    local_table->prev = table;

    compiler->is_in_function = 1;

    for (int i = 0; i < args_count; i++) {
        struct Ssymbol* symbol = Ssymbol_store(local_table, param_names[i], ++compiler->address_counter);
        symbol->is_argument = 1;

        PUSH(code, MAKE_ARGS);
        PUSH(code, symbol->address);
    }

    PUSH(code, START_FUNCTION);

    struct Scode *body = Scompiler_compile(compiler, ast, local_table);

    local_table->is_function_table = 0;
    compiler->is_in_function = 0;

    INSERT(code, body);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_closure_identifier
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    return NULL_CODE_PTR;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_extract_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *extract_value_code = Scompiler_compile(compiler, ast->extract_value, table);
    struct Scode *extract_object_code = Scompiler_compile(compiler, ast->extract_obj, table);
    struct Scode *code = NULL_CODE_PTR;

    INSERT(code, extract_object_code);
    INSERT(code, extract_value_code);
    PUSH(code, LOAD_ITEM);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_store_index
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *extract_value_code = Scompiler_compile(compiler, ast->extract_obj->extract_value, table);
    struct Scode *extract_object_code = Scompiler_compile(compiler, ast->extract_obj->extract_obj, table);
    struct Scode *store_value_code = Scompiler_compile(compiler, ast->extract_value, table);
    struct Scode *code = NULL_CODE_PTR;

    INSERT(code, extract_object_code);
    INSERT(code, extract_value_code);
    INSERT(code, store_value_code);

    PUSH(code, STORE_ITEM);

    return code;
}

SUNY_API struct Scode* 
Scompiler_compile_ast_loop
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table) {
    struct Scode *code = NULL_CODE_PTR;
    int loop_start = compiler->label_counter++;
    int loop_end = compiler->label_counter++;
    compiler->is_in_loop = 1;
    ScompilerUnit_add_loop(compiler, loop_start, loop_end);
    struct Scode *loop_block = Scompiler_compile_ast_body(compiler, ast->block, table, ast->block_size);
    ScompilerUnit_pop_loop(compiler);
    compiler->is_in_loop = 0;
    if (ast->is_times) {
        int LOOP_COUNTER = 14;

        struct Scode *times_expr = Scompiler_compile(compiler, ast->times, table);

        INSERT(code, times_expr);
        PUSH(code, STORE_GLOBAL);
        PUSH(code, LOOP_COUNTER);

        PUSH(code, ADD_LABEL);
        PUSH(code, loop_start);

        PUSH(code, LOOP_PREP);
        PUSH(code, LOOP_COUNTER);
        PUSH(code, loop_end);

        INSERT(code, loop_block);

        PUSH(code, LOOP_STEP);
        PUSH(code, LOOP_COUNTER);
        PUSH(code, loop_start);

        PUSH(code, ADD_LABEL);
        PUSH(code, loop_end);

        ScompilerUnit_pop_loop(compiler);

        return code;
    }

    PUSH(code, ADD_LABEL);
    PUSH(code, loop_start);

    INSERT(code, loop_block);

    PUSH(code, JUMP_TO);
    PUSH(code, loop_start);

    PUSH(code, ADD_LABEL);
    PUSH(code, loop_end);


    return code;    
}
