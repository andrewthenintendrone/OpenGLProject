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

	Terrain(unsigned int gridSizeX = 1, unsigned int gridSizeY = 1);

	void generatePerlin();
	void generateDiamondSquare(int featureSize);

	void Draw(Shader shader);

private:

	// number of vertices along the x and y axis
	unsigned int m_gridSizeX;
	unsigned int m_gridSizeY;

	Array2D<float> m_heights;

	void init();

	unsigned int VBO, EBO;

	void diamondSquare(int stepSize, float scale);

	void sampleSquare(int x, int y, int size, float value);
	void sampleDiamond(int x, int y, int size, float value);

	float sample(int x, int y);
	void setSample(int x, int y, float sample);
};