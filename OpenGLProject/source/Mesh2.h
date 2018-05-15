#pragma once
#include "Vertex2.h"

class Mesh2
{
public:

	Mesh2() : triCount(0), vao(0), ibo(0) {}
	virtual ~Mesh2();

	void initialise(unsigned int vertexCount, const Vertex2* vertices,
		unsigned int indexCount = 0,
		unsigned int* indices = nullptr);

	void initialiseQuad();
	void initialiseBox();
	void initialiseCylinder(float radius, float height, float segments);

	virtual void draw();

protected:

	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};