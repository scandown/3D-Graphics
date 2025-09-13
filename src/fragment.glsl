#version 330 core
out vec4 fragColour;
in vec3 pos;

uniform vec2 point; // line passes through origin (0,0) and this point

#define LINE_THICKNESS 0.02
#define OFFSET 0.1

void main() {
    // circle mask
    float distance = length(pos.xy);
    if (distance > 1) {
        discard;
    }

    vec2 dir = normalize(point);
    float d = abs(pos.x * dir.y - pos.y * dir.x);


    if (d < LINE_THICKNESS && sign(pos.x) == sign(point.x) && sign(pos.y) == sign(point.y)) {
        fragColour = vec4(0, distance, 0, 1);
    } else {
        fragColour = vec4(1, 0, 0, 1);
    }
}

