#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "glad/glad.h"
#include "state.h"
#include "binary_tree.h"
#include "dynamic_array.h"


typedef struct {
	float *vertices;
	unsigned int *vertex_faces;
	int vertex_size;
	int vertex_face_size;
	char *location;
} Model;

void string_search_vertex_point(int *i,float *array, char *line);
void string_search_texture_point(int *i,float *array, char *line);
void string_search_vertex_face(int *i, unsigned int *array, char *line);
void string_search_texture_face(int *i, unsigned int *array, char *line);

int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);
int length_to_token(char *src, char token);

Model model_load(jmp_buf error, char *model_name);
void create_buffers(State *state);
void model_send_to_gpu(State *state, Model *model);
void delete_buffers(State *state);
