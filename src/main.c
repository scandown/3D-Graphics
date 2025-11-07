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
//#include "vec3.h"
//#include "quat.h"

void cursor_position_callback(GLFWwindow* window, double *prev_xpos, double *prev_ypos, float *yaw, float *pitch, float sensitivity);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


int main() {

	GLFWwindow *window = setup_window(SCR_WIDTH, SCR_HEIGHT, "game");
	if (window == NULL) {
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
	unsigned int program = program_create("src/shaderList.txt");

	// data storage
	unsigned int VAO, VBO, EBO;
	model_send_to_gpu(program, &VAO, &VBO, &EBO, vsize, fsize, vtest, ftest);










	// coordinate systems
	struct space model;
	setup_space(&model, "model", program);
	struct space view;
	setup_space(&view, "view", program);
	struct space projection;
	setup_space(&projection, "projection", program);

	vec3 model_position = {0, 0, 0};

	view.translate(&view, (vec3){0, 0, -2});
	model.translate(&model, model_position);
	glm_perspective(45.0, SCR_WIDTH/SCR_HEIGHT, 0.1, 100, projection.matrix);
	//glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, -1, 100.0f, projection.matrix);
	projection.set_uniform(&projection);










	float scalemin_val = -2;
	float scalemax_val = 2;
	float diff = scalemax_val - scalemin_val;

	vec3 cameraPos   = {0.0f, 0.0f,  3.0f};
	vec3 cameraFront = {0.0f, 0.0f, -1.0f};
	vec3 cameraUp    = {0.0f, 1.0f,  0.0f};

	vec3 cam_total_front = {0};

	float pitch = 0;
	float yaw = -90;

	double prev_xpos = 0;
	double prev_ypos = 0;
	
	float x = 0;
	float y = 0;
	float z = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		cursor_position_callback(window, &prev_xpos, &prev_ypos, &yaw, &pitch, 0.10);


		const float speed = 0.01;

		vec3 direction;
		direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
		direction[1] = sin(glm_rad(pitch));
		direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
		glm_vec3_normalize_to(direction, cameraFront);

		glm_vec3_add(cameraPos, cameraFront, cam_total_front);
		glm_lookat(cameraPos, cam_total_front, cameraUp, view.matrix);

		vec3 c_up = {0, 1, 0};
		vec3 cam_right;
		glm_vec3_cross(c_up, direction, cam_right);
		//lookat_cube(model.matrix, direction, model_position, (vec3){0, 1, 0});
		//glm_mat4_print(model.matrix, stdout);

		view.set_uniform(&view);
		model.set_uniform(&model);






		// input
		// use better system where all the inputs are inside one function

		int state = glfwGetKey(window, GLFW_KEY_E);
		if (state == GLFW_PRESS)
		{
			scalemin_val += diff / 100;	
		}
		state = glfwGetKey(window, GLFW_KEY_R);
		if (state == GLFW_PRESS)
		{
			scalemax_val -= diff / 100;	
		}

		vec3 old_cameraPos;
		glm_vec3_copy(cameraPos, old_cameraPos);

		float cameraSpeed = 0.05f;
		state = glfwGetKey(window, GLFW_KEY_W);
		if (state == GLFW_PRESS) {
			vec3 cameraMove;
			glm_vec3_scale(cameraFront, cameraSpeed, cameraMove);
			glm_vec3_add(cameraPos, cameraMove, cameraPos);
		}
		state = glfwGetKey(window, GLFW_KEY_A);
		if (state == GLFW_PRESS) {
		//		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			vec3 cameraRight;
			glm_vec3_cross(cameraFront, cameraUp, cameraRight);
			glm_vec3_normalize(cameraRight);
			glm_vec3_scale(cameraRight, cameraSpeed, cameraRight);

			glm_vec3_sub(cameraPos, cameraRight, cameraPos);
		}
		state = glfwGetKey(window, GLFW_KEY_S);
		if (state == GLFW_PRESS) {
			vec3 cameraMove;
			glm_vec3_scale(cameraFront, cameraSpeed, cameraMove);
			glm_vec3_sub(cameraPos, cameraMove, cameraPos);
		}
		state = glfwGetKey(window, GLFW_KEY_D);
		if (state == GLFW_PRESS) {
			vec3 cameraRight;
			glm_vec3_cross(cameraFront, cameraUp, cameraRight);
			glm_vec3_normalize(cameraRight);
			glm_vec3_scale(cameraRight, cameraSpeed, cameraRight);

			glm_vec3_add(cameraPos, cameraRight, cameraPos);
		}
		state = glfwGetKey(window, GLFW_KEY_SPACE);
		if (state == GLFW_PRESS)
		{
			cameraPos[1] += cameraSpeed;
		}
		state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		if (state == GLFW_PRESS)
		{
			cameraPos[1] -= cameraSpeed;
		}








		int scalemin_loc = glGetUniformLocation(program, "scalemin");
		int scalemax_loc = glGetUniformLocation(program, "scalemax");
		glUniform1f(scalemin_loc, scalemin_val);
		glUniform1f(scalemax_loc, scalemax_val);


		float time = glfwGetTime();

		int timeLoc = glGetUniformLocation(program, "time");
		//printf("%f\n", time);
		glUniform1f(timeLoc, time);




		// quat_slerp(vec4 start, vec4 end, float t, vec4 out);

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


		mat4 m; 
		glm_mat4_identity(m);

		//glm_vec3_copy(cameraPos, to);
		static vec3 forward = {0, 0, 1};
		vec3 up = {0, 1, 0};

		vec4 quat;



		//glm_mat4_print(m, stdout);

		//print_quat(result);

		int matloc = glGetUniformLocation(program, "mat");
		glUniformMatrix4fv(matloc, 1, false, (const float *)m);

		//printf("%f, %f, %f, %f\n", result[0], result[1], result[2], result[3]);
		int rotloc = glGetUniformLocation(program, "rot");
		glUniform4fv(rotloc, 1, result);

		//print_quat(result);










		// clearing up and displaying (Important stuff)
	        glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, fsize, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);

	}

	// freeing unused stuff at end
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(program);

	/*
	free((void *)fragmentShaderSource);
	free((void *)vertexShaderSource);
	*/

	// free model data
	free(vtest);
	free(ftest);

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

