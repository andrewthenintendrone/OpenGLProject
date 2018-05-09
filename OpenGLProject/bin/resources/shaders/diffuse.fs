#version 430 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
uniform vec3 Color;

// constant light direction
const vec3 lightDirection = vec3(-1, -1, 0);

void main()
{	
	// ambient lighting
	vec3 ambient = Color * 0.25;
	
	// diffuse lighting
	vec3 norm = normalize(Normal);
	float diff = max(dot(lightDirection, norm), 0.0);	
	vec3 diffuse = Color * diff;
	
	// add ambient and diffuse
	vec3 result = ambient + diffuse;
	
	// correct gamma
	float gamma = 2.2;
	result = pow(result, vec3(1.0 / gamma));
	
	FragColor = vec4(result, 1);
}