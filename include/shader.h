#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"



char *loadShader(char *file_name, long *file_size);

int createShader(GLenum type, const char *source);

int shaderErrorCheck(unsigned int shader);


int shader_validity_check(char ascii);

int program_create(char *shader_list_source);
