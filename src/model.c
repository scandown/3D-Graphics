#include "model.h"

void string_search_float(int *i,float *array, char *line) {
	char *save = NULL;
	char *vertex = strtok_r(line + 2, " ", &save);
	while (vertex != NULL) {
		array[(*i)++] = atof(vertex);
		vertex = strtok_r(NULL, " ", &save);
	}
}

void string_search_int(int *i, unsigned int *array, char *line) {
	char *save = NULL;
	char *vertex = strtok_r(line + 2, " ", &save);
	while (vertex != NULL) {
		array[(*i)++] = atoi(vertex) - 1;
		vertex = strtok_r(NULL, " ", &save);
	}
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


int model_load(char *model_name, int *num_of_vertices, int *num_of_faces, float **vert, unsigned int **faces) {

	FILE *fptr = fopen(model_name, "r");	
	if (fptr == NULL) {
		printf("Couldn't open: %s\n", model_name);
		return -1;
	}


	fseek(fptr, 0L, SEEK_END);
	long file_size = ftell(fptr);
	rewind(fptr);

	char *file = malloc(file_size + 1);
	char *file_copy = malloc(file_size + 1);
	fread(file, 1, file_size, fptr);
	strncpy(file_copy, file, file_size);

	// 4 for the size of a float
	float *vertices = malloc((file_size) * sizeof(float));
	unsigned int *faces_full = malloc((file_size) * sizeof(unsigned int));


	// get each line of the vertices
	char *save_line;
	char *line = strtok_r(file, "\n", &save_line);
	int i_vertices = 0;
	int i_faces = 0;

	
	while (line != NULL) {
		if (line[0] == 'v' && line[1] == ' ') {
			string_search_float(&i_vertices, vertices, line);
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			string_search_int(&i_faces, faces_full, line);
		}

		line = strtok_r(NULL, "\n", &save_line);
	}

	//printf("%f\n", vertices[0]);


	file[file_size] = '\0';

	fclose(fptr);

	*num_of_vertices = i_vertices;
	*num_of_faces = i_faces;

	*vert = vertices;
	*faces = faces_full;
	return 0;
}

