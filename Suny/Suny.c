#include "Suny.h"

int SunyInstallLibrary(struct Sframe* frame, struct ScompilerUnit* compiler, struct Stable* table) {
    Sinitialize_c_api_func(frame, table, 30, 2, "print", Sprintf);
    Sinitialize_c_api_func(frame, table, 31, 1, "tostring", Stostring);
    Sinitialize_c_api_func(frame, table, 32, 1, "exit", Sexit);
    Sinitialize_c_api_func(frame, table, 33, 1, "puts", Sputs);
    Sinitialize_c_api_func(frame, table, 34, 1, "read", Sread);
    Sinitialize_c_api_func(frame, table, 35, 1, "number", Snumber);
    Sinitialize_c_api_func(frame, table, 36, 1, "size", Ssize);
    Sinitialize_c_api_func(frame, table, 37, 2, "push", Spush);
    Sinitialize_c_api_func(frame, table, 38, 1, "pop", Spop);
    Sinitialize_c_api_func(frame, table, 39, 1, "load_dll", Sload_dll);
    Sinitialize_c_api_func(frame, table, 40, 1, "isdigit", Sisdigit_builtin);
    Sinitialize_c_api_func(frame, table, 41, 1, "tonumber", Sint);
    Sinitialize_c_api_func(frame, table, 42, 3, "range", Srange);
    Sinitialize_c_api_func(frame, table, 43, 2, "cast", Scast);
    Sinitialize_c_api_func(frame, table, 44, 1, "list", Slist_cast);
    Sinitialize_c_api_func(frame, table, 45, 1, "string", Sstring_cast);
    Sinitialize_c_api_func(frame, table, 46, 1, "int", Sint_cast);
    Sinitialize_c_api_func(frame, table, 47, 1, "float", Sfloat_cast);
    Sinitialize_c_api_func(frame, table, 48, 1, "bool", Sbool_cast);
    Sinitialize_c_api_func(frame, table, 49, 1, "type", Stype);
    return 0;
}

int prompt() {
    printf("Suny 1.0 Copyright (C) 2025-present, by dinhsonhai132\n");

    struct Sframe *frame = Sframe_new();
    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();

    SunyInstallLibrary(frame, compiler, table);

    char buff[1024];
    for (;;) {
        printf(">> ");
        if (!fgets(buff, sizeof(buff), stdin)) break;
        
        if (strlen(buff) == 0) continue;

        struct Slexer *lexer = Slexer_init(buff);
        struct Sparser *parser = Sparser_init(lexer);
        struct Sast *ast = Sparser_parse_program(parser);

        if (ast == NULL) {
            printf("Syntax Error!\n");
            continue;
        }

        struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);
        
        frame = Sframe_init(frame, code);
        frame = Svm_run_program(frame);
    }
    return 0;
}

struct Sframe* SunyRunSimpleString(char* str) {
    struct SZIO* zio = Sbuff_read_file(str);

    struct Slexer *lexer = Slexer_init(zio->buffer);
    lexer->file = zio;

    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);
    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();
    struct Sframe *frame = Sframe_new();
    SunyInstallLibrary(frame, compiler, table);

    struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);

    frame = Sframe_init(frame, code);
    frame = Svm_run_program(frame);
    
    return frame;
}

struct Sframe* SunyRunFile(char* file) {
    struct SZIO* zio = Sbuff_read_file(file);

    struct Slexer *lexer = Slexer_init(zio->buffer);
    lexer->file = zio;

    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);

    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();
    struct Sframe *frame = Sframe_new();

    frame->f_heaps = Smem_Calloc(MAX_FRAME_SIZE, sizeof(struct Sobj *));
    frame->gc_pool = Sgc_new_pool();

    SunyInstallLibrary(frame, compiler, table);

    struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);

    frame = Sframe_init(frame, code);
    frame = Svm_run_program(frame);

    return frame;
}

struct Scode* SunyCompileFile(char* file) {
    struct SZIO* zio = Sbuff_read_file(file);

    struct Slexer *lexer = Slexer_init(zio->buffer);
    lexer->file = zio;

    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);

    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();
    struct Sframe *frame = Sframe_new();

    frame->f_heaps = Smem_Calloc(MAX_FRAME_SIZE, sizeof(struct Sobj *));
    frame->gc_pool = Sgc_new_pool();

    SunyInstallLibrary(frame, compiler, table);

    struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);

    return code;
}