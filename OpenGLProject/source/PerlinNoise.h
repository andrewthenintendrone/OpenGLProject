#pragma once
#include <glm\glm.hpp>
#include "Array2D.h"

class PerlinNoise
{
public:

	static PerlinNoise& getInstance();

	float perlin(float x, float y);
	float octavePerlin(float x, float y, int octaves, float persistance);

private:

	PerlinNoise();

	float lerp(float a, float b, float i);
	float smoothStep(float f);
	float dotGridGradient(int ix, int iy, float x, float y);

	void defineGrid();

	Array2D<glm::vec2> m_vectorGrid;
};