// a simple flat color shader
#version 430

out vec4 FragColor;

uniform vec3 Kd = vec3(1, 0, 1);

void main()
{	
	FragColor = vec4(Kd, 1);
}