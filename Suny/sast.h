#ifndef SAST_H
#define SAST_H

#include <stdlib.h>
#include <string.h>

#include "stok.h"
#include "slexer.h"

enum Stok_t;

#define MAX_STATEMENT_SIZE 1024

#define AST(t, v, l) Sast_init(t, v, l)

#define is_expr(ast)  ((ast)->type == AST_EXPRESSION                        \
|| (ast)->type == AST_BINARY_EXPRESSION                                     \
|| (ast)->type == AST_IDENTIFIER                                            \
|| (ast)->type == AST_FUNCTION_CALL_EXPRESSION                              \
|| (ast)->type == AST_CLOSURE_FUNCTION                                      \
|| (ast)->type == AST_TRUE                                                  \
|| (ast)->type == AST_FALSE                                                 \
|| (ast)->type == AST_AND_EXPRESSION                                        \
|| (ast)->type == AST_NULL_EXPRESSION                                       \
|| (ast)->type == AST_OR_EXPRESSION                                         \
|| (ast)->type == AST_CAST_EXPRESSION                                       \
|| (ast)->type == AST_NEGATIVE_EXPRESSION                                   \
|| (ast)->type == AST_FUNCTION_CALL_PRIMARY_EXPRESSION                      \
|| (ast)->type == AST_NOT_EXPRESSION                                        \
|| (ast)->type == AST_COMPARE_EXPRESSION                                    \
|| (ast)->type == AST_LIST                                                  \
|| (ast)->type == AST_ATTRIBUTE_EXPRESSION                                  \
|| (ast)->type == AST_ANONYMOUS_FUNCTION                                    \
|| (ast)->type == AST_EXTRACT                                               \
|| (ast)->type == AST_LITERAL                                               \
|| (ast)->type == AST_STRING_EXPRESSION)

enum Sast_t {
    AST_PROGRAM                                     = -1,
    AST_BLOCK                                       = -2,
    AST_STATEMENT                                   = -3,
    AST_EXPRESSION                                  = -4,
    AST_PRINT                                       = -5,
    AST_ATTRIBUTE_EXPRESSION                        = -6,
    AST_STORE_ATTRIBUTE                             = -7,
    AST_IMPORT                                      = -8,
    AST_INCLUDE                                     = -9,
    AST_BREAK                                       = -10,
    AST_CONTINUE                                    = -11,
    AST_ASSIGNMENT                                  = -12,
    AST_IF                                          = -13,
    AST_ANONYMOUS_FUNCTION                          = -14,
    AST_WHILE                                       = -15,
    AST_VAR_LIST                                    = -16,
    AST_DO_LOOP                                     = -17,
    AST_LOOP                                        = -18,
    AST_COMPARE_EXPRESSION                          = -19,
    AST_AND_EXPRESSION                              = -20,
    AST_RETURN_STATEMENT                            = -21,
    AST_OR_EXPRESSION                               = -22,
    AST_CLOSURE_FUNCTION                            = -23,
    AST_STORE_INDEX                                 = -24,
    AST_TRUE                                        = -25,
    AST_FALSE                                       = -26,
    AST_NOT_EXPRESSION                              = -27,
    AST_FUNCTION_CALL_EXPRESSION                    = -28,
    AST_ASSIGNMENT_STATEMENT                        = -29,
    AST_FUNCTION_CALL_PRIMARY_EXPRESSION            = -30,
    AST_FUNCTION_STATEMENT                          = -31,
    AST_FOR                                         = -32,
    AST_CLASS                                       = -33,
    AST_BINARY_EXPRESSION                           = -34,
    AST_IDENTIFIER                                  = -35,
    AST_CAST_EXPRESSION                             = -36,
    AST_LITERAL                                     = -37,
    AST_STRING_EXPRESSION                           = -38,
    AST_LIST                                        = -39,
    AST_EXTRACT                                     = -40,
    AST_NULL_EXPRESSION                             = -41,
    AST_NEGATIVE_EXPRESSION                         = -42,
    AST_NULL                                        = -43,
};


struct Sast {
    // type and value
    enum Sast_t type;
    float value;
    char *lexeme;

    // class
    char *parent_class_names[1024];
    int parent_class_count;
    int is_extends;
    int is_shared;

    // print
    struct Sast *print_value;
    
    // expression
    struct Sast *expr;

    // var list
    char** var_list;
    int var_list_size;
    int var_list_capacity;
    struct Sast **var_list_values;
    int var_list_values_size;
    int var_list_values_capacity;

    // expression statement
    struct Sast *left;
    struct Sast *right;
    enum Stok_t op;

    // if-else statement
    struct Sast *condition;
    struct Sast *if_body;
    struct Sast *else_body;
    struct Sast **elif_body;
    int elif_count;
    int elif_capacity;

    // function statement
    struct Sast **params;
    struct Sast *body;
    char **param_names;
    int is_having_params;
    struct Sast *ret_val;
    int param_count;
    int args_count;
    int is_lambda;

    // block statement
    struct Sast **block;
    int block_size;
    int block_count;
    int block_capacity;

    // variable
    struct Sast *var_value;
    char *var_name;
    
    // children
    struct Sast **children;

    int child_count;
    int child_capacity;

    // line
    int ast_line;
    int ast_column;

    // list
    struct Sast** list;
    int list_count;
    int list_capacity;

    // extract
    struct Sast *extract_obj;
    struct Sast *extract_value;

    // attribute
    struct Sast *attribute;
    struct Sast *target;
    char* attr_name;

    int is_assign;
    int has_until;

    // loop statement
    int is_times;
    int is_until;
    struct Sast *until;
    struct Sast *times;

    struct Slexer *lexer;

    char* file;
};

int 
Sast_set_line
(struct Slexer *lexer, struct Sast *sast);

int
Sast_make_block_with_stmt
(struct Sast *block, struct Sast *stmt);

struct Sast* 
Sast_new(void);

struct Sast* 
Sast_init
(enum Sast_t type, float value, char *lexeme);

int 
Sast_free
(struct Sast *sast);

int 
Sast_add_child
(struct Sast *parent, struct Sast *child);

int 
Sast_print
(struct Sast *sast);

int 
Sast_add_block
(struct Sast *parent, struct Sast *child);

int
Sast_set_para
(struct Sast *func, char* param_names);

int
Sast_add_args
(struct Sast *func, struct Sast *param);

struct Sast* 
Sast_get_child
(struct Sast *sast, int index);

struct Sast*
Sast_add_element
(struct Sast *list, struct Sast *element);

struct Sast*
Sast_add_var_list_value
(struct Sast *ast, struct Sast *var_value);

struct Sast* 
Sast_add_var_list_name
(struct Sast *sast, char* var_name);

struct Sast* 
Sast_add_var_list_value
(struct Sast *sast, struct Sast *var_value);

struct Sast* 
Sast_assign_var_list_value
(struct Sast *sast);

#endif
