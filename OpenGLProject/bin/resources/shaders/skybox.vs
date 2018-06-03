// a skybox shader
#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;

out vec4 vPosition;
out vec4 vNormal;
out vec3 vTexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vTexCoords = Position.xyz;
	vNormal = Normal;
	vec4 pos = projection * view * vec4(Position.xyz, 1.0);
	vPosition = pos.xyww;
	gl_Position = pos.xyww;
}