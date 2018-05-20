#version 430 core
out vec4 FragColor;

in vec4 FragPos;
in vec4 Normal;

uniform vec4 color;

// constant light direction
const vec3 lightDirection = vec3(-1, -1, 0);

void main()
{	
	// ambient lighting
	vec3 ambient = color.rgb * 0.25;
	
	// diffuse lighting
	vec3 norm = normalize(Normal.xyz);
	float diff = max(dot(norm, -lightDirection), 0.0);
	vec3 diffuse = color.rgb * diff;
	
	// add ambient and diffuse
	vec3 result = ambient + diffuse;
	
	FragColor = vec4(result, 1);
}