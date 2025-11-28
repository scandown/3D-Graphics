#include "model.h"

typedef struct {
	float vertices[3];
	float texture[2];
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

	float temp_vertices[100][3] = {0};
	float temp_uvs[100][2] = {0};


	BST *face_bst = NULL;

	int vertex_i = 0;
	int texture_i = 0;
	int face_i = 0;
	int indicy = 0;
	unsigned int vertexIndex[100][3] = {0};
	unsigned int uvIndex[100][3] = {0};
	while (1) {
		char *lineHeader = NULL;
		size_t size = 0;
		int res = getline(&lineHeader, &size, fptr);

		//printf("%s\n", lineHeader);
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
		else if (strncmp(lineHeader, "f ", 2) == 0) {
			int matches = sscanf(lineHeader + 2, "%d/%d %d/%d %d/%d", &vertexIndex[face_i][0], &uvIndex[face_i][0],
					&vertexIndex[face_i][1], &uvIndex[face_i][1],
					&vertexIndex[face_i][2], &uvIndex[face_i][2]);
			if (face_bst == NULL) {
				insertnumber(&face_bst, vertexIndex[face_i][0], uvIndex[face_i][0], indicy);
				indicy++;
			}
			for (int i = 0; i < 3; i++) {
				if (!getnumber(face_bst, vertexIndex[face_i][i], uvIndex[face_i][i])) {
					insertnumber(&face_bst, vertexIndex[face_i][i], uvIndex[face_i][i], indicy);
					indicy++;
				}
			}
			face_i++;
		}
		free(lineHeader);

	}

	unsigned int index[indicy][3];
	for (int i = 0; i < indicy; i++) {
		for (int j = 0; j < 3; j++) {
			BST *pointer = getvalue(face_bst, vertexIndex[i][j], uvIndex[i][j]);
			if (pointer != NULL) {
				index[i][j] = pointer->linked;
				//printf("%d, ", index[i][j]);
			} else {
				index[i][j] = 0;
			}
		}
		//printf("\n");
	}

	model_vals *verts = malloc(indicy * sizeof(model_vals));
	for (int i = 0; i < indicy; i++) {
		for (int j = 0; j < 3; j++) {
			BST *pointer = getvalue(face_bst, vertexIndex[i][j], uvIndex[i][j]);
			if (pointer != NULL) {
				// 2, 1
				// pointer->linked = 0;
				verts[pointer->linked].vertices[0] = temp_vertices[pointer->value.vertex][0];
				verts[pointer->linked].vertices[1] = temp_vertices[pointer->value.vertex][1];
				verts[pointer->linked].vertices[2] = temp_vertices[pointer->value.vertex][2];
				verts[pointer->linked].texture[0] = temp_uvs[pointer->value.texture][0];
				verts[pointer->linked].texture[1] = temp_uvs[pointer->value.texture][1];
			}
		}
	}


	for (int i = 0; i < indicy; i ++) {

		for (int j = 0; j < 3; j++) {
			printf("%f, ", verts[i].vertices[j]);
		}
		for (int j = 0; j < 2; j++) {
			printf("%f, ", verts[i].texture[j]);
		}
		printf("\n");
	}


	print_tree(face_bst);
	free_bst(&face_bst);


	float *verts_total = (float *)verts;

	//float *verts_total = malloc(15 * sizeof(float));
	/*
	verts_total[0] = 0;
	verts_total[1] = 0;
	verts_total[2] = 0;
	verts_total[3] = 0;
	verts_total[4] = 0;

	verts_total[5] = 0;
	verts_total[6] = 1;
	verts_total[7] = 0;
	verts_total[8] = 0;
	verts_total[9] = 1;

	verts_total[10] = 1;
	verts_total[11] = 0;
	verts_total[12] = 0;
	verts_total[13] = 1;
	verts_total[14] = 1;

	*/
	int verts_size = indicy * 5;

	model.location = model_name;

	model.vertices = (float *)verts_total;
	printf("%d\n", verts_size);
	model.vertex_size = verts_size;

	unsigned int *vertex_faces = malloc(indicy * sizeof(unsigned int));
	printf("HEO);\n");
	int i_vertices = 12 * 3;
	vertex_faces[0] = 0;
	vertex_faces[1] = 1;
	vertex_faces[2] = 2;

	// set indices to be index
	memcpy(vertex_faces, index, 12 * 3);
	
	
	model.vertex_faces = vertex_faces;
	model.vertex_face_size = i_vertices;

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(state->program);
}
