#include "model.h"

void string_search_vertex_point(int *i,float *array, char *line) {
	char *save = NULL;
	char *vertex = strtok_r(line + 2, " ", &save);
	while (vertex != NULL) {
		array[(*i)++] = atof(vertex);
		vertex = strtok_r(NULL, " ", &save);
	}
}
void string_search_texture_point(int *i,float *array, char *line) {
	char *save = NULL;
	char *vertex = strtok_r(line + 2, " ", &save);
	while (vertex != NULL) {
		array[(*i)++] = atof(vertex);
		vertex = strtok_r(NULL, "/", &save);
	}
}


void string_search_vertex_face(int *i, unsigned int *array, char *line) {
	char *save = NULL;
	char *vertex = strtok_r(line + 2, " ", &save);
	while (vertex != NULL) {
		int type_size = length_to_token(vertex, '/');
		if (type_size == -1) {
			// incase of not having texture coords or normals
			type_size = length_to_token(vertex, ' ');
		}
		if (type_size == -1) {
			// incase value is at the end of line
			type_size = length_to_token(vertex, '\n');
		}
		char *end;
		char vertex_copy[type_size];
		strncpy(vertex_copy, vertex, type_size);
		array[(*i)++] = strtol(vertex_copy, &end, 10) - 1;
		vertex = strtok_r(NULL, " ", &save);
	}
}
void string_search_texture_face(int *i, unsigned int *array, char *line) {
	char *save = NULL;
	char *vertex = strtok_r(line + 2, " ", &save);
	while (vertex != NULL) {
		array[(*i)++] = atoi(vertex) - 1;
		vertex = strtok_r(NULL, "/", &save);
	}
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

	char *file = malloc(file_size + 1);
	fread(file, 1, file_size, fptr);

	// 4 for the size of a float
	float *vertices = malloc((file_size) * sizeof(float));
	float *textures = malloc((file_size) * sizeof(float));
	unsigned int *vertex_faces = malloc((file_size) * sizeof(unsigned int));
	unsigned int *texture_faces = malloc((file_size) * sizeof(unsigned int));


	// get each line of the vertices
	char *save_line;
	char *line = strtok_r(file, "\n", &save_line);
	int i_vertices = 0;
	int i_textures = 0;
	int i_normals = 0;
	int i_vertex_faces = 0;
	int i_texture_faces = 0;

	
	while (line != NULL) {
		if (line[0] == 'v' && line[1] == ' ') {
			string_search_vertex_point(&i_vertices, vertices, line);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			string_search_texture_point(&i_textures, textures, line);
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			if (i_vertices && i_textures == 0 && i_normals == 0) {
				string_search_vertex_face(&i_vertex_faces, vertex_faces, line);
			}
			else if (i_vertices && i_textures && i_normals == 0) {
				string_search_vertex_face(&i_vertex_faces, vertex_faces, line);
				string_search_texture_face(&i_texture_faces, texture_faces, line);
			}
		}

		line = strtok_r(NULL, "\n", &save_line);
	}

	//printf("%f\n", vertices[0]);

	file[file_size] = '\0';

	fclose(fptr);

	model.vertices = vertices;
	model.textures = textures;
	model.vertex_faces = vertex_faces;
	model.texture_faces = texture_faces;
	model.vertex_size = i_vertices;
	model.texture_size = i_textures;
	model.vertex_face_size = i_vertex_faces;
	model.texture_face_size = i_texture_faces;
	model.location = model_name;
	return model;
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(state->program);
}
