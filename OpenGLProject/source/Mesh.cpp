#include "Mesh.h"
#include <glad\glad.h>
#include <math.h>
#include "Color.h"
#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

#include <fstream>
#include <iostream>
#include <iomanip>

#include <glm\gtc\constants.hpp>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vbo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialise(std::vector<Vertex> verts, std::vector<unsigned int>* indices, Texture* texture)
{
	assert(vao == 0);

	m_verts = verts;

	if (indices != nullptr)
	{
		m_indices = *indices;
	}
	if (texture != nullptr)
	{
		m_texture = *texture;
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vao);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(Vertex),
		&m_verts[0], GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)sizeof(glm::vec4));

	// enable third element as texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// bind indices if there are any
	if (m_indices.size() != 0)
	{
		glGenBuffers(1, &ibo);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::initialiseQuad()
{
	std::vector<Vertex> verts(4);

	// positions
	verts[0].position = glm::vec4(-0.5f, 0, 0.5f, 1);
	verts[1].position = glm::vec4(0.5f, 0, 0.5f, 1);
	verts[2].position = glm::vec4(-0.5f, 0, -0.5f, 1);
	verts[3].position = glm::vec4(0.5f, 0, -0.5f, 1);

	// normals
	for(int i = 0; i < 4; i++)
	{
		verts[i].normal = glm::vec4(0, 1, 0, 0);
	}

	// texCoords
	verts[0].texcoord = glm::vec2(0, 0);
	verts[1].texcoord = glm::vec2(1, 0);
	verts[2].texcoord = glm::vec2(0, 1);
	verts[3].texcoord = glm::vec2(1, 1);

	// texture
	Texture texture;
	texture.load(fs::current_path().string() + "\\resources\\chess.jpg");

	std::vector<unsigned int> indices{ 0, 1, 2, 2, 1, 3 };

	initialise(verts, &indices, &texture);
}

void Mesh::initialiseBox()
{
	std::vector<Vertex> verts(8);

	// positions
	verts[0].position = glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	verts[1].position = glm::vec4(-0.5f, 0.5f, -0.5f, 1);
	verts[2].position = glm::vec4(0.5f, 0.5f, -0.5f, 1);
	verts[3].position = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	verts[4].position = glm::vec4(-0.5f, -0.5f, 0.5f, 1);
	verts[5].position = glm::vec4(-0.5f, -0.5f, -0.5f, 1);
	verts[6].position = glm::vec4(0.5f, -0.5f, -0.5f, 1);
	verts[7].position = glm::vec4(0.5f, -0.5f, 0.5f, 1);

	// normals
	for (int i = 0; i < 8; i++)
	{
		verts[i].normal = verts[i].position;
		verts[i].normal.w = 0;
	}

	std::vector<unsigned int> indices =
	{
		0, 1, 2,
		2, 3, 0,
		4, 0, 3,
		3, 7, 4,
		7, 3, 2,
		2, 6, 7,
		6, 2, 1,
		1, 5, 6,
		5, 1, 0,
		0, 4, 5,
		5, 4, 7,
		7, 6, 5
	};

	initialise(verts, &indices);
}

void Mesh::initialiseCircle(float radius, int segments)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	// create circle verts / indices
	for (int i = 0; i <= segments; i++)
	{
		Vertex currentVertex;

		// middle of circle
		if (i == 0)
		{
			currentVertex.texcoord = glm::vec2(0.5f);
			verts.push_back(currentVertex);
			continue;
		}

		// place vert correctly
		float angle = glm::radians(i / (float)segments * 360.0f);

		currentVertex.position.x = std::sin(angle) * radius;
		currentVertex.position.z = std::cos(angle) * radius;

		currentVertex.texcoord = glm::vec2(std::sin(angle) * 0.5f + 0.5f, std::cos(angle) * 0.5f + 0.5f);

		verts.push_back(currentVertex);

		// add indices
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i % segments + 1);
	}

	initialise(verts);
}

