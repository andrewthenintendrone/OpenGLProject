#include "Terrain.h"
#include <iostream>
#include <fstream>
#include <random>
#include "PerlinNoise.h"
#include <fstream>

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
	for (int x = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++)
		{
			float perlin = PerlinNoise::getInstance().octavePerlin(x * 0.01f, y * 0.01f, 20, 0.5f);

			m_heights(x, y) = perlin;
		}
	}

	init();
}

// set heights using the diamond square algorithm
void Terrain::generateDiamondSquare(int featureSize)
{
	// seed corners
	m_heights(0, 0) = frand(0, 1);
	m_heights(m_gridSizeX - 1, 0) = frand(0, 1);
	m_heights(0, m_gridSizeY - 1) = frand(0, 1);
	m_heights(m_gridSizeX - 1, m_gridSizeY - 1) = frand(0, 1);

	int sampleSize = featureSize;
	float scale = 1.0f;

	while (sampleSize > 1)
	{
		diamondSquare(sampleSize, scale);

		sampleSize /= 2;
		scale /= 2.0f;
	}

	init();
}

// read terrain from heightmap file
void Terrain::readRaw(const std::string& filename)
{
	std::ifstream file;
	file.open(filename, std::ios::_Nocreate | std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	// read heights into buffer
	for (int x = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++)
		{
			uint16_t currentHeight;

			file.read(reinterpret_cast<char*>(&currentHeight), sizeof(uint16_t));
			m_heights(x, m_gridSizeY - 1 - y) = (float)currentHeight;
		}
	}

	// close file
	file.close();

	// initialize mesh
	init();
}

// draw the Terrain using the supplied Shader
void Terrain::Draw(Shader shader)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// send data to the GPU
void Terrain::init()
{
	// generate mesh
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			Vertex vert;

			// sample heights
			float currentHeight = m_heights(x, y);

			// set vertex position
			vert.Position = glm::vec3(x - m_gridSizeX / 2.0f, currentHeight, y - m_gridSizeY / 2.0f);

			// set texture coordinates (currently unused)
			vert.TexCoords = glm::vec2((float)x / (float)m_gridSizeX, (float)y / (float)m_gridSizeY);

			vertices.push_back(vert);

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

	// calculate vertex normals
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			vertices[i].Normal = getVertexNormal(x, y);
		}
	}

	// reflatten
	//for (int x = 0, i = 0; x < m_gridSizeX; x++)
	//{
	//	for (int y = 0; y < m_gridSizeY; y++, i++)
	//	{
	//		vertices[i].Position = glm::vec3(x, 0, y);
	//	}
	//}

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

// diamond square algorithm
void Terrain::diamondSquare(int stepSize, float scale)
{
	int halfStep = stepSize / 2;

	for (int y = halfStep; y < m_gridSizeY + halfStep; y += stepSize)
	{
		for (int x = halfStep; x < m_gridSizeX + halfStep; x += stepSize)
		{
			sampleSquare(x, y, stepSize, frand(-scale, scale));
		}
	}

	for (int y = 0; y < m_gridSizeY; y += stepSize)
	{
		for (int x = 0; x < m_gridSizeX; x += stepSize)
		{
			sampleDiamond(x + halfStep, y, stepSize, frand(-scale, scale));
			sampleDiamond(x, y + halfStep, stepSize, frand(-scale, scale));
		}
	}
}

void Terrain::sampleSquare(int x, int y, int size, float value)
{
	int hs = size / 2;

	float a = sample(x - hs, y - hs);
	float b = sample(x + hs, y - hs);
	float c = sample(x - hs, y + hs);
	float d = sample(x + hs, y + hs);

	setSample(x, y, ((a + b + c + d) / 4.0f) + value);
}

void Terrain::sampleDiamond(int x, int y, int size, float value)
{
	int hs = size / 2;

	float a = sample(x - hs, y);
	float b = sample(x + hs, y);
	float c = sample(x, y - hs);
	float d = sample(x, y + hs);

	setSample(x, y, ((a + b + c + d) / 4.0f) + value);
}

float Terrain::sample(int x, int y)
{
	return (m_heights(x & (m_gridSizeX - 1), y & (m_gridSizeY - 1)));
}

void Terrain::setSample(int x, int y, float value)
{
	m_heights(x & (m_gridSizeX - 1), y & (m_gridSizeY - 1)) = value;
}

