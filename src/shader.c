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

	char **shader_list = malloc(shader_amount * sizeof(char*));
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
	*size = index;

	return shader_list;
}

/*
	const char *shaders = load_shader_list("src/shaderList.txt");
	unsigned int program = program_create(shaders);
	free(shaders);



	*/


// v:red.glsl
// f:fragment.glsl


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
		const char *shaderSource = loadShader(shader_list[i] + 2, &n_size);
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
			return -1;
		}

		shader = createShader(type, shaderSource);
		int success = shaderErrorCheck(shader);

		if (!(success)) {
			return 1;
		}

		glAttachShader(program, shader);
		glDeleteShader(shader);
	}
	glLinkProgram(program);
	



	// free up all the strings
	for (int i = 0; i < size; i++) {
		printf("%c\n", shader_list[i][0]);
		free(shader_list[i]);
	}
	free(shader_list);
	return program;
}
