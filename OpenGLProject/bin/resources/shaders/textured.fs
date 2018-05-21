// a simple textured shader
#version 430

in vec2 vTexCoord;

uniform sampler2D diffuseTexture;

out vec4 FragColor;

void main()
{	
	FragColor = texture(diffuseTexture, vTexCoord);
}