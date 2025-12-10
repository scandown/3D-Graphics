#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
out vec3 pos;
out vec2 TexCoord;
out vec3 normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main() {

	// Rotate the position using quaternion rotation
	//vec3 rotated_pos = quat_rotate_vector(angle, axis, aPos);

	//rots *= aPos;

	mat4 coordinates = projection * view * model;
	gl_Position = coordinates * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	pos = aPos;
	normal = aNormal;
	TexCoord = aTexCoord;
}
