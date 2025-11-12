#include "camera.h"

void camera_movement(Camera *cam) {
	float cameraSpeed = 0.05f;
	//set this
	vec3 cameraMove;
	vec3 cameraRight;


	int mask = 1;
	switch (cam->key - cam->bracket){
		case GLFW_KEY_W:
			glm_vec3_scale(cam->front, cameraSpeed, cameraMove);
			glm_vec3_add(cam->pos, cameraMove, cam->pos);
			break;
		case GLFW_KEY_A:
			glm_vec3_cross(cam->front, cam->up, cameraRight);
			glm_vec3_normalize(cameraRight);
			glm_vec3_scale(cameraRight, cameraSpeed, cameraRight);

			glm_vec3_sub(cam->pos, cameraRight, cam->pos);
			break;
		case GLFW_KEY_S:
			glm_vec3_scale(cam->front, cameraSpeed, cameraMove);
			glm_vec3_sub(cam->pos, cameraMove, cam->pos);
			break;
		case GLFW_KEY_D:
			glm_vec3_cross(cam->front, cam->up, cameraRight);
			glm_vec3_normalize(cameraRight);
			glm_vec3_scale(cameraRight, cameraSpeed, cameraRight);

			glm_vec3_add(cam->pos, cameraRight, cam->pos);
			break;
		case GLFW_KEY_SPACE:
			cam->pos[1] += cameraSpeed;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			cam->pos[1] -= cameraSpeed;
			break;
		default:
			break;
	}
}


void key_or(int mask_val, int mask, Camera *cam) {
	if (mask_val == 0) {
		cam->mask1 = cam->mask1 | mask;
	} else {
		cam->mask2 = cam->mask2 | mask;
	}
}

void key_not(int mask_val, int mask, Camera *cam) {
	if (mask_val == 0) {
		cam->mask1 = cam->mask1 & ~mask;
	} else {
		cam->mask2 = cam->mask2 & ~mask;
	}
}

int Groups[16] = {
	GLFW_KEY_SPACE,
	GLFW_KEY_SPACE,
	GLFW_KEY_APOSTROPHE,
	GLFW_KEY_APOSTROPHE,
	GLFW_KEY_COMMA,
	GLFW_KEY_GRAVE_ACCENT,
	GLFW_KEY_WORLD_1,
	GLFW_KEY_WORLD_2,
	GLFW_KEY_ESCAPE,
	GLFW_KEY_END,
	GLFW_KEY_CAPS_LOCK,
	GLFW_KEY_PAUSE,
	GLFW_KEY_F1,
	GLFW_KEY_F25,
	GLFW_KEY_KP_0,
	GLFW_KEY_LAST
};

int key_check(int *key) {
	int diff = 0;
	int total_diff = 0;
	for (int start = 0; start < sizeof(Groups) / sizeof(int); start += 2) {
		int end = start + 1;

		if (start == 0) {
			diff = 0;
		} else {
			diff = Groups[start-1] - Groups[start];
		}
		total_diff += diff + 1;

		if ((*key >= Groups[start]) && (*key <= Groups[end])) {
			*key += total_diff;
			return total_diff;
		}
	}
	/*
	if (*key >= a && key < a_end) {
	} else if (*key >= b_end) {
		*key -= b;
		return b;
	} else if (*key >= c_end) {
		*key -= c;
		return c;
	} else if (*key >= d_end) {
		*key -= d;
		return d;
	} else if (*key >= e_end) {
		*key -= e;
		return e;
	} else if (*key >= f_end) {
		*key -= f;
		return f;
	} else if (*key >= g_end) {
		*key -= g;
		return g;
	} else if (*key >= h_end) {
		*key -= h;
		return h;
	}
	*/
	return -1;
}

void get_mask(int key, Camera *cam, void (*key_func)(int, int, Camera *)) {
	int mask = 1;
	cam->bracket = key_check(&key);
	cam->key = key;
	printf("%d\n", cam->key - cam->bracket);
	if (key < sizeof(long)) {
		int new_key = key;
		mask = mask << new_key;
		key_func(0, mask, cam);
	} else if (key - sizeof(long) < sizeof(long)) {
		int new_key = key - 64;
		mask = mask << new_key;
		key_func(1, mask, cam);
	}
}
