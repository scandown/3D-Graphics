#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "glad/glad.h"
#include "binary_tree.h"
#include "uniform.h"
#include "dynamic_array.h"
#include "texture.h"

typedef struct {
	unsigned int *items;
	size_t count;
	size_t capacity;
} Indices_Array;

typedef struct {
	float *items;
	size_t count;
	size_t capacity;
} Array;

typedef struct {
	vec3 *items;
	size_t count;
	size_t capacity;
} v3Array;

typedef struct {
	vec2 *items;
	size_t count;
	size_t capacity;
} v2Array;


typedef struct {
	Uniform uniform;
	unsigned int texture;

	v3Array vertex_arr;
	v2Array uv_arr;
	v3Array normal_arr;
	char *location;

	unsigned int VAO;
	unsigned int vertexVBO;
	unsigned int uvVBO;
	unsigned int normalVBO;
	unsigned int EBO;
	unsigned int instance_UV_VBO;
	unsigned int instance_spr_VBO;

	float x;
	float y;
	float z;
} Model;

int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);
int length_to_token(char *src, char token);

Model obj_load(jmp_buf error, char *model_name);
