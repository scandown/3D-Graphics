#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "glad/glad.h"

void string_search_float(int *i, float *array, char *line);
void string_search_int(int *i, unsigned int *array, char *line);
int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);

int model_load(char *model_name, int *num_of_vertices, int *num_of_faces, float **vert, unsigned int **faces);
void model_send_to_gpu(unsigned int program, unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, int vertex_size,
			int face_size, float *vertex, unsigned int *face);



/*
	int vsize = 0;
	int fsize = 0;
	float *vtest;
	unsigned int *ftest;
	if (model_load("assets/cube.obj", &vsize, &fsize, &vtest, &ftest) == -1) {
		return -1;
	}
*/

typedef struct {
	int vertex_size;
	int face_size;
	float *vertices;
	unsigned int *faces;
	char *model_location;
} Model;
