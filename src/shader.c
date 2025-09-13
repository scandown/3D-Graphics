#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

char *loadShader(char *file_name) {

	FILE *fptr = fopen(file_name, "r");	
	if (fptr == NULL) {
		printf("Couldn't open: %s\n", file_name);
		return NULL;
	}


	fseek(fptr, 0L, SEEK_END);
	long file_size = ftell(fptr);
	rewind(fptr);

	char *file = malloc(file_size + 1);
	fread(file, 1, file_size, fptr);


	file[file_size] = '\0';

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
