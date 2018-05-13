#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

extern unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
	
	// model data
	vector<Texture> loaded_textures;
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

	Model() {}

	// constructor, expects a filepath to a 3D model.
	Model(string const& path, bool gamma = false);

	// draws the model
	void Draw(Shader shader);

private:

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(const string& path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// loads textures if they aren't already loaded
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
#endif