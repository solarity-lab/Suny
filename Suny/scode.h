#ifndef SCODE_H
#define SCODE_H

#include "score.h"
#include "scompiler.h"

struct Stable;
struct ScompilerUnit;

typedef unsigned char byte_t;

#define INSERT_ADDRESS(code, address) INSERT(code, Scode_encryption_address(address));

#define MAX_CODE_SIZE 1024

#define PUSH(code, c) Scode_add(code, c)

#define INSERT(code, c) Scode_push(code, c)

#define IS_BINARY_OPER(c) (c == BINARY_ADD || c == BINARY_SUB || c == BINARY_MUL || c == BINARY_DIV || c == BINARY_BIGGER_EQUAL || c == BINARY_BIGGER || c == BINARY_SMALLER_EQUAL || c == BINARY_SMALLER || c == BINARY_EQUAL || c == BINARY_NOT_EQUAL || c == BINARY_MOD)

struct Sop {
    unsigned char op;
    int line;
    int column;
    int source_line;
    int source_column;
};

struct Scode {
    int size;
    int capacity;
    int op_size;
    int op_capacity;
    char *source;
    char *file;
    unsigned char code[MAX_CODE_SIZE];
    struct Sop op[MAX_CODE_SIZE];
};

struct Scode*
Scode_encryption_address
(int address);

struct Scode*
Scode_get_code_from_file
(char* file, struct ScompilerUnit *compiler, struct Stable *table);

struct Scode *
Scode_insert_to_top
(struct Scode *code, struct Scode *insert);

struct Scode *
Scode_new(void);

int 
Scode_free
(struct Scode *code);

int 
Scode_add
(struct Scode *code, unsigned char c);

int
Scode_push
(struct Scode *scode, struct Scode *code);

int
Scode_print
(struct Scode *code);

void 
Scode_print_disasm(struct Scode *code);

const char* 
print_op(unsigned char c);

#endif // SCODE_H 