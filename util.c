#include <stdio.h>
#include <string.h>
#include <glew.h>
#include "util.h"

void argGetResolution( int argc, char** argv, int* gfx_w, int* gfx_h ) {
	int i = 0;
	for(i = 0; i < argc; ++i) {
		if( strcmp(argv[i], "-res") == 0 && (i < argc)) {
			sscanf(argv[i + 1], "%d,%d", gfx_w, gfx_h);
			break;
		}
	}
}

int argGetFullscreen( int argc, char** argv ) {
	int i = 0;
	for(i = 0; i < argc; ++i) {
		if( strcmp(argv[i], "-fullscreen") == 0 ) {
			return 1;
		}
	}
	return 0;
}

void checkForGLError( const char* identifier ) {
	int err = glGetError();
	char* errorName;
	switch(err) {
		default:
			errorName = "Shit if I knew.";
		case GL_INVALID_VALUE:
			errorName = "GL_INVALID_VALUE";
		case GL_INVALID_ENUM:
			errorName = "GL_INVALID_ENUM";
		case GL_INVALID_OPERATION:
			errorName = "GL_INVALID_OPERATION";
	}
	if(err) { fprintf(stderr, "%s: Yo, encountered gl error no. %d aka %s\n", identifier, err, errorName); }
}
