#pragma once
#include <glm\glm.hpp>
#include <string>
#include "Array2D.h"
#include "Mesh.h"
#include "Vertex.h"

class Terrain : Mesh
{
public:

	Terrain(unsigned int gridSizeX = 1, unsigned int gridSizeY = 1);

	void generatePerlin();
	void generateDiamondSquare(int featureSize);
	void readRaw(const std::string& filename);

private:

	// number of vertices along the x and y axis
	unsigned int m_gridSizeX;
	unsigned int m_gridSizeY;

	Array2D<float> m_heights;

	void init();

	void diamondSquare(int stepSize, float scale);

	void sampleSquare(int x, int y, int size, float value);
	void sampleDiamond(int x, int y, int size, float value);

	float sample(int x, int y);
	void setSample(int x, int y, float sample);

	glm::vec3 getVertexNormal(int x, int y);
	glm::vec3 getTriangleNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
};