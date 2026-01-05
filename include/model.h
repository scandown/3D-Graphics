#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "glad/glad.h"
#include "state.h"
#include "binary_tree.h"
#include "dynamic_array.h"
#include "texture.h"


typedef struct {
	Uniform model_uniform;
	unsigned int texture;

	float *vertices;
	unsigned int *vertex_faces;
	int vertex_size;
	int vertex_face_size;
	char *location;

	unsigned int program;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	float x;
	float y;
	float z;
} Model;

int check_int_equality(int *array1, int array1_length, int *array2, int array2_length);
int check_float_equality(float *array1, int array1_length, float *array2, int array2_length);
int length_to_token(char *src, char token);

Model model_load(jmp_buf error, char *model_name);
void model_create_buffers(Model *model);
void model_send_to_gpu(Model *model);
void model_delete_buffers(Model *model);

void model_draw(Model *model);
void model_init(jmp_buf error, Model *model, unsigned int program, vec3 pos, char *texture_location);
