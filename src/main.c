#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <stdlib.h>

#include "shader.h"
#include "spaces.h"
#include "model.h"
#include "quat.h"
#include "window.h"
#include "state.h"
#include "camera.h"
#include "uniform.h"
//#include "vec3.h"
//#include "quat.h"

void cursor_position_callback(GLFWwindow* window, double *prev_xpos, double *prev_ypos, float *yaw, float *pitch, float sensitivity);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// access camera from input callback
Camera *c_ptr = NULL;

int main() {
	Camera *cam = malloc(sizeof(Camera));
	c_ptr = cam;


	State game;
	game.SCR_WIDTH = 1920;
	game.SCR_HEIGHT = 1080;
	game.title = "game";

	game.window = setup_window(game.SCR_WIDTH, game.SCR_HEIGHT, game.title);
	if (game.window == NULL) {
		return 1;
	}


	// possible model loading class
	int vsize = 0;
	int fsize = 0;
	float *vtest;
	unsigned int *ftest;
	if (model_load("assets/cube.obj", &vsize, &fsize, &vtest, &ftest) == -1) {
		return -1;
	}




	//setup for opengl :3
	// shaders !
	game.program = program_create("src/shaderList.txt");

	// data storage
	unsigned int VAO, VBO, EBO;
	model_send_to_gpu(game.program, &VAO, &VBO, &EBO, vsize, fsize, vtest, ftest);










	// coordinate systems
	struct space model, view, projection;
	setup_space(&model, "model", game.program);
	vec3 model_position = {0, 0, 0};
	model.translate(&model, model_position);
	model.set_uniform(&model);

	setup_space(&view, "view", game.program);
	view.translate(&view, (vec3){0, 0, -2});

	setup_space(&projection, "projection", game.program);
	glm_perspective(45.0, game.SCR_WIDTH/game.SCR_HEIGHT, 0.1, 100, projection.matrix);
	projection.set_uniform(&projection);











	// camera move setup
	glm_vec3_copy((vec3){0, 0, 3}, cam->pos);
	glm_vec3_copy((vec3){0, 0, -1}, cam->front);
	glm_vec3_copy((vec3){0, 1, 0}, cam->up);
	cam->mask1 = 0;


	while (!glfwWindowShouldClose(game.window))
	{
		glfwPollEvents();

		float pitch = 0;
		float yaw = -90;
		double prev_xpos = 0;
		double prev_ypos = 0;
		cursor_position_callback(game.window, &prev_xpos, &prev_ypos, &yaw, &pitch, 0.10);
		glfwSetKeyCallback(game.window, key_callback);



		//camera_look(cam, yaw, pitch, &view);
		vec3 direction;
		direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
		direction[1] = sin(glm_rad(pitch));
		direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
		glm_vec3_normalize_to(direction, cam->front);
		vec3 camera_total_front;
		glm_vec3_copy(cam->front, camera_total_front);
		glm_vec3_add(cam->pos, cam->front, camera_total_front);
		glm_lookat(cam->pos, camera_total_front, cam->up, view.matrix);
		view.set_uniform(&view);






		// input
		camera_movement(cam);
		// end of input

		
		// quaternion stuff
		vec4 result = {1, 0, 0, 0};
		vec4 start = {0, 0, 0, 1};
		vec4 end = {0.5, 0, 0, 0.5};
		static float t = 0;
		quat_slerp(start, end, t, result);

		static float amount = 0.001;
		t += amount;

		if (t >= 1 || t <= 0) {
			amount *= -1;
		}

		Uniform rotation = uniform_init(&game, "rot", UNIFORM_FLOAT4);
		rotation.values.f4[0] = result[0];
		rotation.values.f4[1] = result[1];
		rotation.values.f4[2] = result[2];
		rotation.values.f4[3] = result[3];
		uniform_send(&rotation);
		//int rotloc = glGetUniformLocation(game.program, "rot");
		//glUniform4fv(rotloc, 1, result);


		// end loop

		// clearing up and displaying (Important stuff)
	        glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(game.program);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, fsize, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(game.window);

	}

	// freeing unused stuff at end
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(game.program);


	// free model data
	free(vtest);
	free(ftest);
	free(cam);

	glfwTerminate();
	return 0;
}



void cursor_position_callback(GLFWwindow* window, double *prev_xpos, double *prev_ypos, float *yaw, float *pitch, float sensitivity) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	double xpos_diff = xpos - *prev_xpos;
	double ypos_diff = -(ypos - *prev_ypos);

	*prev_xpos = xpos;
	*prev_ypos = ypos;

	*pitch += ypos_diff * sensitivity;
	if (*pitch > 89.9) {
		*pitch = 89.9;
	} else if (*pitch < -89.9) {
		*pitch = -89.9;
	}
	*yaw += xpos_diff * sensitivity;

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

	Camera *cam = c_ptr;

	if (action == GLFW_PRESS) {
		get_mask(key, cam, key_or);
	} else if (action == GLFW_RELEASE) {
		get_mask(key, cam, key_not);
	}
}

