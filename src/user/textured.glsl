#version 330 core

out vec4 fragColour;
in vec2 uv;
in vec2 spr_num;

uniform sampler2D tex;

vec2 atlasSize = vec2(1.0, 1.0);

uniform float scroll;

void main() {

	vec2 spr_num_offset = spr_num / atlasSize;
	fragColour = texture(tex, uv / atlasSize + spr_num_offset + scroll);
}
