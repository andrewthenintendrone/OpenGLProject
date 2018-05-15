#version 430 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Material
{
	vec3 color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

uniform Material material;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_emissive1;

uniform vec3 lightPosition;
uniform vec3 viewPos;

void main()
{	
	// sample normal map
	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	// sample diffuse map
	vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	
	// sample other map
	float tcl = texture(texture_emissive1, fs_in.TexCoords).b;
	
	color = color + (tcl * material.color);
	
	// ambient
	vec3 ambient = material.ambient * color;
	
	// diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = material.diffuse * diff * color;
	
	// specular
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	vec3 specular = vec3(material.specular) * spec;
	
	vec3 result = ambient + diffuse;
	
	// gamma correction
	// float gamma = 2.2;
	// result = pow(result, vec3(1.0 / gamma));
	
	FragColor = vec4(result, 1.0);
}