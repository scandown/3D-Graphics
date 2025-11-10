#include "camera.h"

void camera_movement() {
	float cameraSpeed = 0.05f;
	//set this
	vec3 cameraMove;
	vec3 cameraRight;
	Camera *cam = c_ptr;

	printf("%i\n", cam->key);

	switch (cam->key){
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

void key_or(int mask, Camera *cam) {
	cam->key = cam->key | mask;
}

void key_not(int mask, Camera *cam) {
	cam->key = cam->key & ~mask;
}

int get_mask(int key, Camera *cam, void (*key_func)(int, Camera *)) {
	int mask = 1;
	int new_key = key - 64;
	if (key <= GLFW_KEY_Z && key >= GLFW_KEY_A) {
		mask = mask << new_key;
		key_func(mask, cam);
	}
}
