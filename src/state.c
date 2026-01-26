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
