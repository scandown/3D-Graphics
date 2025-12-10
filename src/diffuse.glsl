#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform sampler2D tex;
uniform vec3 view_pos;

in vec3 FragPos;
in vec3 normal;
in vec2 TexCoord;

void main() {



	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;


	float specular_strength = 1.5;
	vec3 view_dir = normalize(view_pos - FragPos);
	vec3 reflect_dir = reflect(-lightDir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specular_strength * spec * lightColor;


	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0) * texture(tex, TexCoord);
}
