#pragma once
#include <stdio.h>
#include "glad/glad.h"
#define RGFW_IMPORT
#define RGFW_OPENGL
#include "RGFW.h"



void framebuffer_size_callback(RGFW_window* window, int width, int height);
RGFW_window *window_init(unsigned int width, unsigned int height, char *name);
