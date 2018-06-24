// signed distance field shader
#version 430

in vec2 vTexCoords;

struct Material
{
	sampler2D diffuseTexture;
	vec3 color;
};
uniform Material material;

void main()
{
	// sample texture
	float dist = texture(material.diffuseTexture, vTexCoords).r;

	if(pow(dist, 0.5) < 0.5)
	{
		discard;
	}

	gl_FragColor = vec4(material.color, 1.0);
}
