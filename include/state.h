#pragma once
#include "GLFW/glfw3.h"

typedef struct {
	unsigned int program;
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;
	char *title;
	GLFWwindow *window;
} State;
