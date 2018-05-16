#pragma once
#include <string>

class Texture
{
public:

	unsigned int id = 0; // opengl texture id
	std::string type; // type of texture (diffuse, specular, normal, etc.)
	std::string path; // file path of the texture

	void loadFromFile(const std::string& filename);
};
