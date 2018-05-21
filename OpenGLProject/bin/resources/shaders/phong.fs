// classic Phong fragment shader
#version 430
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D diffuseTexture;
uniform vec3 lightDirection;

out vec4 FragColor;

void main()
{	
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(lightDirection);
	
	// calculate lambert term (negate light direction)
	float lambertTerm = max(0, min(1, dot(N, -L)));
	
	// sample texture
	vec4 tex = texture(diffuseTexture, vTexCoord);
	
	// output lambert mixed with texture
	FragColor = lambertTerm * tex;
	FragColor.a = 1;
}