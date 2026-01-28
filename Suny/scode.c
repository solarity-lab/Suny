#include "scode.h"
#include "smem.h"

const char* print_op(unsigned char c) {
    switch (c) {
        case PUSH_FLOAT: return "PUSH_FLOAT";

        case BINARY_ADD: return "BINARY_ADD";
        case BINARY_SUB: return "BINARY_SUB";
        case BINARY_MUL: return "BINARY_MUL";
        case BINARY_DIV: return "BINARY_DIV";

        case PRINT: return "PRINT";
        case POP_TOP: return "POP_TOP";

        case LOAD_GLOBAL: return "LOAD_GLOBAL";
        case STORE_GLOBAL: return "STORE_GLOBAL";

        case PROGRAM_START: return "PROGRAM_START";
        case PROGRAM_END: return "PROGRAM_END";

        case BINARY_BIGGER: return "BINARY_BIGGER";
        case BINARY_SMALLER: return "BINARY_SMALLER";
        case BINARY_EQUAL: return "BINARY_EQUAL";
        case BINARY_BIGGER_EQUAL: return "BINARY_BIGGER_EQUAL";
        case BINARY_SMALLER_EQUAL: return "BINARY_SMALLER_EQUAL";
        case BINARY_NOT_EQUAL: return "BINARY_NOT_EQUAL";

        case MAKE_FUNCTION: return "MAKE_FUNCTION";
        case END_FUNCTION: return "END_FUNCTION";

        case FUNCTION_CALL: return "FUNCTION_CALL";
        case RETURN_TOP: return "RETURN_TOP";

        case PUSH_STRING: return "PUSH_STRING";

        case PUSH_TRUE: return "PUSH_TRUE";
        case PUSH_FALSE: return "PUSH_FALSE";

        case POP_JUMP_IF_TRUE: return "POP_JUMP_IF_TRUE";
        case POP_JUMP_IF_FALSE: return "POP_JUMP_IF_FALSE";

        case JUMP_TO: return "JUMP_TO";
        case ADD_LABEL: return "ADD_LABEL";

        case JUMP_FORWARD: return "JUMP_FORWARD";
        case JUMP_BACKWARD: return "JUMP_BACKWARD";

        case EXIT_PROGRAM: return "EXIT_PROGRAM";
        case STOP_PROGRAM: return "STOP_PROGRAM";

        case LOAD_ITEM: return "LOAD_ITEM";
        case STORE_ITEM: return "STORE_ITEM";

        case BUILD_LIST: return "BUILD_LIST";
        case LEN_OF: return "LEN_OF";

        case LOAD_TRUE: return "LOAD_TRUE";
        case LOAD_FALSE: return "LOAD_FALSE";

        case CLASS_BEGIN: return "CLASS_BEGIN";
        case CLASS_END: return "CLASS_END";

        case AND_LOG: return "AND_LOG";
        case OR_LOG: return "OR_LOG";
        case NOT_LOG: return "NOT_LOG";

        case SKIP: return "SKIP";

        case LOAD_ATTR: return "LOAD_ATTR";
        case STORE_ATTR: return "STORE_ATTR";
        case LOAD_METHOD: return "LOAD_METHOD";

        case FUNCTION_CALL_VARARGS: return "FUNCTION_CALL_VARARGS";

        case LOAD_NULL: return "LOAD_NULL";

        case SET_SUPER_CLASS: return "SET_SUPER_CLASS";

        case START_BLOCK: return "START_BLOCK";
        case END_BLOCK: return "END_BLOCK";

        case MAKE_CLOSURE: return "MAKE_CLOSURE";
        case END_CLOSURE: return "END_CLOSURE";
        case LOAD_CLOSURE: return "LOAD_CLOSURE";
        case STORE_CLOSURE: return "STORE_CLOSURE";

        case JUMP_TO_N_TIMES: return "JUMP_TO_N_TIMES";

        case LOOP_STEP: return "LOOP_STEP";
        case LOOP_PREP: return "LOOP_PREP";

        case LOAD_LOCAL: return "LOAD_LOCAL";
        case STORE_LOCAL: return "STORE_LOCAL";

        case MAKE_ARGS: return "MAKE_ARGS";
        case START_FUNCTION: return "START_FUNCTION";
    }

    return Sstring("0x%02x", c);
}

struct Scode *
Scode_new(void) {
    struct Scode *code = Smem_Malloc(sizeof(struct Scode));

    code->size = 0;
    code->capacity = MAX_CODE_SIZE;

    return code;
}

