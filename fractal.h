#ifndef _FRACTAL_H_
#define _FRACTAL_H_
void renderFunc();
int mainLoop();
int initGraphics(int gfx_w, int gfx_h, int fullscreen, int vsync);
void generateQuad(); 
void generateTexture();
int initShader(); 
#endif
