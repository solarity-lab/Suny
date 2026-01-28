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

SUNY_MODULE SUNY_OPEN_LIB(char* lib_name) {
#ifdef _WIN32
    return LoadLibrary(lib_name);
#else
    return dlopen(lib_name, RTLD_NOW);
#endif
}

SUNY_FUNCTION SUNY_GET_FUNCTION_FROM(SUNY_MODULE handle, char* func_name) {
#ifdef _WIN32
    return GetProcAddress(handle, func_name);
#else
    return dlsym(handle, func_name);
#endif
}

void SUNY_CLOSE_LIB(SUNY_MODULE handle) {
#ifdef _WIN32
    if (handle) FreeLibrary(handle);
#else
    if (handle) dlclose(handle);
#endif
}