#pragma once
#include <glm\glm.hpp>
#include <string>
#include "Array2D.h"
#include "Mesh.h"

class Terrain
{
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO;

	Terrain(unsigned int gridSizeX = 1, unsigned int gridSizeY = 1) : m_gridSizeX(gridSizeX), m_gridSizeY(gridSizeY) {}

	void generatePerlin();
	void generateDiamondSquare();
	void loadFile(const std::string& filename);

	void Draw(Shader shader);

private:

	// number of vertices along the x and y axis
	unsigned int m_gridSizeX;
	unsigned int m_gridSizeY;

	void init();

	unsigned int VBO, EBO;
};