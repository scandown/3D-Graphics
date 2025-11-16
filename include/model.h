#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "glad/glad.h"


typedef struct {
	int vertex_size;
	int face_size;
	float *vertices;
	unsigned int *faces;
	char *location;
} Model;


void string_search_float(int *i, float *array, char *line);
void string_search_int(int *i, unsigned int *array, char *line);
int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);

Model model_load(char *model_name);
void model_send_to_gpu(unsigned int program, unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, Model *model);

