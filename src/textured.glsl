#version 330 core

out vec4 fragColour;
in vec2 uv;

uniform sampler2D tex;
uniform float yes;

void main() {
	fragColour = texture(tex, vec2(uv.x - yes, uv.y));
}
