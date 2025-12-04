#include "sdebug.h"

void SunyDebug_print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    SUNY_BREAK_POINT;
}

