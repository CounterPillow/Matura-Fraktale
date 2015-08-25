
CC = gcc

CFLAGS += -fPIC -Iinclude

all: gl3w.o

gl3w.o:
	$(CC) $(CFLAGS) -c src/gl3w.c -o gl3w.o
clean:
	rm -f gl3w.o

.PHONY: all clean