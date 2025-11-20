#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "glad/glad.h"
#include "state.h"


typedef struct {
	int vertex_size;
	int texture_size;
	int vertex_face_size;
	int texture_face_size;
	float *vertices;
	float *textures;
	unsigned int *vertex_faces;
	unsigned int *texture_faces;
	char *location;
} Model;

void string_search_vertex_point(int *i,float *array, char *line);
void string_search_texture_point(int *i,float *array, char *line);
void string_search_vertex_face(int *i, unsigned int *array, char *line);

int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);
int length_to_token(char *src, char token);

Model model_load(jmp_buf error, char *model_name);
void model_send_to_gpu(State *state, Model *model);
