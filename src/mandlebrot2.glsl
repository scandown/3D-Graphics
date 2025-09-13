#version 330 core
out vec4 fragColour;
in vec3 pos;

uniform vec2 point; // line passes through origin (0,0) and this point


#define LINE_THICKNESS 0.02
#define OFFSET 0.1
#define MAX_ITERATIONS 64

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
	float a = map(pos.x, -1, 1, scalemin, scalemax);
	float b = map(pos.y, -1, 1, scalemin, scalemax);

	float ca = a;
	float cb = b;

	// f(z) = (c^2 + c) + c

	int i;

	for (i = 0; i < MAX_ITERATIONS; i++) {
		float z0 = pow(a, 2) - pow(b, 2);
		float z1 = 2 * a * b;

		a = ca + z0;
		b = cb + z1;

		if (abs(a + b) > 16) {
			break;
		}

	}

	if (i == MAX_ITERATIONS) {
		fragColour = vec4(0, 0, 0, 1);
	}
	else {
		fragColour = vec4(sqrt(float(i) / MAX_ITERATIONS), 0, 0, 1);
	}
	//fragColour = vec4(1 - 100/i, 0, 0, 1);

}

