#include "Terrain.h"
#include <iostream>
#include <fstream>
#include <random>
#include "PerlinNoise.h"
#include <fstream>

// returns a random float between min and max
float frand(float min, float max)
{
	float f = (float)rand() / RAND_MAX;
	return min + f * (max - min);
}

Terrain::Terrain(unsigned int gridSizeX, unsigned int gridSizeY) : m_gridSizeX(gridSizeX), m_gridSizeY(gridSizeY)
{
	m_heights = Array2D<float>(m_gridSizeX, m_gridSizeY);
}

// generate terrain using perlin noise
void Terrain::generatePerlin()
{
	for (unsigned int x = 0; x < m_gridSizeX; x++)
	{
		for (unsigned int y = 0; y < m_gridSizeY; y++)
		{
			float perlin = PerlinNoise::getInstance().octavePerlin(x * 0.01f, y * 0.01f, 20, 0.5f);

			m_heights(x, y) = perlin;
		}
	}

	init();
}

// create mesh data
void Terrain::init()
{
	// create vectors for vertices and indices
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	// generate mesh
	for (unsigned int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (unsigned int y = 0; y < m_gridSizeY; y++, i++)
		{
			Vertex currentVertex;

			// sample heights
			float currentHeight = m_heights(x, y);

			// set vertex position
			currentVertex.position = glm::vec4(x - m_gridSizeX / 2.0f, currentHeight, y - m_gridSizeY / 2.0f, 1.0f);

			// set texture coordinates (currently unused)
			currentVertex.texcoord = glm::vec2((float)x / (float)m_gridSizeX, (float)y / (float)m_gridSizeY);

			verts.push_back(currentVertex);

			// create triangles
			if (x < m_gridSizeX - 1 && y < m_gridSizeY - 1)
			{
				int i2 = i + 1;
				int i3 = i + m_gridSizeX;
				int i4 = i + m_gridSizeX + 1;

				indices.push_back(i);
				indices.push_back(i2);
				indices.push_back(i4);

				indices.push_back(i4);
				indices.push_back(i3);
				indices.push_back(i);
			}
		}
	}

	initialise(verts, &indices);
}
