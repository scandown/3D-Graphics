#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <stdlib.h>

#include "shader.h"
#include "spaces.h"
#include "model.h"
//#include "vec3.h"
//#include "quat.h"
//
//

void quat_mul(vec4 q1, vec4 q2, vec4 out) {
	vec3 q1_i = {q1[1], q1[2], q1[3]};
	vec3 q2_i = {q2[1], q2[2], q2[3]};

	//float scalar = q1.w * q2.w - glm_vec3_dot(q1_i, q2_i);
	float scalar = q1[0] * q2[0] - glm_vec3_dot(q1_i, q2_i);

	vec3 qm;
	vec3 qm2;
	vec3 qc;
	glm_vec3_scale(q2_i, q1[0], qm);
	glm_vec3_scale(q1_i, q2[0], qm2);
	glm_vec3_add(qm, qm2, qm);
	glm_vec3_cross(q1_i, q2_i, qc);
	vec3 imaginary;// = qm + cross(q1_i, q2_i);
	glm_vec3_add(qm, qc, imaginary);

	out = (vec4){scalar, imaginary[0], imaginary[1], imaginary[2]};
}

void quat_rotate_vector(float theta, vec3 axis, vec4 out) {
	float ca = cos(theta/2.0);
	float sa = sin(theta/2.0);

	glm_normalize(axis);

	vec4 q = {ca, sa * axis[0], sa * axis[1], sa * axis[2]};
	glm_vec3_copy(q, out);
	//out = q;
}








// model
#include "modely.h"

void cursor_position_callback(GLFWwindow* window, double *prev_xpos, double *prev_ypos, float *yaw, float *pitch, float sensitivity);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() {

	if (!glfwInit()) {
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	int vsize = 0;
	int fsize = 0;
	float *vtest;
	unsigned int *ftest;


	if (model_load("assets/cube.obj", &vsize, &fsize, &vtest, &ftest) == -1) {
		return -1;
	}
	
	unsigned int teapot_face_length = sizeof(f_teapot) / sizeof(int);
	unsigned int teapot_vertex_length = sizeof(v_teapot) / sizeof(float);
	//int wrong = check_int_equality(ftest, fsize, f_teapot, teapot_face_length);

	//wrong = check_float_equality(vtest, vsize, v_teapot, teapot_vertex_length);

	bool debug = false;
	bool debug_cube = true;
	bool debug_cube_header = false;
	if (debug) {
		printf("vsize = %i, fsize = %i\n", vsize, fsize);
		free(vtest);
		free(ftest);
		return 0;
	}


	//setup for opengl :3
	const char *fragmentShaderSource = loadShader("src/red.glsl");
	const char *vertexShaderSource = loadShader("src/vertex.glsl");

	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);

	int fragment_success = shaderErrorCheck(fragmentShader);
	int vertex_success = shaderErrorCheck(vertexShader);

	if (!(fragment_success || vertex_success)) {
		return 1;
	}









	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float v_cube[] = {
		// front
		-1, -1, -1, // -1
		1, -1, -1,  // 1
		1, 1, -1,   // 2
		-1, 1, -1,  // 3

		//back
		-1, -1, 1, // 4
		1, -1, 1,  // 5
		1, 1, 1,   // 6
		-1, 1, 1,  // 7
	};

	unsigned int i_cube[] = {
		// front
		0, 1, 2,
		0, 3, 2,

		//side r
		2, 6, 5,
		2, 1, 5,

		// back
		4, 5, 6,
		4, 7, 6,

		// side l
		0, 4, 7,
		0, 3, 7,

		// top
		3, 2, 6,
		3, 7, 6,

		// bottom
		0, 1, 5,
		0, 4, 5
	};

	float vertices[] = {
		-1,  -1, 0,
		1, -1, 0,
		1, 1, 0,
		-1,  1, 0
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,
		0, 3, 2
	};


	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);


	if (debug_cube) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(float), vtest, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, fsize * sizeof(int), ftest, GL_STATIC_DRAW);
	} else if (debug_cube_header) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v_teapot), v_teapot, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(f_teapot), f_teapot, GL_STATIC_DRAW);
	} else {

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v_cube), v_cube, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i_cube), i_cube, GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(program);






	// coordinate systems
	struct space model;
	setup_space(&model, "model", program);
	struct space view;
	setup_space(&view, "view", program);
	struct space projection;
	setup_space(&projection, "projection", program);

	vec3 model_position = {0, 0, 0};

	model.translate(&model, model_position);
	view.translate(&view, (vec3){0, 0, -2});
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

		view.set_uniform(&view);
		model.set_uniform(&model);






		// input

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



		int timeLoc = glGetUniformLocation(program, "time");
		//printf("%f\n", time);
		glUniform1f(timeLoc, time);




		vec4 acc_rot = {1, 0, 0, 0};
		quat_rotate_vector(time, (vec3){1, 0, 0}, acc_rot);

		//printf("%f, %f, %f\n", acc_rot[1], acc_rot[2], acc_rot[3]);
		int rotloc = glGetUniformLocation(program, "rot");
		glUniform4fv(rotloc, 1, acc_rot);










		// clearing up and displaying (Important stuff)
	        glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, fsize, GL_UNSIGNED_INT, 0);

		struct space model2;
		setup_space(&model2, "model", program);
		vec3 model2_position;// = old_cameraPosition;//{3, 0, 0};
		glm_vec3_copy(old_cameraPos, model2_position);
		model2_position[1] -= 4;
		model.translate(&model2, model2_position);
		model2.set_uniform(&model2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, fsize, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);

	}

	// freeing unused stuff at end
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(program);

	free((void *)fragmentShaderSource);
	free((void *)vertexShaderSource);

	// free model data
	free(vtest);
	free(ftest);

	glfwTerminate();
	return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

