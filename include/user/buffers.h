#pragma once
#include <stdbool.h>
#include "model.h"


typedef struct {
	bool is_instanced;

	vec2 *instance_array;
	vec2 *spr_num;
} Instance;


void model_create_buffers(Model *model);
void model_delete_buffers(Model *model);

void model_draw(Model *model, unsigned int program, unsigned int instance_amount);

void set_up_buffers(int translation_size; Model *model, Instance *instance, int translation_size);

void model_init(jmp_buf error, Model *model, vec3 pos, char *texture_location, Instance *instance, int instance_amount);

