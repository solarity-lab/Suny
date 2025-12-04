#ifndef SUNY
#define SUNY

#include "svm.h"
#include "score.h"
#include "smem.h"

#define same_string(a, b) (strcmp(a, b) == 0)

#define compilefile(file) Scode_print(SunyCompileFile((file)))

#define compilefileRAW(file) Scode_print_disasm(SunyCompileFile((file)))

int SunyInstallLibrary(struct Sframe* frame, struct ScompilerUnit* compiler, struct Stable* table);

struct Sframe* SunyRunSimpleString(char* str);

struct Sframe* SunyRunFile(char* file);

struct Scode* SunyCompileFile(char* file);

int prompt(void);

#endif