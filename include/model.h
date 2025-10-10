#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void string_search_float(int *i, float *array, char *line);
void string_search_int(int *i, unsigned int *array, char *line);
int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);

int model_load(char *model_name, int *num_of_vertices, int *num_of_faces, float **vert, unsigned int **faces);
