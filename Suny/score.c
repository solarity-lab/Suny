#include "score.h"

int SDEBUG(const char* format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);
    return ret;
#else
    return 0;
#endif
}