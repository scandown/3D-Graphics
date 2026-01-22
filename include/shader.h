#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"



unsigned int shader_create(char *file_path, GLenum type);
void shader_delete(unsigned int program, unsigned int shader);
int shader_error_check(unsigned int shader);

unsigned int program_create(unsigned int vertex_shader, unsigned int fragment_shader);
unsigned int program_init(jmp_buf error, char *vertex_shader, char *fragment_shader);
void program_delete(unsigned int program);

char *read_file(char *file_location);

