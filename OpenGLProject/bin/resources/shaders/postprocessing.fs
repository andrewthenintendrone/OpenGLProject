// a post processing shader
#version 430

out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D alphaTexture;

uniform float time;

// effects
vec4 BoxBlur();
vec4 Distort();
vec4 Pixelate(int pixelSizeX, int pixelSizeY);
vec4 Invert();
float LinearizeDepth(float depth);

const float near = 0.1;
const float far = 1000.0;

const float offset = 1.0 / 100.0;

void main()
{
	FragColor = texture(diffuseTexture, vTexCoords) + BoxBlur();
}

vec4 Distort()
{
	vec2 mid = vec2(0.5f);
	float distanceFromCentre = distance(vTexCoords, mid);
	vec2 normalizedCoord = normalize(vTexCoords - mid);
	float bias = distanceFromCentre +
	sin(distanceFromCentre * 15) * 0.05f;
	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(diffuseTexture, newCoord);
}

vec4 Pixelate(int pixelSizeX, int pixelSizeY)
{
	vec2 samplePos = vec2(floor(vTexCoords.x * pixelSizeX) / pixelSizeX, floor(vTexCoords.y * pixelSizeY) / pixelSizeY);
	return texture(diffuseTexture, samplePos);
}

vec4 Invert()
{
	return vec4(vec3(1) - texture(diffuseTexture, vTexCoords).xyz, 1.0);
}

float LinearizeDepth(float depth)
{
	// convert to NDC
	float z = depth * 2.0 - 1.0;

	return (2.0 * near * far) / (far + near - z * (far - near));
}

vec4 BoxBlur()
{
	vec2 texel = 1.0f / textureSize(alphaTexture, 0);
	// 9-tap box kernel
	vec4 color = texture(alphaTexture, vTexCoords);
	color += texture(alphaTexture, vTexCoords + texel * vec2(-1,1));
	color += texture(alphaTexture, vTexCoords + texel * vec2(-1,0));
	color += texture(alphaTexture, vTexCoords + texel * vec2(-1,-1));
	color += texture(alphaTexture, vTexCoords + texel * vec2(0,1));
	color += texture(alphaTexture, vTexCoords + texel * vec2(0,-1));
	color += texture(alphaTexture, vTexCoords + texel * vec2(1,1));
	color += texture(alphaTexture, vTexCoords + texel * vec2(1,0));
	color += texture(alphaTexture, vTexCoords + texel * vec2(1,-1));
	return color / 9;
}