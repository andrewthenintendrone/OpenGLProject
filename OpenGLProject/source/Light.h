#pragma once
#include <glm\vec3.hpp>
#include "Shader.h"

// generic Light struct
struct Light
{
	virtual void bind(Shader shader, int index) = 0;

	glm::vec3 ambient = glm::vec3(1);
	glm::vec3 diffuse = glm::vec3(1);
	glm::vec3 specular = glm::vec3(1);
};

// directional light
struct DirectionalLight : public Light
{
	virtual void bind(Shader shader, int index)
	{
		shader.setVec3(std::string("directionalLights[" + std::to_string(index) + "].ambient").c_str(), ambient);
		shader.setVec3(std::string("directionalLights[" + std::to_string(index) + "].diffuse").c_str(), diffuse);
		shader.setVec3(std::string("directionalLights[" + std::to_string(index) + "].specular").c_str(), specular);
		shader.setVec3(std::string("directionalLights[" + std::to_string(index) + "].direction").c_str(), direction);
	}

	glm::vec3 direction = glm::vec3(0, -1, 0);
};

// point light
struct PointLight : public Light
{
	virtual void bind(Shader shader, int index)
	{
		shader.setVec3(std::string("pointLights[" + std::to_string(index) + "].ambient").c_str(), ambient);
		shader.setVec3(std::string("pointLights[" + std::to_string(index) + "].diffuse").c_str(), diffuse);
		shader.setVec3(std::string("pointLights[" + std::to_string(index) + "].specular").c_str(), specular);
		shader.setVec3(std::string("pointLights[" + std::to_string(index) + "].position").c_str(), position);
		shader.setFloat(std::string("pointLights[" + std::to_string(index) + "].falloffDistance").c_str(), falloffDistance);
	}

	glm::vec3 position = glm::vec3(0);
	float falloffDistance = 10.0f;
};

// spot light
struct SpotLight : public Light
{
	virtual void bind(Shader shader, int index)
	{
		shader.setVec3(std::string("spotLights[" + std::to_string(index) + "].ambient").c_str(), ambient);
		shader.setVec3(std::string("spotLights[" + std::to_string(index) + "].diffuse").c_str(), diffuse);
		shader.setVec3(std::string("spotLights[" + std::to_string(index) + "].specular").c_str(), specular);
		shader.setVec3(std::string("spotLights[" + std::to_string(index) + "].position").c_str(), position);
		shader.setFloat(std::string("spotLights[" + std::to_string(index) + "].falloffDistance").c_str(), falloffDistance);
		shader.setFloat(std::string("spotLights[" + std::to_string(index) + "].theta").c_str(), theta);
		shader.setFloat(std::string("spotLights[" + std::to_string(index) + "].phi").c_str(), phi);
	}

	glm::vec3 position = glm::vec3(0);
	float falloffDistance = 10.0f;
	float theta = glm::radians(30.0f);
	float phi = glm::radians(60.0f);
};