#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <stdlib.h>

#include "shader.h"
#include "spaces.h"
#include "model.h"


// model
//#include "modely.h"

void cursor_position_callback(GLFWwindow* window, double *prev_xpos, double *prev_ypos, float *yaw, float *pitch, float sensitivity);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
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

	int vnum = 0;
	int fnum = 0;
	float *vtest;
	unsigned int *ftest;

	if (model_load("assets/teapot.obj", &vnum, &fnum, &vtest, &ftest) == -1) {
		return -1;
	}

	//printf("%i\n", fnum);
/*
	for (int i = 0; i < num; i++) {
		printf("%i\n", ftest[i]);
	}
	*/

	bool debug = false;
	bool debug_cube = true;
	//unsigned int teapot_face_length = sizeof(f_teapot) / sizeof(int);
	//printf("f_teapot length = %li\n", teapot_face_length);
	if (debug) {
		printf("vnum = %i, fnum = %i\n", vnum, fnum);
		free(vtest);
		free(ftest);
		return 1;
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(vtest), vtest, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ftest), ftest, GL_STATIC_DRAW);
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


	model.scale(&model, (vec3){1, 1, 1});
	view.translate(&view, (vec3){0, 0, -2});
	glm_perspective(45.0, SCR_WIDTH/SCR_HEIGHT, 0.1, 100, projection.matrix);
	//glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, -1, 100.0f, projection.matrix);
	projection.set_uniform(&projection);









	float scalemin_val = -2;
	float scalemax_val = 2;
	float diff = scalemax_val - scalemin_val;
	glfwSetKeyCallback(window, key_callback);

	vec3 cameraPos   = {0.0f, 0.0f,  3.0f};
	vec3 cameraFront = {0.0f, 0.0f, -1.0f};
	vec3 cameraUp    = {0.0f, 1.0f,  0.0f};

	vec3 cam_total_front = {0};

	float pitch = 0;
	float yaw = -90;

	double prev_xpos = 0;
	double prev_ypos = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//cursor_position_callback(window, &prev_xpos, &prev_ypos, &yaw, &pitch, 0.10);


		const float speed = 0.01;

		vec3 direction;
		direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
		direction[1] = sin(glm_rad(pitch));
		direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
		glm_vec3_normalize_to(direction, cameraFront);

		glm_vec3_add(cameraPos, cameraFront, cam_total_front);
		glm_lookat(cameraPos, cam_total_front, cameraUp, view.matrix);

		// while loop space stuff
		//view.translate(&view, (vec3){x, -y, 0});
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





		int scalemin_loc = glGetUniformLocation(program, "scalemin");
		int scalemax_loc = glGetUniformLocation(program, "scalemax");
		glUniform1f(scalemin_loc, scalemin_val);
		glUniform1f(scalemax_loc, scalemax_val);












		// clearing up and displaying (Important stuff)
	        glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, fnum, GL_UNSIGNED_INT, 0);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
//    if (key == GLFW_KEY_E && action == GLFW_REPEAT)
//        printf("HIII\n");
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

	*yaw += xpos_diff * sensitivity;
	*pitch += ypos_diff * sensitivity;

}

