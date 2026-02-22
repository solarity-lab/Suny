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
    struct Scode* code;
    
    char* file;
};

#define same_string(a, b) (strcmp(a, b) == 0)

#define compilefile(file) Scode_print(ScodeCompileFile((file)))

#define compilefileRAW(file) Scode_print_disasm(ScodeCompileFile((file)))

int SunyInstallLibrary(struct Sframe* frame, struct ScompilerUnit* compiler, struct Stable* table);

SUNY_API struct Sframe* SframeRunAloneString(char* str);
SUNY_API struct Sframe* SframeRunFile(char* file);
SUNY_API struct Scode* ScodeCompileFile(char* file);
SUNY_API struct Sframe* SframeRunAloneBytecode(unsigned char* code, int size);
SUNY_API struct Suny* SunyNew(void);

SUNY_API struct Sframe* SframeRunString(char* str, struct Suny* suny);
SUNY_API struct Sframe* SframeRunByteCode(unsigned char* code, int size, struct Suny* suny);
int SunyRunFile(struct Suny* suny);

int prompt(void);

#endif