#version 430 core
out vec4 FragColor;

in vec3 FragPos;
flat in vec3 Normal;

struct Material
{
	float diffuse;
	float specular;
	float shininess;
};

uniform Material material;

struct Light
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

uniform vec3 viewPos;

void main()
{	
	// ambient
	vec3 ambient = light.ambient * material.diffuse;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	 vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;
	
	vec3 result = ambient + diffuse + specular;
	
	// gamma correction
	float gamma = 2.2;
	result = pow(result, vec3(1.0 / gamma));
	
	FragColor = vec4(result, 1.0);
}