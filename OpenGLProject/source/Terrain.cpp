#include "Terrain.h"
#include <iostream>
#include <fstream>
#include <random>
#include "PerlinNoise.h"
#include "DiamondSquare.h"
#include <fstream>

// generate terrain using perlin noise
void Terrain::generatePerlin()
{
	// generate mesh
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			Vertex vert;

			// sample perlin noise
			float perlin = PerlinNoise::getInstance().octavePerlin(x * 0.01f, y * 0.01f, 20, 0.5f);

			// set vertex position
			vert.Position = glm::vec3(x - m_gridSizeX / 2.0f, perlin, y - m_gridSizeY / 2.0f);

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
				indices.push_back(i3);

				indices.push_back(i3);
				indices.push_back(i2);
				indices.push_back(i4);
			}
		}
	}

	// calculate vertex normals
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			if (x < m_gridSizeX - 1 && y < m_gridSizeY - 1)
			{
				int i2 = i + 1;
				int i3 = i + m_gridSizeX;
				int i4 = i + m_gridSizeX + 1;

				glm::vec3 posA = vertices[i].Position;
				glm::vec3 posB = vertices[i2].Position;
				glm::vec3 posC = vertices[i3].Position;
				glm::vec3 posD = vertices[i4].Position;

				glm::vec3 n1 = glm::cross((posB - posA), (posC - posA));
				glm::vec3 n2 = glm::cross((posB - posC), (posD - posC));

				vertices[i].Normal = (n1 + n2) * 0.5f;
			}
		}
	}

	init();
}

// generate terrain using the diamond square method
void Terrain::generateDiamondSquare()
{
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			Vertex vert;

			// position
			vert.Position = glm::vec3(x, DiamondSquare::getInstance().sample(x, y), y);

			// uvs
			vert.TexCoords = glm::vec2(x / m_gridSizeX, y / m_gridSizeY);

			vertices.push_back(vert);

			// create triangles
			if (x < m_gridSizeX - 1 && y < m_gridSizeY - 1)
			{
				int i2 = i + 1;
				int i3 = i + m_gridSizeX;
				int i4 = i + m_gridSizeX + 1;

				indices.push_back(i);
				indices.push_back(i2);
				indices.push_back(i3);

				indices.push_back(i3);
				indices.push_back(i2);
				indices.push_back(i4);
			}
		}
	}

	// calculate vertex normals
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			if (x < m_gridSizeX - 1 && y < m_gridSizeY - 1)
			{
				int i2 = i + 1;
				int i3 = i + m_gridSizeX;
				int i4 = i + m_gridSizeX + 1;

				glm::vec3 posA = vertices[i].Position;
				glm::vec3 posB = vertices[i2].Position;
				glm::vec3 posC = vertices[i3].Position;
				glm::vec3 posD = vertices[i4].Position;

				glm::vec3 n1 = glm::cross((posB - posA), (posC - posA));
				glm::vec3 n2 = glm::cross((posB - posC), (posD - posC));

				vertices[i].Normal = (n1 + n2) * 0.5f;
			}
		}
	}

	init();
}

// load terrain from file
void Terrain::loadFile(const std::string& filename)
{
	// attempt to load file
	std::ifstream file(filename, std::ios::_Nocreate | std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	// set grid size
	m_gridSizeX = m_gridSizeY = 256;

	// create 2d heights vector
	Array2D<uint16_t> heights(m_gridSizeX, m_gridSizeY);

	// read heights into vector
	for (int y = 0; y < m_gridSizeY; y++)
	{
		for (int x = 0; x < m_gridSizeX; x++)
		{
			// read current height
			file.read(reinterpret_cast<char*>(&heights(x, y)), sizeof(uint16_t));
		}
	}

	// close file
	file.close();

	// setup mesh
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			// current vertex
			Vertex vert;

			vert.Position = glm::vec3(x, heights(x, y), y);

			vertices.push_back(vert);

			// create triangles
			if (x < m_gridSizeX - 1 && y < m_gridSizeY - 1)
			{
				int i2 = i + 1;
				int i3 = i + m_gridSizeX;
				int i4 = i + m_gridSizeX + 1;

				indices.push_back(i);
				indices.push_back(i2);
				indices.push_back(i3);

				indices.push_back(i3);
				indices.push_back(i2);
				indices.push_back(i4);
			}
		}
	}

	// calculate vertex normals
	for (int x = 0, i = 0; x < m_gridSizeX; x++)
	{
		for (int y = 0; y < m_gridSizeY; y++, i++)
		{
			if (x < m_gridSizeX - 1 && y < m_gridSizeY - 1)
			{
				int i2 = i + 1;
				int i3 = i + m_gridSizeX;
				int i4 = i + m_gridSizeX + 1;

				glm::vec3 posA = vertices[i].Position;
				glm::vec3 posB = vertices[i2].Position;
				glm::vec3 posC = vertices[i3].Position;
				glm::vec3 posD = vertices[i4].Position;

				glm::vec3 n1 = glm::cross((posB - posA), (posC - posA));
				glm::vec3 n2 = glm::cross((posB - posC), (posD - posC));

				vertices[i].Normal = (n1 + n2) * 0.5f;
			}
		}
	}

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
