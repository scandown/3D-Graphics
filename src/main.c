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
	unsigned int light_program = program_create("src/light_shaderlist.txt");
	


	Sprite test = load_sprite(error, (vec3){-3, -2, 0}, "assets/wall.jpg");
	Sprite test2 = load_sprite(error, (vec3){3, -2, 0}, "assets/wall.jpg");

	// coordinate systems
	// 
	// setup_spaces(&model, &view, &projection);
	Space model, view, projection;
	glm_mat4_identity(view.matrix);
	glm_mat4_identity(model.matrix);
	glm_mat4_identity(projection.matrix);
	setup_space(&model, "model", game.program);

	setup_space(&view, "view", game.program);
	view.translate(&view, (vec3){0, 0, -2});

	setup_space(&projection, "projection", game.program);

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

		const int ground_level = 0;
		static float vel = 0;
		if (cam->pos[1] <= ground_level) {
			cam->pos[1] = ground_level;
			vel = 0;
		}

		if (cam->pos[1] > ground_level) {
			cam->pos[1] -= vel;
			vel += 0.0001;
		}
		glfwSetKeyCallback(game.window, key_callback);
		camera_movement(cam);






		// clearing up and displaying (Important stuff)
	        glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glm_perspective(glm_rad(70), 16.0/9.0, 0.1, 1000, projection.matrix);
		projection.set_uniform(&projection);
		setup_space(&projection, "projection", light_program);






		glUseProgram(test.program);


		vec4 rot = {0, 1, 0, 1};

		draw_sprite(&test);
		draw_sprite(&test2);
		test.x+= 0.1;


		glfwSwapBuffers(game.window);

	}
	delete_buffers(&test.plane);

	// freeing unused stuff at end
	glDeleteProgram(game.program);

	free(test.plane.vertices);
	free(test.plane.vertex_faces);

	free(test2.plane.vertices);
	free(test2.plane.vertex_faces);


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

