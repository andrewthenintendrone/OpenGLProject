// a simple flat color shader
#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoords;

uniform mat4 ProjectionViewModel;

out vec2 vTexCoords;

void main()
{
	vTexCoords = TexCoords;
	gl_Position = ProjectionViewModel * Position;
	gl_PointSize = 3;
}