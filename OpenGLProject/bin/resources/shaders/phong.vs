// classic Phong vertex shader
#version 430
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
//layout(location = 2) in vec2 TexCoords;

out vec4 vPosition;
out vec3 vNormal;
//out vec2 vTexCoords;

uniform mat4 ProjectionViewModel;

// used to transform position
uniform mat4 ModelMatrix;

// used to transform normals
uniform mat3 NormalMatrix;

void main()
{
	vPosition = ModelMatrix * Position;
	vNormal = NormalMatrix * Normal.xyz;
	//vTexCoords = TexCoords;
	gl_Position = ProjectionViewModel * Position;
}