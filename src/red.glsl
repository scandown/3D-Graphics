#version 330 core
out vec4 fragColour;
in vec3 pos;

uniform vec2 point; // line passes through origin (0,0) and this point

void main() {
	fragColour = vec4(1, 0, 0, 1);
}

