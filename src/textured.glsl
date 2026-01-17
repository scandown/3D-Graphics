#version 330 core

out vec4 fragColour;
in vec2 TexCoord;

uniform sampler2D tex;
uniform float yes;

void main() {
	fragColour = texture(tex, vec2(TexCoord.x - yes, TexCoord.y));
}
