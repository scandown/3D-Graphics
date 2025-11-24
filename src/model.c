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

	char *file = malloc(file_size + 1);
	fread(file, 1, file_size, fptr);

	// 4 for the size of a float
	// remember to clean up after
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


	char vertex[3][10];
	char texture[2][10];

	
	while (line != NULL) {
		if (line[0] == 'v' && line[1] == ' ') {
			sscanf(line+2, "%s %s %s", vertex[0], vertex[1], vertex[2]);
			printf("%s %s %s\n", vertex[0], vertex[1], vertex[2]);
			i_vertices++;
		}
		else if (line[0] == 'v' && line[1] == 't') {
			sscanf(line+2, "%s %s", texture[0], texture[1]);
			printf("%s %s\n", texture[0], texture[1]);
			i_textures++;
		}
		else if (line[0] == 'f' && line[1] == ' ') {
		}

		line = strtok_r(NULL, "\n", &save_line);
	}

	int verts_size = i_textures + i_vertices;

	// combine vertex array into format for opengl
	printf("%d, %d\n", verts_size, i_vertex_faces);
	model_vals *verts_total = malloc(verts_size * sizeof(model_vals));
	int vert_diff = 0;
	for (int i = 0; i < i_vertex_faces; i++) {
		for (int j = 0; j < 3; j++) {
			// verts[vertex_faces[i]].vertices = vertices[vertex_faces[i]]
			verts_total[i].vertices[j] = vertices[(3 * i) + j];
		}
	}
	for (int i = 0; i < i_texture_faces; i++) {
		for (int j = 0; j < 2; j++) {
			verts_total[i].texture[j] = textures[(2 * i) + j];
		}
	}

	for (int i = 0; i < i_vertex_faces; i++) {
		for (int j = 0; j < 5; j++) {
			printf("%f, ", verts_total[i].vertices[j]);
		}
		printf("\n");
	}
	

	file[file_size] = '\0';

	fclose(fptr);


	model.location = model_name;

	model.vertices = (float *)verts_total;
	model.vertex_size = verts_size;

	model.vertex_faces = vertex_faces;
	model.texture_faces = texture_faces;
	model.vertex_face_size = i_vertex_faces;
	model.texture_face_size = i_texture_faces;

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
