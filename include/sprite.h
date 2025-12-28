#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <setjmp.h>

#include "model.h"
#include "spaces.h"
#include "shader.h"
#include "texture.h"

typedef struct {
	Space model;
	Model plane;
	unsigned int texture;
	unsigned int program;
	float x;
	float y;
	float z;
} Sprite;

Sprite load_sprite(jmp_buf error, vec3 pos, unsigned int scale, char *texture_location);
void draw_sprite(Sprite *sprite);
