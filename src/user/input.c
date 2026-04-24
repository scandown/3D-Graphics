#include "user/input.h"

void key_input(GLFWwindow *window, Camera *cam, float speed) {

	float cameraSpeed = speed;
	vec3 cameraUp;
	vec3 cameraRight;
	vec3 cameraForward;
	vec3 forward_temp;
	vec3 up_temp;


	if (glfwGetKey(window, GLFW_KEY_W)) {
		glm_vec3_copy(cam->front, forward_temp);
		forward_temp[1] = 0;
		glm_vec3_normalize(forward_temp);
		glm_vec3_scale(forward_temp, cameraSpeed, cameraForward);
		glm_vec3_add(cam->pos, cameraForward, cam->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		glm_vec3_cross(cam->front, cam->up, cameraRight);
		glm_vec3_normalize(cameraRight);
		glm_vec3_scale(cameraRight, cameraSpeed, cameraRight);
		glm_vec3_sub(cam->pos, cameraRight, cam->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		glm_vec3_copy(cam->front, forward_temp);
		forward_temp[1] = 0;
		glm_vec3_normalize(forward_temp);
		glm_vec3_scale(forward_temp, -cameraSpeed, cameraForward);
		glm_vec3_add(cam->pos, cameraForward, cam->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		glm_vec3_cross(cam->front, cam->up, cameraRight);
		glm_vec3_normalize(cameraRight);
		glm_vec3_scale(cameraRight, cameraSpeed, cameraRight);
		glm_vec3_add(cam->pos, cameraRight, cam->pos);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		cam->pos[1] += cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		cam->pos[1] -= cameraSpeed;
	}
}
