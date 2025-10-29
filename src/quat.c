#include "quat.h"

void quat_mul(vec4 q1, vec4 q2, vec4 out) {
	float real = q1[0] - (q1[1] * q2[1]) - (q1[2] * q2[2]) - (q1[3] * q2[3]);
	float i = (q1[0] * q2[1]) + (q2[0] * q1[1]) + (q1[2] * q2[3]) - (q1[3] * q2[2]);
	float j = (q1[0] * q2[2]) - (q1[1] * q2[3]) + (q2[0] * q1[2]) + (q1[3] * q2[1]);
	float k = (q1[0] * q2[3]) + (q1[1] * q2[2]) - (q1[2] * q2[1]) + (q2[0] * q1[3]);

	out[0] = real;
	out[1] = i;
	out[2] = j;
	out[3] = k;
}


void get_rotate_quat(float theta, vec3 axis, vec4 out) {
	float ca = cos(theta/2.0);
	float sa = sin(theta/2.0);

	glm_normalize(axis);

	vec4 q = {ca, sa * axis[0], sa * axis[1], sa * axis[2]};
	glm_vec4_copy(q, out);
	//out = q;
}
