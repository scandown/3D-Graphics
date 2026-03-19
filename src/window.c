#include "window.h"


GLFWwindow *window_init(unsigned int width, unsigned int height, char *name) {
	if (!glfwInit()) {
		return NULL;
	}

	GLFWwindow *window = glfwCreateWindow(width, height, name, NULL, NULL);


	if (!window) {
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return NULL;
	}

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
