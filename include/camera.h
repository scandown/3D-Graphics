#pragma once
#include "glad/glad.h"
#include "cglm/cglm.h"
#include "GLFW/glfw3.h"


typedef struct {
	vec3 pos;
	vec3 front;
	vec3 up;
	long mask1;
	long mask2;
	int key;
} Camera;

void camera_movement(Camera *cam);
void key_not(int mask_val, long mask, Camera *cam);
void key_or(int mask_val, long mask, Camera *cam);
void get_mask(int key, Camera *cam, void (*key_func)(int, long, Camera *));

void key_check(int *key);
void key_check_inv(int *inv_key);
