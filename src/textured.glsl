#version 330 core

out vec4 fragColour;

in vec2 TexCoord;
in vec3 pos;
uniform sampler2D tex;

void main() {

	float ambient_strength = 1;
	fragColour = ambient_strength * texture(tex, TexCoord);
}
