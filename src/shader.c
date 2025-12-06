#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

char *loadShader(char *file_name, long *file_size) {

	FILE *fptr = fopen(file_name, "r");	
	if (fptr == NULL) {
		printf("Couldn't open: %s\n", file_name);
		return NULL;
	}


	fseek(fptr, 0L, SEEK_END);
	*file_size = ftell(fptr);
	rewind(fptr);

	char *file = malloc(*file_size + 1);
	fread(file, 1, *file_size, fptr);


	file[*file_size] = '\0';

	fclose(fptr);

	return file;
}



int createShader(GLenum type, const char *source) {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	return shader;
}

int shaderErrorCheck(unsigned int shader) {
	int compiled = 0;
	int type;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	glGetShaderiv(shader, GL_SHADER_TYPE, &type);

	if (!compiled) {
		int logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		char errorLog[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, errorLog);

		switch (type) {
			case GL_FRAGMENT_SHADER:
				printf("fragment: %s\n", errorLog);
				break;
			case GL_VERTEX_SHADER:
				printf("vertex: %s\n", errorLog);
				break;
			default:
				printf("%s\n", errorLog);
				break;
		}

		glDeleteShader(shader);

	}
	return compiled;
}




int shader_validity_check(char ascii) {
	switch (ascii) {
		case 'v':
			return 1;
			break;
		case 'f':
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

char **get_shader_strings(char *shaders, int *size, long file_size) {

	char *save_ptr = NULL;
	// unmodified shader for later;
	char *shader_copy = malloc(sizeof(char) * strlen(shaders));
	strncpy(shader_copy, shaders, strlen(shaders));
	char *token = strtok_r(shaders, "\n", &save_ptr);
	
	int shader_list_size = 0;

	while (token != NULL) {
		printf("%s\n", token);
		token = strtok_r(NULL, "\n", &save_ptr);
		shader_list_size++;
	}


	save_ptr = NULL;
	char **shader_list = malloc(sizeof(char **) * shader_list_size);

	token = strtok_r(shader_copy, "\n", &save_ptr);
	int i = 0;
	printf("%lu\n", strlen(shaders));
	printf("%s YEAAA\n", token);
	while (token != NULL) {
		//printf("%s YEAAA\n", token);
		shader_list[i] = malloc(sizeof(char *) * strlen(token));
		strcpy(shader_list[i], token);
		token = strtok_r(NULL, "\n", &save_ptr);
		i++;
	}
	free(shader_copy);

	*size = shader_list_size;

	return shader_list;
}



int program_create(char *shader_list_source) {
	long file_size;
	char *shaders = loadShader(shader_list_source, &file_size);

	if (shaders == NULL) {
		return -1;
	}

	int size = 0;
	char **shader_list = get_shader_strings(shaders, &size, file_size);

	unsigned int program = glCreateProgram();

	for (int i = 0; i < size; i++) {
		long n_size;
		char *shaderSource = loadShader(shader_list[i] + 2, &n_size);
		char shaderType = shader_list[i][0];

		unsigned int shader;
		GLenum type;
		switch (shaderType) {
			case 'f':
				type = GL_FRAGMENT_SHADER;
				break;
			case 'v':
				type = GL_VERTEX_SHADER;
				break;
			default:
				type = -1;
				break;
		}
		if (type == -1) {
			printf("Invalid type %c\n", shaderType);
			for (int i = 0; i < size; i++) {
				printf("%c\n", shader_list[i][0]);
				free(shader_list[i]);
			}
			free(shader_list);
			free(shaders);
			free(shaderSource);
			return -1;
		}

		shader = createShader(type, shaderSource);
		int success = shaderErrorCheck(shader);

		if (!(success)) {
			for (int i = 0; i < size; i++) {
				printf("%c\n", shader_list[i][0]);
				free(shader_list[i]);
			}
			free(shader_list);
			free(shaderSource);
			free(shaders);
			return 1;
		}

		glAttachShader(program, shader);
		glDeleteShader(shader);
		free(shaderSource);
	}
	glLinkProgram(program);
	



	// free up all the strings
	for (int i = 0; i < size; i++) {
		printf("%c\n", shader_list[i][0]);
		free(shader_list[i]);
	}
	free(shader_list);
	free(shaders);
	return program;
}
