#ifndef _ADVINPUT_H_
#define _ADVINPUT_H_

void initAdvInput();
int keyHit( int key );
void GLFWCALL keyCallbackFun( int key, int action );
void keyFlush();
void GLFWCALL mouseButtonCallbackFun( int button, int action );
int mouseDoubleClicked( int button );
int mouseClicked( int button );
void mouseFlush();

#endif
