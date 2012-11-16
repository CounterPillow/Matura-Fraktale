// Keeps track of key states for things like keyHit to work
#include <GL/glfw.h>

static unsigned char keyReleased[350];

int keyHit( int key ) {
	if(keyReleased[key]) {
		keyReleased[key] = 0;
		return 1;
	}
	return 0;
}

void GLFWCALL keyCallbackFun( int key, int action ) {
	keyReleased[key] = (action == GLFW_RELEASE);
}

void keyFlush() {
	int i = 0;
	for(i = 0; i < 350; ++i) {
		keyReleased[i] = 0;
	}
}

