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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{	
	// sample normal map
	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// sample color
	vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;

	// ambient
	vec3 ambient = 0.1 * color;

	// diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	// sample specular map
	float specmap = texture(texture_specular1, fs_in.TexCoords).r;

	// specular
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), specmap * 32.0);

	vec3 specular = vec3(0.2) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}