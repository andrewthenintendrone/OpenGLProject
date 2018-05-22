// a normal map shader
#version 430

in vec4 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoords;

struct Light
{
	vec3 direction;
	
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
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);

	mat3 TBN = mat3(T, B, N);

	N = TBN * (normalTexture * 2 - 1);
	
	// ambient lightning
	vec3 ambient = light.ambient * material.ambient * diffuseTexture;

	// diffuse lighting
	vec3 L = normalize(light.direction);
	
	float lambertTerm = max(0, min(1, dot(N, -L)));
	vec3 diffuse = light.diffuse * material.diffuse * lambertTerm * diffuseTexture;

	// specular lighting
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);
	float specularTerm = pow(max(0, dot(R, V)), material.specularPower);
	vec3 specular = light.specular * material.specular * specularTerm * specularTexture;
	
	// output final color
	FragColor = vec4(ambient + diffuse + specular, 1);
}
