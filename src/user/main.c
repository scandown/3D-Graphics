#include <stdio.h>
#include <stdlib.h>

#include "engine.h"

#include "user/input.h"
#include "user/instanced_positions.h"

#define num_inst 3


int main() {
	jmp_buf error;
	if (setjmp(error)) {
		printf("Error Detected!\n");
		return 1;
	}


	Camera *cam = malloc(sizeof(Camera));
	camera_init(cam, (vec3){0, 0, 1}, 0, 270);


	GLFWwindow *window = window_init(640, 360, "game");

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	unsigned int program = program_init(error, "src/user/vertex_in.glsl", "src/user/red.glsl");
	unsigned int program3D = program_init(error, "src/user/vertex.glsl", "src/user/red.glsl");



	vec2 instanced_positions[num_inst] = {{0, 0}, {16, 0}, {32, 0}};
	vec2 instanced_spr_num[num_inst] = {{0, 0}, {0, 0}, {0, 1}};

	Sprite test = sprite_init(error, (vec3){100, 0, 0}, 1, "assets/smiley.png", 16, 16);
	buffers_init(&test.plane);
	instanced_buffers_init(&test.plane, instanced_positions, instanced_spr_num, num_inst, true);

	Model rocky = obj_load(error, "assets/cube.obj");
	//Model rocky = obj_load(error, "../projects/models/rocky.obj");
	model_init(error, &rocky, (vec3){-1, 0, 0}, "assets/smiley.png");
	buffers_init(&rocky);


	float yes[2] = {10, 100};
	while (!glfwWindowShouldClose(window)) {
		test.plane.x = cam->pos[0] * 0;
		test.plane.y = cam->pos[1] * 0;
		test.plane.z = 0;


		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "OpenGL error: %d\n", err);
		}



		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        glClearColor(0.1, 0.1, 0.2, 1);
		glfwPollEvents();


		glBindBuffer(GL_ARRAY_BUFFER, test.plane.instance_UV_VBO);
		yes[0] += 0.1;
		glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(float) * 2, sizeof(float) * 2, yes);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glUseProgram(program);

		key_input(window, cam, 0.05);
		cursor_position_callback(window, cam, 0.05);
		
		matrix_init(cam, program, "3D", 640, 360);
		camera_rotate(cam, cam->yaw, cam->pitch);
		uniform_send_to_gpu(&cam->view_uniform, program, "view");


		sprite_draw(&test, program, num_inst);

		glUseProgram(program3D);
		key_input(window, cam, 0.05);
		matrix_init(cam, program3D, "3D", 640, 360);
		cursor_position_callback(window, cam, 0.05);
		camera_rotate(cam, cam->yaw, cam->pitch);
		
		uniform_send_to_gpu(&cam->view_uniform, program3D, "view");



		model_draw(&rocky, program3D, 1);

		glfwSwapBuffers(window);

	}

	
	model_delete_buffers(&rocky);
	glDeleteProgram(program);
	glDeleteProgram(program3D);
	sprite_delete(&test);
	free(cam);

	glfwTerminate();
	return 0;
}
