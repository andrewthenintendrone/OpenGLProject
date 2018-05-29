// a skybox shader
#version 430

layout(location = 0) in vec4 Position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = Position.xyz;
	vec4 pos = projection * view * vec4(Position.xyz, 1.0);
	gl_Position = pos.xyww;
}