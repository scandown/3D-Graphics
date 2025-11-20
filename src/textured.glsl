#version 330 core

in vec2 TexCoord;
uniform sampler2D tex;

void main() {
	fragColour = texture(tex, TexCoord);
}
