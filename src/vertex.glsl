#version 330 core

struct quat{
	float w;
	float i;
	float j;
	float k;
};

quat quat_normalize(quat q) {
	float q_mag = sqrt(q.w * q.w + q.i * q.i + q.j * q.j + q.k * q.k);

	return quat(q.w / q_mag,
			q.i / q_mag,
			q.j / q_mag,
			q.k / q_mag);
}

quat vec_to_quat(vec3 v1) {
	return quat(0.0, v1.x, v1.y, v1.z);
}

quat quat_mul(quat q1, quat q2) {
	vec3 q1_i = vec3(q1.i, q1.j, q1.k);
	vec3 q2_i = vec3(q2.i, q2.j, q2.k);

	float scalar = q1.w * q2.w - dot(q1_i, q2_i);
	vec3 imaginary = (q2_i * q1.w) + (q1_i * q2.w) + cross(q1_i, q2_i);

	return quat(scalar, imaginary.x, imaginary.y, imaginary.z);
}

layout (location = 0) in vec3 aPos;
out vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 rot;


void main() {

	// Rotate the position using quaternion rotation
	//vec3 rotated_pos = quat_rotate_vector(angle, axis, aPos);
	quat rot_q = quat(rot.x, rot.y, rot.z, rot.w);
	quat rot_q_conj = quat(rot.x, -rot.y, -rot.z, -rot.w);
	quat qp = vec_to_quat(aPos);
	quat rotated_pos = quat_mul(rot_q, qp);
	rotated_pos = quat_mul(rotated_pos, rot_q_conj);

	vec3 rots = vec3(rotated_pos.i, rotated_pos.j, rotated_pos.k);

	//rots *= aPos;

	mat4 coordinates = projection * view * model;
	gl_Position = coordinates * vec4(rots, 1.0);
	pos = aPos;
}
