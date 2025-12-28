#version 330 core

struct quat{
       float w;
       float i;
       float j;
       float k;
};

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
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
out vec3 pos;
out vec2 TexCoord;
out vec3 normal;
out vec3 FragPos;
out vec4 yoPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main() {

	//vec4 rot = {1, 0, 1, 0};
	vec4 rot;
	rot.x = -1;
	rot.y = 0;
	rot.z = 0;
	rot.w = 1;

	// Rotate the position using quaternion rotation
	//vec3 rotated_pos = quat_rotate_vector(angle, axis, aPos);

	//rots *= aPos;

	mat4 coordinates = projection * view * model;


	quat rot_q = quat(rot.x, rot.y, rot.z, rot.w);
	quat rot_q_conj = quat(rot.x, -rot.y, -rot.z, -rot.w);
	quat qp = vec_to_quat(aPos);
	quat rotated_pos = quat_mul(rot_q, qp);
	rotated_pos = quat_mul(rotated_pos, rot_q_conj);
	vec3 rots = vec3(rotated_pos.i, rotated_pos.j, rotated_pos.k);


	yoPos = coordinates * vec4(rots, 1.0);
	//gl_Position = coordinates * vec4(aPos, 1.0);
	gl_Position = yoPos;

	FragPos = vec3(model * vec4(aPos, 1.0));
	pos = aPos;
	//normal = aNormal;
	normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoord = aTexCoord;
}