// returns the normal of a vertex by averaging the normals of the surrounding faces
glm::vec3 Terrain::getVertexNormal(int x, int y)
{
	int i = y * m_gridSizeX + x;

	if (x == 0)
	{
		if (y == 0)
		{
			glm::vec3 n1 = getTriangleNormal(vertices[i].Position, vertices[i + 1].Position, vertices[i + m_gridSizeX + 1].Position);
			glm::vec3 n2 = getTriangleNormal(vertices[i + m_gridSizeX + 1].Position, vertices[i + m_gridSizeX].Position, vertices[i].Position);

			return (n1 + n2) / 2.0f;
		}
		else if (y == m_gridSizeY - 1)
		{
			return getTriangleNormal(vertices[i + 1].Position, vertices[i].Position, vertices[i - m_gridSizeX].Position);
		}
		else
		{
			glm::vec3 n1 = getTriangleNormal(vertices[i].Position, vertices[i + 1].Position, vertices[i + m_gridSizeX + 1].Position);
			glm::vec3 n2 = getTriangleNormal(vertices[i + m_gridSizeX + 1].Position, vertices[i + m_gridSizeX].Position, vertices[i].Position);
			glm::vec3 n3 = getTriangleNormal(vertices[i + 1].Position, vertices[i].Position, vertices[i - m_gridSizeX].Position);

			return (n1 + n2 + n3) / 3.0f;
		}
	}
	else if (x == m_gridSizeX - 1)
	{
		if (y == 0)
		{
			return getTriangleNormal(vertices[i - 1].Position, vertices[i].Position, vertices[i + m_gridSizeX].Position);
		}
		else if (y == m_gridSizeY - 1)
		{
			glm::vec3 n1 = getTriangleNormal(vertices[i - m_gridSizeX - 1].Position, vertices[i - m_gridSizeX].Position, vertices[i].Position);
			glm::vec3 n2 = getTriangleNormal(vertices[i].Position, vertices[i - 1].Position, vertices[i - m_gridSizeX - 1].Position);

			return (n1 + n2) / 2.0f;
		}
		else
		{
			glm::vec3 n1 = getTriangleNormal(vertices[i - 1].Position, vertices[i].Position, vertices[i + m_gridSizeX].Position);
			glm::vec3 n2 = getTriangleNormal(vertices[i - m_gridSizeX - 1].Position, vertices[i - m_gridSizeX].Position, vertices[i].Position);
			glm::vec3 n3 = getTriangleNormal(vertices[i].Position, vertices[i - 1].Position, vertices[i - m_gridSizeX - 1].Position);

			return (n1 + n2 + n3) / 3.0f;
		}
	}
	else if (y == 0)
	{
		glm::vec3 n1 = getTriangleNormal(vertices[i - 1].Position, vertices[i].Position, vertices[i + m_gridSizeX].Position);
		glm::vec3 n2 = getTriangleNormal(vertices[i].Position, vertices[i + 1].Position, vertices[i + m_gridSizeX + 1].Position);
		glm::vec3 n3 = getTriangleNormal(vertices[i + m_gridSizeX + 1].Position, vertices[i + m_gridSizeX].Position, vertices[i].Position);

		return (n1 + n2 + n3) / 3.0f;
	}
	else if (y == m_gridSizeY - 1)
	{
		glm::vec3 n1 = getTriangleNormal(vertices[i].Position, vertices[i - 1].Position, vertices[i - m_gridSizeX - 1].Position);
		glm::vec3 n2 = getTriangleNormal(vertices[i - m_gridSizeX - 1].Position, vertices[i - m_gridSizeX].Position, vertices[i].Position);
		glm::vec3 n3 = getTriangleNormal(vertices[i - m_gridSizeX].Position, vertices[i + 1].Position, vertices[i].Position);

		return (n1 + n2 + n3) / 3.0f;
	}
	else
	{
		glm::vec3 n1 = getTriangleNormal(vertices[i - m_gridSizeX - 1].Position, vertices[i - m_gridSizeX].Position, vertices[i].Position);
		glm::vec3 n2 = getTriangleNormal(vertices[i - m_gridSizeX].Position, vertices[i + 1].Position, vertices[i].Position);
		glm::vec3 n3 = getTriangleNormal(vertices[i].Position, vertices[i + 1].Position, vertices[i + m_gridSizeX + 1].Position);
		glm::vec3 n4 = getTriangleNormal(vertices[i + m_gridSizeX + 1].Position, vertices[i + m_gridSizeX].Position, vertices[i].Position);
		glm::vec3 n5 = getTriangleNormal(vertices[i - 1].Position, vertices[i].Position, vertices[i + m_gridSizeX].Position);
		glm::vec3 n6 = getTriangleNormal(vertices[i].Position, vertices[i - 1].Position, vertices[i - m_gridSizeX - 1].Position);

		return (n1 + n2 + n3 + n4 + n5 + n6) / 6.0f;
	}
}

// returns the normal direction of a triangle given 3 points
glm::vec3 Terrain::getTriangleNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	// to find the normal of three points first take two of the edges
	glm::vec3 edge1 = (p2 - p1);
	glm::vec3 edge2 = (p3 - p1);

	// then the cross product of these two edges will point perpendicular to the triangle
	glm::vec3 direction = glm::cross(edge1, edge2);

	return direction;
}
