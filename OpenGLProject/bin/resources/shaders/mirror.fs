// a mirror shader
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

	sampler2D diffuseTexture;
	sampler2D specularTexture;
	sampler2D normalTexture;

	float specularPower;
};

uniform Material material;
uniform vec3 cameraPosition;
uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
	vec3 normalTexture = texture(material.normalTexture, vTexCoords).rgb;
	vec3 N = TBN * (normalTexture * 2 - 1);

	vec3 I = normalize(vPosition.xyz - cameraPosition);
	vec3 R = reflect(I, normalize(N));

	// output final color
	FragColor = vec4(texture(skybox, R).rgb, 1);
}
