#pragma once
#include <glm\glm.hpp>
#include "Texture.h"

struct Material
{
	// lighting
	glm::vec3 ambient = glm::vec3(1.0f); // ambient color
	glm::vec3 diffuse = glm::vec3(1.0f); // diffuse color
	glm::vec3 specular = glm::vec3(1.0f); // specular color
	glm::vec3 emissive = glm::vec3(0.0f); // emissive color

	float specularPower = 32.0f; // specular power (for phong lighting)
	float opacity = 1.0f; // opacity (not implemented)

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

	// create dummy textures
	void createDummyTextures()
	{
		diffuseTexture.createDummy(Color::White());
		alphaTexture.createDummy(Color::White());
		ambientTexture.createDummy(Color::White());
		specularTexture.createDummy(Color::White());
		specularHighlightTexture.createDummy(Color::White());
		normalTexture.createDummy(Color(128, 128, 255, 255));
		displacementTexture.createDummy(Color::Black());
	};
};