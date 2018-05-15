#include "Mesh2.h"
#include <glad\glad.h>
#include "Color.h"
#include <vector>

Mesh2::~Mesh2()
{
	glDeleteVertexArrays(1, &vbo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh2::initialise(unsigned int vertexCount, const Vertex2* vertices, unsigned int indexCount, unsigned int* indices)
{
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vao);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex2),
		vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), 0);

	// enable second element as normal (currently being used for color)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FLOAT, sizeof(Vertex2), (void*)16);

	// bind indices if there are any
	if (indexCount != 0)
	{
		glGenBuffers(1, &ibo);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else
	{
		triCount = vertexCount / 3;
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh2::initialiseQuad()
{
	Vertex2 vertices[4];
	vertices[0].position = glm::vec4(-0.5f, 0, 0.5f, 1);
	vertices[1].position = glm::vec4(0.5f, 0, 0.5f, 1);
	vertices[2].position = glm::vec4(-0.5f, 0, -0.5f, 1);
	vertices[3].position = glm::vec4(0.5f, 0, -0.5f, 1);

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	initialise(4, vertices, 6, indices);
}

void Mesh2::initialiseBox()
{
	Vertex2 vertices[8];
	vertices[0].position = glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	vertices[1].position = glm::vec4(-0.5f, 0.5f, -0.5f, 1);
	vertices[2].position = glm::vec4(0.5f, 0.5f, -0.5f, 1);
	vertices[3].position = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	vertices[4].position = glm::vec4(-0.5f, -0.5f, 0.5f, 1);
	vertices[5].position = glm::vec4(-0.5f, -0.5f, -0.5f, 1);
	vertices[6].position = glm::vec4(0.5f, -0.5f, -0.5f, 1);
	vertices[7].position = glm::vec4(0.5f, -0.5f, 0.5f, 1);

	unsigned int indices[36] =
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

	initialise(8, vertices, 36, indices);
}

void Mesh2::initialiseCylinder(float radius, float height, float segments)
{
	std::vector<Vertex2> verts;
	std::vector<unsigned int> indices;

	// create circle
	for (int i = 0; i < segments + 1; i++)
	{
		// vert

		Vertex2 currentVertex;

		currentVertex.position = glm::vec4(0, 0, 0, 1);

		if (i == 0)
		{
			currentVertex.normal = Color::Black().asVec4();
			verts.push_back(currentVertex);
			continue;
		}

		float delta = glm::radians(i / (float)segments * 360.0f);

		currentVertex.position.x = std::sin(delta);
		currentVertex.position.z = -std::cos(delta);

		currentVertex.normal = Color::White().asVec4();

		verts.push_back(currentVertex);

		// last one
		if (i == segments)
		{
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(1);
		}
		else
		{
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
	}

	initialise(verts.size(), verts.data(), indices.size(), indices.data());
}

void Mesh2::draw()
{
	glBindVertexArray(vao);
	// using indices or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * triCount,
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
}
