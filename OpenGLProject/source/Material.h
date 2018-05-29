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

	float specularPower;
	float opacity;

	// textures
	Texture diffuseTexture; // diffuse texture
	Texture specularTexture; // specular texture
	Texture normalTexture; // normal texture
	Texture alphaTexture; // alpha texture
};