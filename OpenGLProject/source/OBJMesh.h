#pragma once
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include "Vertex.h"
#include "Material.h"
#include "MeshChunk.h"

class OBJMesh
{
public:

	// constructor / destructor
	OBJMesh() {};
	~OBJMesh();

	bool load(const std::string& filename, bool loadTextures = true, bool flipTextureV = false);

	void draw(bool usePatches = false);

	const std::string& getFilename() const { return m_filename; }

	size_t getMaterialCount() const { return m_materials.size(); }
	Material& getMaterial(size_t index) { return m_materials[index]; }

private:

	void calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	std::string				m_filename;
	std::vector<MeshChunk>	m_meshChunks;
	std::vector<Material>	m_materials;
};