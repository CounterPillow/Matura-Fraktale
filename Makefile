#SRCF = $(shell find -name '*.c')
SRCF = $(wildcard *.c)
OBJF = $(addsuffix .o,$(basename $(SRCF)))

CC = gcc

CFLAGS += -Igl3w/include -Wall -O2 `pkg-config --cflags glfw3` -march=native
LDFLAGS += gl3w/gl3w.o `pkg-config --libs glfw3` -lGL -lm -ldl

all: fractal

fractal: $(OBJF) | gl3w
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	$(MAKE) -C gl3w clean
	rm -f $(OBJF) fractal

gl3w: 
	$(MAKE) -C gl3w

.PHONY: clean all gl3w
