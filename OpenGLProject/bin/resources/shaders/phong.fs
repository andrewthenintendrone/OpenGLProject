// phong shader
#version 430

in vec4 vPosition;
in mat3 TBN;
in vec2 vTexCoords;
in vec4 vColor;

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
	// colors
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;

	// textures
	sampler2D diffuseTexture;
	sampler2D alphaTexture;
	sampler2D ambientTexture;
	sampler2D specularTexture;
	sampler2D specularHighlightTexture;
	sampler2D normalTexture;
	sampler2D displacementTexture;

	// properties
	float specularPower;
	float opacity;
};
uniform Material material;

uniform vec3 cameraPosition;

out vec4 FragColor;

void main()
{
	// sample textures
	vec3 ambientTexture = texture(material.ambientTexture, vTexCoords).rgb;
	vec3 diffuseTexture = texture(material.diffuseTexture, vTexCoords).rgb;
	vec3 specularTexture = texture(material.specularTexture, vTexCoords).rgb;
	vec3 specularHighlightTexture = texture(material.specularHighlightTexture, vTexCoords).rgb;
	vec3 normalTexture = texture(material.normalTexture, vTexCoords).rgb;

	// ambient lightning
	vec3 ambient = light.ambient * material.ambient * ambientTexture;

	// use normals
	vec3 N = TBN[2];

	// diffuse lighting
	vec3 L = normalize(vPosition.xyz - light.position);
	float lambertTerm = max(0, min(1, dot(N, -L)));
	vec3 diffuse = light.diffuse * material.diffuse * lambertTerm;

	// specular lighting
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);
	float specularTerm = pow(max(0, dot(R, V)), material.specularPower);
	vec3 specular = light.specular * material.specular * specularTerm;
	
	// output final color
	FragColor = vec4(ambient + diffuse + specular, 1);
}
