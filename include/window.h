#pragma once
#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow *window_init(unsigned int width, unsigned int height, char *name);
