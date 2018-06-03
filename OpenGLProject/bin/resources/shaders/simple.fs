// a simple flat color shader
#version 430

out vec4 FragColor;

uniform vec3 color = vec3(1, 0, 1);

void main()
{
	FragColor = vec4(color, 1);
}