struct Scode*
Scode_encryption_address(int address) {
    struct Scode *code = Scode_new();
    
    byte_t u1 = address & 0xFF;
    byte_t u2 = (address >> 8) & 0xFF;
    byte_t u3 = (address >> 16) & 0xFF;
    byte_t u4 = (address >> 24) & 0xFF;

    PUSH(code, u1);
    PUSH(code, u2);
    PUSH(code, u3);
    PUSH(code, u4);

    return code;
}

int
Scode_free
(struct Scode *code) {
    Smem_Free(code);
    return 0;
}

int 
Scode_add
(struct Scode *code, unsigned char c) {
    code->code[code->size++] = c;
    return 0;
}

int
Scode_push
(struct Scode *scode, struct Scode *code) {
    for (int i = 0; i < code->size; i++) {
        Scode_add(scode, code->code[i]);
    }

    return 0;
}   

struct Scode *
Scode_insert_to_top(struct Scode *code, struct Scode *insert) {
    struct Scode *new_code = Scode_new();

    for (int i = 0; i < insert->size; i++) {
        Scode_add(new_code, insert->code[i]);
    }

    for (int i = 0; i < code->size; i++) {
        Scode_add(new_code, code->code[i]);
    }

    return new_code;
}

int Scode_print(struct Scode *code) {

    for (int i = 0; i < code->size; i++) {
        unsigned char c = code->code[i];

        printf("[%03d] 0x%02X  ", i, c);

        switch (c) {
            case PUSH_FLOAT: {
                float f;
                memcpy(&f, &code->code[i + 1], 4);
                printf("PUSH_FLOAT %f\n", f);
                i += 4;
                break;
            }

            case PUSH_STRING: {
                int size = code->code[++i];
                printf("PUSH_STRING (len=%d) \"", size);
                for (int j = 0; j < size; j++)
                    printf("%c", code->code[i + 1 + j]);
                printf("\"\n");
                i += size;
                break;
            }

            case LOAD_CLOSURE:
                printf("LOAD_CLOSURE %d\n", code->code[i + 1]);
                i++;
                break;
            
            case STORE_CLOSURE:
                printf("STORE_CLOSURE %d\n", code->code[i + 1]);
                i++;
                break;

            case LOAD_GLOBAL:
                printf("LOAD_GLOBAL %d\n", code->code[i + 1]);
                i++;
                break;

            case STORE_GLOBAL:
                printf("STORE_GLOBAL %d\n", code->code[i + 1]);
                i++;
                break;
            
            case LOAD_LOCAL:
                printf("LOAD_LOCAL %d\n", code->code[i + 1]);
                i++;
                break;
            
            case STORE_LOCAL:
                printf("STORE_LOCAL %d\n", code->code[i + 1]);
                i++;
                break;

            case LOAD_MEMBER:
                printf("LOAD_MEMBER %d\n", code->code[i + 1]);
                i++;
                break;

            case STORE_MEMBER:
                printf("STORE_MEMBER %d\n", code->code[i + 1]);
                i++;
                break;

            case LOAD_ATTR:
                printf("LOAD_ATTR %d\n", code->code[i + 1]);
                i++;
                break;

            case STORE_ATTR:
                printf("STORE_ATTR %d\n", code->code[i + 1]);
                i++;
                break;

            case ADD_LABEL:
                printf("ADD_LABEL %d\n", code->code[i + 1]);
                i++;
                break;

            case JUMP_TO:
                printf("JUMP_TO %d\n", code->code[i + 1]);
                i++;
                break;

            case POP_JUMP_IF_TRUE:
                printf("POP_JUMP_IF_TRUE  %d\n", code->code[i + 1]);
                i++;
                break;

            case POP_JUMP_IF_FALSE:
                printf("POP_JUMP_IF_FALSE %d\n", code->code[i + 1]);
                i++;
                break;

            case BUILD_LIST:
                printf("BUILD_LIST   count=%d\n", code->code[i + 1]);
                i++;
                break;

            case MAKE_FUNCTION:
                printf("MAKE_FUNCTION size=%d\n", code->code[i + 1]);
                i++;
                break;
            
            case JUMP_TO_N_TIMES:
                printf("JUMP_TO_N_TIMES  %d\n", code->code[i + 1]);
                i++;
                break;

            case MAKE_CLOSURE:
                printf("MAKE_CLOSURE size=%d\n", code->code[i + 1]);
                i++;
                break;
            
            case LOOP_STEP:
                printf("LOOP_STEP var=%d addr=%d\n", code->code[i + 1], code->code[i + 2]);
                i += 2;
                break;

            case LOOP_PREP:
                printf("LOOP_PREP var=%d addr=%d\n", code->code[i + 1], code->code[i + 2]);
                i += 2;
                break;
            
            case SET_SUPER_CLASS:
                printf("SET_SUPER_CLASS %d\n", code->code[i + 1]);
                i++;
                break;
            
            case MAKE_ARGS:
                printf("MAKE_ARGS %d\n", code->code[i + 1]);
                i++;
                break;

            case CLASS_BEGIN:        printf("CLASS_BEGIN\n"); break;
            case CLASS_END:          printf("CLASS_END\n"); break;
            case FUNCTION_CALL:      printf("FUNCTION_CALL\n"); break;
            case STORE_ITEM:         printf("STORE_ITEM\n"); break;
            case END_CLOSURE:        printf("END_CLOSURE\n"); break;
            case END_FUNCTION:       printf("END_FUNCTION\n"); break;
            case PRINT:              printf("PRINT\n"); break;
            case START_FUNCTION:     printf("START_FUNCTION\n"); break;
            case RETURN_TOP:         printf("RETURN_TOP\n"); break;
            case POP_TOP:            printf("POP_TOP\n"); break;
            case BINARY_ADD:         printf("BINARY_ADD\n"); break;
            case BINARY_SUB:         printf("BINARY_SUB\n"); break;
            case BINARY_MUL:         printf("BINARY_MUL\n"); break;
            case LEN_OF:             printf("LEN_OF\n"); break;
            case BINARY_DIV:         printf("BINARY_DIV\n"); break;
            case START_BLOCK:        printf("START_BLOCK\n"); break;
            case END_BLOCK:          printf("END_BLOCK\n"); break;
            case BINARY_EQUAL:       printf("BINARY_EQUAL\n"); break;
            case BINARY_NOT_EQUAL:   printf("BINARY_NOT_EQUAL\n"); break;
            case LOAD_NULL:          printf("LOAD_NULL\n"); break;
            case LOAD_TRUE:          printf("LOAD_TRUE\n"); break;
            case LOAD_FALSE:         printf("LOAD_FALSE\n"); break;
            case BINARY_BIGGER:      printf("BINARY_BIGGER\n"); break;
            case BINARY_SMALLER:     printf("BINARY_SMALLER\n"); break;
            case BINARY_BIGGER_EQUAL:printf("BINARY_BIGGER_EQUAL\n"); break;
            case BINARY_SMALLER_EQUAL:printf("BINARY_SMALLER_EQUAL\n"); break;
            case LOAD_ITEM:          printf("LOAD_ITEM\n"); break;
            case AND_LOG:            printf("AND_LOG\n"); break;
            case OR_LOG:             printf("OR_LOG\n"); break;
            case NOT_LOG:            printf("NOT_LOG\n"); break;
            case PROGRAM_START:      printf("PROGRAM_START\n"); break;
            case PROGRAM_END:        printf("PROGRAM_END\n"); break;
            default:
                printf("Unknown opcode: 0x%02X\n", c);
                break;
        }
    }

    return 0;
}

