#version 330 core

out vec4 fragColour;
in vec3 normal;
in vec3 FragPos;

vec3 lightPos = vec3(100, 100, 100);

vec3 lightDir = normalize(lightPos - FragPos);
vec3 light_colour = vec3(1, 1, 1);
vec3 object_colour = vec3(1, 0, 0);

vec3 norm = normalize(normal);

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * light_colour;


void main() {
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * light_colour;

	vec3 result = (ambient + diffuse) * object_colour;
	fragColour = vec4(result, 1.0);
}
