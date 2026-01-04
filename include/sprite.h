#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <setjmp.h>

#include "model.h"
#include "shader.h"
#include "texture.h"
#include "uniform.h"

typedef struct {
	Uniform model_uniform;
	Model plane;
	unsigned int texture;
	float x;
	float y;
	float z;
} Sprite;

Sprite sprite_init(jmp_buf error, unsigned int program, vec3 pos, unsigned int scale, char *texture_location);
void sprite_draw(Sprite *sprite);
void sprite_delete(Sprite *sprite);
