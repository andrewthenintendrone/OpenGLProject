#pragma once
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include "Vertex.h"
#include "Material.h"
#include "MeshChunk.h"
#include "Shader.h"

class OBJMesh
{
public:

	// constructor / destructor
	OBJMesh() {};
	OBJMesh(const std::string& filename) { load(filename); }
	~OBJMesh();

	bool load(const std::string& filename);

	void toggleNormalMaps();

	void draw(Shader shader, bool usePatches = false);

	const std::string& getFilename() const { return m_filename; }

	size_t getMaterialCount() const { return m_materials.size(); }
	Material& getMaterial(size_t index) { return m_materials[index]; }

private:

	void calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	std::string				m_filename;
	std::vector<MeshChunk>	m_meshChunks;
	std::vector<Material>	m_materials;
};