#include "state.h"

State setup_state(jmp_buf error, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, char *name) {
	State state;
	state.SCR_WIDTH = SCR_WIDTH;
	state.SCR_HEIGHT = SCR_HEIGHT;
	state.title = name;

	state.window = setup_window(state.SCR_WIDTH, state.SCR_HEIGHT, state.title);

	if (state.window == NULL) {
		longjmp(error, 1);
	}

	return state;
}


void setup_scene(State *game, unsigned int program, char *dimension, vec3 initial_position) {
	mat4 view_matrix;
	mat4 projection_matrix;
	mat4 model_matrix;
	glm_mat4_identity(view_matrix);
	glm_mat4_identity(model_matrix);
	glm_mat4_identity(projection_matrix);


	if (strncmp(dimension, "2D", 2) == 0) {
		glm_ortho(0, 640, 0, 360, 0.1, 1000, projection_matrix);
	} else if (strncmp(dimension, "3D", 2) == 0) {
		glm_perspective(glm_rad(70), 16.0/9.0, 0.1, 1000, projection_matrix);
	} else {
		fprintf(stderr, "Invalid dimension parameter: %s\n", dimension);
		return;
	}

	glm_translate(game->view_uniform.value.m4, initial_position);

	game->view_uniform = uniform_init(program, "view", view_matrix, UNIFORM_MAT4);
	game->model_uniform = uniform_init(program, "model", model_matrix, UNIFORM_MAT4);
	game->projection_uniform = uniform_init(program, "projection", projection_matrix, UNIFORM_MAT4);

	uniform_send(&game->model_uniform);
	uniform_send(&game->view_uniform);
	uniform_send(&game->projection_uniform);
}
