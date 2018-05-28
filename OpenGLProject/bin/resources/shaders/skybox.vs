// a skybox shader
#version 430

layout(location = 0) in vec4 Position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = Position.xyz;
	gl_Position = projection * view * Position;
}