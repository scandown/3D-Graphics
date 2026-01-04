#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <stdlib.h>
#include <setjmp.h>

#include "shader.h"
#include "spaces.h"
#include "model.h"
#include "quat.h"
#include "window.h"
#include "state.h"
#include "camera.h"
#include "uniform.h"
#include "texture.h"
#include "dynamic_array.h"
#include "sprite.h"

void cursor_position_callback(GLFWwindow* window, Camera *cam, float sensitivity);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

typedef struct {
	float *items;
	size_t count;
	size_t capacity;
} Array;



// access camera from input callback
Camera *c_ptr = NULL;

int main() {
	Camera *cam = malloc(sizeof(Camera));
	c_ptr = cam;
	//setup for opengl :3
	

	jmp_buf error;
	if (setjmp(error)) {
		printf("Error Detected!\n");
		return 1;
	}

	State game = setup_state(error, 1920, 1080, "game", "src/light_shaderlist.txt");
	Sprite test = load_sprite(error, (vec3){320, 180, 1}, 8, "assets/smiley.png");

	glUseProgram(test.program);
	setup_scene(&game, test.program, "2D", (vec3){0, 0, -2});

	// camera move setup
	glm_vec3_copy((vec3){0, 0, 20}, cam->pos);
	glm_vec3_copy((vec3){0, 0, -1}, cam->front);
	glm_vec3_copy((vec3){0, 1, 0}, cam->up);
	cam->mask1 = 0;


	while (!glfwWindowShouldClose(game.window)) {
		// clearing up and displaying (Important stuff)
	        glClearColor(0.1, 0.1, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glfwPollEvents();
		cam->pitch = 0;
		cam->yaw = -90;
		cam->prev_xpos = 0;
		cam->prev_ypos = 0;
		camera_look(cam, cam->yaw, cam->pitch, game.view_uniform.value.m4, &game.view_uniform, test.program);
		uniform_send(&game.view_uniform);



		glfwSetKeyCallback(game.window, key_callback);
		camera_movement(cam);




		draw_sprite(&test);

		glfwSwapBuffers(game.window);

	}

	// freeing unused stuff at end
	glDeleteProgram(game.program);
	glDeleteProgram(test.program);

	delete_sprite(&test);
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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

	Camera *cam = c_ptr;

	if (action == GLFW_PRESS) {
		set_input_mask(key, cam, key_or);
	} else if (action == GLFW_RELEASE) {
		set_input_mask(key, cam, key_not);
	}
}

