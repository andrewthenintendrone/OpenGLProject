// a normal map shader
#version 430

in vec4 vPosition;
in mat3 TBN;
in vec2 vTexCoords;

struct Light
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

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
	// sample textures
	vec3 diffuseTexture = texture(material.diffuseTexture, vTexCoords).rgb;
	vec3 specularTexture = texture(material.specularTexture, vTexCoords).rgb;
	vec3 normalTexture = texture(material.normalTexture, vTexCoords).rgb;
	
	// tangent space normals
	//vec3 N = TBN * (normalTexture * 2 - 1);
	vec3 N = TBN[2];
	
	// ambient lightning
	vec3 ambient = light.ambient * material.ambient;

	// diffuse lighting
	vec3 L = normalize(vPosition.xyz - light.position);
	
	float lambertTerm = max(0, min(1, dot(N, -L)));
	vec3 diffuse = light.diffuse * material.diffuse * lambertTerm;

	// specular lighting
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);
	float specularTerm = pow(max(0, dot(R, V)), material.specularPower);
	vec3 specular = light.specular * material.specular * specularTerm;
	
	// multiply by textures
	ambient *= diffuseTexture;
	diffuse *= diffuseTexture;
	specular *= specularTexture;

	// output final color
	FragColor = vec4(ambient + diffuse + specular, 1);
}
