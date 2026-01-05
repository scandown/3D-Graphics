#include "model.h"


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


Model model_load(jmp_buf error, char *model_name) {
	Model model;
	memset(&model, 0, sizeof(model));

	FILE *fptr = fopen(model_name, "r");	
	if (fptr == NULL) {
		fprintf(stderr, "Error: Couldn't open: %s\n", model_name);
		model.location = NULL;
		longjmp(error, 1);
	}




	// get file_size
	fseek(fptr, 0L, SEEK_END);
	long file_size = ftell(fptr);
	rewind(fptr);

	Array temp_vertices[3] = {0};
	Array temp_uvs[2] = {0};
	Array temp_normals[3] = {0};

	const int num_elements = 8;

	BST *face_bst = NULL;

	int vertex_i = 0;
	int texture_i = 0;
	int normal_i = 0;
	int face_i = 0;
	int indicy = 0;

	Indices_Array vertexIndex[3] = {0};
	Indices_Array uvIndex[3] = {0};
	Indices_Array normalIndex[3] = {0};
	Array verts = {0};


	char *lineHeader = NULL;
	size_t size = 0;
	while (1) {
		int res = getline(&lineHeader, &size, fptr);

		if (res == EOF) {
			break;
		}

		if (strncmp(lineHeader, "v ", 2) == 0) {
			float vertex[3];
			sscanf(lineHeader + 2, "%f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			DA_PUSH(temp_vertices[0], vertex[0]);
			DA_PUSH(temp_vertices[1], vertex[1]);
			DA_PUSH(temp_vertices[2], vertex[2]);
			vertex_i++;
		}
		else if (strncmp(lineHeader, "vt", 2) == 0) {
			float uv[2];
			sscanf(lineHeader + 2, "%f %f", &uv[0], &uv[1]);
			DA_PUSH(temp_uvs[0], uv[0]);
			DA_PUSH(temp_uvs[1], uv[1]);
			texture_i++;
		}
		else if (strncmp(lineHeader, "vn", 2) == 0) {
			float normal[3];
			sscanf(lineHeader + 2, "%f %f %f", &normal[0], &normal[1], &normal[2]);
			DA_PUSH(temp_normals[0], normal[0]);
			DA_PUSH(temp_normals[1], normal[1]);
			DA_PUSH(temp_normals[2], normal[2]);
			normal_i++;
		}
		else if (strncmp(lineHeader, "f ", 2) == 0) {
			unsigned int v_index[3];
			unsigned int uv_index[3];
			unsigned int vn_index[3];
			int matches = sscanf(lineHeader + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &v_index[0], &uv_index[0], &vn_index[0],
					&v_index[1], &uv_index[1], &vn_index[1],
					&v_index[2], &uv_index[2], &vn_index[2]);


			if (matches != 9) {
				fprintf(stderr, "%s isn't in format vertex/texture/normal\n", model_name);
				free(lineHeader);
				free(verts.items);
				bst_delete(&face_bst);
				fclose(fptr);
				longjmp(error, 1);
			}

			for (int i = 0; i < 3; i++) {
				DA_PUSH(vertexIndex[i], v_index[i]-1);
				DA_PUSH(uvIndex[i], uv_index[i]-1);
				DA_PUSH(normalIndex[i], vn_index[i]-1);
			}

			if (face_bst == NULL) {
				bst_insert(&face_bst, vertexIndex[0].items[face_i], uvIndex[0].items[face_i], normalIndex[0].items[face_i], indicy);

				BST *pointer = bst_get_value(face_bst, vertexIndex[0].items[face_i], uvIndex[0].items[face_i], normalIndex[0].items[face_i]);
				for (int i = 0; i < 3; i++) {
					DA_PUSH(verts, temp_vertices[i].items[pointer->value.vertex]);
				}
				for (int i = 0; i < 2; i++) {
					DA_PUSH(verts, temp_uvs[i].items[pointer->value.texture]);
				}
				for (int i = 0; i < 3; i++) {
					DA_PUSH(verts, temp_normals[i].items[pointer->value.normal]);
				}
				indicy++;
			}
			for (int i = 0; i < 3; i++) {
				if (!bst_get_number(face_bst, vertexIndex[i].items[face_i], uvIndex[i].items[face_i], normalIndex[i].items[face_i])) {
					bst_insert(&face_bst, vertexIndex[i].items[face_i], uvIndex[i].items[face_i], normalIndex[i].items[face_i], indicy);
					BST *pointer = bst_get_value(face_bst, vertexIndex[i].items[face_i], uvIndex[i].items[face_i], normalIndex[i].items[face_i]);
					for (int i = 0; i < 3; i++) {
						DA_PUSH(verts, temp_vertices[i].items[pointer->value.vertex]);
					}
					for (int i = 0; i < 2; i++) {
						DA_PUSH(verts, temp_uvs[i].items[pointer->value.texture]);
					}
					for (int i = 0; i < 3; i++) {
						DA_PUSH(verts, temp_normals[i].items[pointer->value.normal]);
					}
					indicy++;
				}
			}
			face_i++;
		}

	}
	free(lineHeader);
	unsigned int index2[face_i * 3];
	for (int i = 0; i < face_i; i++) {
		for (int j = 0; j < 3; j++) {
			BST *pointer = bst_get_value(face_bst, vertexIndex[j].items[i], uvIndex[j].items[i], normalIndex[j].items[i]);
			if (pointer != NULL) {
				index2[i * 3 + j] = pointer->linked;
			} else {
				index2[i * 3 + j] = 0;
			}
		}
	}



	model.location = model_name;
	model.vertices = (float *)verts.items;
	model.vertex_size = indicy * num_elements;


	unsigned int *vertex_faces = malloc(sizeof(index2));
	memcpy(vertex_faces, index2, sizeof(index2));
	
	model.vertex_faces = vertex_faces;
	model.vertex_face_size = sizeof(index2) / sizeof(unsigned int);




	for (int i = 0; i < 3; i++) {
		free(temp_vertices[i].items);
		free(temp_normals[i].items);
		free(vertexIndex[i].items);
		free(uvIndex[i].items);
		free(normalIndex[i].items);
	}
	for (int i = 0; i < 2; i++) {
		free(temp_uvs[i].items);
	}

	bst_delete(&face_bst);
	


	fclose(fptr);
	return model;
}





// helper functions
int length_to_token(char *src, char token) {
	int i = 0;
	while (src[i] != '\0' && src[i] != token) {
		i++;
	}
	if (src[i] == '\0') {
		return -1;
	}
	return i;
}

int check_int_equality(int *array1, int array1_length, int *array2, int array2_length) {
	int wrong = 0;
	
	if (array1_length != array2_length) {
		return -1;
	}
	
	for (int i = 0; i < array1_length; i++) {
		if (array1[i] != array2[i]) {
			wrong++;
			printf("array1[%i]: %i != array2[%i]: %i\n", i, array1[i], i, array2[i]);
		}
		
	}

	return wrong;
}

int check_float_equality(float *array1, int array1_length, float *array2, int array2_length) {
	int wrong = 0;
	
	if (array1_length != array2_length) {
		return -1;
	}
	
	for (int i = 0; i < array1_length; i++) {
		// convert float value to int for accurate comparison
		// might have to change how I convert if its buggy
		if (array1[i] != array2[i]) {
			wrong++;
			printf("array1[%i]: %f != array2[%i]: %f\n", i, array1[i], i, array2[i]);
		}
		
	}

	return wrong;
}



void model_create_buffers(Model *model) {
	glGenVertexArrays(1, &model->VAO);
	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);
}

