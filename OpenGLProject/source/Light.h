#pragma once
#include <glm\vec3.hpp>

struct Light
{
	// direction that the light is facing
	glm::vec3 direction = glm::vec3(0);

	// ambient color
	glm::vec3 ambient = glm::vec3(0);

	// diffuse color
	glm::vec3 diffuse = glm::vec3(0);

	// specular color
	glm::vec3 specular = glm::vec3(0);
};