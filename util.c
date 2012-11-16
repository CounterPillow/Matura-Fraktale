#include <stdio.h>
#include <string.h>
#include <glew.h>
#include "util.h"

void parseArgs( int argc, char** argv, cliArgs *dest ) {
	int i;
	for(i = 1; i < argc; ++i) {
		if( strcmp(argv[i], "--resolution") == 0 && (i < argc)) {
			sscanf(argv[i + 1], "%d,%d", &(*dest).x_resolution, &(*dest).y_resolution);
			++i;
		} else if( strcmp(argv[i], "--fullscreen") == 0 ) {
			(*dest).useFullscreen = 1;
		} else if( strcmp(argv[i], "--novsync") == 0 ) {
			(*dest).noVSync = 1;
		} else if( strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 ) {
			(*dest).showHelp = 1;	
		} else if( strcmp(argv[i], "--palette") == 0 && (i < argc)) {
			(*dest).paletteFile = argv[i + 1];
			++i;
		} else {
			fprintf(stderr, "Unknown argument: '%s'\n", argv[i]);
		}
	}
}	

void argGetResolution( int argc, char** argv, int* gfx_w, int* gfx_h ) {
	int i = 0;
	for(i = 0; i < argc; ++i) {
		if( strcmp(argv[i], "--resolution") == 0 && (i < argc)) {
			sscanf(argv[i + 1], "%d,%d", gfx_w, gfx_h);
			break;
		}
	}
}

int argGetFullscreen( int argc, char** argv ) {
	int i = 0;
	for(i = 0; i < argc; ++i) {
		if( strcmp(argv[i], "--fullscreen") == 0 ) {
			return 1;
		}
	}
	return 0;
}

int argGetVsync( int argc, char** argv ) {
	int i = 0;
	for(i = 0; i < argc; ++i) {
		if( strcmp(argv[i], "--novsync") == 0 ) {
			return 0;
		}
	}
	return 1;
}

int argNeedsHelp( int argc, char** argv ) {
	int i = 0;
	for(i = 0; i < argc; ++i) {
		if( strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 ) {
			return 1;
		}
	}
	return 0;
}

void outputHelpText(void) {
	printf(
		"Usage: fractal.exe [OPTION]\n"
		"\n"
		"Command line options:\n"
		"	--resolution width,height	Set window resolution\n"
		"	--fullscreen		Start in fullscreen mode\n"
		"	--novsync			Disable vsync\n"
		"	-h, --help			Print this help and exit\n"
	      );
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
	if(err) { fprintf(stderr, "%s: Encountered OpenGL error %d, also known as '%s'\n", identifier, err, errorName); }
}
