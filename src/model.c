#include "model.h"


float *model_load(char *model_name, int *num_of_vertices) {

	FILE *fptr = fopen(model_name, "r");	
	if (fptr == NULL) {
		printf("Couldn't open: %s\n", model_name);
		return NULL;
	}


	fseek(fptr, 0L, SEEK_END);
	long file_size = ftell(fptr);
	rewind(fptr);

	char *file = malloc(file_size + 1);
	fread(file, 1, file_size, fptr);

	// 4 for the size of a float
	float *vertices = malloc((file_size) * sizeof(float));


	// get each line of the vertices
	char *save_line, *save_vertex;
	char *line = strtok_r(file, "\n", &save_line);
	int i = 0;

	while (line != NULL) {
		if (line[0] == 'v' && line[1] == ' ') {
			char *vertex = strtok_r(line + 2, " ", &save_vertex);
			while (vertex != NULL) {
				vertices[i++] = atof(vertex);
				vertex = strtok_r(NULL, " ", &save_vertex);
			}
		}
		line = strtok_r(NULL, "\n", &save_line);
	}

	printf("Number of floats: %i\n", i);

	//printf("%f\n", vertices[0]);


	file[file_size] = '\0';

	fclose(fptr);

	*num_of_vertices = i;

	return vertices;
}

