#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 q;

// source: https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
mat4 quatToMat4(vec4 q_in) {
    float w = q_in.x;
    float x = q_in.y;
    float y = q_in.z;
    float z = q_in.w;

    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    return mat4(
        1.0 - 2.0 * (yy + zz),  2.0 * (xy + wz),        2.0 * (xz - wy),        0.0,
        2.0 * (xy - wz),        1.0 - 2.0 * (xx + zz),  2.0 * (yz + wx),        0.0,
        2.0 * (xz + wy),        2.0 * (yz - wx),        1.0 - 2.0 * (xx + yy),  0.0,
        0.0,                    0.0,                    0.0,                    1.0
    );
}

void main() {


	// new rot = q * p * q^-1

	mat4 rot = quatToMat4(q);

	mat4 coordinates = projection * view * rot *model;

	gl_Position = coordinates * vec4(aPos, 1.0);
	//gl_Position = coordinates * vec4(aPos, 1.0);
	pos = aPos;
};
