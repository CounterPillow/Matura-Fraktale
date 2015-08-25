#ifndef _SHADER_H_
#define _SHADER_H_
int loadShader( unsigned int shader, const char * path );
void compileShader( unsigned int shader );
int buildShader( unsigned int vs, unsigned int fs, unsigned int po );
int checkProgForErrors( unsigned int prog );
#endif
