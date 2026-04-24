#version 330 core

out vec4 fragColour;
in vec3 normal;

vec3 lightDir = vec3(1, 1, 1);
float ambient = 0.1;

void main() {
	fragColour = ambient + dot(normal, lightDir) * vec4(1.0, 0.0, 0.0, 1.0);
}
