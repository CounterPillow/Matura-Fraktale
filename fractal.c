#include <stdlib.h>
#include <stdio.h>
#include "fractal.h"
#include "errorcodes.h"

#include <glew.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

// Stores resources for OpenGL in a global struct
static struct {
	GLuint vbo;	// Vertex Buffer Object
	GLuint ibo;	// Index Buffer Object
} gl_data;

int main(int argc, char **argv) {
	initGraphics(&argc, argv, 800, 600);
	glutMainLoop();
	return 0;
};

void renderFunc() { // Main rendering function
	
};

void idleFunc() { // Idle function, called between rendering frames
	
};

int initGraphics(int *argc, char **argv, int gfx_w, int gfx_h) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// Color Buffer + Double Buffering for framebuffer
	glutInitWindowSize(gfx_w, gfx_h);
	glutCreateWindow("Fractals are awesome");
	glutDisplayFunc(&renderFunc);
	glutIdleFunc(&idleFunc);

	glewInit();
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL Version 2.0 is not available on your system.\n");
		return ERR_GLEW_NOT_2_0;
	}
	return 0;
};
