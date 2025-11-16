#pragma once
#include "glad/glad.h"
#include "cglm/cglm.h"
#include "GLFW/glfw3.h"
#include "spaces.h"


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

void camera_movement(Camera *cam);
void key_not(int mask_val, long mask, Camera *cam);
void key_or(int mask_val, long mask, Camera *cam);
void get_mask(int key, Camera *cam, void (*key_func)(int, long, Camera *));

void key_check(int *key);
void key_check_inv(int *inv_key);
void camera_look(Camera *cam, float yaw, float pitch, Space *view);
