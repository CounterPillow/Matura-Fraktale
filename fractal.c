#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <glew.h>
#include <GL/glfw.h>
#include "fractal.h"
#include "errorcodes.h"
#include "shader.h"
#include "util.h"

// Stores resources for OpenGL in a global struct
static struct {
	GLuint vbo;	// Vertex Buffer Object
	GLuint ibo;	// Index Buffer Object
	GLuint frag_shader;
	GLuint vert_shader;
	GLuint prog_object;

	struct {
		GLint position;
	} shader_attrib;

	struct {
		GLfloat val;
	} shader_uniform;

} gl_data;

static float value = 0;

int main(int argc, char **argv) {
	int gfx_w = 800;
	int gfx_h = 600;
	argGetResolution(argc, argv, &gfx_w, &gfx_h);
	initGraphics(gfx_w, gfx_h, argGetFullscreen(argc, argv));
	int glErr = glGetError();
	if(glErr == GL_INVALID_OPERATION) {
		printf("Whoops! Encountered %d!\n", glErr);
	}
	return mainLoop();	// Call the main loop and return it's return code when quitting
};

int mainLoop() {
	int isRunning = true;
	float x = 0;
	while(isRunning) {
		x = x + 0.01;
		value = sinf(x);
		renderFunc();

		if(glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED )) {
			isRunning = false;
		}
	}
	glfwTerminate();
	return 0;
};

void renderFunc() { // Main rendering function
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBindBuffer(GL_ARRAY_BUFFER, gl_data.vbo);
	glUseProgram(gl_data.prog_object);
	glUniform1f(gl_data.shader_uniform.val, value);	

	glVertexAttribPointer(	gl_data.shader_attrib.position,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(GLfloat)*2,
				(void*)0);
	glEnableVertexAttribArray(gl_data.shader_attrib.position);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_data.ibo);
	glDrawRangeElements(GL_TRIANGLE_FAN, 0, 3, 4, GL_UNSIGNED_SHORT, (void*)0);	
	
	// Return to 'clean' state
	glUseProgram(0);	
	glDisableVertexAttribArray(0);	
	
	// Swap Back- and Frontbuffer
	glfwSwapBuffers();
};

int initGraphics(int gfx_w, int gfx_h, int fullscreen) {
	if(!glfwInit()) {
		fprintf(stderr, "Unable to initialize GLFW!\n");
	}
	int mode = GLFW_WINDOW;
	if(fullscreen) {
		mode = GLFW_FULLSCREEN;
	}
	if(!glfwOpenWindow(
		gfx_w, gfx_h,	// Window Resolution
		8, 8, 8, 8,	// R,G,B,A bits (= 32bit depth in total)
		0, 0,		// depth- and stencilbuffer. We don't need those.
		mode )
	  ) {
		fprintf(stderr, "Unable to open GLFW window.\n");			
	}

	glewInit();
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL Version 2.0 is not available on your system.\n");
		return ERR_GLEW_NOT_2_0;
	}
	generateQuad();
	initShader();
	loadShader(gl_data.frag_shader, "shader/fragtests.txt");
	loadShader(gl_data.vert_shader, "shader/vertexshader.txt");
	compileShader(gl_data.vert_shader, gl_data.frag_shader, gl_data.prog_object);
	if(glIsProgram(gl_data.prog_object) == GL_FALSE) {
		printf("not a program\n");
	}
	if(glGetError()/* == GL_INVALID_OPERATION*/) {
		printf("derp1\n");
	}
	gl_data.shader_attrib.position = glGetAttribLocation(gl_data.prog_object, "position");	
	if(glGetError() == GL_INVALID_OPERATION) {
		printf("derp2\n");
	}
	gl_data.shader_uniform.val = glGetUniformLocation(gl_data.prog_object, "val");
	return 0;

};

void generateQuad() {
	const GLfloat vertex_data[] = {
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f
	};
	const GLushort element_data[] = { 0, 1, 2, 3 };
	
	// Generate Vertex Buffer Object
	glGenBuffers(1, &gl_data.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl_data.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	// Generate Index Buffer Object
	glGenBuffers(1, &gl_data.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_data.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_data), element_data, GL_STATIC_DRAW);
};

int initShader() {
	gl_data.prog_object = glCreateProgram();
	gl_data.vert_shader = glCreateShader(GL_VERTEX_SHADER);
	gl_data.frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	return 0;
};


