// phong shader
#version 430

in vec4 vPosition;
in mat3 TBN;
in vec2 vTexCoords;
in vec4 vColor;

uniform bool correctGamma;

// point light(s)
uniform int pointLightCount;
struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform PointLight pointLights[20];

// directional light(s)
uniform int directionalLightCount;
struct DirectionalLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight directionalLights[20];

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
	sampler2D emissiveTexture;

	// properties
	float specularPower;
	float opacity;
	bool useNormalMap;
};
uniform Material material;

uniform vec3 cameraPosition;

out vec4 FragColor;

vec4 gammaCorrection(vec4 color);

void main()
{
	// transparency
	if(texture(material.diffuseTexture, vTexCoords).a < 0.5)
	{
		discard;
	}

	// sample textures
	vec3 diffuseTexture = texture(material.diffuseTexture, vTexCoords).rgb;
	vec3 alphaTexture = texture(material.alphaTexture, vTexCoords).rgb;
	vec3 ambientTexture = texture(material.ambientTexture, vTexCoords).rgb;
	vec3 specularTexture = texture(material.specularTexture, vTexCoords).rgb;
	vec3 specularHighlightTexture = texture(material.specularHighlightTexture, vTexCoords).rgb;
	vec3 normalTexture = texture(material.normalTexture, vTexCoords).rgb;
	vec3 displacementTexture = texture(material.displacementTexture, vTexCoords).rgb;
	vec3 emissiveTexture = texture(material.emissiveTexture, vTexCoords).rgb;

	// ambient lighting
	vec3 ambient = material.ambient * ambientTexture;

	// use normals
	vec3 N;

	if(material.useNormalMap)
	{
		N = (normalTexture * 2.0 - 1.0);
	}
	else
	{
		N = TBN[2];
	}

	// direction from fragment position to camera position
	vec3 V = normalize(cameraPosition - vPosition.xyz);

	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	// point lights
	for(int i = 0; i < pointLightCount; i++)
	{
		// diffuse lighting
		// direction from fragment position to light position
		vec3 L = normalize(pointLights[i].position - vPosition.xyz);
		float lambertTerm = max(dot(N, L), 0.0);
		diffuse += pointLights[i].diffuse * material.diffuse * lambertTerm * diffuseTexture;

		// specular lighting
		vec3 R = reflect(-L, N);
		float specularTerm = pow(max(dot(R, V), 0.0), material.specularPower);
		specular += pointLights[i].specular * material.specular * specularTerm * specularTexture;
	}

	// directional lights
	for(int i = 0; i < directionalLightCount; i++)
	{
		// diffuse lighting
		vec3 L = normalize(-directionalLights[i].direction);
		float lambertTerm = max(dot(N, L), 0.0);
		diffuse += directionalLights[i].diffuse * material.diffuse * lambertTerm * diffuseTexture;

		// specular lighting
		vec3 R = reflect(-L, N);
		float specularTerm = pow(max(dot(R, V), 0.0), material.specularPower);
		specular += directionalLights[i].specular * material.specular * specularTerm * specularTexture;
	}

	FragColor = vec4(ambient + diffuse + specular + (emissiveTexture * vec3(1, 0, 0)), 1.0);

	// gamma correction (if needed)
	if(correctGamma)
	{
		FragColor = gammaCorrection(FragColor);
	}
}

// applys gamma correction
vec4 gammaCorrection(vec4 color)
{
	return vec4(pow(color.xyz, vec3(1.0 / 2.2)), color.w);
}
