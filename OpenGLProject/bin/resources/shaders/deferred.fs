// deferred shader that passes on data into render buffers
#version 430

in vec4 vPosition;
in mat3 TBN;
in vec2 vTexCoords;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;

	float roughness;
	float reflectionCoefficient;

	sampler2D diffuseTexture;
	sampler2D specularTexture;
	sampler2D normalTexture;
	sampler2D alphaTexture;
};
uniform Material material;

uniform float time;

uniform vec3 cameraPosition;

out vec4 FragDiffuse;
out vec4 FragSpecular;
out vec4 FragNormal;
out vec4 FragAlpha;

void main()
{
	FragDiffuse = texture(material.diffuseTexture, vTexCoords) * vec4(material.diffuse, 1.0);
	FragSpecular = texture(material.specularTexture, vTexCoords) * vec4(material.specular, 1.0);
	FragNormal = vec4(TBN * (texture(material.normalTexture, vTexCoords).xyz * 0.5 + 0.5), 1.0);
	FragAlpha = texture(material.alphaTexture, vTexCoords) * vec4(material.emissive, 1.0);
}
