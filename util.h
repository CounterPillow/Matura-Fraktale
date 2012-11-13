#ifndef _UTIL_H_
#define _UTIL_H_
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void argGetResolution( int argc, char** argv, int* gfx_w, int* gfx_h );
int argGetFullscreen( int argc, char** argv );
int argGetVsync( int argc, char** argv );
int argNeedsHelp( int argc, char** argv );
void outputHelpText(void);
void checkForGLError( const char* identifier );
#endif
