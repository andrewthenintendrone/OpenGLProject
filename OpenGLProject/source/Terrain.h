#pragma once
#include <glm\glm.hpp>
#include <string>
#include "Mesh.h"
#include "Vertex.h"
#include "Array2D.h"

class Terrain : public Mesh
{
public:

	Terrain(unsigned int gridSizeX = 1, unsigned int gridSizeY = 1);

	void generatePerlin();

	void readRaw(const std::string& filename);

	void generateRandom();

private:

	// number of vertices along the x and y axis
	unsigned int m_gridSizeX;
	unsigned int m_gridSizeY;

	Array2D<float> m_heights;

	glm::vec3 getTriangleDirection(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	void init();
};