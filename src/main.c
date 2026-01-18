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

#define WIDTH 640
#define HEIGHT 360

#define CHAR_WIDTH 80
#define CHAR_HEIGHT 24

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
	jmp_buf error;
	if (setjmp(error)) {
		printf("Error Detected!\n");
		return 1;
	}


	Camera *cam = malloc(sizeof(Camera));
	camera_init(cam, (vec3){0, 0, 1}, 0, -90);
	c_ptr = cam;





	//setup for opengl :3
	State game = state_init(error, 1920, 1080, "game");


	unsigned int vertex_shader = shader_create("src/vertex_in.glsl", GL_VERTEX_SHADER);
	unsigned int fragment_shader = shader_create("src/textured.glsl", GL_FRAGMENT_SHADER);

	if (vertex_shader == 0 || fragment_shader == 0) {
		longjmp(error, 1);
	}

	unsigned int program = program_create(vertex_shader, fragment_shader);


	vec2 instanced_positions[CHAR_WIDTH * CHAR_HEIGHT];
	vec2 instanced_spr_num[CHAR_WIDTH * CHAR_HEIGHT];
	int index = 0;
	for (int y = 0; y < CHAR_HEIGHT; y++) {
		for (int x = 0; x < CHAR_WIDTH; x++) {
			vec2 translation;
			translation[0] = x * (WIDTH / CHAR_WIDTH);
			translation[1] = y * (HEIGHT / CHAR_HEIGHT);

			vec2 spr_num;
			spr_num[0] = x;
			spr_num[1] = y;

			glm_vec2_copy(translation, instanced_positions[index]);
			glm_vec2_copy(spr_num, instanced_spr_num[index]);
			index++;
		}
	}



	Sprite test = sprite_init(error, (vec3){0, 0, 0}, 1, "assets/smiley.png", instanced_positions, instanced_spr_num, CHAR_WIDTH * CHAR_HEIGHT, 8, 15);

	//glBindBuffer(GL_ARRAY_BUFFER, model->instance_spr_VBO);



		float b[2] = {0.0, 0.0};
	while (!glfwWindowShouldClose(game.window)) {
		glBindBuffer(GL_ARRAY_BUFFER, test.plane.instance_spr_VBO);
		b[0] += 0.01;
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec2) * 10, sizeof(vec2), b);


	        glClearColor(0.1, 0.1, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "OpenGL error: %d\n", err);
		}


		glfwPollEvents();




		glfwSetKeyCallback(game.window, key_callback);
		key_input(cam);
		cursor_position_callback(game.window, cam, 0.05);


		glUseProgram(program);
		
		matrix_init(&game, program, "2D");
		camera_rotate(cam, cam->yaw, cam->pitch, game.view_uniform.value.m4);
		uniform_send_to_gpu(&game.view_uniform, program, "view");

		sprite_draw(&test, program, CHAR_WIDTH * CHAR_HEIGHT);

		glfwSwapBuffers(game.window);


		static float b = 0.1;
		Uniform yes = uniform_set_data(&b, UNIFORM_FLOAT1);
		uniform_send_to_gpu(&yes, program, "yes");
		b += 0.01;
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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

	Camera *cam = c_ptr;

	if (action == GLFW_PRESS) {
		set_input_mask(key, cam, key_or);
	} else if (action == GLFW_RELEASE) {
		set_input_mask(key, cam, key_not);
	}
}

