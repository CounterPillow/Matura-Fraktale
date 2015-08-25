// Keeps track of key states for things like keyHit to work
#include <GLFW/glfw3.h>
#include "advinput.h"

#define MOUSE_SINGLE 1
#define MOUSE_DOUBLE 2
#define MOUSE_NUM_BUTTONS 5
#define MOUSE_DOUBLE_INTERVAL 0.250

static unsigned char keyReleased[350];
static unsigned char mouseIsClicked[MOUSE_NUM_BUTTONS];
static double mouseLastClicked[MOUSE_NUM_BUTTONS];
static double scroll;

void initAdvInput(GLFWwindow* window) {
	glfwSetKeyCallback(window, &keyCallbackFun);	
	glfwSetMouseButtonCallback(window, &mouseButtonCallbackFun);
	glfwSetScrollCallback(window, &scrollCallbackFun);
}

int keyHit( int key ) {
	if(keyReleased[key]) {
		keyReleased[key] = 0;
		return 1;
	}
	return 0;
}

void scrollCallbackFun(GLFWwindow* window, double xoffset, double yoffset) {
	scroll = scroll + yoffset;
}

void keyCallbackFun( GLFWwindow* window, 
					 int key, int scancode, int action, int mods ) {
	keyReleased[key] = (action == GLFW_RELEASE);
}

void keyFlush() {
	int i = 0;
	for(i = 0; i < 350; ++i) {
		keyReleased[i] = 0;
	}
}

void mouseButtonCallbackFun( GLFWwindow* window,
							 int button, int action, int mods ) {
	if(action == GLFW_RELEASE) {
		mouseIsClicked[button] = MOUSE_SINGLE;
		if(glfwGetTime() - mouseLastClicked[button] < MOUSE_DOUBLE_INTERVAL) {
			mouseIsClicked[button] = MOUSE_DOUBLE;
		}
		mouseLastClicked[button] = glfwGetTime();
	}
}

int mouseDoubleClicked( int button ) {
	return (mouseIsClicked[button] == MOUSE_DOUBLE);
}

int mouseClicked( int button ) {
	return (mouseIsClicked[button] > 0);
}

double mouseScroll() {
	return scroll;
}

void mouseFlush() {
	int i = 0;
	for(i = 0; i < MOUSE_NUM_BUTTONS; ++i) {
		mouseIsClicked[i] = 0;
	}
	scroll = 0;
}
