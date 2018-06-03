// a skybox shader
#version 430

out vec4 FragDiffuse;
out vec4 FragSpecular;
out vec4 FragNormal;
out vec4 FragAlpha;

in vec4 vPosition;
in vec4 vNormal;
in vec3 vTexCoords;
uniform samplerCube skybox;

void main()
{
	//FragPosition = vPosition;
	FragDiffuse = texture(skybox, vTexCoords);
	FragSpecular = vec4(vec3(0), 1);
	FragNormal = vNormal;
	FragAlpha = vec4(vec3(0.0), 1.0);
}