#include "window.h"


GLFWwindow *setup_window(unsigned int width, unsigned int height, char *name) {
	// GLFWwindow *setup_window(unsigned int width, unsigned int height, char *name);
	if (!glfwInit()) {
		return NULL;
	}

	GLFWwindow *window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (!window)
	{
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

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return window;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
