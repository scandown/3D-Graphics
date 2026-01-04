#include "sprite.h"


Sprite load_sprite(jmp_buf error, unsigned int program, vec3 pos, unsigned int scale, char *texture_location) {

	Sprite sprite;

	Model plane = model_load(error, "assets/plane.obj");

	unsigned int texture = texture_setup(error, GL_RGBA, texture_location);

	mat4 model_matrix;
	glm_mat4_identity(model_matrix);

	sprite.model_uniform = uniform_init(program, "model", model_matrix, UNIFORM_MAT4);

	create_buffers(&plane);
	model_send_to_gpu(&plane);



	sprite.plane = plane;
	sprite.texture = texture;

	glm_mat4_scale(sprite.model_uniform.value.m4, scale);

	sprite.x = pos[0];
	sprite.y = pos[1];
	sprite.z = pos[2];


	return sprite;
}

void draw_sprite(Sprite *sprite) {

		glBindTexture(GL_TEXTURE_2D, sprite->texture);

		sprite->model_uniform.value.m4[3][0] = sprite->x;
		sprite->model_uniform.value.m4[3][1] = sprite->y;
		sprite->model_uniform.value.m4[3][2] = sprite->z;
		sprite->model_uniform.value.m4[3][3] = 1;

		uniform_send(&sprite->model_uniform);

		glBindVertexArray(sprite->plane.VAO);
		glDrawElements(GL_TRIANGLES, sprite->plane.vertex_face_size, GL_UNSIGNED_INT, 0);
}


void delete_sprite(Sprite *sprite) {
	delete_buffers(&sprite->plane);


	free(sprite->plane.vertices);
	free(sprite->plane.vertex_faces);
}
