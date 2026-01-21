#ifndef SUNY
#define SUNY

#include "svm.h"
#include "score.h"
#include "sdebug.h"
#include "smem.h"

struct Suny {
    struct Sframe* frame;
    struct ScompilerUnit* compiler;
    struct Stable* table;
    struct Garbage_pool* gc_pool;
};

#define same_string(a, b) (strcmp(a, b) == 0)

#define compilefile(file) Scode_print(SunyCompileFile((file)))

#define compilefileRAW(file) Scode_print_disasm(SunyCompileFile((file)))

int SunyInstallLibrary(struct Sframe* frame, struct ScompilerUnit* compiler, struct Stable* table);

SUNY_API struct Sframe* SunyRunAloneString(char* str);
SUNY_API struct Sframe* SunyRunFile(char* file);
SUNY_API struct Scode* SunyCompileFile(char* file);
SUNY_API struct Sframe* SunyRunAloneBytecode(unsigned char* code, int size);
SUNY_API struct Suny* SunyNew(void);

SUNY_API struct Sframe* SunyRunString(char* str, struct Suny* suny);
SUNY_API struct Sframe* SunyRunByteCode(unsigned char* code, int size, struct Suny* suny);

int prompt(void);

#endif