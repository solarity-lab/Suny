CC = gcc

ifeq ($(OS),Windows_NT)
	PLATFORM = windows
else
	PLATFORM = linux
endif

SRCS = \
	MathLib.c \
	complex/LibComplex.c \
	complex/scomplex.c	\
	Math/smath.c

CFLAGS = -Wall -Wextra -std=c11 -I. -Icomplex
LDFLAGS = -shared

ifeq ($(PLATFORM),windows)
	TARGET = mathlib.dll
	LDLIBS = -lSuny
	CLEAN  = del /Q
else
	TARGET = mathlib.so
	CFLAGS += -fPIC
	LDLIBS = 
	CLEAN  = rm -f
endif

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(TARGET) $(LDLIBS)

clean:
	$(CLEAN) $(TARGET) 2>nul || true

.PHONY: all clean