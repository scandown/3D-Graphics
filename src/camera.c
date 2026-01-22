#include "camera.h"

void camera_rotate(Camera *cam, float yaw, float pitch, mat4 view) {
	//camera_look(cam, yaw, pitch, &view);
	vec3 direction;
	direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
	direction[1] = sin(glm_rad(pitch));
	direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
	glm_vec3_normalize_to(direction, cam->front);
	vec3 camera_total_front;
	glm_vec3_copy(cam->front, camera_total_front);
	glm_vec3_add(cam->pos, cam->front, camera_total_front);
	glm_lookat(cam->pos, camera_total_front, cam->up, view);
}

void camera_init(Camera *cam, vec3 pos, float pitch, float yaw) {
	glm_vec3_copy(pos, cam->pos);
	glm_vec3_copy((vec3){0, 0, -1}, cam->front);
	glm_vec3_copy((vec3){0, 1, 0}, cam->up);
	cam->pitch = pitch;
	cam->yaw = yaw;
	cam->prev_xpos = 0;
	cam->prev_ypos = 0;
	cam->mask1 = 0;
}
