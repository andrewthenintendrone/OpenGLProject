#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{	
	// sample texture and call it a day
	FragColor = texture(texture_diffuse1, TexCoords);
}