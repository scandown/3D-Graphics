#pragma once

#include <setjmp.h>
#include <stdio.h>
#include "glad/glad.h"


unsigned int texture_init(jmp_buf error, GLenum format, char *name);
