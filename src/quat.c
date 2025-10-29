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

void quat_conj(vec4 q1, vec4 out) {
	glm_vec4_copy((vec4){q1[0], -q1[1], -q1[2], -q1[3]}, out);
}

float quat_mag(vec4 q1) {
	return sqrt((q1[0] * q1[0]) + (q1[1] * q1[1]) + (q1[2] * q1[2]) + (q1[3] * q1[3]));
}

void print_quat(vec4 q1) {
	printf("q = {%f, %f, %f, %f}\n", q1[0], q1[1], q1[2], q1[3]);
}

void quat_inverse(vec4 q1, vec4 out) {
	vec4 q_conj;
	quat_conj(q1, q_conj);

	vec4 q1_square;
	quat_mul(q1, q1, q1_square);

	float q1_square_mag = quat_mag(q1_square);

	for (int i = 0; i < 4; i++) {
		//q_conj[0] / q1_square_mag, q_conj[1] / q1_square_mag, q_conj[2]
		q_conj[i] /= q1_square_mag;
	}
	glm_vec4_copy(q_conj, out);
}

void quat_exp(vec4 q1, vec4 out) {
	float s = exp(q1[0]);
}
