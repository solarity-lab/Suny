#include <stdio.h>
#include <string.h>

#include "Suny.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        prompt();
        return 1;
    }

    char* file_name = argv[1];

    if (same_string(file_name, "-c")) {
        file_name = argv[2];
        compilefile(file_name);
        return 0;
    }

    if (same_string(argv[1], "-m")) {
        if (argc < 3) {
            printf("Usage: suny -m <file> [-o output]\n");
            return 1;
        }

        struct Scode *code = SunyCompileFile(argv[2]);
        const char *out = "a.o";

        if (argc >= 5 && same_string(argv[3], "-o")) {
            out = argv[4];
        }

        struct SZIO *zio = Sbuff_write_bytecode_file(code->code, code->size, out);

        Sbuff_free(zio);
        Scode_free(code);

        return 0;
    }

    if (same_string(file_name, "-cr")) {
        file_name = argv[2];
        compilefileRAW(file_name);
        return 0;
    }

    if (same_string(file_name, "-p")) {
        prompt();
        return 0;
    }

    if (same_string(file_name, "-h")) {
        printf("Suny 1.0 Copyright (C) 2025-present, by dinhsonhai132\n");
        printf("Usage: suny [options] [file]\n");
        printf("Options:\n");
        printf("  -c [file] Compile the file\n");
        printf("  -p Run the prompt\n");
        printf("  -h Show this help\n");
        return 0;
    }

    if (same_string(file_name, "-v")) {
        printf("Suny 1.0 Copyright (C) 2025-present, by dinhsonhai132\n");
        return 0;
    }

    SunyRunFile(file_name);

    return 0;
}
