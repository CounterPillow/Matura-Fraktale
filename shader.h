#ifndef _SHADER_H_
#define _SHADER_H_
int loadShader( unsigned int shader, const char * path );
int compileShader( unsigned int vs, unsigned int fs, unsigned int po );
int checkProgForErrors( unsigned int prog );
#endif
