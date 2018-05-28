#include "Cubemap.h"
#include <cassert>
#include <iostream>
#include <glad\glad.h>
#include <stb\stb_image.h>

void Cubemap::load(const std::vector<std::string>& filenames)
{
	assert(id == 0);

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, channels;
	for (unsigned int i = 0; i < filenames.size(); i++)
	{
		unsigned char* data = stbi_load(filenames[i].c_str(), &width, &height, &channels, 0);

		if (data)
		{
			std::cout << "Loaded texture from " << filenames[i] << std::endl;

			// determine texture format
			GLenum format;
			switch (channels)
			{
				// 1 channel texture
			case(1):
				format = GL_RED;
				break;
				// 3 channel rgb texture
			case(3):
				format = GL_RGB;
				break;
				// 4 channel rgba texture
			case(4):
				format = GL_RGBA;
				break;
			default:
				std::cout << "Unknown number of channels\n";
				break;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load a texture from " << filenames[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
