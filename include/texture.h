#pragma once

#include <setjmp.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"


unsigned int texture_setup(jmp_buf error, GLenum format, char *name);
