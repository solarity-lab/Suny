#include "serror.h"
#include "smem.h"

struct Serror *
Serror_new(void) {
    struct Serror *error = Smem_Malloc(sizeof(struct Serror));

    error->type = NULL;
    error->message = NULL;

    error->line = 1;
    error->column = 1;
    error->file = NULL;

    error->lexer = Slexer_new();

    return error;
}

int
Serror_free
(struct Serror *error) {
    Slexer_free(error->lexer);
    Smem_Free(error);
    return 0;
}

struct Serror *
Serror_set(char *type, char *message, struct Slexer *lexer) {
    struct Serror *error = Serror_new();

    error->type = type;
    error->message = message;

    error->line = lexer->line;
    error->column = lexer->column;

    error->lexer = lexer;

    error->file = lexer->file->file;

    return error;
}

int
Serror_syntax_error
(struct Serror *error) {
    printf("%s: %s\n", error->type, error->message);
    printf("At file '%s', line %d\n", error->file, error->line);
    SUNY_BREAK_POINT;
    return 0;
}

int
Serror_expected_expression(struct Sast *sast) {
    Sast_set_line(sast->lexer, sast);

    if (!sast) {
        struct Serror *error = Serror_set("SYNTAX_ERROR", "Expected expression", sast->lexer);
        Serror_syntax_error(error);
        return 0;
    };

    if (!is_expr(sast)) {
        struct Serror *error = Serror_set("SYNTAX_ERROR", "Expected expression", sast->lexer);
        Serror_syntax_error(error);
        return 0;
    }

    return 0;
}

int 
Serror_parser
(char *message, struct Slexer *lexer) {
    struct Serror *error = Serror_set("SYNTAX_ERROR", message, lexer);
    Serror_syntax_error(error);
    return 0;
}

int 
Serror_at
(struct Serror *error) {
    printf("%s: %s\n", error->type, error->message);
    printf("At file '%s', line %d\n", error->file, error->line);
    SUNY_BREAK_POINT;
    return 0;
}

int 
Serror_compiler_error
(char *message, struct Sast *ast) {
    struct Serror *error = Serror_set("COMPILER_ERROR", message, ast->lexer);
    
    error->line = ast->ast_line;
    error->column = ast->ast_column;

    error->file = ast->lexer->file->file;

    Serror_at(error);
    return 0;
}