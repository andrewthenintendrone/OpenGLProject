#include "OBJMesh.h"
#include <glad\glad.h>
#include <glm\geometric.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

OBJMesh::~OBJMesh()
{
	for (auto& c : m_meshChunks)
	{
		glDeleteVertexArrays(1, &c.vao);
		glDeleteBuffers(1, &c.vbo);
		glDeleteBuffers(1, &c.ibo);
	}
}

// load an obj file
bool OBJMesh::load(const std::string& filename, bool loadTextures, bool flipTextureV)
{
	// don't load if already initialised
	if (m_meshChunks.empty() == false)
	{
		printf("Mesh already initialised, can't re-initialise!\n");
		return false;
	}

	// create vectors of tinyobj formats
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error = "";

	// get file and folder name
	std::string file = filename;
	std::string folder = file.substr(0, file.find_last_of('\\') + 1);

	// attempt to load model using tinyobj
	bool success = tinyobj::LoadObj(shapes, materials, error, filename.c_str(), folder.c_str());

	// check for errors
	if (success == false)
	{
		printf("%s\n", error.c_str());
		return false;
	}

	// store filename
	m_filename = filename;

	// resize internal material array
	m_materials.resize(materials.size());

	int index = 0;
	for (auto& m : materials)
	{
		// get constant values from material
		m_materials[index].ambient = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
		m_materials[index].diffuse = glm::vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
		m_materials[index].specular = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
		m_materials[index].emissive = glm::vec3(m.emission[0], m.emission[1], m.emission[2]);
		m_materials[index].specularPower = m.shininess;
		m_materials[index].opacity = m.dissolve;

		// load / generate material textures
		if (!m_materials[index].alphaTexture.load((folder + m.alpha_texname).c_str()))
		{
			unsigned char pixels[4] { 255, 255, 255, 255 };

			m_materials[index].alphaTexture.create(1, 1, GL_RGBA, pixels);
		}
		if (!m_materials[index].ambientTexture.load((folder + m.ambient_texname).c_str()))
		{
			unsigned char pixels[4]{ 255, 255, 255, 255 };

			m_materials[index].ambientTexture.create(1, 1, GL_RGBA, pixels);
		}
		if (!m_materials[index].diffuseTexture.load((folder + m.diffuse_texname).c_str()))
		{
			unsigned char pixels[4]{ 255, 255, 255, 255 };

			m_materials[index].diffuseTexture.create(1, 1, GL_RGBA, pixels);
		}
		if (!m_materials[index].specularTexture.load((folder + m.specular_texname).c_str()))
		{
			unsigned char pixels[4]{ 255, 255, 255, 255 };

			m_materials[index].specularTexture.create(1, 1, GL_RGBA, pixels);
		}
		if (!m_materials[index].normalTexture.load((folder + m.bump_texname).c_str()))
		{
			unsigned char pixels[4]{ 128, 128, 255, 255 };

			m_materials[index].normalTexture.create(1, 1, GL_RGBA, pixels);
		}
		if (!m_materials[index].displacementTexture.load((folder + m.displacement_texname).c_str()))
		{
			unsigned char pixels[4]{ 0, 0, 0, 255 };

			m_materials[index].displacementTexture.create(1, 1, GL_RGBA, pixels);
		}

		index++;
	}

	// allocate memory for mesh chunks
	m_meshChunks.reserve(shapes.size());
	for (auto& s : shapes)
	{
		MeshChunk chunk;

		// generate buffers
		glGenBuffers(1, &chunk.vbo);
		glGenBuffers(1, &chunk.ibo);
		glGenVertexArrays(1, &chunk.vao);

		// bind vertex array aka a mesh wrapper
		glBindVertexArray(chunk.vao);

		// set the index buffer data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			s.mesh.indices.size() * sizeof(unsigned int),
			s.mesh.indices.data(), GL_STATIC_DRAW);

		// store index count for rendering
		chunk.indexCount = (unsigned int)s.mesh.indices.size();

		// create vertex data
		std::vector<Vertex> vertices;
		vertices.resize(s.mesh.positions.size() / 3);
		size_t vertCount = vertices.size();

		bool hasPosition = s.mesh.positions.empty() == false;
		bool hasNormal = s.mesh.normals.empty() == false;
		bool hasTexture = s.mesh.texcoords.empty() == false;

		for (size_t i = 0; i < vertCount; ++i)
		{
			if (hasPosition)
			{
				vertices[i].position = glm::vec4(s.mesh.positions[i * 3 + 0], s.mesh.positions[i * 3 + 1], s.mesh.positions[i * 3 + 2], 1);
			}
			if (hasNormal)
			{
				vertices[i].normal = glm::vec4(s.mesh.normals[i * 3 + 0], s.mesh.normals[i * 3 + 1], s.mesh.normals[i * 3 + 2], 0);
			}

			// flip the T / V (might not always be needed, depends on how mesh was made)
			if (hasTexture)
			{
				vertices[i].texcoord = glm::vec2(s.mesh.texcoords[i * 2 + 0], flipTextureV ? 1.0f - s.mesh.texcoords[i * 2 + 1] : s.mesh.texcoords[i * 2 + 1]);
			}
		}

		// calculate for normal mapping
		if (hasNormal && hasTexture)
		{
			calculateTangents(vertices, s.mesh.indices);
		}

		// bind vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo);

		// fill vertex buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		// enable first element as positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// enable second element as normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// enable third element as texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

		// enable forth element as tangents
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		// enable fifth element as colors
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// bind 0 for safety
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// set chunk material
		chunk.materialID = s.mesh.material_ids.empty() ? -1 : s.mesh.material_ids[0];

		m_meshChunks.push_back(chunk);
	}

	// load obj
	return true;
}

