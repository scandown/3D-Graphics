#include "model.h"



// If you cant find the vertex, normal and uv in the new vertex array then
// set index to i in the for loop

// then check found bool, if found add the new vertices to the unique
// vertex array and also add the index to the elements buffer array

bool get_similar_index(v3Array vertex, v2Array uv, v3Array normal,
		       v3Array out_vertex, v2Array out_uv, v3Array out_normal,
		       Indices_Array out_indices) {
	int i;
	for (i = 0; i < vertex.count; ++i) {
		if (vertex.items[i][0] == out_vertex.items[i][0] &&
		    vertex.items[i][1] == out_vertex.items[i][1] &&
		    vertex.items[i][2] == out_vertex.items[i][2] &&
                                                                
		    normal.items[i][0] == out_normal.items[i][0] &&
		    normal.items[i][1] == out_normal.items[i][1] &&
		    normal.items[i][2] == out_normal.items[i][2] &&
		    uv.items[i][0]     == out_uv.items[i][0]     &&
		    uv.items[i][1]     == out_uv.items[i][1]     &&
		    uv.items[i][2]     == out_uv.items[i][2]) {
			return false;
		}
	}

	DA_PUSH_VEC(out_vertex, vertex.items[i], 3);
	DA_PUSH_VEC(out_normal, normal.items[i], 3);
	DA_PUSH_VEC(out_uv, uv.items[i], 2);
	DA_PUSH(out_indices, i);

	return true;

}

void index_VBO(v3Array vertex, v2Array uv, v3Array normal,
		v3Array out_vertex, v2Array out_uv, v3Array out_normal,
		Indices_Array out_indices) {


	if (get_similar_index(vertex, uv, normal, out_vertex, out_uv, out_normal, out_indices)) {
		
	}


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

	v3Array vertex = {0};
	v3Array normal = {0};
	v2Array uv = {0};

	int count = 0;
	int vcount = 0;
	int vncount = 0;
	int vtcount = 0;
	int first_face_byte_offset = 0;
	int total_face = 0;
	int face_newline_byte_offset = 0;
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

						DA_PUSH_VEC(vertex, v, 3);

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

						DA_PUSH_VEC(normal, vn, 3);
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

						DA_PUSH_VEC(uv, vt, 2);
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
					int j = i;
					while(buffer[j] != '\n') {
						j++;
					}

					face_newline_byte_offset = j;
				}
				break;
			default:
				break;
		}
	}


	int vertex_index = 0;
	int *vertex_face = malloc(total_face * 3 * sizeof(int));
	int *uv_face = malloc(total_face * 3 * sizeof(int));
	int *normal_face = malloc(total_face * 3 * sizeof(int));

	v3Array vertex_ordered = {0};
	v3Array normal_ordered = {0};
	v2Array uv_ordered = {0};

	int vertex_i = 0;
	int normal_i = 0;
	int uv_i = 0;

	int initial_point = first_face_byte_offset;
	index = 0;
	int init_index = 0;
	int prev_index = index;

	int face_version = 0;
	const int FACE_STRING_OFFSET = 2;
	for (int i = first_face_byte_offset; i < sz; ++i) {
		if (init_index >= FACE_STRING_OFFSET) {
			line_buffer[index] = buffer[i];
			index++;
			if (buffer[i] == '/' || buffer[i] == ' ' || buffer[i] == '\n') {
				line_buffer[index - 1] = 0;
				if (face_version == 0) {
					// VERTEX
					vertex_face[vertex_i] = atoi(line_buffer + prev_index);
					DA_PUSH_VEC(vertex_ordered, vertex.items[vertex_face[vertex_i] - 1], 3);
					vertex_i++;
					face_version++;
				} else if (face_version == 1) {
					// UV
					uv_face[uv_i] = atoi(line_buffer + prev_index);
					DA_PUSH_VEC(uv_ordered, uv.items[uv_face[uv_i] - 1], 2);
					uv_i++;
					face_version++;
				} else if (face_version == 2) {
					// NORMAL
					normal_face[normal_i] = atoi(line_buffer + prev_index);
					DA_PUSH_VEC(normal_ordered, normal.items[normal_face[normal_i] - 1], 3);
					normal_i++;
					face_version++;
				}
				//printf("%d\n", atoi(line_buffer + prev_index));
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
			//printf("%s\n", line_buffer);
			index = 0;
			prev_index = index;
			init_index = 0;

		}
	}

	// use faces

	//printf("%s\n", buffer);
	printf("Total file size: %d\n", sz);
	printf("New lines: %d\n", count);
	printf("Vertices: %d\n", vcount);
	printf("Vertex Normals: %d\n", vncount);
	printf("Uvs: %d\n", vtcount);
	printf("face_byte_offset: %c\n",buffer[first_face_byte_offset]);
	free(buffer);

	model.vertex_arr = vertex_ordered;
	model.uv_arr = uv_ordered;
	model.normal_arr = normal_ordered;

	free(vertex_face);
	free(uv_face);
	free(normal_face);

	free(vertex.items);
	free(uv.items);
	free(normal.items);

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

