// a physically based shader
#version 430

const float e = 2.71828182845904523536028747135;
const float pi = 3.1415926535897932384626433832;

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
	float roughness;
	float reflectionCoefficient;
	float opacity;
	bool useNormalMap;
};
uniform Material material;

uniform vec3 cameraPosition;

out vec4 FragColor;

float OrenNayer(vec3 E, vec3 N, vec3 L);
float CookTorrance(vec3 E, vec3 N, vec3 L);

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

	vec3 E = normalize(cameraPosition - vPosition.xyz);

	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 L = normalize(vPosition.xyz - pointLights[i].position);

		diffuse += OrenNayer(E, N, L) * pointLights[i].diffuse * material.diffuse * diffuseTexture;

		specular += CookTorrance(E, N, L) * pointLights[i].specular * material.specular * specularTexture;
	}

	for(int i = 0; i < directionalLightCount; i++)
	{
		vec3 L = normalize(-directionalLights[i].direction);

		diffuse += OrenNayer(E, N, L) * directionalLights[i].diffuse * material.diffuse * diffuseTexture;

		specular += CookTorrance(E, N, L) * directionalLights[i].specular * material.specular * specularTexture;
	}

	FragColor = vec4(ambient + diffuse + specular, 1.0);

	// gamma correction (if needed)
	if(correctGamma)
	{
		FragColor = gammaCorrection(FragColor);
	}
}

// calculates Oren-Nayer Diffuse Reflectance
float OrenNayer(vec3 E, vec3 N, vec3 L)
{
	float NdL = max(0.0f, dot(N, L));
	float NdE = max(0.0f, dot(N, E));

	float R2 = material.roughness * material.roughness;

	// Oren-Nayer Diffuse Term
	float A = 1.0 - 0.5 * R2 / (R2 + 0.33);
	float B = 0.45 * R2 / (R2 + 0.09);

	// CX = Max(0, cos(l, e))
	vec3 lightProjected = normalize(L - N * NdL);
	vec3 viewProjected = normalize(E - N * NdE);
	float CX = max(0.0, dot(lightProjected, viewProjected));

	// DX = sin(alpha) * tan(beta)
	float alpha = sin(max(acos(NdE), acos(NdL)));
	float beta = tan(min(acos(NdE), acos(NdL)));
	float DX = alpha * beta;

	// Calculate Oren-Nayer
	float OrenNayer = NdL * (A + B * CX * DX);

	return OrenNayer;
}

// calculates Cook-Torrance Specular Reflectance
float CookTorrance(vec3 E, vec3 N, vec3 L)
{
	vec3 H = normalize(L + E);
	float R2 = material.roughness * material.roughness;

	float NdL = max(0.0, dot(N, L));
	float NdE = max(0.0, dot(N, E));
	float NdH = max(0.0, dot(N, H));
	float NdH2 = NdH * NdH;

	// Beckman's Distribution Function D
	float exponent = -(1 - NdH2) / (NdH2 * R2);
	float D = pow(e, exponent) / (R2 * NdH2 * NdH2);

	// Fresnel Term F
	float F = material.reflectionCoefficient + (1.0 - material.reflectionCoefficient) * pow(1 - NdE, 5);

	// Geometric Attenuation Factor G
	float X = 2.0 * NdH / dot(E, H);
	float G = min(1, min(X * NdL, X * NdE));

	// Calculate Cook-Torrance
	float CookTorrance = max((D * G * F) / (NdE * pi), 0.0);

	return CookTorrance;
}

// applys gamma correction
vec4 gammaCorrection(vec4 color)
{
	return vec4(pow(color.xyz, vec3(1.0 / 2.2)), color.w);
}