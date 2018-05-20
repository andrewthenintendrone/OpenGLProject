#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <glad\glad.h>
#include <iostream>

void Texture::load(const std::string& filename)
{
	// don't try to load if this texture is already initialised
	assert(id == 0);

	// store file path
	path = filename;

	// generate textures
	glGenTextures(1, &id);

	// create variables for texture size and format
	int width, height, channels;

	// attempt to read texture
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	// if data was read succesfully
	if (data)
	{
		std::cout << "Loaded texture from " << path << std::endl;

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

		// bind our texture id
		glBindTexture(GL_TEXTURE_2D, id);

		// transfer texture data to gpu
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// generate mip maps
		glGenerateMipmap(GL_TEXTURE_2D);

		// set texture repeat settings
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filter settings
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// free image data (it is already on the GPU)
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load a texture from " << path << std::endl;
		stbi_image_free(data);
		return;
	}
}
