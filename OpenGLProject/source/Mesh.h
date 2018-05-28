#pragma once
#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>

class Mesh
{
public:

	Mesh() {}
	virtual ~Mesh();

	void initialise(std::vector<Vertex> verts, std::vector<unsigned int>* indices = nullptr, Texture* texture = nullptr);

	void initialiseQuad();
	void initialiseBox();
	void initialiseCircle(float radius, int segments);
	void initialiseCylinder(float radius, float height, int segments);
	void initialiseSphere(float radius, int rows, int columns);

	virtual void draw(Shader shader);

	const unsigned int getVertexArrayObject() { return vao; }
	const unsigned int getVertexBufferObject() { return vbo; }
	const unsigned int getIndexBufferObject() { return ibo; }

protected:

	unsigned int vao, vbo, ibo;

	std::vector<Vertex> m_verts;
	std::vector<unsigned int> m_indices;

	Texture m_texture;
};