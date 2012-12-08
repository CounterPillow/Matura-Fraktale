#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "fractal.h"
#include "shader.h"
#include "util.h"
#include "imaging.h"
#include "advinput.h"

// Stores resources for OpenGL in a global struct
static struct {
	GLuint vbo;	// Vertex Buffer Object
	GLuint ibo;	// Index Buffer Object
	GLuint vao;	// Vertex Array Object
	GLuint frag_shader;
	GLuint vert_shader;
	GLuint prog_object;
	GLuint color_palette;

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


static double zoom = 1.0;
static double zoomAccel = 0.0;
const double zoomFactor = 0.01;
const double zoomInertia = 0.85;

static struct {
	int scroll;
	int coords[2];
	int oldCoords[2];
} mouse;

static double offsetCoords[2] = {0.0, 0.0};


static struct {
	int width;
	int height;
	float ratio;
} window;

static cliArgs config;

static int max_iterations = 100;

int main(int argc, char **argv) {
	parseArgs(argc, argv, &config);
	// Check whether the help command line option was specified and react accordingly
	if(config.showHelp) {
		outputHelpText();
		return 0;	
	}

	// default values for resolution
	window.width = fallback(config.x_resolution, 800);
	window.height = fallback(config.y_resolution, 600);

	// set the starting offsets
	offsetCoords[0] = fallback(config.startOffsetRe, 0.0);
	offsetCoords[1] = fallback(config.startOffsetIm, 0.0);
	zoom = fallback(config.startZoom, 1.0);
	max_iterations = fallback(config.startIterations, 100);
	config.showFPS = fallback(config.showFPS, 0);

	if(initGraphics(window.width, window.height, config.useFullscreen, config.noVSync, config.numFSAASamples)) {
		return 1;	// if there has been an error, don't even bother to continue	
	}

	mouse.scroll = glfwGetMouseWheel(); // Set the scroll value to the current mousewheel position
	window.ratio = (float)window.height / (float)window.width;

	// Set the input callback function pointers for advinput
	initAdvInput();

	return mainLoop();	// Call the main loop and return it's return code when quitting
};

int mainLoop() {
	int isRunning = true;
	int framesPassed = 0;
	glfwSetTime(0.0);
	double frameTimer = glfwGetTime();

	// Won't change this at runtime
	glUseProgram(gl_data.prog_object);
	
	// these uniforms remain constant or are only updated on certain events
	glUniform1f(gl_data.shader_uniform.ratio, window.ratio);	
	glUniform1i(gl_data.shader_uniform.iter, max_iterations);
	glUniform2d(gl_data.shader_uniform.offset, offsetCoords[0], offsetCoords[1]);	

	glVertexAttribPointer(	gl_data.shader_attrib.position,	// index
				2,				// size
				GL_FLOAT,			// type
				GL_FALSE,			// normalized
				sizeof(GLfloat)*2,		// stride
				NULL);				// pointer

	// no need to ever disable this, so we'll enable it here.
	glEnableVertexAttribArray(gl_data.shader_attrib.position);
	
	// Binding the buffers, they will stay bound through the entire course of the program.
	glBindBuffer(GL_ARRAY_BUFFER, gl_data.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_data.ibo);

	while(isRunning) {
		
		// FPS (Frames-Per-Second) measurement
		if(glfwGetTime() - frameTimer >= 1.0) {
			if(config.showFPS) {
				printf("FPS: %lf\n", (double)framesPassed / (glfwGetTime() - frameTimer));
			}
			frameTimer = glfwGetTime();
			framesPassed = 0;
		}
		
		// Update mouse coordinate variables
		mouse.oldCoords[0] = mouse.coords[0]; mouse.oldCoords[1] = mouse.coords[1];
		glfwGetMousePos(&mouse.coords[0], &mouse.coords[1]);
		
		
		/*************************/
		/*** Zoom calculations ***/
		/*************************/

		zoomAccel *= zoomInertia;
		
		// Scroll to zoom
		zoomAccel -= (float)(glfwGetMouseWheel() - mouse.scroll) * zoomFactor;
		
		// Double-Click to zoom
		zoomAccel += (mouseDoubleClicked(GLFW_MOUSE_BUTTON_RIGHT) - mouseDoubleClicked(GLFW_MOUSE_BUTTON_LEFT)) * 0.1;

		// This line may need explanation
		// it cuts off the zoomAccel value if it's too small to make a noticeable change within one frame
		// and thus eliminates some weird "flickering"
		// No precise math going on here.
		zoomAccel = fabs(zoomAccel) < 0.01 ? 0.0 : zoomAccel;
		
		zoom += zoomAccel * fabs(zoom);
		mouse.scroll = glfwGetMouseWheel();

		
		// Get the change in mouse coordinates, multiply by zoom, add to offset
		if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
			offsetCoords[0] -= (double)(mouse.coords[0] - mouse.oldCoords[0]) / (double)((window.width + window.height) / 2.0) * zoom;
			offsetCoords[1] += (double)(mouse.coords[1] - mouse.oldCoords[1]) / (double)((window.width + window.height) / 2.0) * zoom;
			glUniform2d(gl_data.shader_uniform.offset, offsetCoords[0], offsetCoords[1]);	
		}

		if(glfwGetKey( GLFW_KEY_KP_ADD ) || glfwGetKey( GLFW_KEY_KP_SUBTRACT )) {
			max_iterations += 1*( max_iterations > 0 ? (glfwGetKey( GLFW_KEY_KP_ADD ) - glfwGetKey( GLFW_KEY_KP_SUBTRACT )) : glfwGetKey( GLFW_KEY_KP_ADD ));
			glUniform1i(gl_data.shader_uniform.iter, max_iterations);	
			printf("Maximum iterations: %d\n", max_iterations);
		}
		
		// For polling reasons, this is flushed before rendering
		mouseFlush();
		keyFlush();
		
		// All drawing calls, here we'll spend most of the time
		renderFunc();

		++framesPassed;	// For FPS measurement

		// Screenshot
		if(keyHit( GLFW_KEY_F2 )) {
			saveScreenshot(window.width, window.height);	
		}

		// In case anyone wants to output the current coordinates
		if(keyHit( GLFW_KEY_F3 )) {
			printf("You are currently at: Re: %lf, Im: %lf\n", offsetCoords[0], offsetCoords[1]);
		}
		
		// Exit on Escape/Click on X
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

	glUniform1d(gl_data.shader_uniform.zoom, zoom);	
	
	glDrawRangeElements(GL_TRIANGLE_FAN, 0, 3, 4, GL_UNSIGNED_SHORT, NULL);	
	
	// Swap Back- and Frontbuffer, also polls input
	glfwSwapBuffers();
};

int initGraphics(int gfx_w, int gfx_h, int fullscreen, int disableVSync, int fsaa) {
	if(!glfwInit()) {
		fprintf(stderr, "Unable to initialize GLFW!\n");
	}

	int mode = fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW;
	
	// Set some hints
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);	// We require OpenGL 4, which means older cards/Intel GPUs won't be able to run this.
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwOpenWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );		// Disallow window resizing (pain in the ass to handle)
	
	if(fsaa) {
		glfwOpenWindowHint( GLFW_FSAA_SAMPLES, fsaa );
	}
	
	if(!glfwOpenWindow(
		gfx_w, gfx_h,	// Window Resolution
		8, 8, 8, 8,	// R,G,B,A bits (= 32bit depth in total)
		0, 0,		// depth- and stencilbuffer. We don't need those.
		mode )		// Whether it should start in fullscreen or windowed mode
	  ) {
		fprintf(stderr, "Unable to open GLFW window.\n");
		return 1;
	}
	
	// VSync off
	if( disableVSync ) {
		glfwSwapInterval(0);
	}
	
	// Window Title
	glfwSetWindowTitle( "Fractals are awesome!" );

	
	// Experimental, because we're just that crazy!
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW failed to initialize.\n");
		return 1;	
	}
	if (!GLEW_VERSION_4_0) {
		fprintf(stderr, "OpenGL Version 4.0 is not available on your system.\n");
		return 1;
	}

	// Discard the first GL error we get, which is caused by GLEW
	checkForGLError("Non-fatal GLEW bug (fix it GLEW!)");

	// Generate some ressources for OpenGL i.e. the quad and the 1D texture
	generateQuad();
	if(initTexture(&gl_data.color_palette, config.paletteFile)) {
		fprintf(stderr, "Couldn't load '%s' for some reason.\n", config.paletteFile);
	}
	
	// Init shader objects & load shaders
	initShader();
	loadShader(gl_data.frag_shader, "shader/mandelbrot_unrolled.glsl");
	loadShader(gl_data.vert_shader, "shader/vertexshader.glsl");
	
	// Compile, link and error-check them
	if(compileShader(gl_data.vert_shader, gl_data.frag_shader, gl_data.prog_object) == 1) {
		return 1;	
	}
	
	// Get the attribute position/uniform locations
	gl_data.shader_attrib.position = glGetAttribLocation(gl_data.prog_object, "position");	// vertex position
	gl_data.shader_uniform.zoom = glGetUniformLocation(gl_data.prog_object, "zoom");	// zoom value
	gl_data.shader_uniform.ratio = glGetUniformLocation(gl_data.prog_object, "ratio");	// aspect ratio
	gl_data.shader_uniform.offset = glGetUniformLocation(gl_data.prog_object, "offset");	// Offset coordinates
	gl_data.shader_uniform.iter = glGetUniformLocation(gl_data.prog_object, "iter");	// Max iterations
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


