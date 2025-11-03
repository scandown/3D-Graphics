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

float vec_mag(vec3 v1) {
	return sqrt((v1[0] * v1[0]) + (v1[1] * v1[1]) + (v1[2] * v1[2]));
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
	float w = expf(q1[0]);
	glm_vec4_copy(q1, out);
	
	// v = Vector component of q1
	// exp(s) * cos(|v|);
	//
	// v: v / |v| * sin(|v|)
	
	vec3 v = {q1[1], q1[2], q1[3]};
	float v_mag = vec_mag(v);
	if (v_mag < 0.0001f) {  // Small epsilon to avoid division by zero
		out[0] = w;         // exp(w) * cos(0) = exp(w) * 1
		out[1] = 0.0f;
		out[2] = 0.0f;
		out[3] = 0.0f;
	} else {
		out[0] = w * cos(v_mag);
		out[1] = w * ((v[0] / v_mag) * sin(v_mag));
		out[2] = w * ((v[1] / v_mag) * sin(v_mag));
		out[3] = w * ((v[2] / v_mag) * sin(v_mag));
	}
}


void quat_log(vec4 q1, vec4 out) {
	// w = ln(|q|);
	// v = v / |v| * arccos(w / |q|)
	
	float w = log(quat_mag(q1));

	vec3 v = {q1[1], q1[2], q1[3]};
	float v_mag = vec_mag(v);
	float q_mag = quat_mag(q1);
	out[0] = w;
	out[1] = (v[0] / v_mag) * acosf(q1[0] / q_mag);
	out[2] = (v[1] / v_mag) * acosf(q1[0] / q_mag);
	out[3] = (v[2] / v_mag) * acosf(q1[0] / q_mag);
}

void quat_power(vec4 q1, float power, vec4 out) {
	// q^t = exp(ln(q) * t);
	
	vec4 q_log;
	quat_log(q1, q_log);

	vec4 scaled_log = {
		q_log[0] * power,
		q_log[1] * power,
		q_log[2] * power,
		q_log[3] * power
	};

	quat_exp(scaled_log, out);

}


void lookat_cube(mat4 mat, vec3 trans, vec3 target, vec3 up) {
	vec3 new_up, right, forward;

	for (int i = 0; i < 3; i++) {
		forward[i] = trans[i] - target[i];
	}

	glm_vec3_normalize(forward);
	glm_vec3_cross(forward, up, right);
	glm_vec3_normalize(right);

	glm_vec3_cross(right, forward, new_up);
	glm_vec3_normalize(new_up);


	for (int i = 0; i < 3; i++) {
		mat[0][i] = right[i];
		mat[1][i] = new_up[i];
		mat[2][i] = -forward[i];
	}
}
