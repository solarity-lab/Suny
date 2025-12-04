CC = gcc
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
OUT = suny
DEBUG = dsuny
LIB = libSuny.a

ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXT = .exe
else
    RM = rm -f
    EXT =
endif

all: $(OUT)$(EXT) $(DEBUG)$(EXT) $(LIB)

superdebug:
	@echo Building super debug exe...
	@$(CC) -g -O0 -fsanitize=address -fno-omit-frame-pointer $(SRC) -o sdsuny

bin: $(OUT)$(EXT)

debug: $(DEBUG)$(EXT)

lib: $(LIB)

$(OUT)$(EXT): $(SRC)
	@echo Building release exe...
	@$(CC) -g --static $(SRC) -o $@

$(DEBUG)$(EXT): $(SRC)
	@echo Building debug exe...
	@$(CC) -g -O0 -Wall -Wextra -DDEBUG $(SRC) -o $@


$(LIB): $(OBJ)
	@echo Building static library...
	ar rcs $(LIB) $(OBJ)

%.o: %.c Suny.h
	$(CC) -c $< -o $@

clean:
	$(RM) *.o
