#version 330 core

vec4 quat_mult(vec4 q1, vec4 q2)
{ 
  vec4 qr;
  qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return qr;
}

vec4 quat_mult2(vec4 q1, vec4 q2) {
	vec3 q1_i = vec3(q1.x, q1.y, q1.z);
	vec3 q2_i = vec3(q2.x, q2.y, q2.z);

	float scalar = q1.w * q2.w - dot(q1_i, q2_i);

	vec3 imaginary = (q2_i * q1.w) + (q1_i * q2.w) + cross(q1_i, q2_i);

	return vec4(imaginary.x, imaginary.y, imaginary.z, scalar);
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

	rot.x = 0;
	rot.y = 0;
	rot.z = 1;
	rot.w = 1;
	rot = normalize(rot);

	float fang = angle;

	rot.w *= cos(fang/2);
	rot.x *= sin(fang/2);
	rot.y *= sin(fang/2);
	rot.z *= sin(fang/2);

	mat4 coordinates = projection * view * model;


	vec4 rot_q = rot;
	vec4 rot_q_conj = vec4(-rot.x, -rot.y, -rot.z, rot.w);
	
	vec4 rotated_pos = quat_mult2(quat_mult2(rot_q, vec4(aPos, 0)), rot_q_conj);
	vec3 rots = vec3(rotated_pos.x, rotated_pos.y, rotated_pos.z);


	vec4 rotated_normal = quat_mult2(rot_q, vec4(aNormal, 0));
	rotated_normal = quat_mult2(rotated_normal, rot_q_conj);
	vec3 rotn = vec3(rotated_normal.x, rotated_normal.y, rotated_normal.z);


	gl_Position = coordinates * vec4(rots, 1.0);

	FragPos = vec3(model * vec4(rotn, 1.0));
	normal = rotn;
	uv = aUV;
}
