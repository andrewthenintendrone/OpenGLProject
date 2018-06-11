// phong shader
#version 430
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec4 Tangent;
layout(location = 4) in vec4 Color;

out vec4 vPosition;
out mat3 TBN;
out vec2 vTexCoords;
out vec4 vColor;

uniform mat4 ProjectionViewModel;

// used to transform position
uniform mat4 ModelMatrix;

// used to transform normals
uniform mat3 NormalMatrix;

void main()
{
	vPosition = ModelMatrix * Position;
	
	// calculate TBN in vertex shader for efficiency
	vec3 N = normalize(NormalMatrix * Normal.xyz);
	vec3 T = normalize(NormalMatrix * Tangent.xyz);
	vec3 B = cross(N, T) * Tangent.w;

	TBN = mat3(T, B, N);

	vTexCoords = TexCoords;
	vColor = Color;

	gl_Position = ProjectionViewModel * Position;
}