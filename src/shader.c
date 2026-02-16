#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"


unsigned int shader_create(char *file_path, GLenum type) {
	unsigned int shader = glCreateShader(type);
	
	char *string = read_file(file_path);

	if (string == NULL) {
		fprintf(stderr, "Error: Couldn't create shader %s, invalid file path\n", file_path);
		return 0;
	}

	glShaderSource(shader, 1, (const char* const *)&string, NULL);
	glCompileShader(shader);
	
	if (shader_error_check(shader) == true) {
		return 0;
	}

	free(string);

	return shader;
}

int shader_error_check(unsigned int shader) {
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == false) {
		int log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		char log[log_length];

		glGetShaderInfoLog(shader, log_length, &log_length, log);

		fprintf(stderr, "Shader Error:\n%s\n", log);
		return 1;
	}

	return 0;
}

void shader_delete(unsigned int program, unsigned int shader) {
	glDetachShader(program, shader);
	glDeleteShader(shader);
}



unsigned int program_create(unsigned int vertex_shader, unsigned int fragment_shader) {
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);


	// cleanup
	shader_delete(program, vertex_shader);
	shader_delete(program, fragment_shader);

	return program;
}

char *read_file(char *file_location) {
	FILE *fptr = fopen(file_location, "r");	
	int file_size = 0;
	if (fptr == NULL) {
		fprintf(stderr, "Error: Couldn't open: %s\n", file_location);
		return NULL;
	}


	fseek(fptr, 0L, SEEK_END);

	file_size = ftell(fptr);
	rewind(fptr);

	char *file = malloc(file_size + 1);
	fread(file, 1, file_size, fptr);


	file[file_size] = '\0';

	fclose(fptr);

	return file;
}


unsigned int program_init(jmp_buf error, char *vertex_shader, char *fragment_shader) {
	unsigned int vertex_shader_id = shader_create(vertex_shader, GL_VERTEX_SHADER);
	unsigned int fragment_shader_id = shader_create(fragment_shader, GL_FRAGMENT_SHADER);

	if (vertex_shader_id == 0 || fragment_shader_id == 0) {
		longjmp(error, 1);
	}

	unsigned int program = program_create(vertex_shader_id, fragment_shader_id);
	return program;
}
