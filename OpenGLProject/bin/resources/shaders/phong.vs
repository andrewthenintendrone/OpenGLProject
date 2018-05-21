// classic Phong vertex shader
#version 430
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoord;

out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

// used to transform normals
uniform mat3 NormalMatrix;

void main()
{
	vNormal = NormalMatrix * Normal.xyz;
	vTexCoord = TexCoord;
	gl_Position = ProjectionViewModel * Position;
}