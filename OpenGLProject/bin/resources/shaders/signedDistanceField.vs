// signed distance field shader
#version 430
layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TexCoords;

out vec2 vTexCoords;

uniform mat4 ProjectionViewModel;

void main()
{
	gl_Position = ProjectionViewModel * Position;
	vTexCoords = TexCoords;
}