#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 pos;

uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 coordinates = projection * view * model;
	gl_Position = coordinates * vec4(aPos, 1.0);
	pos = aPos;
};
