#version 330 core

out vec4 fragColour;
in vec2 uv;

uniform sampler2D tex;
uniform float yes;

int sizeX = 2;
int sizeY = 1;

int sprX = 2;
int sprY = 1;

void main() {
	// get percentage of sprX to sizeX to put it into uv coords
	float uvX = sprX / sizeX;
	float uvY = sprY / sizeY; 
	fragColour = texture(tex, vec2((uv.x / sizeX) + uvX, (uv.y / sizeY)));
}
