#pragma once
#include <glm\vec3.hpp>
#include "Shader.h"

// generic Light struct
struct Light
{
	virtual void bind(Shader shader) = 0;

	glm::vec3 ambient = glm::vec3(1);
	glm::vec3 diffuse = glm::vec3(1);
	glm::vec3 specular = glm::vec3(1);
};

// directional light
struct DirectionalLight : public Light
{
	virtual void bind(Shader shader)
	{
		shader.setVec3("directionalLight.ambient", ambient);
		shader.setVec3("directionalLight.diffuse", diffuse);
		shader.setVec3("directionalLight.specular", specular);
		shader.setVec3("directionalLight.direction", direction);
	}

	glm::vec3 direction = glm::vec3(0, -1, 0);
};

// point light
struct PointLight : public Light
{
	virtual void bind(Shader shader)
	{
		shader.setVec3("pointLight.ambient", ambient);
		shader.setVec3("pointLight.diffuse", diffuse);
		shader.setVec3("pointLight.specular", specular);
		shader.setVec3("pointLight.position", position);
		shader.setFloat("pointLight.falloffDistance", falloffDistance);
	}

	glm::vec3 position = glm::vec3(0);
	float falloffDistance = 10.0f;
};

// spot light
struct SpotLight : public Light
{
	virtual void bind(Shader shader)
	{
		shader.setVec3("pointLight.ambient", ambient);
		shader.setVec3("pointLight.diffuse", diffuse);
		shader.setVec3("pointLight.specular", specular);
		shader.setVec3("pointLight.position", position);
		shader.setFloat("pointLight.falloffDistance", falloffDistance);
		shader.setFloat("pointLight.theta", theta);
		shader.setFloat("pointLight.phi", phi);
	}

	glm::vec3 position = glm::vec3(0);
	float falloffDistance = 10.0f;
	float theta = glm::radians(30.0f);
	float phi = glm::radians(60.0f);
};