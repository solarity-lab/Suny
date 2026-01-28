#ifndef SDEBUG_H
#define SDEBUG_H

#include "score.h"

void SunyDebug_print(const char* format, ...);

#define __ERROR(format, ...) SunyDebug_print(format, ##__VA_ARGS__)
#define __VMERROR(format, ...) SunyDebug_print(format, ##__VA_ARGS__)

#endif // SDEBUG_H