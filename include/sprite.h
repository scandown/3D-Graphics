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
	Model plane;
} Sprite;

Sprite sprite_init(int instance_amount; jmp_buf error, vec3 pos, unsigned int scale, char *texture_location,
		vec2 translation[instance_amount], vec2 spr_num[instance_amount], int instance_amount, int width, int height);
void sprite_draw(Sprite *sprite, unsigned int program, int instance_amount);
void sprite_delete(Sprite *sprite);


void sprite_generate_model(Model *model, int width, int height);
void sprite_send_instanced_positions(vec2 *instanced_positions, vec2 *instanced_spr_num, int instance_x, int instance_y);
