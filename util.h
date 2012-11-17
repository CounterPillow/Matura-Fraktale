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
#define clamp(a,b,c) { min(max(a,b),c); }

typedef struct {
	int useFullscreen;
	int noVSync;
	int x_resolution;
	int y_resolution;
	int showHelp;
	char* paletteFile; 
	int numFSAASamples;
} cliArgs;

void parseArgs( int argc, char** argv, cliArgs* dest );
void outputHelpText(void);
void checkForGLError( const char* identifier );
#endif
