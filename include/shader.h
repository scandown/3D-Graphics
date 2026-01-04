#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"



unsigned int create_shader(char *file_path, GLenum type);
void delete_shader(unsigned int program, unsigned int shader);
int check_for_shader_errors(unsigned int shader);

unsigned int create_program(unsigned int vertex_shader, unsigned int fragment_shader);
void delete_program(unsigned int program);

char *read_file(char *file_location);

