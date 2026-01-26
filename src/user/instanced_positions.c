#include "user/instanced_positions.h"

void sprite_send_instanced_positions(vec2 *instanced_positions, vec2 *instanced_spr_num, int instance_x, int instance_y) {
	int index = 0;
	for (int y = 0; y < instance_y; y++) {
		for (int x = 0; x < instance_x; x++) {
			vec2 translation;
			translation[0] = x * 16;
			translation[1] = y * 16;

			vec2 spr_num;
			spr_num[0] = 0;
			spr_num[1] = 1;

			glm_vec2_copy(translation, instanced_positions[index]);
			glm_vec2_copy(spr_num, instanced_spr_num[index]);
			index++;
		}
	}
}
