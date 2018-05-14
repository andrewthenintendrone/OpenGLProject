#version 430 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material
{
	vec3 color;
	
	float ambient;
	float diffuse;
	float specular;
	
	float shininess;
};

uniform Material material;

uniform vec3 lightDirection;

uniform vec3 viewPos;

void main()
{	
	// ambient
	vec3 ambient = material.ambient * material.color;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-lightDirection);
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * material.diffuse * material.color;
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
	vec3 specular = vec3(material.specular * spec);
	
	vec3 result = (ambient + diffuse + specular);
	
	// correct gamma
	float gamma = 2.2;
	result = pow(result, vec3(1.0/gamma));
	
	FragColor = vec4(result, 1.0);
}