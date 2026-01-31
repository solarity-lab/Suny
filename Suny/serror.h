#ifndef SERROR_H
#define SERROR_H

#include <stdio.h>
#include <stdlib.h>

#include "slexer.h"
#include "sast.h"
#include "score.h"

struct Sast;

struct Serror {
    char* type;
    char* message;
  
    int line;
    int column;

    char* file;

    struct Slexer *lexer;
};

struct Serror *
Serror_new(void);

struct Serror *
Serror_set(char *type, char *message, struct Slexer *lexer);

int
Serror_syntax_error
(struct Serror *error);

int
Serror_free
(struct Serror *error);

int
Serror_expected_expression
(struct Sast *sast);

int 
Serror_parser
(char *message, struct Slexer *lexer);

int 
Serror_compiler_error
(char *message, struct Sast *ast);

int 
Serror_at
(struct Serror *error);

#endif