#include "window.h"


RGFW_window *window_init(unsigned int width, unsigned int height, char *name) {
	RGFW_init("example", RGFW_initOpenGL);

	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 4;
	hints->minor = 6;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window *window = RGFW_createWindow(name, width, height, width, height, RGFW_windowCenter | RGFW_windowOpenGL | RGFW_windowAllowDND);

	if (!window) {
		return NULL;
	}
	RGFW_window_makeCurrentContext_OpenGL(window);


	if (!gladLoadGLLoader((GLADloadproc)RGFW_getProcAddress_OpenGL))
	{
		printf("Failed to initialize GLAD\n");
		return NULL;
	}

	return window;
}

void framebuffer_size_callback(RGFW_window *window, int width, int height) {
    glViewport(0, 0, width, height);
}
