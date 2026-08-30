#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "engine.h"

#define RGFW_OPENGL
#define RGFW_IMPORT
#include "RGFW.h"

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


	RGFW_window *window = window_init(1366, 768, "game");
	if (window == NULL) {
		fprintf(stderr, "Error: Couldn't create window\n");
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	RGFW_window_showMouse(window, false);
	RGFW_window_captureRawMouse(window, RGFW_TRUE);

	unsigned int program = program_init(error, "src/user/vertex_in.glsl", "src/user/textured.glsl");
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

	Sprite spr = sprite_init(error, 1, "assets/smiley.png", 16, 16);
	buffers_gen_and_init(&spr.plane);
	instanced_buffers_init(&spr.plane, instanced_positions, instanced_spr_num, num_inst, true);


	model_init(error, &rocky, "assets/smiley.png");
	buffers_gen_and_init(&rocky);


	float yes[2] = {10, 100};
	RGFW_event event;
	while (!RGFW_window_shouldClose(window)) {
		//RGFW_window_moveMouse(window, 30, 30);
		

		while (RGFW_window_checkEvent(window, &event)) {
			switch (event.type) {
				case RGFW_mouseMotion:
					break;
				case RGFW_mouseRawMotion:
					int dev_x = event.delta.x;
					int dev_y = event.delta.y;

					cam->yaw += (float)dev_x / 15.0;
					cam->pitch -= (float)dev_y / 15.0;
				default:
					break;
			}
		}


		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "OpenGL error: %d\n", err);
		}



		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        glClearColor(0.1, 0.1, 0.2, 1);
		RGFW_pollEvents();


		glUseProgram(program3D);
		key_input(window, cam, 0.05);
		matrix_init(cam, program3D, "3D", 640, 360);
		camera_rotate(cam, cam->yaw, cam->pitch);
		
		uniform_send_to_gpu(&cam->view_uniform, program3D, "view");

		static float angle = 0;
		//angle += 0.01;
		/*
		Uniform angle_uniform = uniform_set_data(&angle, UNIFORM_FLOAT1);
		uniform_send_to_gpu(&angle_uniform, program3D, "angle");
		*/
		uniform_apply(&angle, UNIFORM_FLOAT1, program3D, "angle");



		model_draw(&rocky, (vec3){0, 0, 0}, program3D, 1);

		glUseProgram(program);
		matrix_init(cam, program, "3D", 640, 360);
		camera_rotate(cam, cam->yaw, cam->pitch);
		uniform_send_to_gpu(&cam->view_uniform, program, "view");
		sprite_draw(&spr, (vec3){10, 0, 0}, program, 3);

		RGFW_window_swapBuffers_OpenGL(window);

	}

	
	model_delete_buffers(&rocky);
	sprite_delete(&spr);
	glDeleteProgram(program);
	glDeleteProgram(program3D);
	free(cam);

	RGFW_window_close(window);
	RGFW_deinit();
	return 0;
}
