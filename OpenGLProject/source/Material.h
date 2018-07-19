#pragma once
#include <glm\glm.hpp>
#include "Texture.h"
#include "Shader.h"

struct Material
{
	// lighting
	glm::vec3 ambient = glm::vec3(1.0f); // ambient color
	glm::vec3 diffuse = glm::vec3(1.0f); // diffuse color
	glm::vec3 specular = glm::vec3(1.0f); // specular color
	glm::vec3 emissive = glm::vec3(0.0f); // emissive color

	float specularPower = 128.0f; // specular power (for phong lighting)
	float opacity = 1.0f; // opacity (not implemented)
	bool useNormalMap = true; // use normal map?

	float roughness = 0.5f; // roughness (for physically based lighting)
	float reflectionCoefficient = 0.5f; // reflection coefficient (for physically based lighting)

	// textures
	Texture diffuseTexture; // 0
	Texture alphaTexture; // 1
	Texture ambientTexture; // 2
	Texture specularTexture; // 3
	Texture specularHighlightTexture; // 4
	Texture normalTexture; // 5
	Texture displacementTexture; // 6
	Texture emissiveTexture; // 7

	// create dummy textures
	void createDummyTextures()
	{
		diffuseTexture.createDummy(Color::White());
		alphaTexture.createDummy(Color::White());
		ambientTexture.createDummy(Color::White());
		specularTexture.createDummy(Color::Black());
		specularHighlightTexture.createDummy(Color::Black());
		normalTexture.createDummy(Color(128, 128, 255, 255));
		displacementTexture.createDummy(Color::Black());
		emissiveTexture.createDummy(Color::Black());
	};

	// send material information to a shader
	void bind(Shader shader)
	{
		shader.setVec3("material.ambient", ambient);
		shader.setVec3("material.diffuse", diffuse);
		shader.setVec3("material.specular", specular);
		shader.setVec3("material.emissive", emissive);

		shader.setFloat("material.specularPower", specularPower);
		shader.setFloat("material.opacity", opacity);

		shader.setBool("material.useNormalMap", useNormalMap);

		shader.setFloat("material.roughness", roughness);
		shader.setFloat("material.reflectionCoefficient", reflectionCoefficient);

		// set textures
		shader.setInt("material.diffuseTexture", 0);
		shader.setInt("material.alphaTexture", 1);
		shader.setInt("material.ambientTexture", 2);
		shader.setInt("material.specularTexture", 3);
		shader.setInt("material.specularHighlightTexture", 4);
		shader.setInt("material.normalTexture", 5);
		shader.setInt("material.displacementTexture", 6);
		shader.setInt("material.emissiveTexture", 7);

		diffuseTexture.bind(0);
		alphaTexture.bind(1);
		ambientTexture.bind(2);
		specularTexture.bind(3);
		specularHighlightTexture.bind(4);
		normalTexture.bind(5);
		displacementTexture.bind(6);
		emissiveTexture.bind(7);
	}
};