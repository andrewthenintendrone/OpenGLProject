#version 430 core
out vec4 FragColor;

in vec3 FragPos;
uniform vec3 Color;

void main()
{	
	// correct for gamma	
	float gamma = 2.2;
	
	FragColor = vec4(pow(Color, vec3(1.0/gamma)), 1);
}