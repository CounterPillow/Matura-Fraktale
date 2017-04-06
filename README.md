Matura-Fraktale
===============

This repository contains code that I have written for my final thesis paper
for the Swiss matura. The code in the state it was handed in can be found under
the git tag `sent-in`. Since then, I've made the code work with GLFW3, and added
small quality-of-life fixes.

The first public release is tagged with the git tag `v1.0`.

Dependencies
------------

* A C compiler (use mingw-w64 on Windows)
* GLFW3
* OpenGL 3.2 Core support with the `GL_ARB_gpu_shader_fp64` extension

Building
--------

**On Linux**: `make all`

**On Windows**: `make -f Makefile.mingw all`

License
-------

The code in `stb_image.c`, `stb_image.h` and `stb_image_write.h` is released
into the public domain by the author(s) thereof.

Unless noted otherwise, the rest of the code in this repository is licensed
under the zlib license, see the file `LICENSE` for the details.
