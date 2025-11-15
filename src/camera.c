#include "camera.h"

int Groups[11][2] = {
	{GLFW_KEY_SPACE, GLFW_KEY_SPACE},
	{GLFW_KEY_APOSTROPHE, GLFW_KEY_SPACE},
	{GLFW_KEY_COMMA, GLFW_KEY_APOSTROPHE},
	{GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_RIGHT_BRACKET},
	{GLFW_KEY_WORLD_1, GLFW_KEY_GRAVE_ACCENT},
	{GLFW_KEY_ESCAPE, GLFW_KEY_WORLD_2},
	{GLFW_KEY_CAPS_LOCK, GLFW_KEY_END},
	{GLFW_KEY_F1, GLFW_KEY_PAUSE},
	{GLFW_KEY_KP_0, GLFW_KEY_F25},
	{GLFW_KEY_LEFT_SHIFT, GLFW_KEY_KP_EQUAL},
	{GLFW_KEY_LAST, GLFW_KEY_LAST}
};

int Groups2[11];


void key_or(int mask_val, long mask, Camera *cam) {
	if (mask_val == 0) {
		cam->mask1 = cam->mask1 | mask;
	} else {
		cam->mask2 = cam->mask2 | mask;
	}
}

void key_not(int mask_val, long mask, Camera *cam) {
	if (mask_val == 0) {
		cam->mask1 = cam->mask1 & ~mask;
	} else {
		cam->mask2 = cam->mask2 & ~mask;
	}
}


void key_check(int *key) {
	int total_diff = 0;
	for (int i = 0; i < 11; i++) {
		total_diff += Groups[i][0] - Groups[i][1];

		Groups2[i] = Groups[i][0] - Groups[i][1];

		if ((*key < Groups[i+1][0]) && (*key >= Groups[i][0])) {
			*key -= total_diff + 32;
			*key += i;
		}
	}
}

void key_check_inv(int *inv_key) {
	int total_diff=0;
	int Groups3[11];
	for (int i = 0; i < 11; i++) {
		total_diff+=Groups2[i];
		int lower_bracket = Groups[i][0] - total_diff + i;
		Groups3[i] = lower_bracket;
	}

	total_diff=0;
	for (int i = 0; i < 11; i++) {
		total_diff+=Groups2[i];
		int offset = i;
		int lower = Groups3[offset] - 32;
		if (i + 1 < 11) {
			offset = i+1;
		}
		int upper = Groups3[offset] - 32;


		if (*inv_key < upper && *inv_key >= lower) {
			*inv_key = *inv_key + total_diff + 32 - i;
			break;
		}
	}
}


void get_mask(int key, Camera *cam, void (*key_func)(int, long, Camera *)) {
	long mask = 1;
	key_check(&key);
	cam->key = key;
	if (cam->key < 64) {
		long new_key = cam->key;
		mask = mask << new_key;
		key_func(0, mask, cam);
	} else {
		long new_key = cam->key - 64;
		mask = mask << new_key;
		key_func(1, mask, cam);
	}
}
void camera_movement(Camera *cam) {
	long mask = 1;
	int amount = 0;

	for (int i = 0; i < 64; i++) {
		mask = mask << 1;	
		amount++;
		int amount_copy = amount;

		if ((cam->mask1 & mask) != 0) {
			key_check_inv(&amount_copy);
			printf("%d\n", amount_copy);
		}
		if ((cam->mask2 & mask) != 0) {
			amount_copy += 64;
			key_check_inv(&amount_copy);
			printf("%d\n", amount_copy);
		}

	}
}
