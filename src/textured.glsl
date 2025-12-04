#version 330 core

out vec4 fragColour;

in vec2 TexCoord;
in vec3 pos;
in vec3 normal;

// world position of the current fragment not accounting for perspective or view
in vec3 FragPos;
uniform sampler2D tex;

void main() {
	vec3 lightPos = vec3(0, 3, 0);

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.1);
	vec3 diffuse = diff * vec3(1, 1, 1);
	vec3 result = (0.1 + diffuse);
	fragColour = vec4(result, 1) * texture(tex, TexCoord);
}
