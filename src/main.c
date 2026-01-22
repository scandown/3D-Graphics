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
#include "dynamic_array.h"
#include "sprite.h"
#include "input.h"

#define WIDTH 640
#define HEIGHT 360

#define CHAR_WIDTH 80
#define CHAR_HEIGHT 24

void cursor_position_callback(GLFWwindow* window, Camera *cam, float sensitivity);

typedef struct {
	float *items;
	size_t count;
	size_t capacity;
} Array;

int main() {
	jmp_buf error;
	if (setjmp(error)) {
		printf("Error Detected!\n");
		return 1;
	}


	Camera *cam = malloc(sizeof(Camera));
	camera_init(cam, (vec3){0, 0, 1}, 0, -90);
	//setup for opengl :3
	State game = state_init(error, 1920, 1080, "game");


	unsigned int program = program_init(error, "src/vertex_in.glsl", "src/fragment.glsl");



	vec2 instanced_positions[CHAR_WIDTH * CHAR_HEIGHT];
	vec2 instanced_spr_num[CHAR_WIDTH * CHAR_HEIGHT];
	sprite_send_instanced_positions(instanced_positions, instanced_spr_num, CHAR_WIDTH, CHAR_HEIGHT);
	Sprite test = sprite_init(error, (vec3){0, 0, 0}, 1, "assets/smiley.png", instanced_positions, instanced_spr_num, CHAR_WIDTH * CHAR_HEIGHT, 16, 16);



	while (!glfwWindowShouldClose(game.window)) {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "OpenGL error: %d\n", err);
		}



		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        glClearColor(0.1, 0.1, 0.2, 1);
		glfwPollEvents();


		glBindBuffer(GL_ARRAY_BUFFER, test.plane.instance_spr_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec2) * (CHAR_WIDTH * (int)cam->pos[1] + (int)cam->pos[0]), sizeof(vec2), (vec2){1.0, 1.0});
		glBindBuffer(GL_ARRAY_BUFFER, 0);




		cursor_position_callback(game.window, cam, 0.05);
		key_input(game.window, cam);


		glUseProgram(program);
		
		matrix_init(&game, program, "2D");
		camera_rotate(cam, cam->yaw, cam->pitch, game.view_uniform.value.m4);
		uniform_send_to_gpu(&game.view_uniform, program, "view");

		sprite_draw(&test, program, CHAR_WIDTH * CHAR_HEIGHT);

		glfwSwapBuffers(game.window);

	}

	
	program_delete(program);
	sprite_delete(&test);
	free(cam);

	glfwTerminate();
	return 0;
}



void cursor_position_callback(GLFWwindow* window, Camera *cam, float sensitivity) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	double xpos_diff = xpos - cam->prev_xpos;
	double ypos_diff = -(ypos - cam->prev_ypos);

	cam->prev_xpos = xpos;
	cam->prev_ypos = ypos;

	cam->pitch += ypos_diff * sensitivity;
	if (cam->pitch > 89.9) {
		cam->pitch = 89.9;
	} else if (cam->pitch < -89.9) {
		cam->pitch = -89.9;
	}
	cam->yaw += xpos_diff * sensitivity;

}
