#include "sprite.h"


Sprite sprite_init(jmp_buf error, vec3 pos, unsigned int scale, char *texture_location, vec2 *translation, int instance_amount) {

	Sprite sprite;

	Model plane = model_load(error, "assets/plane.obj");
	model_init_instanced(error, &plane, pos, texture_location, translation, instance_amount);
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

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	//glEnableVertexAttribArray(2);


void sprite_generate_model(Model *model, int width, int height) {
	// vertex, uv, normal
	unsigned int vertices[8 * 4] = {
		0, 0, 0,	0, 0,	0, 0, 0,
		1, 0, 0,	0, 0,	0, 0, 0,
		1, 1, 0,	0, 0,	0, 0, 0,
		0, 1, 0,	0, 0,	0, 0, 0
	}

	unsigned int faces[3 * 2] = {
		0, 2, 1,
		0, 3, 2
	}

	memcpy(model->vertices, vertices, 8 * 4);
	memcpy(model->vertex_faces, faces, 3 * 2);
	model->vertex_size = 8 * 4;
	model->vertex_face_size = 3 * 2;

}
