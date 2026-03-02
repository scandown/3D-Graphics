#pragma once
#include <stdbool.h>
#include "model.h"

void model_create_buffers(Model *model);
void model_delete_buffers(Model *model);
void model_draw(Model *model, unsigned int program, unsigned int instance_amount);
void model_init(jmp_buf error, Model *model, vec3 pos, char *texture_location);

void buffers_init(Model *model, Instance *instance, int translation_size);
void gen_buffers(Model *model, Instance *instance, int instance_amount);
