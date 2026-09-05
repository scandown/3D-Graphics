#include "model.h"



// If you cant find the vertex, normal and uv in the new vertex array then
// set index to i in the for loop

// then check found bool, if found add the new vertices to the unique
// vertex array and also add the index to the elements buffer array

bool get_similar_index(vec3 *vertex_array, vec2 *uv_array, vec3 *normal_array,
		       vec3 *out_vertex_array, vec2 *out_uv_array, vec3 *out_normal_array,
		       unsigned int *out_indices_array) {
	int i;
	for (i = 0; i < arrlen(vertex_array); ++i) {
		if (vertex_array[i][0] == out_vertex_array[i][0] &&
		    vertex_array[i][1] == out_vertex_array[i][1] &&
		    vertex_array[i][2] == out_vertex_array[i][2] &&
                                                                
		    normal_array[i][0] == out_normal_array[i][0] &&
		    normal_array[i][1] == out_normal_array[i][1] &&
		    normal_array[i][2] == out_normal_array[i][2] &&
		    uv_array[i][0]     == out_uv_array[i][0]     &&
		    uv_array[i][1]     == out_uv_array[i][1]     &&
		    uv_array[i][2]     == out_uv_array[i][2]) {
			return false;
		}
	}

	unsigned int vertex_len = arrlen(out_vertex_array);
	unsigned int normal_len = arrlen(out_normal_array);
	unsigned int uv_len = arrlen(out_uv_array);
	arraddn(out_vertex_array, 1);
	arraddn(out_normal_array, 1);
	arraddn(out_uv_array, 1);

	for (int j = 0; j < 3; j++) {
		out_vertex_array[vertex_len][j] = vertex_array[i][j];
		out_normal_array[normal_len][j] = normal_array[i][j];
		if (j < 2) {
			out_uv_array[uv_len][j] = uv_array[i][j];
		}
	}
	arrput(out_indices_array, i);

	return true;

}


Model obj_load(jmp_buf error, char *model_name) {
	Model model = {0};
	FILE *fptr = fopen(model_name, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error: Couldn't open file %s\n", model_name);
		longjmp(error, 1);
	}

	fseek(fptr, 0L, SEEK_END);
	size_t sz = ftell(fptr);

	rewind(fptr);
	// added one to buffer size to account for indexing over the array size 
	char *buffer = malloc((sz + 1) * sizeof(char));
	buffer[sz] = 0;

	fread(buffer, sizeof(*buffer), sz, fptr);
	rewind(fptr);

	vec3 *vertex_array = NULL;
	vec3 *normal_array = NULL;
	vec2 *uv_array = NULL;

	int count = 0;
	int vcount = 0;
	int vncount = 0;
	int vtcount = 0;
	int first_face_byte_offset = 0;
	int total_face = 0;
	char line_buffer[BUFSIZ] = {0};
	int index = 0;
	for (int i = 0; i < sz; ++i) {
		line_buffer[index] = buffer[i];
		switch (buffer[i]) {
			int space_diff;
			case '\n':
				count++;
				break;
			case 'v':
				switch(buffer[i+1]) {
					case ' ':
						float v[3];
						v[0] = atof(buffer + i+2);
						space_diff = 0;
						while (buffer[i+2+space_diff] != ' ' && buffer[i+2+space_diff] != '\n') {
							space_diff++;
						}
						v[1] = atof(buffer + i+2+space_diff);
						space_diff++;
						while (buffer[i+2+space_diff] != ' ' && buffer[i+2+space_diff] != '\n') {
							space_diff++;
						}
						v[2] = atof(buffer + i+2+space_diff);

						arrput_vector(vertex_array, v, 3);

						vcount++;
						break;
					case 'n':
						float vn[3];
						vn[0] = atof(buffer + i+2);
						space_diff = 1;
						while (buffer[i+2+space_diff] != ' ' && buffer[i+2+space_diff] != '\n') {
							space_diff++;
						}
						vn[1] = atof(buffer + i+2+space_diff);
						space_diff++;
						while (buffer[i+2+space_diff] != ' ' && buffer[i+2+space_diff] != '\n') {
							space_diff++;
						}
						vn[2] = atof(buffer + i+2+space_diff);

						arrput_vector(normal_array, vn, 3);
						vncount++;
						break;
					case 't':
						float vt[2];
						vt[0] = atof(buffer + i+2);
						space_diff = 1;
						while (buffer[i+2+space_diff] != ' ' && buffer[i+2+space_diff] != '\n') {
							space_diff++;
						}
						vt[1] = atof(buffer + i+2+space_diff);
						space_diff++;

						arrput_vector(uv_array, vt, 2);
						vtcount++;
						break;
					default:
						break;
				}
				break;
			case 'f':
				total_face++;
				if (first_face_byte_offset == 0) {
					first_face_byte_offset = i;
				}
				break;
			default:
				break;
		}
	}


	int vertex_index = 0;
	vec3 *vertex_ordered_array = NULL;
	vec3 *normal_ordered_array = NULL;
	vec2 *uv_ordered_array = NULL;

	index = 0;
	int init_index = 0;
	int prev_index = index;

	// move into case 'f'
	int face_version = 0;
	const int FACE_STRING_OFFSET = 2;
	for (int i = first_face_byte_offset; i < sz; ++i) {
		if (init_index >= FACE_STRING_OFFSET) {
			line_buffer[index] = buffer[i];
			index++;
			if (buffer[i] == '/' || buffer[i] == ' ' || buffer[i] == '\n') {
				line_buffer[index - 1] = 0;
				switch (face_version) {
					case 0:
						// VERTEX
						int vertex_index = atoi(line_buffer + prev_index);
						arrput_vector(vertex_ordered_array, vertex_array[vertex_index - 1], 3);
						face_version++;
						break;
					case 1:
						// UV
						int uv_index = atoi(line_buffer + prev_index);
						arrput_vector(uv_ordered_array, uv_array[uv_index - 1], 2);
						face_version++;
						break;
					case 2:
						// NORMAL
						int normal_index = atoi(line_buffer + prev_index);
						arrput_vector(normal_ordered_array, normal_array[normal_index - 1], 3);
						face_version++;
						break;
					default:
						face_version = 0;
						break;
				}
				prev_index = index;
			}

			if (face_version > 2) {
				face_version = 0;
			}
		} else {
			init_index++;
		}

		if (buffer[i] == '\n') {
			line_buffer[index] = 0;
			index = 0;
			prev_index = index;
			init_index = 0;

		}
	}

	// use faces

	free(buffer);
	model.vertex_array = vertex_ordered_array;
	model.uv_array = uv_ordered_array;
	model.normal_array = normal_ordered_array;


	arrfree(vertex_array);
	arrfree(uv_array);
	arrfree(normal_array);

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

