#ifndef _ADVINPUT_H_
#define _ADVINPUT_H_

void initAdvInput(GLFWwindow* window);
int keyHit( int key );
void keyCallbackFun( GLFWwindow* window, int key,
					int scancode, int action, int mods );
void keyFlush();
void mouseButtonCallbackFun( GLFWwindow* window, int button, int action,
							 int mods );
void scrollCallbackFun( GLFWwindow* window, double xoffset, double yoffset);
int mouseDoubleClicked( int button );
int mouseClicked( int button );
double mouseScroll();
void mouseFlush();

#endif
