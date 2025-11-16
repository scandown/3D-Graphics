#pragma once
#include "cglm/cglm.h"
#include "glad/glad.h"

typedef struct space {

	mat4 matrix;
	int location;

	void (*scale)(struct space *space, vec3 scale);
	void (*translate)(struct space *space, vec3 translation);
	// maybe rename apply uniform
	void (*set_uniform)(struct space *space);
} Space;

void setup_space(Space *space, char *name, int program);
