#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glew.h>
#include "util.h"

void parseArgs( int argc, char** argv, cliArgs *dest ) {
	int i;
	for(i = 1; i < argc; ++i) {
		if( strcmp(argv[i], "--resolution") == 0 && (i < argc - 1)) {
			sscanf(argv[i + 1], "%d,%d", &(*dest).x_resolution, &(*dest).y_resolution);
			++i;
		} else if( strcmp(argv[i], "--fullscreen") == 0 ) {
			(*dest).useFullscreen = 1;
		} else if( strcmp(argv[i], "--novsync") == 0 ) {
			(*dest).noVSync = 1;
		} else if( strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 ) {
			(*dest).showHelp = 1;	
		} else if( strcmp(argv[i], "--palette") == 0 && (i < argc - 1)) {
			(*dest).paletteFile = argv[i + 1];
			++i;
		} else if( strcmp(argv[i], "--fsaa") == 0 && (i < argc - 1)) {
			(*dest).numFSAASamples = atoi(argv[i + 1]);
			++i;		
		} else {
			fprintf(stderr, "Unknown argument: '%s'\n", argv[i]);
		}
	}
}	

void outputHelpText(void) {
	printf(
		"Usage: fractal.exe [OPTIONS...]\n"
		"\n"
		"Command line options:\n\n"
		"	--resolution width,height	Set window resolution\n\n"
		"	--fullscreen			Start in fullscreen mode\n\n"
		"	--novsync			Disable vsync\n\n"
		"	-h, --help			Print this help and exit\n\n"
		"	--palette file			Specify a file to load the color\n"
		"					palette from (PNG, BMP, JPEG, TGA,\n"
		"					PSD (don't actually use that), ...)\n\n"
		"	--fsaa amount			Specify the number of FSAA samples to\n"
		"					use, i.e. 2, 4, 8, 16,... (Default: 0)\n"
		"					Be warned: Eats up a lot of performance.\n"
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
