#version 330 core

struct quat{
       float i;
       float j;
       float k;
       float w;
};

quat vec_to_quat(vec3 v1) {
       return quat(v1.x, v1.y, v1.z, 0.0);
}

quat quat_mul(quat q1, quat q2) {
       vec3 q1_i = vec3(q1.i, q1.j, q1.k);
       vec3 q2_i = vec3(q2.i, q2.j, q2.k);

       float scalar = q1.w * q2.w - dot(q1_i, q2_i);
       vec3 imaginary = (q2_i * q1.w) + (q1_i * q2.w) + cross(q1_i, q2_i);

       return quat(imaginary.x, imaginary.y, imaginary.z, scalar);
}


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
out vec2 uv;
out vec3 normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float angle;



void main() {

	vec4 rot;
	rot.x = sin(0/2);
	rot.y = sin(angle/2);
	rot.z = sin(0/2);
	rot.w = cos(angle/2);

	/*
	rot.x = 0;
	rot.y = 1;
	rot.z = 0;
	rot.w = 1;
	*/

	mat4 coordinates = projection * view * model;


	quat rot_q = quat(rot.x, rot.y, rot.z, rot.w);
	quat rot_q_conj = quat(-rot.x, -rot.y, -rot.z, rot.w);
	quat qp = vec_to_quat(aPos);
	quat rotated_pos = quat_mul(rot_q, qp);
	rotated_pos = quat_mul(rotated_pos, rot_q_conj);
	vec3 rots = vec3(rotated_pos.i, rotated_pos.j, rotated_pos.k);


	quat qn = vec_to_quat(aNormal);
	quat rotated_normal = quat_mul(rot_q, qn);
	rotated_normal = quat_mul(rotated_normal, rot_q_conj);
	vec3 rotn = vec3(rotated_normal.i, rotated_normal.j, rotated_normal.k);


	gl_Position = coordinates * vec4(rots, 1.0);

	FragPos = vec3(model * vec4(rots, 1.0));
	normal = rotn;
	uv = aUV;
}
