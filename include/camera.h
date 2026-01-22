#pragma once
#include "glad/glad.h"
#include "cglm/cglm.h"
#include "GLFW/glfw3.h"
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

	mat4 view;
} Camera;



void camera_rotate(Camera *cam, float yaw, float pitch, mat4 view);
void camera_init(Camera *cam, vec3 pos, float pitch, float yaw);
void cursor_position_callback(GLFWwindow* window, Camera *cam, float sensitivity);
