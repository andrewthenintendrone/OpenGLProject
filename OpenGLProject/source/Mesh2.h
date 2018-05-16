#pragma once
#include "Vertex2.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>

class Mesh2
{
public:

	Mesh2() {}
	virtual ~Mesh2();

	void initialise(std::vector<Vertex2> verts, std::vector<unsigned int>* indices = nullptr, Texture* texture = nullptr);

	void initialiseQuad();
	void initialiseBox();
	void initialiseCircle(float radius, int segments);
	void initialiseCylinder(float radius, float height, int segments);

	virtual void draw(Shader shader);

protected:

	unsigned int vao, vbo, ibo;

	std::vector<Vertex2> m_verts;
	std::vector<unsigned int> m_indices;

	Texture m_texture;
};