// draw mesh
void OBJMesh::draw(Shader shader, bool usePatches)
{
	// set texture slots (these don't change per material)
	shader.setInt("material.diffuseTexture", 0);
	shader.setInt("material.alphaTexture", 1);
	shader.setInt("material.ambientTexture", 2);
	shader.setInt("material.specularTexture", 3);
	shader.setInt("material.specularHighlightTexture", 4);
	shader.setInt("material.normalTexture", 5);
	shader.setInt("material.alphaTexture", 6);

	int currentMaterial = -1;

	// draw the mesh chunks
	for (auto& c : m_meshChunks)
	{
		// bind material
		if (currentMaterial != c.materialID)
		{
			currentMaterial = c.materialID;

			shader.setVec3("material.ambient", m_materials[currentMaterial].ambient);
			shader.setVec3("material.diffuse", m_materials[currentMaterial].diffuse);
			shader.setVec3("material.specular", m_materials[currentMaterial].specular);
			shader.setVec3("material.emissive", m_materials[currentMaterial].emissive);

			shader.setFloat("material.opacity", m_materials[currentMaterial].opacity);
			shader.setFloat("material.specularPower", m_materials[currentMaterial].specularPower);

			shader.setFloat("material.roughness", m_materials[currentMaterial].roughness);
			shader.setFloat("material.reflectionCoefficiant", m_materials[currentMaterial].reflectionCoefficient);

			m_materials[currentMaterial].diffuseTexture.bind(0);
			m_materials[currentMaterial].alphaTexture.bind(1);
			m_materials[currentMaterial].ambientTexture.bind(2);
			m_materials[currentMaterial].specularTexture.bind(3);
			m_materials[currentMaterial].specularHighlightTexture.bind(4);
			m_materials[currentMaterial].normalTexture.bind(5);
			m_materials[currentMaterial].alphaTexture.bind(6);
		}

		// bind and draw geometry
		glBindVertexArray(c.vao);
		if (usePatches)
			glDrawElements(GL_PATCHES, c.indexCount, GL_UNSIGNED_INT, 0);
		else
			glDrawElements(GL_TRIANGLES, c.indexCount, GL_UNSIGNED_INT, 0);
	}
}

void OBJMesh::calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	unsigned int vertexCount = (unsigned int)vertices.size();
	glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
	glm::vec4* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

	unsigned int indexCount = (unsigned int)indices.size();
	for (unsigned int a = 0; a < indexCount; a += 3)
	{
		long i1 = indices[a];
		long i2 = indices[a + 1];
		long i3 = indices[a + 2];

		const glm::vec4& v1 = vertices[i1].position;
		const glm::vec4& v2 = vertices[i2].position;
		const glm::vec4& v3 = vertices[i3].position;

		const glm::vec2& w1 = vertices[i1].texcoord;
		const glm::vec2& w2 = vertices[i2].texcoord;
		const glm::vec2& w3 = vertices[i3].texcoord;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r, 0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r, 0);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (unsigned int a = 0; a < vertexCount; a++)
	{
		const glm::vec3& n = glm::vec3(vertices[a].normal);
		const glm::vec3& t = glm::vec3(tan1[a]);

		// Gram-Schmidt orthogonalize
		vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);

		// Calculate handedness (direction of bitangent)
		vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;

		// calculate bitangent (ignoring for our Vertex, here just for reference)
		//vertices[a].bitangent = glm::vec4(glm::cross(glm::vec3(vertices[a].normal), glm::vec3(vertices[a].tangent)) * vertices[a].tangent.w, 0);
		//vertices[a].tangent.w = 0;
	}

	delete[] tan1;
}
