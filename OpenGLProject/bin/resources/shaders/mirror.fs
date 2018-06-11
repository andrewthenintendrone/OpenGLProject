// a mirror shader
#version 430

in vec4 vPosition;
in mat3 TBN;
in vec2 vTexCoords;

uniform samplerCube skybox;

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

out vec4 FragColor;

void main()
{
	// index of refraction (water)
	float ratio = 1.0 / 1.33;

	vec3 normalTexture = texture(material.normalTexture, vTexCoords).rgb;
	vec3 N = TBN * (normalTexture * 2 - 1);

	vec3 I = normalize(vPosition.xyz - cameraPosition);

	// reflect
	vec3 R = reflect(I, normalize(N));

	// refract
	//vec3 R = refract(I, normalize(N), ratio);

	// sample skybox
	vec3 skyColor = texture(skybox, R).rgb;

	// output final color
	FragColor = vec4(skyColor, 1);
}
