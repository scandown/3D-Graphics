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
	jmp_buf error;
	if (setjmp(error)) {
		printf("Error Detected!\n");
		return 1;
	}


	Camera *cam = malloc(sizeof(Camera));
	camera_setup(cam, (vec3){0, 0, 1}, 0, -90);
	c_ptr = cam;





	//setup for opengl :3
	State game = setup_state(error, 1920, 1080, "game");


	unsigned int vertex_shader = create_shader("src/vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fragment_shader = create_shader("src/textured.glsl", GL_FRAGMENT_SHADER);

	if (vertex_shader == 0 || fragment_shader == 0) {
		longjmp(error, 1);
	}

	unsigned int program = create_program(vertex_shader, fragment_shader);
	glUseProgram(program);




	Model cube = load_model(error, "assets/cube.obj");
	create_buffers(&cube);
	send_model_to_gpu(&cube);

	setup_scene(&game, program, "3D");
	Sprite test = load_sprite(error, program, (vec3){0, 0, 0}, 8, "assets/smiley.png");




	while (!glfwWindowShouldClose(game.window)) {
	        glClearColor(0.1, 0.1, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glfwPollEvents();
		camera_look(cam, cam->yaw, cam->pitch, game.view_uniform.value.m4, &game.view_uniform, program);
		uniform_send(&game.view_uniform);



		glfwSetKeyCallback(game.window, key_callback);
		camera_movement(cam);
		cursor_position_callback(game.window, cam, 0.05);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "OpenGL error: %d\n", err);
		}




		draw_sprite(&test);

		game.model_uniform.value.m4[3][0] = 0;
		game.model_uniform.value.m4[3][1] = 0;
		game.model_uniform.value.m4[3][2] = 0;
		game.model_uniform.value.m4[3][3] = 1;
		game.model_uniform = uniform_init(program, "model", game.model_uniform.value.m4, UNIFORM_MAT4);
		uniform_send(&game.model_uniform);

		glBindVertexArray(cube.VAO);
		glDrawElements(GL_TRIANGLES, cube.vertex_face_size, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(game.window);

	}

	
	delete_program(program);
	delete_buffers(&cube);

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