void Mesh::initialiseCylinder(float radius, float height, int segments)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	// create top circle verts / indices
	for (int i = 0; i <= segments; i++)
	{
		Vertex currentVertex;
		currentVertex.position = glm::vec4(0, height * 0.5f, 0, 1);

		// middle of circle
		if (i == 0)
		{
			currentVertex.texcoord = glm::vec2(0.5f);
			verts.push_back(currentVertex);
			continue;
		}

		// place vert correctly
		float angle = glm::radians(i / (float)segments * 360.0f);

		currentVertex.position.x = std::sin(angle) * radius;
		currentVertex.position.z = std::cos(angle) * radius;

		currentVertex.texcoord = glm::vec2(std::sin(angle) * 0.5f + 0.5f, std::cos(angle) * 0.5f + 0.5f);

		verts.push_back(currentVertex);

		// add indices
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i % segments + 1);
	}
	// create bottom circle verts / indices
	for (int i = 0; i <= segments; i++)
	{
		Vertex currentVertex;
		currentVertex.position = glm::vec4(0, -height * 0.5f, 0, 1);

		// middle of circle
		if (i == 0)
		{
			currentVertex.texcoord = glm::vec2(0.5f);
			verts.push_back(currentVertex);
			continue;
		}

		// place vert correctly
		float angle = glm::radians(i / (float)segments * 360.0f);

		currentVertex.position.x = std::sin(angle) * radius;
		currentVertex.position.z = std::cos(angle) * radius;

		currentVertex.texcoord = glm::vec2(std::sin(angle) * 0.5f + 0.5f, std::cos(angle) * 0.5f + 0.5f);

		verts.push_back(currentVertex);

		// add indices
		indices.push_back(i % segments + segments + 2);
		indices.push_back(i + segments + 1);
		indices.push_back(segments + 1);
	}

	// create outer indices
	for (int i = 1; i <= segments; i++)
	{
		indices.push_back(i + segments + 1);
		indices.push_back(i % segments + 1);
		indices.push_back(i);

		indices.push_back(i + segments + 1);
		indices.push_back(i % segments + segments + 2);
		indices.push_back(i % segments + 1);
	}

	initialise(verts, &indices);
}

void Mesh::initialiseSphere(float radius, int rows, int columns)
{
	float invRadius = 1.0f / radius;
	float invColumns = 1.0f / columns;
	float invRows = 1.0f / rows;

	std::vector<Vertex> verts;
	//verts.resize(rows * columns + columns);
	std::vector<unsigned int> indices;

	// for each row of the mesh
	for (int row = 0; row <= rows; row++)
	{
		float xRatio = float(row) * invRows;
		float xRadians = glm::radians(xRatio * 180.0f - 90.0f);
		float y = radius * sin(xRadians);
		float z = radius * cos(xRadians);

		// for each row of the mesh
		for (int col = 0; col <= columns; col++)
		{
			float yRatio = float(col) * invColumns;
			float theta = glm::radians(yRatio * 360.0f);
			glm::vec3 v4Point(-z * sinf(theta), y, -z * cosf(theta));
			glm::vec3 v4Normal(invRadius * v4Point);

			// create vertex
			Vertex v;
			v.position = glm::vec4(v4Point, 1);
			v.normal = glm::vec4(v4Normal, 1);
			v.texcoord = glm::vec2(yRatio, -xRatio);

			//verts[index] = v;
			verts.push_back(v);
		}
	}

	for (int i = 0; i < verts.size() - columns - 1; i++)
	{
		int i2 = i + 1;

		indices.push_back(i2 + columns);
		indices.push_back(i);
		indices.push_back(i2);

		indices.push_back(i2 + columns);
		indices.push_back(i + columns);
		indices.push_back(i);
	}

	initialise(verts, &indices);
}

void Mesh::draw(Shader shader)
{
	glBindVertexArray(vao);

	// using indices or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, m_indices.size(),
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_PATCHES, 0, m_verts.size());
	}
}
