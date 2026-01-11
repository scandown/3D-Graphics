#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aOffset;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 coordinates = projection * view * model;

	gl_Position = coordinates * vec4(aPos.xy + aOffset, -0.1, 1);

	TexCoord = aTexCoord;
}