void Scode_print_disasm(struct Scode *code) {
    for (int i = 0; i < code->size;) {
        unsigned char op = code->code[i];
        printf("%04d: 0x%02X", i, op);
        i++;

        switch (op) {

        case PUSH_FLOAT: {
            if (i + 4 <= code->size) {
                float f;
                for (int j = 0; j < 4; j++)
                    printf(" 0x%02X", code->code[i + j]);

                memcpy(&f, &code->code[i], 4);
                printf("  // PUSH_FLOAT %f\n", f);
                i += 4;
            } else {
                printf("  // PUSH_FLOAT (incomplete)\n");
            }
            break;
        }

        case PUSH_STRING: {
            if (i < code->size) {
                int len = code->code[i++];
                printf(" 0x%02X  // PUSH_STRING len=%d \"", len, len);
                for (int j = 0; j < len && i + j < code->size; j++)
                    printf("%c", code->code[i + j]);
                printf("\"\n");
                i += len;
            }
            break;
        }

        case LOOP_STEP:
        case LOOP_PREP:
            if (i + 1 < code->size) {
                printf(" 0x%02X 0x%02X  // %s\n",
                       code->code[i], code->code[i + 1],
                       op == LOOP_STEP ? "LOOP_STEP" : "LOOP_PREP");
                i += 2;
            }
            break;

        case LOAD_GLOBAL:
        case STORE_GLOBAL:
        case LOAD_LOCAL:
        case STORE_LOCAL:
        case STORE_MEMBER:
        case LOAD_MEMBER:
        case LOAD_CLOSURE:
        case STORE_CLOSURE:
        case LOAD_ATTR:
        case STORE_ATTR:
        case ADD_LABEL:
        case JUMP_TO:
        case JUMP_TO_N_TIMES:
        case POP_JUMP_IF_TRUE:
        case POP_JUMP_IF_FALSE:
        case BUILD_LIST:
        case MAKE_FUNCTION:
        case MAKE_CLOSURE:
            if (i < code->size) {
                printf(" 0x%02X  // %s\n", code->code[i],
                    op == LOAD_GLOBAL ? "LOAD_GLOBAL" :
                    op == STORE_GLOBAL ? "STORE_GLOBAL" :
                    op == LOAD_CLOSURE ? "LOAD_CLOSURE" :
                    op == STORE_CLOSURE ? "STORE_CLOSURE" :
                    op == LOAD_ATTR ? "LOAD_ATTR" :
                    op == STORE_ATTR ? "STORE_ATTR" :
                    op == ADD_LABEL ? "ADD_LABEL" :
                    op == JUMP_TO ? "JUMP_TO" :
                    op == JUMP_TO_N_TIMES ? "JUMP_TO_N_TIMES" :
                    op == POP_JUMP_IF_TRUE ? "POP_JUMP_IF_TRUE" :
                    op == POP_JUMP_IF_FALSE ? "POP_JUMP_IF_FALSE" :
                    op == BUILD_LIST ? "BUILD_LIST" :
                    op == MAKE_CLOSURE ? "MAKE_CLOSURE" :
                    "MAKE_FUNCTION");
                i++;
            }
            break;

        case CLASS_BEGIN:        printf("  // CLASS_BEGIN\n"); break;
        case CLASS_END:          printf("  // CLASS_END\n"); break;
        case FUNCTION_CALL:      printf("  // FUNCTION_CALL\n"); break;
        case END_FUNCTION:       printf("  // END_FUNCTION\n"); break;
        case STORE_ITEM:         printf("  // STORE_ITEM\n"); break;
        case PRINT:              printf("  // PRINT\n"); break;
        case LEN_OF:             printf("  // LEN_OF\n"); break;
        case LOAD_NULL:          printf("  // LOAD_NULL\n"); break;
        case LOAD_TRUE:          printf("  // LOAD_TRUE\n"); break;
        case LOAD_FALSE:         printf("  // LOAD_FALSE\n"); break;
        case RETURN_TOP:         printf("  // RETURN_TOP\n"); break;
        case POP_TOP:            printf("  // POP_TOP\n"); break;
        case BINARY_ADD:         printf("  // BINARY_ADD\n"); break;
        case BINARY_SUB:         printf("  // BINARY_SUB\n"); break;
        case BINARY_MUL:         printf("  // BINARY_MUL\n"); break;
        case BINARY_DIV:         printf("  // BINARY_DIV\n"); break;
        case BINARY_EQUAL:       printf("  // BINARY_EQUAL\n"); break;
        case BINARY_NOT_EQUAL:   printf("  // BINARY_NOT_EQUAL\n"); break;
        case BINARY_BIGGER:      printf("  // BINARY_BIGGER\n"); break;
        case BINARY_SMALLER:     printf("  // BINARY_SMALLER\n"); break;
        case BINARY_BIGGER_EQUAL:printf("  // BINARY_BIGGER_EQUAL\n"); break;
        case BINARY_SMALLER_EQUAL:printf("  // BINARY_SMALLER_EQUAL\n"); break;
        case AND_LOG:            printf("  // AND_LOG\n"); break;
        case OR_LOG:             printf("  // OR_LOG\n"); break;
        case NOT_LOG:            printf("  // NOT_LOG\n"); break;
        case PROGRAM_START:      printf("  // PROGRAM_START\n"); break;
        case PROGRAM_END:        printf("  // PROGRAM_END\n"); break;

        default:
            printf("  // UNKNOWN OPCODE\n");
            break;
        }
    }
}

struct Scode*
Scode_get_code_from_file
(char* file, struct ScompilerUnit *compiler, struct Stable *table) {
    struct SZIO* zio = Sbuff_read_file(file);

    struct Slexer *lexer = Slexer_init(zio->buffer);
    lexer->file = zio;

    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);
    struct Scode *code = Scode_new();

    for (int i = 0; i < ast->child_count; i++) {
        struct Scode *child = Scompiler_compile(compiler, ast->children[i], table);
        INSERT(code, child);

        if (is_expr(ast->children[i])) {
            PUSH(code, POP_TOP);
        }
    }

    return code;
}