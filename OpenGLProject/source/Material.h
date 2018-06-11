#pragma once
#include <glm\glm.hpp>
#include "Texture.h"

struct Material
{
	// constructor / destructor
	Material() : ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1) {}
	~Material() {}

	// lighting
	glm::vec3 ambient; // ambient color
	glm::vec3 diffuse; // diffuse color
	glm::vec3 specular; // specular color
	glm::vec3 emissive; // emissive color

	float specularPower; // specular power (for phong lighting)
	float opacity; // opacity (not implemented)

	float roughness; // roughness (for physically based lighting)
	float reflectionCoefficient; // reflection coefficient (for physically based lighting)

	// textures
	Texture diffuseTexture; // 0
	Texture alphaTexture; // 1
	Texture ambientTexture; // 2
	Texture specularTexture; // 3
	Texture specularHighlightTexture; // 4
	Texture normalTexture; // 5
	Texture displacementTexture; // 6
};