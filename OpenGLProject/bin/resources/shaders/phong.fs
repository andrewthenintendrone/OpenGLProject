// phong shader
#version 430

in vec4 vPosition;
in mat3 TBN;
in vec2 vTexCoords;
in vec4 vColor;

// point light
struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform PointLight pointLight;

// directional light
struct DirectionalLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight directionalLight;

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
	// transparency
	if(texture(material.diffuseTexture, vTexCoords).a < 0.1)
	{
		discard;
	}

	// sample textures
	vec3 ambientTexture = texture(material.ambientTexture, vTexCoords).rgb;
	vec3 diffuseTexture = texture(material.diffuseTexture, vTexCoords).rgb;
	vec3 specularTexture = texture(material.specularTexture, vTexCoords).rgb;
	vec3 specularHighlightTexture = texture(material.specularHighlightTexture, vTexCoords).rgb;
	vec3 normalTexture = texture(material.normalTexture, vTexCoords).rgb;

	// ambient lightning
	vec3 ambient = directionalLight.ambient * pointLight.ambient * material.ambient * ambientTexture;

	// use normals
	vec3 N = TBN * (normalTexture * 2.0 - 1.0);

	// diffuse lighting
	vec3 L = normalize(directionalLight.direction);
	float lambertTerm = max(dot(N, -L), 0.0);
	vec3 diffuse = directionalLight.diffuse * material.diffuse * lambertTerm * diffuseTexture;

	// specular lighting
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);
	float specularTerm = pow(max(dot(R, V), 0.0), material.specularPower);
	vec3 specular = directionalLight.specular * material.specular * specularTerm * specularTexture;

	// second lighting (point light)
	L = normalize(pointLight.position - vPosition.xyz);
	lambertTerm = max(dot(N, -L), 0.0);
	diffuse += pointLight.diffuse * material.diffuse * lambertTerm * diffuseTexture;
	diffuse /= 2.0;

	R = reflect(L, N);
	specularTerm = pow(max(dot(R, V), 0.0), material.specularPower);
	specular += pointLight.specular * material.specular * specularTerm * specularTexture;
	specular /= 2.0;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
