#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuseTexture;

void main()
{	
	// sample texture and call it a day
	FragColor = texture(diffuseTexture, TexCoords);
}