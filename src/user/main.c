#include <stdio.h>
#include <stdlib.h>

#include "engine.h"

#define num_inst 3


int main() {
	jmp_buf error;
	if (setjmp(error)) {
		printf("Error Detected!\n");
		return 1;
	}


	Camera *cam = malloc(sizeof(Camera));
	camera_init(cam, (vec3){0, 0, 1}, 0, -90);
	State game = state_init(error, 1920, 1080, "game");


	unsigned int program = program_init(error, "src/user/vertex_in.glsl", "src/user/textured.glsl");



	vec2 instanced_positions[num_inst] = {{0, 0}, {16, 0}, {32, 0}};
	vec2 instanced_spr_num[num_inst] = {{0, 0}, {0, 0}, {0, 1}};
	Sprite test = sprite_init(error, (vec3){100, 0, 0}, 1,
			"assets/smiley.png", instanced_positions, instanced_spr_num, num_inst, 16, 16);



	float yes[2] = {10, 100};
	while (!glfwWindowShouldClose(game.window)) {
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

		key_input(game.window, cam);
		
		matrix_init(&game, program, "2D");
		camera_rotate(cam, cam->yaw, cam->pitch, game.view_uniform.value.m4);
		uniform_send_to_gpu(&game.view_uniform, program, "view");


		sprite_draw(&test, program, num_inst);

		glfwSwapBuffers(game.window);

	}

	
	glDeleteProgram(program);
	sprite_delete(&test);
	free(cam);

	glfwTerminate();
	return 0;
}
