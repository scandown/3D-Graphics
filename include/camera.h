#pragma once
#include "glad/glad.h"
#include "cglm/cglm.h"
#define RGFW_IMPORT
#include "RGFW.h"
#include "uniform.h"


typedef struct {
	vec3 pos;
	vec3 front;
	vec3 up;
	long mask1;
	long mask2;
	int key;


	float pitch;
	float yaw;
	double prev_xpos;
	double prev_ypos;

	Uniform projection_uniform;
	Uniform view_uniform;
} Camera;



void camera_rotate(Camera *cam, float yaw, float pitch);
void camera_init(Camera *cam, vec3 pos, float pitch, float yaw);
void cursor_position_callback(RGFW_window *window, Camera *cam, float sensitivity);
