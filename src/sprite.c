#include "sprite.h"

Sprite sprite_init(jmp_buf error, vec3 pos, unsigned int scale, char *texture_location, float width, float height) {

	Sprite sprite;

	Model plane = {0};
	sprite_generate_model(&plane, width, height);

	model_init(error, &plane, pos, texture_location);
	glm_mat4_scale(plane.uniform.value.m4, scale);
	sprite.plane = plane;

	return sprite;
}

void sprite_draw(Sprite *sprite, unsigned int program, int instance_amount) {
	model_draw(&sprite->plane, program, instance_amount);
}


void sprite_delete(Sprite *sprite) {
	model_delete_buffers(&sprite->plane);
}


void sprite_generate_model(Model *model, float width, float height) {
	// vertex, uv, normal
	vec3 vertex[] = {
		{0, 0, 0},
		{width, 0, 0},
		{width, height, 0},

		{0, 0, 0},
		{width, height, 0},
		{0, height, 0}
	};
	vec2 uv[] = {
		{0, 0},
		{1, 0},
		{1, 1},

		{0, 0},
		{1, 1},
		{0, 1}
	};

	vec3 normal[] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},

		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	unsigned int faces[3 * 2] = {
		0, 1, 2,
		0, 3, 2
	};

	

	for (int i = 0; i < 6; ++i) {
		DA_PUSH_VEC(model->vertex_arr, vertex[i], 3);
		DA_PUSH_VEC(model->normal_arr, normal[i], 3);
		DA_PUSH_VEC(model->uv_arr, uv[i], 2);
	}
}


