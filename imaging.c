#include <time.h>
#include <string.h>
#include <stdio.h>
#include <GL/gl3w.h>
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Default palette
static unsigned char defaultPalette[] = {
       				0,0,0,
				0,0,128,
				50,0,128,
				100,100,128,
				100,150,255,
				200,255,100,
				255,255,255
				};
 
int initTexture( GLuint* dest, const char* filename ) {
	unsigned char * palette;
	int width;
	int height;
	if(filename == NULL) {
		palette = defaultPalette;
		width = 7;
		height = 1;
	} else {
		int comp;
		palette = stbi_load(filename, &width, &height, &comp, 3);
		if(palette == NULL) {
			return 1;
		}
	}

	if(glIsTexture(*dest)){
		glDeleteTextures(1, dest);
	}
	glGenTextures(1, dest);
	glBindTexture(GL_TEXTURE_1D, *dest);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, width, 0, GL_RGB, GL_UNSIGNED_BYTE, palette);
	return 0;
};

void saveScreenshot( int width, int height ) {
	// Messing around with filenames
	char filename[24];
	char fullpath[36];
	strcpy(fullpath, "screenshots/");	// prepend directory
	
	time_t timestamp;
	time(&timestamp);
	struct tm * ltime = localtime(&timestamp);
	strftime(filename, 20, "%H_%M_%S_%d_%m_%Y", ltime); // Will break on Jan 1st 10,000 00:00. oh well.
	
	strcat(filename, ".png"); // append file extension
	strcat(fullpath, filename);	// glue directory and filename together
	
	unsigned char * pixeldata = malloc(width * height * 3);
	unsigned char * pixeldata_mirrored = malloc(width * height * 3);

	// Unfortunately, OpenGL gives us the pixel data flipped upside down
	// There sure is some better way to solve this but this works fine.
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixeldata);
	int i;
	for(i = 0; i < height; ++i) {
		memmove(pixeldata_mirrored + width * (height - 1 - i) * 3, pixeldata + width * i * 3, width * 3);
	}
	
	free(pixeldata);
	
	stbi_write_png(fullpath, width, height, 3, pixeldata_mirrored, width * 3);
	
	free(pixeldata_mirrored);
	
	printf("Screenshot saved as %s\n", fullpath);
		
}
