// a simple shader
#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;

uniform mat4 ProjectionViewModel;
out vec4 vNormal;

void main()
{
	vNormal = Normal;
	gl_Position = ProjectionViewModel * Position;
}