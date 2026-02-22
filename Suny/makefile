CC := gcc
AR := ar

SRC_ALL := $(wildcard *.c)
SRC_LIB := $(filter-out main.c, $(SRC_ALL))
SRC_EXE := $(SRC_ALL)

OBJ_LIB := $(SRC_LIB:.c=.o)
OBJ_EXE := $(SRC_EXE:.c=.o)

OUT   := suny
LIB   := libSuny.a

CFLAGS := -g -Wall -Wextra
CFLAGS_OPT := -O3 -flto -march=native -fomit-frame-pointer -DNDEBUG
CFLAGS_DEBUG := -g -O0 -DDEBUG
CFLAGS_ASAN := -g -O0 -fsanitize=address -fno-omit-frame-pointer

ifeq ($(OS),Windows_NT)
    RM := del /Q
else
    RM := rm -f
endif

bin: $(OUT)

lib: $(LIB)

$(OUT): $(OBJ_EXE)
	@echo Building bin...
	$(CC) $(CFLAGS) $^ -o $@

$(LIB): $(OBJ_LIB)
	@echo Building static library...
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(OUT)