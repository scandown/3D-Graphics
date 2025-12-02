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

void cursor_position_callback(GLFWwindow* window, Camera *cam, float sensitivity);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



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

	State game = setup_state(error, 1920, 1080, "game", "src/shaderList.txt");
	Model cube = model_load(error, "assets/pen.obj");


	unsigned int texture = texture_setup(error, GL_RGB, "assets/wall.jpg");
	

	glUseProgram(game.program);



	// coordinate systems
	// 
	Space model, view, projection;
	setup_space(&model, "model", game.program);
	vec3 model_position = {0, 0, 0};
	model.translate(&model, model_position);
	model.set_uniform(&model);

	setup_space(&view, "view", game.program);
	view.translate(&view, (vec3){0, 0, -2});

	setup_space(&projection, "projection", game.program);
	glm_perspective(glm_rad(70), game.SCR_WIDTH/game.SCR_HEIGHT, 0.1, 100, projection.matrix);
	projection.set_uniform(&projection);

	// camera move setup
	glm_vec3_copy((vec3){0, 0, 3}, cam->pos);
	glm_vec3_copy((vec3){0, 0, -1}, cam->front);
	glm_vec3_copy((vec3){0, 1, 0}, cam->up);
	cam->mask1 = 0;


	while (!glfwWindowShouldClose(game.window))
	{
		glfwPollEvents();

		cam->pitch = 0;
		cam->yaw = -90;
		cam->prev_xpos = 0;
		cam->prev_ypos = 0;
		cursor_position_callback(game.window, cam, 0.10);
		camera_look(cam, cam->yaw, cam->pitch, &view);

		glfwSetKeyCallback(game.window, key_callback);
		camera_movement(cam);


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
		Uniform rotation = uniform_init(&game, "rot", &result, UNIFORM_FLOAT4);
		uniform_send(&rotation);

		// clearing up and displaying (Important stuff)
	        glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// draw call
		model_send_to_gpu(&game, &cube);
		glUseProgram(game.program);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(game.VAO);
		glDrawElements(GL_TRIANGLES, cube.vertex_face_size, GL_UNSIGNED_INT, 0);



		glfwSwapBuffers(game.window);

	}

	// freeing unused stuff at end
	glDeleteVertexArrays(1, &game.VAO);
	glDeleteBuffers(1, &game.VBO);
	glDeleteBuffers(1, &game.EBO);

	glDeleteProgram(game.program);

	free(cube.vertices);
	free(cube.vertex_faces);


	// free model data
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

