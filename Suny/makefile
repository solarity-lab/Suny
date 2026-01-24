CC      	 := gcc
SRC     	 := $(wildcard *.c)
OBJ     	 := $(SRC:.c=.o)

OUT     	 := suny
DEBUG   	 := dsuny
OPT     	 := osuny
ASAN    	 := sdsuny
LIB     	 := libSuny.a

CFLAGS_DEBUG := -g -O0 -Wall -Wextra -DDEBUG
CFLAGS_REL   := -g
CFLAGS_OPT   := -O3 -flto -march=native -fomit-frame-pointer -DNDEBUG
CFLAGS_ASAN  := -g -O0 -fsanitize=address -fno-omit-frame-pointer

ifeq ($(OS),Windows_NT)
    EXT 	:=
    RM  	:= del /Q
    EXPORT  :=
    AR 		:= ar
else
    EXT 	:=
    RM  	:= rm -f
    EXPORT  := -rdynamic
    AR 		:= ar
endif

all: bin debug optimize lib

bin:  	  $(OUT)   $(EXT)

debug: 	  $(DEBUG) $(EXT)

optimize: $(OPT)   $(EXT)

asan:     $(ASAN)  $(EXT)

lib:      $(LIB)

$(OUT)$(EXT): $(SRC)
	@echo Building RELEASE...
	@$(CC) $(CFLAGS_REL) $(EXPORT) $^ -o $@

$(DEBUG)$(EXT): $(SRC)
	@echo Building DEBUG...
	@$(CC) $(CFLAGS_DEBUG) $(EXPORT) $^ -o $@

$(OPT)$(EXT): $(SRC)
	@echo Building OPTIMIZED...
	@$(CC) $(CFLAGS_OPT) $(EXPORT) $^ -o $@

$(ASAN)$(EXT): $(SRC)
	@echo Building ASAN...
	@$(CC) $(CFLAGS_ASAN) $(EXPORT) $^ -o $@

$(LIB): $(OBJ)
	@echo Building STATIC LIB...
	@$(AR) rcs $@ $^

%.o: %.c
	@$(CC) -c $< -o $@

clean:
	$(RM) *.o
