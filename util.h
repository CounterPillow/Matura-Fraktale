#ifndef _UTIL_H_
#define _UTIL_H_
void argGetResolution( int argc, char** argv, int* gfx_w, int* gfx_h );
int argGetFullscreen( int argc, char** argv );
void checkForGLError( const char* identifier );
#endif
