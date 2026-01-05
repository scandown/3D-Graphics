#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 offsets[100];


void main() {
	vec2 offset = offsets[gl_InstanceID];
	mat4 coordinates = projection * view * model;

	gl_Position = coordinates * vec4(aPos.xy + offset, 0.0, 1.0);

	TexCoord = aTexCoord;
}
