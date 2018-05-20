#version 430 core
out vec4 FragColor;

in vec4 Normal;

void main()
{
	vec4 norm = normalize(Normal);
	norm.b += 0.5;
	FragColor = norm;
}