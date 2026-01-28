#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <stdlib.h>
#include <setjmp.h>

#include "shader.h"
#include "model.h"
#include "quat.h"
#include "window.h"
#include "state.h"
#include "camera.h"
#include "uniform.h"
#include "texture.h"
#include "sprite.h"

#include "user/input.h"
#include "user/instanced_positions.h"

void cursor_position_callback(GLFWwindow* window, Camera *cam, float sensitivity);


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



	vec2 instanced_positions[1] = {{0, 0}};
	vec2 instanced_spr_num[1] = {{0, 0}};
	Sprite test = sprite_init(error, (vec3){100, 0, 0}, 1,
			"assets/smiley.png", instanced_positions, instanced_spr_num, 1, 16, 16);



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




		cursor_position_callback(game.window, cam, 0.05);
		key_input(game.window, cam);


		glUseProgram(program);
		
		matrix_init(&game, program, "3D");
		camera_rotate(cam, cam->yaw, cam->pitch, game.view_uniform.value.m4);
		uniform_send_to_gpu(&game.view_uniform, program, "view");

		sprite_draw(&test, program, 1);

		glfwSwapBuffers(game.window);

	}

	
	program_delete(program);
	sprite_delete(&test);
	free(cam);

	glfwTerminate();
	return 0;
}
