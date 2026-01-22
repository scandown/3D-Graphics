#include "sprite.h"


Sprite sprite_init(int instance_amount; jmp_buf error, vec3 pos, unsigned int scale, char *texture_location,
		vec2 translation[instance_amount], vec2 spr_num[instance_amount], int instance_amount, int width, int height) {

	Sprite sprite;

	Model plane;
	sprite_generate_model(&plane, width, height);

	model_init_instanced(error, &plane, pos, texture_location, translation, spr_num, instance_amount);
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


void sprite_generate_model(Model *model, int width, int height) {
	// vertex, uv, normal
	float vertices[8 * 4] = {
		0, 0, 0,		0, 0,	0, 0, 0,
		width, 0, 0,		1, 0,	0, 0, 0,
		width, height, 0,	1, 1,	0, 0, 0,
		0, height, 0,		0, 1,	0, 0, 0
	};

	unsigned int faces[3 * 2] = {
		0, 1, 2,
		0, 3, 2
	};

	model->vertices = malloc(sizeof(float) * 8 * 4);
	model->vertex_faces = malloc(sizeof(unsigned int) * 3 * 2);
	memcpy(model->vertices, vertices, sizeof(float) * 8 * 4);
	memcpy(model->vertex_faces, faces, sizeof(unsigned int) * 3 * 2);
	model->vertex_size = 8 * 4;
	model->vertex_face_size = 3 * 2;

}


void sprite_send_instanced_positions(vec2 *instanced_positions, vec2 *instanced_spr_num, int instance_x, int instance_y) {
	int index = 0;
	for (int y = 0; y < instance_y; y++) {
		for (int x = 0; x < instance_x; x++) {
			vec2 translation;
			translation[0] = x * 16;
			translation[1] = y * 16;

			vec2 spr_num;
			spr_num[0] = 0;
			spr_num[1] = 1;

			glm_vec2_copy(translation, instanced_positions[index]);
			glm_vec2_copy(spr_num, instanced_spr_num[index]);
			index++;
		}
	}
}
