// a simple flat color shader
#version 430

out vec4 FragColor;

uniform vec4 color = vec4(1, 0, 1, 1);

void main()
{	
	FragColor = color;
}