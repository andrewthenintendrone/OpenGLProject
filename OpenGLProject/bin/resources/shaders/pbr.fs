// a physically based shader
#version 430

const float e = 2.71828182845904523536028747135;
const float pi = 3.1415926535897932384626433832;

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
	float emissive;
	float roughness;
	float reflectionCoefficient;

	sampler2D diffuseTexture;
	sampler2D specularTexture;
	//sampler2D normalTexture;
	//sampler2D alphaTexture;
};
uniform Material material;

uniform vec3 cameraPosition;

out vec4 FragColor;

void main()
{
	// sample textures
	vec3 diffuseTexture = texture(material.diffuseTexture, vTexCoords).rgb;
	vec3 specularTexture = texture(material.specularTexture, vTexCoords).rgb;
	//vec3 normalTexture = texture(material.normalTexture, vTexCoords).rgb;
	//vec3 alphaTexture = texture(material.alphaTexture, vTexCoords).rgb * material.emissive;
	
	// tangent space normals
	//vec3 N = TBN * (normalTexture * 2 - 1);
	vec3 N = TBN[2];
	vec3 L = normalize(light.position - vPosition.xyz);
	vec3 E = normalize(cameraPosition - vPosition.xyz);
	vec3 H = normalize(L + E);

	// ambient lightning
	vec3 ambient = light.ambient * material.ambient;

	// diffuse lighting

	float NdL = max(0, dot(N, L));
	float NdE = max(0, dot(N, E));

	float R2 = pow(material.roughness, 2.0);

	// Oren-Nayar Diffuse Term
	float A = 1.0 - 0.5 * R2 / (R2 + 0.33);
	float B = 0.45 * R2 / (R2 + 0.09);

	// CX = Max(0, cos(l, e))
	vec3 lightProjected = normalize(L - N * NdL);
	vec3 viewProjected = normalize(E - N * NdE);
	float CX = max(0, dot(lightProjected, viewProjected));

	// DX = sin(alpha) * tan(beta)
	float alpha = sin(max(acos(NdE), acos(NdL)));
	float beta = tan(min(acos(NdE), acos(NdL)));
	float DX = alpha * beta;

	// Calculate Oren-Nayar, replaces the Phong Lambert Term
	float OrenNayar = NdL * (A + B * CX * DX);

	vec3 diffuse = light.diffuse * material.diffuse * OrenNayar;

	// specular lighting
	float NdH = max(0, dot(N, H));
	float NdH2 = pow(NdH, 2.0);

	// Beckman's Distribution Function D
	float exponent = -(1 - NdH2) / (NdH2 * R2);
	float D = pow(e, exponent) / (R2 * pow(NdH2, 2.0));

	// Fresnel Term F
	float F = material.reflectionCoefficient + (1.0 - material.reflectionCoefficient) * pow(1.0 - NdE, 5.0);

	// Geometric Attenuation Factor G
	float X = 2.0 * NdH / dot(E, H);
	float G = min(1, min(X * NdL, X * NdE));

	// Calculate Cook-Torrance
	float CookTorrance = max((D * G * F) / (NdE * pi), 0.0);

	vec3 specular = light.specular * material.specular * CookTorrance;


	// multiply by textures
	ambient *= diffuseTexture;
	diffuse *= diffuseTexture;
	specular *= specularTexture;

	// output final color
	FragColor = vec4(ambient + diffuse + specular, 1);
}
