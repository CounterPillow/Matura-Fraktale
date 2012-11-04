#ifndef _FRACTAL_H_
#define _FRACTAL_H_
void renderFunc();
void idleFunc();
int initGraphics(int *argc, char **argv, int gfx_w, int gfx_h);
void flushKeyHits(); 
void generateQuad(); 
int initShader(); 
int loadShader( unsigned int shader, const char * path );
int compileShader( unsigned int vs, unsigned int fs, unsigned int po );
#endif
