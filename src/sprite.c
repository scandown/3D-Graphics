#include "sprite.h"


Sprite sprite_init(jmp_buf error, unsigned int program, vec3 pos, unsigned int scale, char *texture_location) {

	Sprite sprite;

	Model plane = model_load(error, "assets/plane.obj");
	model_init(error, &plane, program, pos, texture_location);
	sprite.plane = plane;

	return sprite;
}

void sprite_draw(Sprite *sprite) {
	model_draw(&sprite->plane);
}


void sprite_delete(Sprite *sprite) {
	model_delete_buffers(&sprite->plane);
}