void model_send_to_gpu(Model *model) {

	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(float), model->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->vertex_face_size * sizeof(unsigned int), model->vertex_faces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void model_delete_buffers(Model *model) {
	glDeleteVertexArrays(1, &model->VAO);
	glDeleteBuffers(1, &model->VBO);
	glDeleteBuffers(1, &model->EBO);

	free(model->vertices);
	free(model->vertex_faces);
}

void model_draw(Model *model, unsigned int program) {
	model->uniform.value.m4[3][0] = model->x;
	model->uniform.value.m4[3][1] = model->y;
	model->uniform.value.m4[3][2] = model->z;
	model->uniform.value.m4[3][3] = 1;
	uniform_send_to_gpu(&model->uniform, program, "model");

	glBindTexture(GL_TEXTURE_2D, model->texture);
	glBindVertexArray(model->VAO);
	glDrawElements(GL_TRIANGLES, model->vertex_face_size, GL_UNSIGNED_INT, 0);
}


void model_init(jmp_buf error, Model *model, vec3 pos, char *texture_location) {
	unsigned int texture = texture_init(error, GL_RGBA, texture_location);

	mat4 model_matrix;
	glm_mat4_identity(model_matrix);


	model_create_buffers(model);
	model_send_to_gpu(model);

	model->x = pos[0];
	model->y = pos[1];
	model->z = pos[2];
	
	model->uniform = uniform_set_data(model_matrix, UNIFORM_MAT4);
	model->texture = texture;
}
