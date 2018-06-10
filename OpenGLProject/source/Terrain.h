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

private:

	// number of vertices along the x and y axis
	unsigned int m_gridSizeX;
	unsigned int m_gridSizeY;

	Array2D<float> m_heights;

	void init();
};