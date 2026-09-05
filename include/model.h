#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "glad/glad.h"
#include "binary_tree.h"
#include "uniform.h"
#include "texture.h"

#include "stb_ds.h"
#include "user/global_defines.h"


typedef struct {
	Uniform uniform;
	unsigned int texture;

	vec3 *vertex_array;
	vec2 *uv_array;
	vec3 *normal_array;
	char *location;

	unsigned int VAO;
	unsigned int vertexVBO;
	unsigned int uvVBO;
	unsigned int normalVBO;
	unsigned int EBO;
	unsigned int instance_UV_VBO;
	unsigned int instance_spr_VBO;

	vec3 pos;
} Model;

bool get_similar_index(vec3 *vertex_array, vec2 *uv_array, vec3 *normal_array,
		       vec3 *out_vertex_array, vec2 *out_uv_array, vec3 *out_normal_array,
		       unsigned int *out_indices_array);

int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);
int length_to_token(char *src, char token);

Model obj_load(jmp_buf error, char *model_name);
