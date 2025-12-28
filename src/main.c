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

	State game = setup_state(error, 1920, 1080, "game", "src/shaderList.txt");
	unsigned int texture = texture_setup(error, GL_RGB, "assets/wall.jpg");
	unsigned int light_program = program_create("src/shaderList.txt");
	
	Model cube = model_load(error, "assets/cube.obj");
	create_buffers(&cube);
	model_send_to_gpu(&cube);




	//Sprite test2 = load_sprite(error, (vec3){200, 200, 5}, 1, "assets/wall.jpg");
	Sprite test = load_sprite(error, (vec3){320, 180, 1}, 8, "assets/smiley.png");


	mat4 view_matrix;
	mat4 projection_matrix;
	mat4 model_matrix;
	glm_mat4_identity(view_matrix);
	glm_mat4_identity(model_matrix);
	glm_mat4_identity(projection_matrix);

	Uniform projection_uniform;
	Uniform view_uniform;
	Uniform model_uniform;


	view_uniform = uniform_init(test.program, "view", view_matrix, UNIFORM_MAT4);
	model_uniform = uniform_init(test.program, "model", model_matrix, UNIFORM_MAT4);
	projection_uniform = uniform_init(test.program, "projection", projection_matrix, UNIFORM_MAT4);
	uniform_send(&model_uniform);
	uniform_send(&view_uniform);

	glm_translate(view_matrix, (vec3){0, 0, -2});


	// camera move setup
	glm_vec3_copy((vec3){0, 0, 20}, cam->pos);
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
		camera_look(cam, cam->yaw, cam->pitch, view_matrix, &view_uniform, test.program);

		uniform_send(&view_uniform);



		glfwSetKeyCallback(game.window, key_callback);
		camera_movement(cam);






		// clearing up and displaying (Important stuff)
	        glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//glm_perspective(glm_rad(70), 16.0/9.0, 0.1, 1000, projection.matrix);
		//glm_ortho_default(16.0/9.0, projection.matrix);
		glm_ortho(0, 640, 0, 360, 0.1, 1000, projection_matrix);






		glUseProgram(test.program);

		projection_uniform = uniform_init(test.program, "projection", projection_matrix, UNIFORM_MAT4);
		uniform_send(&projection_uniform);


		vec4 rot = {0, 1, 0, 1};

		draw_sprite(&test);
		// Add this debug code temporarily
		//draw_sprite(&test2);
		test.x+= 0.1;
		//
		model_matrix[3][0] = 1;
		model_matrix[3][1] = -1;
		model_matrix[3][2] = 10;
		model_matrix[3][3] = 1;

		model_uniform = uniform_init(test.program, "model", model_matrix, UNIFORM_MAT4);
		uniform_send(&model_uniform);

		glm_perspective(glm_rad(70), 16.0/9.0, 0.1, 1000, projection_matrix);
		projection_uniform = uniform_init(test.program, "projection", projection_matrix, UNIFORM_MAT4);
		uniform_send(&projection_uniform);
		//projection.set_uniform(&projection);
		glBindVertexArray(cube.VAO);
		glDrawElements(GL_TRIANGLES, cube.vertex_face_size, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(game.window);

	}
	delete_buffers(&test.plane);
	delete_buffers(&cube);

	// freeing unused stuff at end
	glDeleteProgram(game.program);

	free(test.plane.vertices);
	free(test.plane.vertex_faces);


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

