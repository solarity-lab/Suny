#include "sast.h"
#include "smem.h"

int 
Sast_print
(struct Sast *sast) {
    switch(sast->type) { 
        case AST_PROGRAM:
            printf("Program\n");
            break;
        case AST_BLOCK:
            printf("Block\n");
            break;
        case AST_STATEMENT:
            printf("Statement\n");
            break;
        case AST_EXPRESSION:
            printf("Expression\n");
            break;
        case AST_ASSIGNMENT:
            printf("Assignment\n");
            break;
        case AST_IF:
            printf("If\n");
            break;
        case AST_WHILE:
            printf("While\n");
            break;
        case AST_DO_LOOP:
            printf("Do Loop\n");
            break;
        case AST_FOR:
            printf("For\n");
            break;
        case AST_BINARY_EXPRESSION:
            printf("Binary Expression\n");
            printf("Operator: %s\n", Stok_t_print(sast->op));
            Sast_print(sast->left);
            Sast_print(sast->right);
            break;
        case AST_IDENTIFIER:
            printf("Identifier ");
            break;
        case AST_LITERAL:
            printf("Literal\n");
            break;
        case AST_STRING_EXPRESSION:
            printf("String Expression\n");
            break;
        case AST_FUNCTION_CALL_EXPRESSION:
            printf("Function Call\n");
            break;
        case AST_FUNCTION_STATEMENT:
            printf("Function\n");
            break;
        case AST_NULL:
            printf("Null\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    return 0;
}
 
int Sast_set_line(struct Slexer *lexer, struct Sast *sast) {
    sast->ast_line = lexer->line;
    sast->ast_column = lexer->column;
    sast->lexer = lexer;
    sast->file = lexer->file->file;
    return 0;
}

struct Sast* Sast_new(void) {
    struct Sast *sast = Smem_Malloc(sizeof(struct Sast));

    sast->type = AST_NULL;
    sast->parent_class_names[0] = NULL;

    sast->op = NULL_TOK;

    sast->ast_line = 1;
    sast->ast_column = 1;

    sast->lexer = Slexer_new();

    return sast;
}

int
Sast_add_key(struct Sast* ast, struct Sast* key) {
    if (!ast->keys) {
        ast->keys = Sast_init(AST_BLOCK, 0, NULL);
    }

    Sast_add_block(ast->keys, key);
    return 0;
}

int
Sast_add_value(struct Sast* ast, struct Sast* value)  {
    if (!ast->values) {
        ast->values = Sast_init(AST_BLOCK, 0, NULL);
    }

    Sast_add_block(ast->values, value);
    return 0;
}

struct Sast* 
Sast_init
(enum Sast_t type, float value, char *lexeme) {
    struct Sast *sast = Sast_new();

    sast->type = type;
    sast->value = value;
    sast->lexeme = lexeme;

    return sast;
}

int Sast_free(struct Sast *sast) {
    if (!sast) return 0;

    Smem_Free(sast->left);
    Smem_Free(sast->right);
    Smem_Free(sast->expr);
    Smem_Free(sast->condition);
    Smem_Free(sast->ret_val);

    Smem_Free(sast->else_body);

    Smem_Free(sast->body);

    for (int i = 0; i < sast->param_count; i++) {
        Sast_free(sast->params[i]);
    }
    Smem_Free(sast->params);

    Smem_Free(sast->param_names);

    for (int i = 0; i < sast->block_size; i++) {
        Sast_free(sast->block[i]);
    }
    Smem_Free(sast->block);

    for (int i = 0; i < sast->list_count; i++) {
        Sast_free(sast->list[i]);
    }
    Smem_Free(sast->list);

    for (int i = 0; i < sast->child_count; i++) {
        Sast_free(sast->children[i]);
    }
    Smem_Free(sast->children);

    Smem_Free(sast->times);
    Smem_Free(sast->until);

    Smem_Free(sast->extract_obj);
    Smem_Free(sast->extract_value);
    Smem_Free(sast->attribute);
    Smem_Free(sast->target);
    Smem_Free(sast->attr_name);

    Smem_Free(sast->print_value);

    if (sast->lexer) {
        Slexer_free(sast->lexer);
        sast->lexer = NULL;
    }

    Smem_Free(sast);
    return 0;
}


int 
Sast_add_child
(struct Sast *parent, struct Sast *child) {
    if (parent->child_capacity == 0) {
        parent->child_capacity = MAX_STATEMENT_SIZE;
        parent->children = Smem_Calloc(MAX_STATEMENT_SIZE, sizeof(struct Sast *));
    }

    if (parent->child_count >= parent->child_capacity) {
        parent->child_capacity *= 2;
        parent->children = (struct Sast **)Smem_Realloc(parent->children, sizeof(struct Sast *) * parent->child_capacity);
    }

    parent->children[parent->child_count++] = child;

    return 0;
}

int 
Sast_add_block
(struct Sast *parent, struct Sast *child) {
    if (parent->block_capacity == 0) {
        parent->block_capacity = 1024;
        parent->block = (struct Sast **)Smem_Malloc(sizeof(struct Sast *) * parent->block_capacity);
    }
    
    if (parent->block_count >= parent->block_capacity) {
        parent->block_capacity *= 2;
        parent->block = (struct Sast **)Smem_Realloc(parent->block, sizeof(struct Sast *) * parent->block_capacity);
    }

    parent->block[parent->block_count++] = child;

    return 0;
}

struct Sast* 
Sast_get_child
(struct Sast *sast, int index) {
    return sast->children[index];
}

int
Sast_set_para
(struct Sast *func, char* param_names) {
    if (func->param_count == 0) {
        func->param_names = Smem_Calloc(MAX_STATEMENT_SIZE, sizeof(struct Sast *));
    }

    func->param_names[func->param_count++] = param_names;
    return 0;
}

int
Sast_add_args
(struct Sast *func, struct Sast *param) {
    if (func->param_count == 0) {
        func->params = Smem_Calloc(MAX_STATEMENT_SIZE, sizeof(struct Sast *));
    }

    func->params[func->param_count++] = param;
    return 0;
}

struct Sast*
Sast_add_element(struct Sast *list, struct Sast *element) {
    if (list->list_capacity == 0) {
        list->list_capacity = 1024;
        list->list = (struct Sast **)Smem_Malloc(sizeof(struct Sast *) * list->list_capacity);
    } 
    
    if (list->list_count >= list->list_capacity) {
        list->list_capacity *= 2;
        list->list = (struct Sast **)Smem_Realloc(list->list, sizeof(struct Sast *) * list->list_capacity);
    }

    list->list[list->list_count++] = element;
    return list;
}

int
Sast_make_block_with_stmt
(struct Sast *block, struct Sast *stmt) {
    Sast_add_block(block, stmt);
    return 0;
}

int
Sast_add_operands(struct Sast *sast, struct Sast *oper) {
    if (sast->operand_capacity == 0) {
        sast->operand_capacity = MAX_STATEMENT_SIZE;
        sast->operands = Smem_Calloc(MAX_STATEMENT_SIZE, sizeof(struct Sast *));
    }

    if (sast->operand_count >= sast->operand_capacity) {
        sast->operand_capacity *= 2;
        sast->operands = (struct Sast **)Smem_Realloc(sast->operands, sizeof(struct Sast *) * sast->operand_capacity);
    }

    sast->operands[sast->operand_count++] = oper;
    return 0;
}

int
Sast_add_ops
(struct Sast *sast, enum Stok_t op) {
    if (sast->ops_capacity == 0) {
        sast->ops_capacity = 1024;
        sast->ops = (enum Stok_t *) Smem_Malloc(sizeof(enum Stok_t) * sast->ops_capacity);
    }

    if (sast->ops_count >= sast->ops_capacity) {
        sast->ops_capacity *= 2;
        sast->ops = (enum Stok_t *)Smem_Realloc(sast->ops, sizeof(enum Stok_t) * sast->ops_capacity);
    }

    sast->ops[sast->ops_count++] = op;
    return 0;
}
