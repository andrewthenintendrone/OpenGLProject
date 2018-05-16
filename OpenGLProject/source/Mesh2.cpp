#include "Mesh2.h"
#include <glad\glad.h>
#include "Color.h"

Mesh2::~Mesh2()
{
	glDeleteVertexArrays(1, &vbo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh2::initialise(std::vector<Vertex2> verts, std::vector<unsigned int>* indices, Texture* texture)
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
	glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(Vertex2),
		&m_verts[0], GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), 0);

	// enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)sizeof(glm::vec4));

	// enable third element as texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)(sizeof(glm::vec4) * 2));

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

void Mesh2::initialiseQuad()
{
	std::vector<Vertex2> verts(4);
	verts[0].position = glm::vec4(-0.5f, 0, 0.5f, 1);
	verts[1].position = glm::vec4(0.5f, 0, 0.5f, 1);
	verts[2].position = glm::vec4(-0.5f, 0, -0.5f, 1);
	verts[3].position = glm::vec4(0.5f, 0, -0.5f, 1);

	std::vector<unsigned int> indices { 0, 1, 2, 2, 1, 3 };

	initialise(verts, &indices);
}

void Mesh2::initialiseBox()
{
	std::vector<Vertex2> verts(8);
	verts[0].position = glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	verts[1].position = glm::vec4(-0.5f, 0.5f, -0.5f, 1);
	verts[2].position = glm::vec4(0.5f, 0.5f, -0.5f, 1);
	verts[3].position = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	verts[4].position = glm::vec4(-0.5f, -0.5f, 0.5f, 1);
	verts[5].position = glm::vec4(-0.5f, -0.5f, -0.5f, 1);
	verts[6].position = glm::vec4(0.5f, -0.5f, -0.5f, 1);
	verts[7].position = glm::vec4(0.5f, -0.5f, 0.5f, 1);

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

void Mesh2::initialiseCircle(float radius, int segments)
{
	std::vector<Vertex2> verts;
	std::vector<unsigned int> indices;

	// create circle verts / indices
	for (int i = 0; i < segments + 1; i++)
	{
		Vertex2 currentVertex;
		currentVertex.position = glm::vec4(0, 0, 0, 1);

		// middle of circle
		if (i == 0)
		{
			currentVertex.texCoord = glm::vec2(0.5f);
			verts.push_back(currentVertex);
			continue;
		}

		// place vert correctly
		float angle = glm::radians(i / (float)segments * 360.0f);

		currentVertex.position.x = std::cos(angle) * radius;
		currentVertex.position.z = std::sin(angle) * radius;

		currentVertex.texCoord = glm::vec2(std::cos(angle) * 0.5f + 0.5f, std::sin(angle) * 0.5f + 0.5f);

		verts.push_back(currentVertex);

		// add indices
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i % segments + 1);
	}

	// load texture
	Texture texture;
	texture.loadFromFile("C:\\Users\\s170837\\Pictures\\shrek.png");

	initialise(verts, &indices, &texture);
}

void Mesh2::initialiseCylinder(float radius, float height, int segments)
{
	std::vector<Vertex2> verts;
	std::vector<unsigned int> indices;

	// create top circle verts / indices
	for (int i = 0; i < segments + 1; i++)
	{
		Vertex2 currentVertex;
		currentVertex.position = glm::vec4(0, height * 0.5f, 0, 1);

		// middle of circle
		if (i == 0)
		{
			currentVertex.texCoord = glm::vec2(0.5f);
			verts.push_back(currentVertex);
			continue;
		}

		// place vert correctly
		float angle = glm::radians(i / (float)segments * 360.0f);

		currentVertex.position.x = std::cos(angle) * radius;
		currentVertex.position.z = std::sin(angle) * radius;

		currentVertex.texCoord = glm::vec2(std::cos(angle) * 0.5f + 0.5f, std::sin(angle) * 0.5f + 0.5f);

		verts.push_back(currentVertex);

		// add indices
		indices.push_back(0);
		indices.push_back(i);
		if (i == segments)
		{
			indices.push_back(1);
		}
		else
		{
			indices.push_back(i + 1);
		}
	}
	// create bottom circle verts / indices
	for (int i = 0; i < segments + 1; i++)
	{
		Vertex2 currentVertex;
		currentVertex.position = glm::vec4(0, -height * 0.5f, 0, 1);

		// middle of circle
		if (i == 0)
		{
			currentVertex.texCoord = glm::vec2(0.5f);
			verts.push_back(currentVertex);
			continue;
		}

		// place vert correctly
		float angle = glm::radians(i / (float)segments * 360.0f);

		currentVertex.position.x = std::cos(angle) * radius;
		currentVertex.position.z = std::sin(angle) * radius;

		currentVertex.texCoord = glm::vec2(std::cos(angle) * 0.5f + 0.5f, std::sin(angle) * 0.5f + 0.5f);

		verts.push_back(currentVertex);

		// add indices
		indices.push_back(segments + 1);
		indices.push_back(segments + 1 + i);
		if (i == segments)
		{
			indices.push_back(segments + 2);
		}
		else
		{
			indices.push_back(segments + 2 + i);
		}
	}

	// create outer indices
	for (int i = 1; i < segments + 1; i++)
	{
		indices.push_back(i);
		if (i == segments)
		{
			indices.push_back(1);
		}
		else
		{
			indices.push_back(i + 1);
		}
		indices.push_back(i + segments + 1);

		indices.push_back(i + segments + 1);
		if (i == segments)
		{
			indices.push_back(segments + 2);
		}
		else
		{
			indices.push_back(i + segments + 2);
		}
		indices.push_back(i);
	}

	// load texture
	Texture texture;
	texture.loadFromFile("C:\\Users\\s170837\\Pictures\\shrek.png");

	initialise(verts, &indices, &texture);
}

void Mesh2::draw(Shader shader)
{
	// use texture
	glUniform1i(glGetUniformLocation(shader.ID, "colorTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, m_texture.id);

	glBindVertexArray(vao);
	// using indices or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, m_indices.size(),
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
	}
}
