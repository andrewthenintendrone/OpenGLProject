#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad\glad.h>
#include <iostream>

void Texture::loadFromFile(const std::string& filename)
{
	// do not try to load if this texture is already loaded
	assert(id == 0);

	// store filename
	path = filename;

	glGenTextures(1, &id);

	// load file with stbi_image
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	// if the image loaded succesfully
	if (data)
	{
		// determine image format from number of channels
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		// bind our texture id
		glBindTexture(GL_TEXTURE_2D, id);

		// generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// create mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// make the image repeat and use linear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// release image from memory (it is on the gpu now)
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load a Texture from " << path << std::endl;

		// release image from memory
		stbi_image_free(data);
	}
}
