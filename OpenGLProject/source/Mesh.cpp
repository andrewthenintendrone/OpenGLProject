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

void Mesh::initialise(std::vector<Vertex> verts, std::vector<unsigned int>* indices)
{
	assert(vao == 0);

	m_verts = verts;

	if (indices != nullptr)
	{
		m_indices = *indices;
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// enable third element as texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	// enable fourth element as tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// enable fifth element as color
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 5, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

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

	m_material.createDummyTextures();

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::initialiseQuad()
{
	std::vector<Vertex> verts(4);

	// positions
	verts[0].position = glm::vec4(-1, -1, 0, 1);
	verts[1].position = glm::vec4(1, -1, 0, 1);
	verts[2].position = glm::vec4(-1, 1, 0, 1);
	verts[3].position = glm::vec4(1, 1, 0, 1);

	// normals
	for(int i = 0; i < 4; i++)
	{
		verts[i].normal = glm::vec4(0, 0, 1, 0);
	}

	// texCoords
	verts[0].texcoord = glm::vec2(0, 0);
	verts[1].texcoord = glm::vec2(1, 0);
	verts[2].texcoord = glm::vec2(0, 1);
	verts[3].texcoord = glm::vec2(1, 1);

	// texture
	std::vector<unsigned int> indices{ 0, 1, 2, 2, 1, 3 };

	initialise(verts, &indices);
}

void Mesh::initialiseBox()
{
	std::vector<Vertex> verts;

	// positions
	for (float x = -0.5f; x < 1.0f; x++)
	{
		for (float y = -0.5f; y < 1.0f; y++)
		{
			for (float z = -0.5f; z < 1.0f; z++)
			{
				Vertex v;
				v.position = glm::vec4(x, y, z, 1.0f);
				v.normal = glm::vec4(glm::normalize(glm::vec3(v.position)), 0.0f);
				
				float tx = x == -0.5 ? 0 : 1;
				float ty = z == 0.5 ? 0 : 1;

				v.texcoord = glm::vec2(tx, ty);

				verts.push_back(v);
			}
		}
	}

	std::vector<unsigned int> indices =
	{
		7, 3, 1,
		1, 5, 7,
		4, 5, 1,
		1, 0, 4,
		6, 7, 5,
		5, 4, 6,
		2, 3, 7,
		7, 6, 2,
		0, 1, 3,
		3, 2, 0,
		6, 4, 0,
		0, 2, 6
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

	initialise(verts, &indices);
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
			glm::vec3 v4Normal(glm::normalize(v4Point));

			// create vertex
			Vertex v;
			v.position = glm::vec4(v4Point, 1);
			v.normal = glm::vec4(v4Normal, 1);
			v.texcoord = glm::vec2(yRatio, xRatio);
			v.tangent = glm::vec4(glm::cross(glm::vec3(0, 1, 0), glm::vec3(v4Normal)), 0);

			//verts[index] = v;
			verts.push_back(v);
		}
	}

	for (unsigned int i = 0; i < verts.size() - columns - 1; i++)
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

void Mesh::initialiseIcosahedron()
{
	std::vector<glm::vec4> positions
	{
		glm::vec4(0.000f, 0.000f, 1.000f, 1.0f),
		glm::vec4(0.894f, 0.000f, 0.447f, 1.0f),
		glm::vec4(0.276f, 0.851f, 0.447f, 1.0f),
		glm::vec4(-0.724f, 0.526f, 0.447f, 1.0f),
		glm::vec4(-0.724f, -0.526f, 0.447f, 1.0f),
		glm::vec4(0.276f, -0.851f, 0.447f, 1.0f),
		glm::vec4(0.724f, 0.526f, -0.447f, 1.0f),
		glm::vec4(-0.276f, 0.851f, -0.447f, 1.0f),
		glm::vec4(-0.894f, 0.000f, -0.447f, 1.0f),
		glm::vec4(-0.276f, -0.851f, -0.447f, 1.0f),
		glm::vec4(0.724f, -0.526f, -0.447f, 1.0f),
		glm::vec4(0.000f, 0.000f, -1.000f, 1.0f)
	};

	std::vector<unsigned int> indices
	{
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,
		11, 6, 7,
		11, 7, 8,
		11, 8, 9,
		11, 9, 10,
		11, 10,6,
		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,
		2, 7, 6,
		3, 8, 7,
		4, 9, 8,
		5, 10, 9,
		1, 6, 10
	};

	std::vector<Vertex> verts;

	for (int i = 0; i < positions.size(); i++)
	{
		Vertex v;
		v.position = positions[i];
		v.normal = glm::vec4(glm::normalize(glm::vec3(v.position)), 0.0f);
		v.tangent = glm::vec4(glm::cross(glm::vec3(v.normal), glm::vec3(0, 1, 0)), 0.0f);
		v.texcoord = glm::vec2(0, 0);

		verts.push_back(v);
	}

	initialise(verts, &indices);
}

void Mesh::draw(Shader shader)
{
	m_material.bind(shader);

	glBindVertexArray(vao);

	// using indices or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(),
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_verts.size());
	}
}
