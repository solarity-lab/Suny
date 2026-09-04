CC = gcc

ifeq ($(OS),Windows_NT)
	PLATFORM = windows
else
	PLATFORM = linux
endif

SRCS = \
	FileLib.c \
	File/sfile.c \
	smain.c

CFLAGS = -Wall -Wextra -std=c11 -I. -Icomplex
LDFLAGS = -shared

ifeq ($(PLATFORM),windows)
	TARGET = OS.dll
	LDLIBS = -lSuny
	CLEAN  = del /Q
else
	TARGET = OS.so
	CFLAGS += -fPIC
	LDLIBS = 
	CLEAN  = rm -f
endif

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(TARGET) $(LDLIBS)

clean:
	$(CLEAN) $(TARGET) 2>nul || true

.PHONY: all clean