#pragma once

#include <setjmp.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "window.h"
#include "uniform.h"

typedef struct Uniform Uniform;

typedef struct {
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;
	char *title;
	GLFWwindow *window;

	Uniform projection_uniform;
	Uniform view_uniform;
	Uniform model_uniform;
} State;


State state_init(jmp_buf error, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, char *name);
void matrix_init(State *game, unsigned int program, char *dimension);
