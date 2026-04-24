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
	memset(&model, 0, sizeof(model));

	FILE *fptr = fopen(model_name, "r");	
	if (fptr == NULL) {
		fprintf(stderr, "Error: Couldn't open: %s\n", model_name);
		model.location = NULL;
		longjmp(error, 1);
	}




	fseek(fptr, 0L, SEEK_END);
	rewind(fptr);

	Array total_verts = {0};
	v3Array vertex_arr = {0};
	v3Array normal_arr = {0};
	v2Array uv_arr = {0};

	Indices_Array total_faces = {0};
	Indices_Array vertexIndex[3] = {0};
	Indices_Array uvIndex[3] = {0};
	Indices_Array normalIndex[3] = {0};


	// reasonable size for obj file
	char lineHeader[BUFSIZ];
	while (fgets(lineHeader, sizeof(lineHeader), fptr)) {

		if (strncmp(lineHeader, "v ", 2) == 0) {
			float vertex[3];
			sscanf(lineHeader + 2, "%f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			DA_PUSH_VEC(vertex_arr, vertex, 3);
		}
		else if (strncmp(lineHeader, "vt", 2) == 0) {
			float uv[2];
			sscanf(lineHeader + 2, "%f %f", &uv[0], &uv[1]);
			DA_PUSH_VEC(uv_arr, uv, 2);
		}
		else if (strncmp(lineHeader, "vn", 2) == 0) {
			float normal[3];
			sscanf(lineHeader + 2, "%f %f %f", &normal[0], &normal[1], &normal[2]);
			DA_PUSH_VEC(normal_arr, normal, 3);
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
				fclose(fptr);

				free(normal_arr.items);
				free(vertex_arr.items);
				free(uv_arr.items);

				longjmp(error, 1);
			}

			for (int i = 0; i < 3; ++i) {
				DA_PUSH(vertexIndex[i], v_index[i]-1);
				DA_PUSH(normalIndex[i], vn_index[i]-1);
				DA_PUSH(uvIndex[i], uv_index[i]-1);
			}

		}

	}

	for (int i = 0; i < vertexIndex[0].count; ++i) {
		for (int j = 0; j < 3; ++j) {
			unsigned int vertex_in = vertexIndex[j].items[i];
			unsigned int uv_in = uvIndex[j].items[i];
			unsigned int normal_in = normalIndex[j].items[i];

			vec3 vertex;
			vec3 uv;
			vec3 normal;

			vertex[0] = vertex_arr.items[vertex_in][0];
			vertex[1] = vertex_arr.items[vertex_in][1];
			vertex[2] = vertex_arr.items[vertex_in][2];

			uv[0] = uv_arr.items[uv_in][0];
			uv[1] = uv_arr.items[uv_in][1];
			uv[2] = uv_arr.items[uv_in][2];

			normal[0] = normal_arr.items[normal_in][0];
			normal[1] = normal_arr.items[normal_in][1];
			normal[2] = normal_arr.items[normal_in][2];


			DA_PUSH_VEC(model.vertex_arr, vertex, 3);
			DA_PUSH_VEC(model.uv_arr, uv, 2);
			DA_PUSH_VEC(model.normal_arr, normal, 3);
		}
	}

	model.location = model_name;

	free(normal_arr.items);
	free(vertex_arr.items);
	free(uv_arr.items);
	for (int i = 0; i < 3; ++i) {
		free(vertexIndex[i].items);
		free(uvIndex[i].items);
		free(normalIndex[i].items);
	}
	


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

