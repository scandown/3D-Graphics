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



/*
	const char *shaders = load_shader_list("src/shaderList.txt");
	unsigned int program = program_create(shaders);
	free(shaders);


	const char *fragmentShaderSource = loadShader("src/red.glsl");
	const char *vertexShaderSource = loadShader("src/vertex.glsl");

	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	int fragment_success = shaderErrorCheck(fragmentShader);
	int vertex_success = shaderErrorCheck(vertexShader);

	if (!(fragment_success || vertex_success)) {
		return 1;
	}

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	*/


// v:red.glsl
// f:fragment.glsl


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

int program_create(char *shader_list_source) {
	long file_size;
	char *shaders = loadShader(shader_list_source, &file_size);

	if (shaders == NULL) {
		return -1;
	}

	int shader_amount = 0;

	// edge case for when first line has a shader file on it
	shader_amount += shader_validity_check(shaders[0]);
	for (int i = 0; i < file_size; i++) {

		if (shaders[i+1] == '\0') {
			break;
		}

		// check new line to see if you should check for shader files
		if (shaders[i-1] == '\n') {
			shader_amount += shader_validity_check(shaders[i]);
		}
	}

	char *shader_list[shader_amount];
	int index = 0;

	for (int i = 0; i < file_size; i++) {

		if (shaders[i+1] == '\0') {
			break;
		}

		if (i != 0) {
			if (shaders[i-1] != '\n') {
				continue;
			}
		}


		if (shader_validity_check(shaders[i])) {
			size_t string_space = 0;
			
			// go to end of the shader file name and declare amount of space required
			int j = 0;
			for (j = i; shaders[j] != '\n'; j++) {
				string_space++;
			}
			// set the null terminator so strcpy copies to that point
			shaders[j] = '\0';

			shader_list[index] = malloc(string_space);
			// even though strcpy is unsafe, I have declared how much space there is so its okay
			strcpy(shader_list[index], shaders + i);
			shaders[j] = '\n';

			// set i so that it skips the null terminator
			i = j;
			index++;
		}
	}


	// free up all the strings
	for (int i = 0; i < index; i++) {
		free(shader_list[i]);
	}
	return 1;
}
