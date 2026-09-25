#include "user/buffers.h"

void buffers_gen(Model *model) {
	glGenVertexArrays(1, &model->VAO);
	unsigned int VBO_amount = 6;
	arraddn(model->VBO_array, VBO_amount);
	glGenBuffers(VBO_amount, model->VBO_array);
}

void buffers_init(Model *model) {

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, arrlen(model->vertex_array) * sizeof(vec3), model->vertex_array, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_UV]);
	glBufferData(GL_ARRAY_BUFFER, arrlen(model->uv_array) * sizeof(vec2), model->uv_array, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, arrlen(model->normal_array) * sizeof(vec3), model->normal_array, GL_STATIC_DRAW);

	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_VERTEX]);
	glVertexAttribPointer(VBO_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(VBO_VERTEX);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_UV]);
	glVertexAttribPointer(VBO_UV, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));
	glEnableVertexAttribArray(VBO_UV);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_NORMAL]);
	glVertexAttribPointer(VBO_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
	glEnableVertexAttribArray(VBO_NORMAL);

}

void buffers_gen_and_init(Model *model) {
	buffers_gen(model);
	buffers_init(model);
}

void instanced_buffers_init(Model *model, vec3 *instance_pos, vec2 *instance_uv, int translation_size, bool setup) {
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_I_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * translation_size, instance_pos, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_I_UV]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * translation_size, instance_uv, GL_DYNAMIC_DRAW);

	if (setup) {
		glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_I_POSITION]);
		glVertexAttribPointer(VBO_I_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));
		glVertexAttribDivisor(VBO_I_POSITION, 1);
		glEnableVertexAttribArray(VBO_I_POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, model->VBO_array[VBO_I_UV]);
		glVertexAttribPointer(VBO_I_UV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(0));
		glVertexAttribDivisor(VBO_I_UV, 1);
		glEnableVertexAttribArray(VBO_I_UV);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void model_delete_buffers(Model *model) {
	glDeleteVertexArrays(1, &model->VAO);
	glDeleteBuffers(arrlen(model->VBO_array), model->VBO_array);

	arrfree(model->VBO_array);

	arrfree(model->vertex_array);
	arrfree(model->normal_array);
	arrfree(model->uv_array);
}

void model_draw(Model *model, vec3 pos, unsigned int program, unsigned int instance_amount) {
	glm_vec3_copy(pos, model->pos);
	glm_vec3_copy(model->pos, model->uniform.value.m4[3]);
	model->uniform.value.m4[3][3] = 1;
	uniform_send_to_gpu(&model->uniform, program, "model");

	glBindTexture(GL_TEXTURE_2D, model->texture);
	glBindVertexArray(model->VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, arrlen(model->vertex_array), instance_amount);
}

void model_init(jmp_buf error, Model *model, char *texture_location) {

	unsigned int texture = texture_init(error, GL_RGBA, texture_location);

	glm_mat4_identity(model->uniform.value.m4);
	model->uniform.type = UNIFORM_MAT4;
	model->texture = texture;
}
