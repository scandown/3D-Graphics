#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include <stdlib.h>

#include "shader.h"

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





	//setup for opengl :3
	const char *fragmentShaderSource = loadShader("src/mandlebrot2.glsl");
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


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(program);






	// coordinate systems


	mat4 model;
	mat4 view;
	mat4 projection;

	glm_mat4_identity(model);
	glm_mat4_identity(view);
	glm_mat4_identity(projection);

	int model_loc = glGetUniformLocation(program, "model");
	int view_loc = glGetUniformLocation(program, "view");
	int projection_loc = glGetUniformLocation(program, "projection");



	vec3 scale = {800, 600, 1};
	glm_scale(model, scale);


	vec3 translation = {250, 250, 0};
	glm_translate(view, translation);


	glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, -1, 100.0f, projection);
	glUniformMatrix4fv(projection_loc, 1, false, (const float *)projection);





	int time_loc = glGetUniformLocation(program, "time");
	float x = 0;
	float y = 0;

	float scalex = 0;
	float scaley = 0;

	float scalemin_val = -2;
	float scalemax_val = 2;
	glfwSetKeyCallback(window, key_callback);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int present = glfwJoystickPresent(GLFW_JOYSTICK_2);

		float diff = scalemax_val - scalemin_val;


		present = 0;
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

		if (present) {
			int axesCount;
			const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axesCount);

			x = axes[0];
			y = axes[1];

			scalex = axes[3];
			scaley = axes[4];
			//printf("x: %f, y: %f\n", scalex, scaley);

		}

		vec3 translation = {x, -y, 0};
		glm_translate(view, translation);
		glUniformMatrix4fv(view_loc, 1, false, (const float *)view);



		glUniformMatrix4fv(model_loc, 1, false, (const float *)model);



		int point_loc = glGetUniformLocation(program, "point");

		glUniform2f(point_loc, scalex * 100, scaley * -100);


		//uniform float scalemin;
		//uniform float scalemax;

		int scalemin_loc = glGetUniformLocation(program, "scalemin");
		int scalemax_loc = glGetUniformLocation(program, "scalemax");
		glUniform1f(scalemin_loc, scalemin_val);
		glUniform1f(scalemax_loc, scalemax_val);


		glUniform1f(time_loc, x);
	        glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(program);

	free((void *)fragmentShaderSource);
	free((void *)vertexShaderSource);

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
