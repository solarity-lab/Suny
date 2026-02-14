#include "stool.h"
#include "smem.h"

double 
Satof(const char *str) {
    double result = 0.0;
    double frac = 0.0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }

    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    if (str[i] == '.') {
        i++;
        double base = 0.1;
        while (str[i] >= '0' && str[i] <= '9') {
            frac += (str[i] - '0') * base;
            base *= 0.1;
            i++;
        }
    }

    return sign * (result + frac);
}

char* 
Sto_char(char chr[], int len) {
    char* new_char = Smem_Malloc(sizeof(char) * (len + 1));
    strcpy(new_char, chr);
    return new_char;
}


int 
Stoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }

    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

void 
Sreverse(void **array, int size) {
    int start = 0;
    int end = size - 1;

    while (start < end) {
        void *tmp = array[start];
        array[start] = array[end];
        array[end] = tmp;
        start++;
        end--;
    }
}

char*
Sftchr(float value) {
    char* buffer = (char*) Smem_Malloc(sizeof(char) * 32);
    sprintf(buffer, "%f", value);



    return buffer;
}

char
Sgchrfromf(float value, int index) {
    char* buffer = (char*) Smem_Malloc(sizeof(char) * 32);
    sprintf(buffer, "%f", value);
    if ((size_t)index >= strlen(buffer)) {
        printf("Index out of range\n");
        return '\0';
    };
    return buffer[index];
}

int
Scharcmp_bigger(char *a, char *b, int size_a, int size_b) {
    int min = size_a < size_b ? size_a : size_b;

    for (int i = 0; i < min; i++) {
        if (a[i] > b[i]) return 1; 
        if (a[i] < b[i]) return 0; 
    }

    return size_a > size_b;
}

int
Scharcmp_smaller(char *a, char *b, int size_a, int size_b) {
    int min = size_a < size_b ? size_a : size_b;

    for (int i = 0; i < min; i++) {
        if (a[i] < b[i]) return 1; 
        if (a[i] > b[i]) return 0; 
    }
    return size_a < size_b;
}

int
Scharcmp_equal(char *a, char *b, int size_a, int size_b) {
    if (size_a != size_b) return 0;

    for (int i = 0; i < size_a; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    
    return 1;
}

char*
Sadd_1_char(char c1) {
    char* buffer = (char*) Smem_Malloc(sizeof(char) * 1);
    buffer[0] = c1;
    buffer[1] = '\0';
    return buffer;
}

char*
Sadd_2_char(char c1, char c2) {
    char* buffer = (char*) Smem_Malloc(sizeof(char) * 3);
    buffer[0] = c1;
    buffer[1] = c2;
    buffer[2] = '\0';
    return buffer;
}

char*
Sadd_3_char(char c1, char c2, char c3) {
    char* buffer = (char*) Smem_Malloc(sizeof(char) * 4);
    buffer[0] = c1;
    buffer[1] = c2;
    buffer[2] = c3;
    buffer[3] = '\0';
    return buffer;
}

char*
Sadd_4_char(char c1, char c2, char c3, char c4) {
    char* buffer = (char*) Smem_Malloc(sizeof(char) * 5);
    buffer[0] = c1;
    buffer[1] = c2;
    buffer[2] = c3;
    buffer[3] = c4;
    buffer[4] = '\0';
    return buffer;
}

int
Slen(char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len; 
}

int
Sisdigit(char chr) {
    if (chr >= '0' && chr <= '9') return 1;
    return 0;
}

int
Sisstrdigit(char *str) {
    int dot_count = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (c == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (!Sisdigit(c)) {
            return 0;
        }
    }

    return 1;
}

char* Sstring(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        va_end(args);
        return NULL;
    }

    char* buf = (char*)Smem_Malloc(len + 1);
    if (!buf) {
        va_end(args);
        return NULL;
    }

    vsnprintf(buf, len + 1, fmt, args);
    va_end(args);

    return buf;
}

