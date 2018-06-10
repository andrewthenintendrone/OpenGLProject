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
void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord);
float LinearizeDepth(float depth);

const float near = 0.1;
const float far = 1000.0;

const float offset = 1.0 / 100.0;

void main()
{
	vec4 n[9];
	make_kernel(n, diffuseTexture, vTexCoords);

	vec4 sobel_edge_h = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);
  	vec4 sobel_edge_v = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

	float intensity = sobel.x + sobel.y + sobel.z;
	intensity /= 3.0;

	FragColor = vec4(vec3(1.0 - intensity), 1.0) * texture(diffuseTexture, vTexCoords);
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

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)
{
	float w = 1.0 / 1280.0;
	float h = 1.0 / 720.0;

	n[0] = texture2D(tex, coord + vec2( -w, -h));
	n[1] = texture2D(tex, coord + vec2(0.0, -h));
	n[2] = texture2D(tex, coord + vec2(  w, -h));
	n[3] = texture2D(tex, coord + vec2( -w, 0.0));
	n[4] = texture2D(tex, coord);
	n[5] = texture2D(tex, coord + vec2(  w, 0.0));
	n[6] = texture2D(tex, coord + vec2( -w, h));
	n[7] = texture2D(tex, coord + vec2(0.0, h));
	n[8] = texture2D(tex, coord + vec2(  w, h));
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