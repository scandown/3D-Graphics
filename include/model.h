#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void string_search_float(int *i, float *array, char *line);
void string_search_int(int *i, unsigned int *array, char *line);

int model_load(char *model_name, int *num_of_vertices, int *num_of_faces, float **vert, unsigned int **faces);
