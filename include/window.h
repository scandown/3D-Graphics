#pragma once
#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow *setup_window(unsigned int width, unsigned int height, char *name);
