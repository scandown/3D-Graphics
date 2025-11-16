#include "state.h"

State setup_state(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, char *name, char *shader_location) {
	State state;
	state.SCR_WIDTH = SCR_WIDTH;
	state.SCR_HEIGHT = SCR_HEIGHT;
	state.title = name;

	state.window = setup_window(state.SCR_WIDTH, state.SCR_HEIGHT, state.title);
	state.program = program_create(shader_location);

	return state;
}
