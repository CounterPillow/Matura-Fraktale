#include <stdio.h>
#include <string.h>
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
