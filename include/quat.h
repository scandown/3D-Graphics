#pragma once
#include "cglm/cglm.h"
#include <math.h>
#include <stdio.h>


void print_quat(vec4 q1);
void quat_mul(vec4 q1, vec4 q2, vec4 out);
void get_rotate_quat(float theta, vec3 axis, vec4 out);

void quat_conj(vec4 q1, vec4 out);
float quat_mag(vec4 q1);
void quat_inverse(vec4 q1, vec4 out);
void quat_exp(vec4 q1, vec4 out);
