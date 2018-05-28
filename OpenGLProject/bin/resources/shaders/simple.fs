// a simple flat color shader
#version 430

out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D diffuseTexture;

uniform vec3 color = vec3(1, 0, 1);

void main()
{	
	FragColor = texture(diffuseTexture, vTexCoords);
}