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

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id; // opengl texture id
	string type; // type of texture (diffuse, specular, normal, etc.)
	string path; // file path of the texture
};

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

	// constructor using list of vertices, indices, and textures
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	// render the mesh using the specified shader
	void Draw(Shader shader);

protected:
	
	unsigned int VBO; // vertex buffer object
	unsigned int EBO; // element buffer object

	
	// initializes all the buffer objects/arrays
	void setupMesh();
};
#endif