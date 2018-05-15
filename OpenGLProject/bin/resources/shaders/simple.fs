// a simple flat color shader
#version 430

in vec4 vNormal;
out vec4 FragColor;

uniform vec4 glowColor;

void main()
{
	FragColor = vNormal * glowColor;
}