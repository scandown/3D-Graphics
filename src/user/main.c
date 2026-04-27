#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "engine.h"

#include "user/input.h"
#include "user/instanced_positions.h"

#define num_inst 3


int main() {
	jmp_buf error;
	if (setjmp(error)) {
		printf("Error Detected!\n");
		return 1;
	}


	Camera *cam = malloc(sizeof(Camera));
	camera_init(cam, (vec3){0, 0, 1}, 0, 270);


	GLFWwindow *window = window_init(640, 360, "game");

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	unsigned int program = program_init(error, "src/user/vertex_in.glsl", "src/user/red.glsl");
	unsigned int program3D = program_init(error, "src/user/vertex.glsl", "src/user/red.glsl");



	vec3 instanced_positions[num_inst] = {{0, 0, 0}, {16, 0, 0}, {32, 0, 0}};
	vec2 instanced_spr_num[num_inst] = {{0, 0}, {0, 0}, {0, 1}};

	Model rocky = obj_load(error, "assets/cube.obj");
	clock_t t;
	t = clock();
	//Model rocky = obj_load(error, "../projects/models/rocky.obj");
	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC;
	printf("Time elapsed: %f\n", time_taken);
	//return 1;
	model_init(error, &rocky, (vec3){0, 0, 0}, "assets/smiley.png");
	buffers_init(&rocky);


	float yes[2] = {10, 100};
	while (!glfwWindowShouldClose(window)) {


		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "OpenGL error: %d\n", err);
		}



		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        glClearColor(0.1, 0.1, 0.2, 1);
		glfwPollEvents();


		glUseProgram(program3D);
		key_input(window, cam, 0.05);
		matrix_init(cam, program3D, "3D", 640, 360);
		cursor_position_callback(window, cam, 0.05);
		camera_rotate(cam, cam->yaw, cam->pitch);
		
		uniform_send_to_gpu(&cam->view_uniform, program3D, "view");

		static float angle = 0;
		angle += 0.01;
		Uniform angle_uniform = uniform_set_data(&angle, UNIFORM_FLOAT1);
		uniform_send_to_gpu(&angle_uniform, program3D, "angle");



		model_draw(&rocky, program3D, 1);

		glfwSwapBuffers(window);

	}

	
	model_delete_buffers(&rocky);
	glDeleteProgram(program);
	glDeleteProgram(program3D);
	free(cam);

	glfwTerminate();
	return 0;
}
