#include "Suny.h"
#include "sdebug.h"

int SunyInstallLibrary(struct Sframe *frame, struct ScompilerUnit *, struct Stable *table)
{
    Sinitialize_variables(frame, table, "inf", 29, Svalue(INFINITY));
    Sinitialize_variables(frame, table, "nan", 30, Svalue(0.0 / 0.0));

    Sinitialize_c_api_func(frame, table, 31, 2, "print", Sprint);
    Sinitialize_c_api_func(frame, table, 32, 1, "exit", Sexit);
    Sinitialize_c_api_func(frame, table, 33, 1, "puts", Sputs);
    Sinitialize_c_api_func(frame, table, 34, 1, "read", Sread);
    Sinitialize_c_api_func(frame, table, 35, 1, "size", Ssize);
    Sinitialize_c_api_func(frame, table, 36, 2, "push", Spush);
    Sinitialize_c_api_func(frame, table, 37, 1, "pop", Spop);
    Sinitialize_c_api_func(frame, table, 38, 1, "load", Sload);
    Sinitialize_c_api_func(frame, table, 39, 3, "range", Srange);
    Sinitialize_c_api_func(frame, table, 40, 1, "list", Slist_cast);
    Sinitialize_c_api_func(frame, table, 41, 1, "string", Sstring_cast);
    Sinitialize_c_api_func(frame, table, 42, 1, "int", Sint_cast);
    Sinitialize_c_api_func(frame, table, 43, 1, "float", Sfloat_cast);
    Sinitialize_c_api_func(frame, table, 44, 1, "bool", Sbool_cast);
    Sinitialize_c_api_func(frame, table, 45, 1, "type", Stype);
    Sinitialize_c_api_func(frame, table, 46, 1, "copy", Scopy);
    Sinitialize_c_api_func(frame, table, 47, 1, "system", Ssystem);
    Sinitialize_c_api_func(frame, table, 48, 1, "char", Schar_cast);

    return 0;
}

struct Suny *SunyNew(void)
{
    struct Suny *suny = Smem_Malloc(sizeof(struct Suny));
    suny->frame = Sframe_new();
    suny->compiler = ScompilerUnit_new();
    suny->table = Stable_new();
    suny->gc_pool = Sgc_new_pool();
    suny->compiler->frame = suny->frame;
    suny->frame->compiler = suny->compiler;
    suny->frame->gc_pool = suny->gc_pool;
    SunyInstallLibrary(suny->frame, suny->compiler, suny->table);
    return suny;
}

struct Sframe *SunyRunString(char *str, struct Suny *suny)
{
    struct Slexer *lexer = Slexer_init(str);
    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);
    struct Scode *code = Scompiler_compile_ast_program(suny->compiler, ast, suny->table);

    suny->frame = Sframe_init(suny->frame, code);
    suny->frame = Svm_run_program(suny->frame);

    return suny->frame;
}

SUNY_API struct Sframe *SunyRunByteCode(unsigned char *code, int size, struct Suny *suny)
{
    struct Scode *c = Scode_new();

    if (size > MAX_CODE_SIZE)
    {
        __ERROR("Bytecode too large\n");
        return NULL;
    }

    memcpy(c->code, code, size);
    c->size = size;

    suny->frame = Sframe_init(suny->frame, c);
    suny->frame = Svm_run_program(suny->frame);

    return suny->frame;
}

int prompt()
{
    printf("Suny 1.0 Copyright (C) 2025-present, by dinhsonhai132\n");

    struct Sframe *frame = Sframe_new();
    struct ScompilerUnit *compiler = ScompilerUnit_new();
    compiler->frame = frame;
    frame->compiler = compiler;
    struct Stable *table = Stable_new();

    SunyInstallLibrary(frame, compiler, table);

    frame->gc_pool = Sgc_new_pool();

    char buff[1024];
    for (;;)
    {
        printf(">> ");
        if (!fgets(buff, sizeof(buff), stdin))
            break;

        if (strlen(buff) == 0)
            continue;

        struct Slexer *lexer = Slexer_init(buff);
        struct Sparser *parser = Sparser_init(lexer);
        struct Sast *ast = Sparser_parse_program(parser);

        struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);

        frame = Sframe_init(frame, code);
        frame = Svm_run_program(frame);
    }
    return 0;
}

struct Sframe *SunyRunAloneString(char *str)
{
    struct SZIO *zio = Sbuff_read_file(str);

    struct Slexer *lexer = Slexer_init(zio->buffer);
    lexer->file = zio;

    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);
    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();
    struct Sframe *frame = Sframe_new();
    compiler->frame = frame;
    frame->compiler = compiler;
    frame->gc_pool = Sgc_new_pool();

    SunyInstallLibrary(frame, compiler, table);

    struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);

    frame = Sframe_init(frame, code);
    frame = Svm_run_program(frame);

    return frame;
}

struct Sframe *SunyRunFile(char *file)
{
    struct SZIO *zio = Sbuff_read_file(file);

    struct Slexer *lexer = Slexer_init(zio->buffer);
    lexer->file = zio;

    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);

    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();
    struct Sframe *frame = Sframe_new();

    compiler->frame = frame;
    frame->compiler = compiler;

    frame->gc_pool = Sgc_new_pool();

    SunyInstallLibrary(frame, compiler, table);

    struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);

    frame = Sframe_init(frame, code);
    frame = Svm_run_program(frame);

    return frame;
}

struct Scode *SunyCompileFile(char *file)
{
    struct SZIO *zio = Sbuff_read_file(file);

    struct Slexer *lexer = Slexer_init(zio->buffer);
    lexer->file = zio;

    struct Sparser *parser = Sparser_init(lexer);
    struct Sast *ast = Sparser_parse_program(parser);

    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();
    struct Sframe *frame = Sframe_new();
    compiler->frame = frame;
    frame->compiler = compiler;
    frame->gc_pool = Sgc_new_pool();

    SunyInstallLibrary(frame, compiler, table);

    struct Scode *code = Scompiler_compile_ast_program(compiler, ast, table);

    return code;
}

struct Sframe *SunyRunAloneBytecode(unsigned char *code, int size)
{
    struct ScompilerUnit *compiler = ScompilerUnit_new();
    struct Stable *table = Stable_new();
    struct Sframe *frame = Sframe_new();
    compiler->frame = frame;
    frame->compiler = compiler;
    frame->gc_pool = Sgc_new_pool();

    SunyInstallLibrary(frame, compiler, table);

    struct Scode *c = Scode_new();

    if (size > MAX_CODE_SIZE)
    {
        __ERROR("Bytecode too large\n");
        return NULL;
    }

    memcpy(c->code, code, size);
    c->size = size;

    frame = Sframe_init(frame, c);
    frame = Svm_run_program(frame);

    return frame;
}
