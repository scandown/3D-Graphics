#include "user/buffers.h"

void buffers_init(Model *model, Instance *instance, int translation_size) {
	glGenVertexArrays(1, &model->VAO);
	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);
	glGenBuffers(1, &model->instance_UV_VBO);
	glGenBuffers(1, &model->instance_spr_VBO);


	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(float), model->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->vertex_face_size * sizeof(unsigned int), model->vertex_faces, GL_STATIC_DRAW);



	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	if (instance->is_instanced) {
		glBindBuffer(GL_ARRAY_BUFFER, model->instance_UV_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * translation_size, instance->instance_array, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, model->instance_spr_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * translation_size, instance->spr_num, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, model->instance_UV_VBO);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(0));
		glVertexAttribDivisor(3, 1);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, model->instance_spr_VBO);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(0));
		glVertexAttribDivisor(4, 1);
		glEnableVertexAttribArray(4);
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void model_delete_buffers(Model *model) {
	glDeleteVertexArrays(1, &model->VAO);
	glDeleteBuffers(1, &model->VBO);
	glDeleteBuffers(1, &model->EBO);
	glDeleteBuffers(1, &model->instance_UV_VBO);
	glDeleteBuffers(1, &model->instance_spr_VBO);

	free(model->vertices);
	free(model->vertex_faces);
}

void model_draw(Model *model, unsigned int program, unsigned int instance_amount) {
	model->uniform.value.m4[3][0] = model->x;
	model->uniform.value.m4[3][1] = model->y;
	model->uniform.value.m4[3][2] = model->z;
	model->uniform.value.m4[3][3] = 1;
	uniform_send_to_gpu(&model->uniform, program, "model");

	glBindTexture(GL_TEXTURE_2D, model->texture);
	glBindVertexArray(model->VAO);
	glDrawElementsInstanced(GL_TRIANGLES, model->vertex_face_size, GL_UNSIGNED_INT, 0, instance_amount);
}

void model_init(jmp_buf error, Model *model, vec3 pos, char *texture_location, Instance *instance, int instance_amount) {

	unsigned int texture = texture_init(error, GL_RGBA, texture_location);

	mat4 model_matrix;
	glm_mat4_identity(model_matrix);

	if (instance->is_instanced) {
		buffers_init(model, instance, instance_amount);
	} else {
		buffers_init(model, instance, 0);
	}

	model->uniform = uniform_set_data(model_matrix, UNIFORM_MAT4);
	model->x = pos[0];
	model->y = pos[1];
	model->z = pos[2];
	model->texture = texture;
}
