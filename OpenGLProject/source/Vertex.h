#pragma once
#include <glm\glm.hpp>

struct Vertex
{
	glm::vec4 position = glm::vec4(0, 0, 0, 1); // vertex position
	glm::vec4 normal = glm::vec4(0, 1, 0, 0); // vertex normal
	glm::vec2 texcoord = glm::vec2(0.5f, 0.5f); // vertex UV coordinates
	glm::vec4 tangent = glm::vec4(1, 0, 0, 1); // vertex tangent
	glm::vec4 color = glm::vec4(1, 1, 1, 1); // vertex color
};