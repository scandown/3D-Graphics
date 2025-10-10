#version 330 core
out vec4 fragColour;
in vec3 pos;


#define LINE_THICKNESS 0.02
#define OFFSET 0.1

// a is x
// b is y

// use pos


// set pos so that its between a smaller range
float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

uniform float scalemin;
uniform float scalemax;

void main() {
	int maxIterations = 100;
	//fragColour = vec4(0, 1, 0, 1);

	float a = map(pos.x, -1, 1, scalemin, scalemax);
	float b = map(pos.y, -1, 1, scalemin, scalemax);

	float ca = a;
	float cb = b;

	int i = 0;

	for (i; i < maxIterations; i++) {

		float c0 = pow(a, 2) - pow(b, 2);
		float c1 = 2 * a * b;

		a = c0 + ca;
		b = c1 + cb;

		if (abs(a + b) > 16) {
			break;
		}
	}

	/*
	if (z < 16) {
		discard;
	}
	*/

	if(i == maxIterations) {
		fragColour = vec4(0, 0, 0, 1);
	} else {
		fragColour = vec4(sqrt(float(i) / maxIterations), 0, 0, 1);
	}

}

