#pragma once
#include <glm\glm.hpp>

class Vertex
{
public:
	glm::vec4 position = glm::vec4(0, 0, 0, 1);
	glm::vec2 texcoord = glm::vec2(0.5f, 0.5f);
	glm::vec4 normal = glm::vec4(0, 1, 0, 1);
};