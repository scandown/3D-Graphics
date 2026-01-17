#include "state.h"

State state_init(jmp_buf error, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, char *name) {
	State state;
	state.SCR_WIDTH = SCR_WIDTH;
	state.SCR_HEIGHT = SCR_HEIGHT;
	state.title = name;

	state.window = window_init(state.SCR_WIDTH, state.SCR_HEIGHT, state.title);

	if (state.window == NULL) {
		longjmp(error, 1);
	}

	return state;
}


void matrix_init(State *game, unsigned int program, char *dimension) {
	mat4 view_matrix;
	mat4 projection_matrix;
	mat4 model_matrix;
	glm_mat4_identity(view_matrix);
	glm_mat4_identity(model_matrix);
	glm_mat4_identity(projection_matrix);


	if (strncmp(dimension, "2D", 2) == 0) {
		glm_ortho(0, 640, 0, 360, 0.1, 100000, projection_matrix);
	} else if (strncmp(dimension, "3D", 2) == 0) {
		glm_perspective(glm_rad(70), 16.0/9.0, 0.1, 100000, projection_matrix);
	} else {
		fprintf(stderr, "Invalid dimension parameter: %s\n", dimension);
		return;
	}

	game->view_uniform = uniform_set_data(view_matrix, UNIFORM_MAT4);
	game->model_uniform = uniform_set_data(model_matrix, UNIFORM_MAT4);
	game->projection_uniform = uniform_set_data(projection_matrix, UNIFORM_MAT4);

	uniform_send_to_gpu(&game->model_uniform, program, "model");
	uniform_send_to_gpu(&game->view_uniform, program, "view");
	uniform_send_to_gpu(&game->projection_uniform, program, "projection");
}
