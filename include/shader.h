#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

/*
const char *fragmentShaderSource = loadShader("fragment.glsl");
const char *vertexShaderSource = loadShader("vertex.glsl");

unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);

int fragment_success = shaderErrorCheck(fragmentShader);
int vertex_success = shaderErrorCheck(vertexShader);

if (!(fragment_success || vertex_success)) {
	return 1;
}

free(fragmentShaderSource);
free(vertexShaderSource);
*/



char *loadShader(char *file_name);
int createShader(GLenum type, const char *source);
int shaderErrorCheck(unsigned int shader);
