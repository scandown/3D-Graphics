#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "window.h"
#include <setjmp.h>

typedef struct {
	unsigned int program;
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;
	char *title;
	GLFWwindow *window;
} State;


State setup_state(jmp_buf error, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, char *name, char *shader_location);
