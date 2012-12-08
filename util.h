#ifndef _UTIL_H_
#define _UTIL_H_

// returns a if a != NULL, else b
#define fallback(a,b) a ? a : b

// always handy
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define clamp(a,b,c) { min(max(a,b),c); }

// configuration struct!
typedef struct {
	int useFullscreen;
	int noVSync;
	int x_resolution;
	int y_resolution;
	int showHelp;
	char* paletteFile; 
	int numFSAASamples;
	double startOffsetRe;
	double startOffsetIm;
	double startZoom;
	int startIterations;
	int showFPS;
} cliArgs;

void parseArgs( int argc, char** argv, cliArgs* dest );
void outputHelpText(void);
void checkForGLError( const char* identifier );
#endif
