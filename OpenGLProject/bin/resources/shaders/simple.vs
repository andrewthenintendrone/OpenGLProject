// a simple flat color shader
#version 430

layout(location = 0) in vec4 Position;

uniform mat4 ProjectionViewModel;

void main()
{
	gl_Position = ProjectionViewModel * Position;
}