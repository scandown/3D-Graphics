#pragma once
#include "model.h"

void model_create_buffers(Model *model);
void model_delete_buffers(Model *model);

void model_draw(Model *model, unsigned int program);
void model_draw_instanced(Model *model, unsigned int program, unsigned int instance_amount);
void model_send_to_gpu_instanced(int translation_size; Model *model, vec2 translations[translation_size], vec2 spr_num[translation_size], int translation_size);
void model_init(jmp_buf error, Model *model, vec3 pos, char *texture_location);
void model_init_instanced(int instance_amount; jmp_buf error, Model *model, vec3 pos, char *texture_location,
		vec2 instance_array[instance_amount], vec2 spr_num[instance_amount], int instance_amount);

