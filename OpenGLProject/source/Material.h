#pragma once
#include <glm\glm.hpp>
#include "Texture.h"

struct Material
{
	// constructor / destructor
	Material() : ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1) {}
	~Material() {}

	// lighting
	glm::vec3 ambient; // ambient light
	glm::vec3 diffuse; // diffuse light
	glm::vec3 specular; // specular light
	glm::vec3 emissive; // emissive light

	float specularPower;
	float opacity;

	// textures
	Texture diffuseTexture;
	Texture alphaTexture;
	Texture ambientTexture;
	Texture specularTexture;
	Texture specularHighlightTexture;
	Texture normalTexture;
	Texture displacementTexture;
};