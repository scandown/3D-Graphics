#include "user/buffers.h"

void model_send_to_gpu(Model *model) {

	glGenVertexArrays(1, &model->VAO);
	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);


	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(float), model->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->vertex_face_size * sizeof(unsigned int), model->vertex_faces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void model_send_to_gpu_instanced(int translation_size; Model *model, vec2 translations[translation_size], vec2 spr_num[translation_size], int translation_size) {

	// move these into less generalised functions
	// you don't want to hide away the opengl logic
	//
	// -- especially in this case as this stuff is volatile and
	// -- changes drastically depending on the project
	//
	// possibly make sprite have its own function which deals with buffer
	// functions directly and also any other rendering task in the future
	//
	//
	// make src_user folder to have code that the user of this engine
	// can create / modify for this exact use case


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

	glBindBuffer(GL_ARRAY_BUFFER, model->instance_UV_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * translation_size, translations, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, model->instance_spr_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * translation_size, spr_num, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, model->instance_UV_VBO);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(0));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, model->instance_spr_VBO);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(0));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);


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

void model_draw(Model *model, unsigned int program) {
	model->uniform.value.m4[3][0] = model->x;
	model->uniform.value.m4[3][1] = model->y;
	model->uniform.value.m4[3][2] = model->z;
	model->uniform.value.m4[3][3] = 1;
	uniform_send_to_gpu(&model->uniform, program, "model");

	glBindTexture(GL_TEXTURE_2D, model->texture);
	glBindVertexArray(model->VAO);
	glDrawElements(GL_TRIANGLES, model->vertex_face_size, GL_UNSIGNED_INT, 0);
}

void model_draw_instanced(Model *model, unsigned int program, unsigned int instance_amount) {
	model->uniform.value.m4[3][0] = model->x;
	model->uniform.value.m4[3][1] = model->y;
	model->uniform.value.m4[3][2] = model->z;
	model->uniform.value.m4[3][3] = 1;
	uniform_send_to_gpu(&model->uniform, program, "model");

	glBindTexture(GL_TEXTURE_2D, model->texture);
	glBindVertexArray(model->VAO);
	glDrawElementsInstanced(GL_TRIANGLES, model->vertex_face_size, GL_UNSIGNED_INT, 0, instance_amount);
}


void model_init_instanced(int instance_amount; jmp_buf error, Model *model, vec3 pos, char *texture_location,
		vec2 instance_array[instance_amount], vec2 spr_num[instance_amount], int instance_amount) {

	unsigned int texture = texture_init(error, GL_RGBA, texture_location);

	mat4 model_matrix;
	glm_mat4_identity(model_matrix);

	model_send_to_gpu_instanced(model, instance_array, spr_num, instance_amount);


	model->uniform = uniform_set_data(model_matrix, UNIFORM_MAT4);
	model->x = pos[0];
	model->y = pos[1];
	model->z = pos[2];
	model->texture = texture;
}

void model_init(jmp_buf error, Model *model, vec3 pos, char *texture_location) {
	unsigned int texture = texture_init(error, GL_RGBA, texture_location);

	mat4 model_matrix;
	glm_mat4_identity(model_matrix);

	model_send_to_gpu(model);
	model->uniform = uniform_set_data(model_matrix, UNIFORM_MAT4);
	model->x = pos[0];
	model->y = pos[1];
	model->z = pos[2];

	model->texture = texture;
}
