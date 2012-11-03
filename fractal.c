#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
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
	initGraphics(&argc, argv, 800, 600);
	glutMainLoop();
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
	glutSwapBuffers();
};

void idleFunc() { // Idle function, called between rendering frames?
	int runtime = glutGet(GLUT_ELAPSED_TIME);
	value = sinf((float)runtime * 0.001f);
	glutPostRedisplay();
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
	generateQuad();
	initShader();
	loadShader(gl_data.frag_shader, "shader/fragtests.txt");
	loadShader(gl_data.vert_shader, "shader/vertexshader.txt");
	compileShader(gl_data.vert_shader, gl_data.frag_shader, gl_data.prog_object);
	gl_data.shader_attrib.position = glGetAttribLocation(gl_data.prog_object, "position");	
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

int loadShader( unsigned int shader, const char * path ) {
	// First, let's get the filesize
	struct stat s;
	stat(path, &s);
	int shader_length = s.st_size;
	
	// Open the file in read-only mode
	FILE * shaderfile;
	shaderfile = fopen(path, "r");
	
	// Allocate required mem
	char * shadercode;
	shadercode = malloc(shader_length + 1); // + extra byte for \0
	shadercode[shader_length] = '\0';

	// Read the data
	fread(shadercode, sizeof(char), shader_length, shaderfile);
	
	// Tell OpenGL what the source is
	glShaderSource(shader, 1, (const char **)&shadercode, &shader_length);
	
	// Clean up behind ourselves!
	free(shadercode);
	fclose(shaderfile);
	
	return 0;
};

// Fun fact: also links the shaders.
int compileShader( unsigned int vs, unsigned int fs, unsigned int po ) {
	glCompileShader(vs);
	glCompileShader(fs);
	glAttachShader(po, vs);
	glAttachShader(po, fs);
	glLinkProgram(po);
	return 0;
};
