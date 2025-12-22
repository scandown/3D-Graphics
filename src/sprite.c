#include "sprite.h"


Sprite load_sprite(jmp_buf error, vec3 pos, char *texture_location) {

	Sprite sprite;

	Model plane = model_load(error, "assets/plane.obj");


	unsigned int texture = texture_setup(error, GL_RGB, texture_location);
	unsigned int light_program = program_create("src/light_shaderlist.txt");
	sprite.program = light_program;



	Space model;
	glm_mat4_identity(model.matrix);
	setup_space(&model, "model", sprite.program);


	create_buffers(&plane);
	model_send_to_gpu(&plane);



	sprite.plane = plane;
	sprite.texture = texture;
	sprite.model = model;

	sprite.x = pos[0];
	sprite.y = pos[1];
	sprite.z = pos[2];

	return sprite;
}

void draw_sprite(Sprite *sprite) {
		glUseProgram(sprite->program);

		glBindTexture(GL_TEXTURE_2D, sprite->texture);

		sprite->model.matrix[3][0] = sprite->x;
		sprite->model.matrix[3][1] = sprite->y;
		sprite->model.matrix[3][2] = sprite->z;
		sprite->model.matrix[3][3] = 1;
		sprite->model.set_uniform(&sprite->model);

		glBindVertexArray(sprite->plane.VAO);
		glDrawElements(GL_TRIANGLES, sprite->plane.vertex_face_size, GL_UNSIGNED_INT, 0);
}
