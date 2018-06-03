// classic Phong fragment shader
#version 430

in vec4 vPosition;
in vec3 vNormal;
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

	sampler2D diffuseTexture;
	sampler2D specularTexture;

	float specularPower;
};
uniform Material material;

uniform vec3 cameraPosition;

out vec4 FragColor;

void main()
{
	// sample diffuse texture
	vec3 diffuseTexture = texture(material.diffuseTexture, vTexCoords).rgb;

	// sample specular
	vec3 specularTexture = texture(material.specularTexture, vTexCoords).rgb;

	// ambient lightning
	vec3 ambient = light.ambient * material.ambient * diffuseTexture * 0.25;

	// diffuse lighting
	vec3 N = normalize(vNormal);
	vec3 L = normalize(vPosition.xyz - light.position);
	float lambertTerm = max(0, min(1, dot(N, -L)));
	vec3 diffuse = light.diffuse * material.diffuse * lambertTerm * diffuseTexture;

	// specular lighting
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);
	float specularTerm = pow(max(0, dot(R, V)), material.specularPower);
	vec3 specular = light.specular * material.specular * specularTerm;
	
	// output final color
	FragColor = vec4(ambient + diffuse + specular, 1);
}
