#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

#include "Texture.h"
#include "Vertex.h"

class Mesh
{

public:
	
	// mesh data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO; // vertex array object

	// default constructor
	Mesh() {}

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void Draw(Shader shader);

protected:
	
	unsigned int VBO; // vertex buffer object
	unsigned int EBO; // element buffer object

	void setupMesh();
};
#endif