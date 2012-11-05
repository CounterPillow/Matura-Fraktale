#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <glew.h>
#include "shader.h"
int loadShader( unsigned int shader, const char * path ) {
	// First, let's get the filesize
	struct stat s;
	stat(path, &s);
	int shader_length = s.st_size;
	
	// Open the file in read-only mode
	FILE * shaderfile;
	shaderfile = fopen(path, "rb");
	
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
	//checkForErrors(vs);
	glCompileShader(fs);
	//checkForErrors(fs);
	glAttachShader(po, vs);
	glAttachShader(po, fs);
	glLinkProgram(po);
	checkProgForErrors(po);
	return 0;
};

int checkProgForErrors( unsigned int prog ) {
	int errorLength = 0;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &errorLength);
	if(errorLength != 0) {
		char* errorMessage;
		errorMessage = malloc(errorLength);
		glGetProgramInfoLog(prog, errorLength, NULL, errorMessage);
		fprintf(stderr, "%s\n", errorMessage);
		free(errorMessage);
		return 1;
	}
	return 0;
}
