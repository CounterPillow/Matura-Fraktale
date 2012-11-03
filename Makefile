SRCF = $(shell find -name '*.c')
OBJF = $(addsuffix .o,$(basename $(SRCF)))

CC = gcc
LD = ld

# Adjust these as you need
GLEW_PATH = "D:/glew/include/GL"
GLEW_LIB_PATH = "D:/glew/lib"
GLUT_PATH = "D:/freeglut/include"
GLUT_LIB_PATH = "D:/freeglut/lib"

# Don't 'adjust' these though...
INCLUDEDIRS = -I$(GLEW_PATH) -I$(GLUT_PATH)
CFLAGS = -Wall $(INCLUDEDIRS)
LDFLAGS = -L$(GLEW_LIB_PATH) -L$(GLUT_LIB_PATH) -lglew32 -lfreeglut -lopengl32 # -Wl,--subsystem,windows
# subsystem,windows is to build it as "GUI app", throwing all stdout/stderr into a black hole.

all: fractal

fractal: $(OBJF)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm $(OBJF)

.PHONY: clean all
