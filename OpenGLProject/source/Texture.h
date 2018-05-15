#pragma once
#include <string>

class Texture
{
public:

	unsigned int id; // opengl texture id
	std::string type; // type of texture (diffuse, specular, normal, etc.)
	std::string path; // file path of the texture
};
