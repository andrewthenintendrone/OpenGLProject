#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 ProjectionViewModel;

void main()
{
	TexCoords = aTexCoords;
    gl_Position = ProjectionViewModel * vec4(aPos, 1.0);
}