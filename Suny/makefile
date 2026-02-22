CC := gcc
AR := ar

SRC_ALL := $(wildcard *.c)
SRC_LIB := $(filter-out main.c, $(SRC_ALL))
SRC_EXE := $(SRC_ALL)

OBJ_LIB := $(SRC_LIB:.c=.o)
OBJ_EXE := $(SRC_EXE:.c=.o)

OUT      := suny
OUT_DBG  := dsuny
OUT_OPT  := osuny
OUT_ASAN := asuny
LIB      := libSuny.a

CFLAGS      := -g -Wall -Wextra
CFLAGS_OPT  := -O3 -flto -march=native -fomit-frame-pointer -DNDEBUG
CFLAGS_DEBUG := -g -O0 -DDEBUG
CFLAGS_ASAN := -g -O0 -fsanitize=address -fno-omit-frame-pointer

ifeq ($(OS),Windows_NT)
    RM := del /Q
else
    RM := rm -f
endif

bin: $(OUT)

lib: $(LIB)

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: $(OUT_DBG)

opt: CFLAGS := $(CFLAGS_OPT)
opt: $(OUT_OPT)

asan: CFLAGS := $(CFLAGS_ASAN)
asan: $(OUT_ASAN)

$(OUT): $(OBJ_EXE)
	@echo Building bin...
	$(CC) $(CFLAGS) $^ -o $@

$(OUT_DBG): $(OBJ_EXE)
	@echo Building debug...
	$(CC) $(CFLAGS) $^ -o $@

$(OUT_OPT): $(OBJ_EXE)
	@echo Building opt...
	$(CC) $(CFLAGS) $^ -o $@

$(OUT_ASAN): $(OBJ_EXE)
	@echo Building asan...
	$(CC) $(CFLAGS) $^ -o $@

$(LIB): $(OBJ_LIB)
	@echo Building static library...
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o