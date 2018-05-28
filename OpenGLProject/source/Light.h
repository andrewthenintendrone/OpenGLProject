#pragma once
#include <glm\vec3.hpp>

struct Light
{
	// ambient color
	glm::vec3 ambient = glm::vec3(0);

	// diffuse color
	glm::vec3 diffuse = glm::vec3(0);

	// specular color
	glm::vec3 specular = glm::vec3(0);
};

// directional light
struct DirectionalLight : public Light
{
	glm::vec3 direction = glm::vec3(0, -1, 0);
};

// point light
struct PointLight : public Light
{
	glm::vec3 position = glm::vec3(0);
};