char* remove_substr(char* str, const char* to_remove) {
    if (!str || !to_remove) return str;

    size_t len_str = strlen(str);
    size_t len_rem = strlen(to_remove);
    if (len_rem == 0) return str;

    char* result = Smem_Malloc(len_str + 1);
    if (!result) return NULL;

    char* p = str;
    char* r = result;

    while (*p) {
        if (strncmp(p, to_remove, len_rem) == 0) {
            p += len_rem;
        } else {
            *r++ = *p++;
        }
    }

    *r = '\0';
    return result;
}

int if_file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0 && S_ISREG(buffer.st_mode));
}

int if_file_exists_in(const char* filename, const char* path) {
    if (!filename || !path) return 0;

    size_t len = strlen(filename) + strlen(path) + 2;
    char *fullpath = Smem_Malloc(len);
    if (!fullpath) return 0;

    snprintf(fullpath, len, "%s/%s", path, filename);
    int exists = if_file_exists(fullpath);
    Smem_Free(fullpath);

    return exists;
}

int if_folder_exists(const char* foldername) {
    struct stat buffer;
    return (stat(foldername, &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

int if_folder_exists_in(const char* foldername, const char* path) {
    if (!foldername || !path) return 0;

    size_t len = strlen(foldername) + strlen(path) + 2;
    char *fullpath = Smem_Malloc(len);
    if (!fullpath) return 0;

    snprintf(fullpath, len, "%s/%s", path, foldername);
    int exists = if_folder_exists(fullpath);
    Smem_Free(fullpath);

    return exists;

}

void Sreverse_char(char* str, int size) {
    for (int i = 0; i < size / 2; i++) {
        char temp = str[i];
        str[i] = str[size - 1 - i];
        str[size - 1 - i] = temp;
    }
}

void Sdelete_char(char* str, int size, int index) {
    for (int i = index; i < size - 1; i++) {
        str[i] = str[i + 1];
    }
    
    if (size > 0) {
        str[size - 1] = '\0';
    }    
};

void
Sarray_copy(void** dest, void** src,int src_size) {
    for (int i = 0; i < src_size; i++) {
        dest[i] = src[i];
    }
}

void
Sarray_insert(void** arr1, void** arg2, int arg2_size, int index) {
    for (int i = 0; i < arg2_size; i++) {
        arr1[index + i] = arg2[i];
    }
}

void
Sarray_push_to_start(void** arr, void* value, int size)
{
    for (int i = size; i > 0; i--) {
        arr[i] = arr[i - 1];
    }
    arr[0] = value;
}

void
Sarray_push(void** arr, void* value, int size) {
    arr[size] = value;
}

void SmodChars(char* out, const char* fmt, const char* value) {
    const char* p = strstr(fmt, "%s");
    if (!p) {
        strcpy(out, fmt);
        return;
    }

    size_t prefix_len = p - fmt;
    strncpy(out, fmt, prefix_len);
    out[prefix_len] = '\0';

    strcat(out, value);

    strcat(out, p + 2);
}

char* read_fast_content(char* filename) {
    FILE* f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buf = malloc(size + 1);
    fread(buf, 1, size, f);
    buf[size] = 0;

    fclose(f);
    return buf;
}

char STo_hex(char hi, char lo) {
    int h, l;

    if (hi >= '0' && hi <= '9')      h = hi - '0';
    else if (hi >= 'a' && hi <= 'f') h = hi - 'a' + 10;
    else if (hi >= 'A' && hi <= 'F') h = hi - 'A' + 10;
    else return 0;

    if (lo >= '0' && lo <= '9')      l = lo - '0';
    else if (lo >= 'a' && lo <= 'f') l = lo - 'a' + 10;
    else if (lo >= 'A' && lo <= 'F') l = lo - 'A' + 10;
    else return 0;

    return (char)((h << 4) | l);
}