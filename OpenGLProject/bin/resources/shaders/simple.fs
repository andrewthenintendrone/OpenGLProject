// a simple flat color shader
#version 430

in vec4 vNormal;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D colorTexture;

void main()
{	
	FragColor = texture(colorTexture, vTexCoord);
}