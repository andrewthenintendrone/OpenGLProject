#pragma once
#include <glm\vec3.hpp>

// generic Light struct
struct Light
{
	glm::vec3 ambient = glm::vec3(1);
	glm::vec3 diffuse = glm::vec3(1);
	glm::vec3 specular = glm::vec3(1);
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
	float falloffDistance = 10.0f;
};

// spot light
struct SpotLight : public Light
{
	glm::vec3 position = glm::vec3(0);
	float falloffDistance = 10.0f;
	float theta = glm::radians(30.0f);
	float phi = glm::radians(60.0f);
};