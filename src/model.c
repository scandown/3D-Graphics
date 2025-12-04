#include "model.h"

typedef struct {
	float vertex[3];
	float texture[2];
	float normal[3];
} model_vals;

Model model_load(jmp_buf error, char *model_name) {
	Model model;
	memset(&model, 0, sizeof(model));

	FILE *fptr = fopen(model_name, "r");	
	if (fptr == NULL) {
		printf("Couldn't open: %s\n", model_name);
		model.location = NULL;
		longjmp(error, 1);
	}


	// get file_size
	fseek(fptr, 0L, SEEK_END);
	long file_size = ftell(fptr);
	rewind(fptr);

	float temp_vertices[10000][3] = {0};
	float temp_uvs[10000][2] = {0};
	float temp_normals[10000][2] = {0};

	const int num_elements = 8;

	BST *face_bst = NULL;

	int vertex_i = 0;
	int texture_i = 0;
	int normal_i = 0;
	int face_i = 0;
	int indicy = 0;
	unsigned int vertexIndex[10000][3] = {0};
	unsigned int uvIndex[10000][3] = {0};
	unsigned int normalIndex[10000][3] = {0};
	model_vals *verts = malloc(50000 * sizeof(model_vals));
	while (1) {
		char *lineHeader = NULL;
		size_t size = 0;
		int res = getline(&lineHeader, &size, fptr);

		if (res == EOF) {
			break;
		}

		if (strncmp(lineHeader, "v ", 2) == 0) {
			float vertex[3];
			sscanf(lineHeader + 2, "%f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			temp_vertices[vertex_i][0] = vertex[0];
			temp_vertices[vertex_i][1] = vertex[1];
			temp_vertices[vertex_i][2] = vertex[2];
			vertex_i++;
		}
		else if (strncmp(lineHeader, "vt", 2) == 0) {
			float uv[2];
			sscanf(lineHeader + 2, "%f %f", &uv[0], &uv[1]);
			temp_uvs[texture_i][0] = uv[0];
			temp_uvs[texture_i][1] = uv[1];
			texture_i++;
		}
		else if (strncmp(lineHeader, "vn", 2) == 0) {
			float normal[3];
			sscanf(lineHeader + 2, "%f %f %f", &normal[0], &normal[1], &normal[2]);
			temp_normals[normal_i][0] = normal[0];
			temp_normals[normal_i][1] = normal[1];
			temp_normals[normal_i][2] = normal[2];
			normal_i++;
		}
		else if (strncmp(lineHeader, "f ", 2) == 0) {
			int matches = sscanf(lineHeader + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[face_i][0], &uvIndex[face_i][0], &normalIndex[face_i][0],
					&vertexIndex[face_i][1], &uvIndex[face_i][1], &normalIndex[face_i][1],
					&vertexIndex[face_i][2], &uvIndex[face_i][2], &normalIndex[face_i][2]);

			if (matches != 9) {
				fprintf(stderr, "%s isn't in format vertex/texture/normal\n", model_name);
				longjmp(error, 1);
			}

			for (int i = 0; i < 3; i++) {
				vertexIndex[face_i][i]--;
				uvIndex[face_i][i]--;
				normalIndex[face_i][i]--;
			}

			if (face_bst == NULL) {
				insertnumber(&face_bst, vertexIndex[face_i][0], uvIndex[face_i][0], normalIndex[face_i][0], indicy);

				BST *pointer = getvalue(face_bst, vertexIndex[face_i][0], uvIndex[face_i][0], normalIndex[face_i][0]);
				memcpy(verts[indicy].vertex, temp_vertices[pointer->value.vertex], 3 * sizeof(float));
				memcpy(verts[indicy].texture, temp_uvs[pointer->value.texture], 2 * sizeof(float));
				memcpy(verts[indicy].normal, temp_normals[pointer->value.normal], 3 * sizeof(float));
				indicy++;
			}
			for (int i = 0; i < 3; i++) {
				if (!getnumber(face_bst, vertexIndex[face_i][i], uvIndex[face_i][i], normalIndex[face_i][i])) {
					insertnumber(&face_bst, vertexIndex[face_i][i], uvIndex[face_i][i], normalIndex[face_i][i], indicy);
					BST *pointer = getvalue(face_bst, vertexIndex[face_i][i], uvIndex[face_i][i], normalIndex[face_i][i]);
					memcpy(verts[indicy].vertex, temp_vertices[pointer->value.vertex], 3 * sizeof(float));
					memcpy(verts[indicy].texture, temp_uvs[pointer->value.texture], 2 * sizeof(float));
					memcpy(verts[indicy].normal, temp_normals[pointer->value.normal], 3 * sizeof(float));
					indicy++;
				}
			}
			face_i++;
		}
		free(lineHeader);

	}
	unsigned int index2[face_i * 3];
	for (int i = 0; i < face_i; i++) {
		for (int j = 0; j < 3; j++) {
			BST *pointer = getvalue(face_bst, vertexIndex[i][j], uvIndex[i][j], normalIndex[i][j]);
			if (pointer != NULL) {
				index2[i * 3 + j] = pointer->linked;
			} else {
				index2[i * 3 + j] = 0;
			}
		}
	}

	//print_tree(face_bst);
	free_bst(&face_bst);


	model.location = model_name;
	model.vertices = (float *)verts;
	model.vertex_size = indicy * num_elements;


	unsigned int *vertex_faces = malloc(sizeof(index2));
	memcpy(vertex_faces, index2, sizeof(index2));
	
	model.vertex_faces = vertex_faces;
	model.vertex_face_size = sizeof(index2) / sizeof(unsigned int);
	printf("%d\n", model.vertex_face_size);

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






void model_send_to_gpu(State *state, Model *model) {
	glGenVertexArrays(1, &state->VAO);
	glGenBuffers(1, &state->VBO);
	glGenBuffers(1, &state->EBO);

	glBindVertexArray(state->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, state->VBO);
	glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(float), model->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->vertex_face_size * sizeof(int), model->vertex_faces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
