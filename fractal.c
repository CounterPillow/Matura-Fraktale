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
	GLuint vao;	// Vertex Array Object
	GLuint frag_shader;
	GLuint vert_shader;
	GLuint prog_object;

	struct {
		GLint position;
	} shader_attrib;

	struct {
		GLint zoom;
		GLint offset;
		GLint ratio;
		GLint iter;
	} shader_uniform;

} gl_data;

// Input Values
static double zoom = 1.0;
static double zoomAccel = 0.0;
static int mouseScroll;
const double zoomFactor = 0.01;
const double zoomInertia = 0.8;

static int mouseCoords[2];
static int oldMouseCoords[2];
static double offsetCoords[2] = {0.0, 0.0};//= {-2.0, -1.1};

static float resolutionRatio = 0;

static int wres_w;
static int wres_h;

static GLuint max_iterations = 100;

int main(int argc, char **argv) {
	wres_w = 800;
	wres_h = 600;
	argGetResolution(argc, argv, &wres_w, &wres_h);
	initGraphics(wres_w, wres_h, argGetFullscreen(argc, argv));
	int glErr = glGetError();
	if(glErr == GL_INVALID_OPERATION) {
		printf("Whoops! Encountered %d!\n", glErr);
	}

	mouseScroll = glfwGetMouseWheel();
	resolutionRatio = (float)wres_h / (float)wres_w;
	printf("%f\n", resolutionRatio);

	return mainLoop();	// Call the main loop and return it's return code when quitting
};

int mainLoop() {
	int isRunning = true;
	while(isRunning) {
		oldMouseCoords[0] = mouseCoords[0]; oldMouseCoords[1] = mouseCoords[1];
		glfwGetMousePos(&mouseCoords[0], &mouseCoords[1]);
		zoomAccel *= zoomInertia;
		zoomAccel -= (glfwGetMouseWheel() - mouseScroll) * zoomFactor * fabs(zoom);
		// This line may need explanation
		// it cuts off the zoomAccel value if it's too small to make a noticeable change within one frame
		// and thus eliminates some weird "flickering"
		// No precise math going on here.
		zoomAccel = fabs(zoomAccel) < (0.2 * zoomFactor * zoom) ? 0.0 : zoomAccel;
		zoom += zoomAccel;
		mouseScroll = glfwGetMouseWheel();

		// Get the change in mouse coordinates, multiply by zoom, add to offset
		if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
			offsetCoords[0] -= (double)(mouseCoords[0] - oldMouseCoords[0]) / (double)wres_w * zoom;
			offsetCoords[1] += (double)(mouseCoords[1] - oldMouseCoords[1]) / (double)wres_h * zoom;
		}

		// max_iterations = 30;// * (1.0 / zoom);
		if(glfwGetKey( GLFW_KEY_KP_ADD ) || glfwGetKey( GLFW_KEY_KP_SUBTRACT )) {
			max_iterations += 10*(glfwGetKey( GLFW_KEY_KP_ADD ) - glfwGetKey( GLFW_KEY_KP_SUBTRACT ));
			printf("Maximum iterations: %u\n", max_iterations);
		}
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
	glUniform1d(gl_data.shader_uniform.zoom, zoom);	
	glUniform1f(gl_data.shader_uniform.ratio, resolutionRatio);	
	glUniform2d(gl_data.shader_uniform.offset, offsetCoords[0], offsetCoords[1]);	
	glUniform1ui(gl_data.shader_uniform.iter, max_iterations);	

	glVertexAttribPointer(	gl_data.shader_attrib.position,	// index
				2,				// size
				GL_FLOAT,			// type
				GL_FALSE,			// normalized
				sizeof(GLfloat)*2,		// stride
				NULL);				// pointer
	checkForGLError("mainloop");
	glEnableVertexAttribArray(gl_data.shader_attrib.position);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_data.ibo);
	glDrawRangeElements(GL_TRIANGLE_FAN, 0, 3, 4, GL_UNSIGNED_SHORT, NULL);	
	
	// Return to 'clean' state
	//glUseProgram(0);	
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
	
	// Set some hints
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );	// Only use OpenGL 3.0 and above (sorry intel.)
	glfwOpenWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );	// Only use OpenGL 3.0 and above (sorry intel.)
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );		// Disallow window resizing (pain in the ass to handle)
	
	if(!glfwOpenWindow(
		gfx_w, gfx_h,	// Window Resolution
		8, 8, 8, 8,	// R,G,B,A bits (= 32bit depth in total)
		0, 0,		// depth- and stencilbuffer. We don't need those.
		mode )		// Whether it should start in fullscreen or windowed mode
	  ) {
		fprintf(stderr, "Unable to open GLFW window.\n");			
	}
	
	// Window Title
	glfwSetWindowTitle( "Fractals are awesome!" );
	
	// Experimental, because we're just that crazy!
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW failed to initialize.\n");	
	}
	printf("Okay glew should be started now...\n");
	if (!GLEW_VERSION_3_3) {
		fprintf(stderr, "OpenGL Version 3.3 is not available on your system.\n");
		return ERR_GLEW_NOT_3_3;
	}
	checkForGLError("Non-fatal GLEW bug (fix it GLEW!)");
	generateQuad();
	initShader();
	//loadShader(gl_data.frag_shader, "shader/fragtests.txt");
	loadShader(gl_data.frag_shader, "shader/mandelbrot.glsl");
	//loadShader(gl_data.frag_shader, "shader/red.txt");
	loadShader(gl_data.vert_shader, "shader/vertexshader.txt");
	compileShader(gl_data.vert_shader, gl_data.frag_shader, gl_data.prog_object);
	if(glIsProgram(gl_data.prog_object) == GL_FALSE) {
		printf("not a program\n");
	}
	gl_data.shader_attrib.position = glGetAttribLocation(gl_data.prog_object, "position");	
	gl_data.shader_uniform.zoom = glGetUniformLocation(gl_data.prog_object, "zoom");
	gl_data.shader_uniform.ratio = glGetUniformLocation(gl_data.prog_object, "ratio");
	gl_data.shader_uniform.offset = glGetUniformLocation(gl_data.prog_object, "offset");
	gl_data.shader_uniform.iter = glGetUniformLocation(gl_data.prog_object, "iter");
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

	// Generate Vertex Array Object
	glGenVertexArrays(1, &gl_data.vao);
	glBindVertexArray(gl_data.vao);
};

int initShader() {
	gl_data.prog_object = glCreateProgram();
	gl_data.vert_shader = glCreateShader(GL_VERTEX_SHADER);
	gl_data.frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	return 0;
};


