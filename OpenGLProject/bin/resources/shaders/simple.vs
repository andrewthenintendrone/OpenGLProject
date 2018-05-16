// a simple shader
#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoord;

out vec4 vNormal;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

void main()
{
	vNormal = Normal;
	vTexCoord = TexCoord;
	gl_Position = ProjectionViewModel * Position;
}