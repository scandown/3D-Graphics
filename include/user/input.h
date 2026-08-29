#pragma once
#include "cglm/cglm.h"
#include "glad/glad.h"
#define RGFW_IMPORT
#include "RGFW.h"

#include "camera.h"

void key_input(RGFW_window *window, Camera *cam, float speed);
