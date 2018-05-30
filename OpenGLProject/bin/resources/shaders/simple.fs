// a simple flat color shader
#version 430

out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D colorTexture;

uniform float time;

uniform vec3 color = vec3(1, 0, 1);

vec4 BoxBlur();
vec4 Distort(vec2 texcoord);

void main()
{
	float pixelSizeX = 160;
	float pixelSizeY = 144;
	vec2 samplePos = vec2(floor(vTexCoords.x * pixelSizeX) / pixelSizeX, floor(vTexCoords.y * pixelSizeY) / pixelSizeY);
	FragColor = Distort(samplePos) * vec4(vec3(139, 172, 15) / 255.0, 1.0);
}

// simple box blur
vec4 BoxBlur()
{
	vec2 texel = 1.0f / textureSize(colorTexture, 0);

	// 9-tap box kernel
	vec4 colour = texture(colorTexture, vTexCoords);
	colour += texture(colorTexture, vTexCoords + texel * vec2(-1,1));
	colour += texture(colorTexture, vTexCoords + texel * vec2(-1,0));
	colour += texture(colorTexture, vTexCoords + texel * vec2(-1,-1));
	colour += texture(colorTexture, vTexCoords + texel * vec2(0,1));
	colour += texture(colorTexture, vTexCoords + texel * vec2(0,-1));
	colour += texture(colorTexture, vTexCoords + texel * vec2(1,1));
	colour += texture(colorTexture, vTexCoords + texel * vec2(1,0));
	colour += texture(colorTexture, vTexCoords + texel * vec2(1,-1));

	return colour / 9;
}

vec4 Distort(vec2 texcoord)
{
	vec2 mid = vec2(0.5f);
	float distanceFromCentre = distance(texcoord, mid);
	vec2 normalizedCoord = normalize(texcoord - mid);
	float bias = distanceFromCentre +
	sin(distanceFromCentre * 15) * 0.05f;
	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(colorTexture, newCoord);
}