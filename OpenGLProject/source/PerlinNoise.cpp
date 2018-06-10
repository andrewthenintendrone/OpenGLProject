#include "PerlinNoise.h"
#include <random>
#include <cmath>

PerlinNoise::PerlinNoise()
{
	defineGrid();
}

PerlinNoise& PerlinNoise::getInstance()
{
	static PerlinNoise instance;
	return instance;
}

// Function to linearly interpolate between a and b
// i should be in the range [0.0, 1.0]
float PerlinNoise::lerp(float a, float b, float i)
{
	return (1.0f - i) * a + i * b;
}

// used to smooth noise
float PerlinNoise::smoothStep(float f)
{
	return 6 * std::pow(f, 5) - 15 * std::pow(f, 4) + 10 * std::pow(f, 3);
}

// Computes the dot product of the distance and gradient vectors.
float PerlinNoise::dotGridGradient(int ix, int iy, float x, float y)
{
	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx * m_vectorGrid(ix, iy).x + dy * m_vectorGrid(ix, iy).y);
}

// Compute Perlin noise at coordinates x, y
float PerlinNoise::perlin(float x, float y)
{
	// ensure x and y are within range
	x = fmod(x, 255.0f);
	y = fmod(y, 255.0f);

	// Determine grid cell coordinates
	int x0 = (int)floor(x);
	int x1 = x0 + 1;
	int y0 = (int)floor(y);
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = smoothStep(x - (float)x0);
	float sy = smoothStep(y - (float)y0);

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;
	n0 = dotGridGradient(x0, y0, x, y);
	n1 = dotGridGradient(x1, y0, x, y);
	ix0 = lerp(n0, n1, sx);
	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	ix1 = lerp(n0, n1, sx);
	value = lerp(ix0, ix1, sy);

	// clamp between 0 and 1
	value = value * 0.5f + 0.5f;

	return value;
}

float PerlinNoise::octavePerlin(float x, float y, int octaves, float persistance)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;
	for (int i = 0; i < octaves; i++)
	{
		total += perlin(x * frequency, y * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistance;
		frequency *= 2;
	}

	return total / maxValue;
}

// defines the vector grid
void PerlinNoise::defineGrid()
{
	m_vectorGrid = Array2D<glm::vec2>(256, 256);

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			glm::vec2 gradient;

			float delta = (rand() % 36000) / 100.0f;
			delta *= 0.017453f;

			gradient.x = std::sin(delta);
			gradient.y = std::cos(delta);

			m_vectorGrid(x, y) = gradient;
		}
	}
}
