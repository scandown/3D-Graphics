#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "engine.h"
#include "platform/time.h"

#define RGFW_OPENGL
#define RGFW_IMPORT
#include "RGFW.h"

#include "user/input.h"
#include "user/instanced_positions.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

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




	Sprite spr = sprite_init(error, 1, "assets/smiley.png", 16, 16);
	buffers_gen_and_init(&spr.plane);
	instanced_buffers_init(&spr.plane, instanced_positions, instanced_spr_num, num_inst, true);

	Sprite grid = sprite_init(error, 1, "assets/grid.png", 640, 360);
	for (int i = 0; i < arrlen(grid.plane.uv_array); ++i) {
		grid.plane.uv_array[i][0] *= 20.0 / (640 / 360);
		grid.plane.uv_array[i][1] *= 20.0;
	}
	buffers_gen_and_init(&grid.plane);
	instanced_buffers_init(&grid.plane, instanced_positions, instanced_spr_num, num_inst, true);



	model_init(error, &rocky, "assets/smiley.png");
	buffers_gen_and_init(&rocky);


	float yes[2] = {10, 100};
	RGFW_event event;
	struct timeval t1, t2;
	gettimeofday(&t2, NULL);
	while (!RGFW_window_shouldClose(window)) {
		double elapsedTime;

		float delta = time_delta(&t2, NULL);


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

		glDisable(GL_DEPTH_TEST);
		glUseProgram(program);
		matrix_init(cam, program, "2D", 640, 360);
		camera_rotate(cam, -90, 0);
		uniform_send_to_gpu(&cam->view_uniform, program, "view");
		static float scroll = 0;
		scroll += 1 * delta;
		scroll = scroll >= 1 ? 0 : scroll;
		uniform_apply(&scroll, UNIFORM_FLOAT1, program, "scroll");
		sprite_draw(&grid, (vec3){30, 0, 0}, program, 1);

		glEnable(GL_DEPTH_TEST);


		glUseProgram(program3D);
		key_input(window, cam, 10 * delta);
		matrix_init(cam, program3D, "3D", 640, 360);
		camera_rotate(cam, cam->yaw, cam->pitch);
		
		uniform_send_to_gpu(&cam->view_uniform, program3D, "view");

		static float angle = 0;
		uniform_apply(&angle, UNIFORM_FLOAT1, program3D, "angle");

		model_draw(&rocky, (vec3){0, 0, 0}, program3D, 1);

		glUseProgram(program);
		matrix_init(cam, program, "3D", 640, 360);
		camera_rotate(cam, cam->yaw, cam->pitch);
		uniform_send_to_gpu(&cam->view_uniform, program, "view");
		float new_scroll = 0;
		uniform_apply(&new_scroll, UNIFORM_FLOAT1, program, "scroll");
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
