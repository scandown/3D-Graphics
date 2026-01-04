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
} Camera;




void key_or(int mask_val, long mask, Camera *cam);
void key_not(int mask_val, long mask, Camera *cam);
void key_check(int *key);
void key_check_inv(int *inv_key);


void set_input_mask(int key, Camera *cam, void (*key_func)(int, long, Camera *));
void key_input(Camera *cam);
void camera_rotate(Camera *cam, float yaw, float pitch, mat4 view_matrix, Uniform *view_uniform, unsigned int program);
void camera_init(Camera *cam, vec3 pos, float pitch, float yaw);
