#include "state.h"

State setup_state(jmp_buf error, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, char *name, char *shader_location) {
	State state;
	state.SCR_WIDTH = SCR_WIDTH;
	state.SCR_HEIGHT = SCR_HEIGHT;
	state.title = name;

	state.window = setup_window(state.SCR_WIDTH, state.SCR_HEIGHT, state.title);
	state.program = program_create(shader_location);

	if (state.window == NULL) {
		longjmp(error, 1);
	}

	glUseProgram(state.program);

	return state;
}


void setup_scene(State *game, unsigned int program) {
	mat4 view_matrix;
	mat4 projection_matrix;
	mat4 model_matrix;
	glm_mat4_identity(view_matrix);
	glm_mat4_identity(model_matrix);
	glm_mat4_identity(projection_matrix);

	game->view_uniform = uniform_init(program, "view", view_matrix, UNIFORM_MAT4);
	game->model_uniform = uniform_init(program, "model", model_matrix, UNIFORM_MAT4);
	game->projection_uniform = uniform_init(program, "projection", projection_matrix, UNIFORM_MAT4);
	uniform_send(&game->model_uniform);
	uniform_send(&game->view_uniform);
}
