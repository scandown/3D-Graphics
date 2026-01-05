#include "sprite.h"


Sprite sprite_init(jmp_buf error, vec3 pos, unsigned int scale, char *texture_location) {

	Sprite sprite;

	Model plane = model_load(error, "assets/plane.obj");
	model_init(error, &plane, pos, texture_location);
	glm_mat4_scale(plane.uniform.value.m4, scale);
	sprite.plane = plane;

	return sprite;
}

void sprite_draw(Sprite *sprite, unsigned int program, int instance_amount) {
	model_draw_instanced(&sprite->plane, program, instance_amount);
}


void sprite_delete(Sprite *sprite) {
	model_delete_buffers(&sprite->plane);
}
