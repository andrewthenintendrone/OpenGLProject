#include "Cubemap.h"
#include <cassert>
#include <iostream>
#include <glad\glad.h>
#include <stb\stb_image.h>

// use one file for all sides
void Cubemap::load(std::string filename)
{
	// don't try to load if this cubemap is already initialised
	assert(m_glHandle == 0);

	// generate textures
	glGenTextures(1, &m_glHandle);

	// bind the cube map
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_glHandle);

	// create variables for texture size and format
	int x = 0, y = 0, comp = 0;
	unsigned char* data;

	// attempt to read texture
	data = stbi_load(filename.c_str(), &x, &y, &comp, 0);

	// if data was read succesfully
	if (data)
	{
		// determine texture format
		switch (comp)
		{
			// 1 channel texture
		case(1):
			m_format = GL_RED;
			break;
			// 3 channel rgb texture
		case(3):
			m_format = GL_RGB;
			break;
			// 4 channel rgba texture
		case(4):
			m_format = GL_RGBA;
			break;
		default:
			std::cout << "Unknown number of channels\n";
			break;
		}

		// transfer texture data to gpu
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_format, x, y, 0, m_format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
		}

		// free image data (it is already on the GPU)
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load a texture from " << filename << std::endl;
		stbi_image_free(data);
	}

	// enable texture filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// enable texture clamp
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::load(std::vector<std::string> filenames)
{
	// don't try to load if this cubemap is already initialised
	assert(m_glHandle == 0);

	if (filenames.size() != 6)
	{
		std::cout << "Cubemap must use 6 textures\n";
		return;
	}

	// store filenames
	m_filenames = filenames;

	// generate textures
	glGenTextures(1, &m_glHandle);

	// bind the cube map
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_glHandle);

	// create variables for texture size and format
	int x = 0, y = 0, comp = 0;
	unsigned char* data;

	// for each texture
	for (GLuint i = 0; i < filenames.size(); i++)
	{
		// attempt to read texture
		data = stbi_load(filenames[i].c_str(), &x, &y, &comp, 0);

		// if data was read succesfully
		if (data)
		{
			// determine texture format
			switch (comp)
			{
				// 1 channel texture
			case(1):
				m_format = GL_RED;
				break;
				// 3 channel rgb texture
			case(3):
				m_format = GL_RGB;
				break;
				// 4 channel rgba texture
			case(4):
				m_format = GL_RGBA;
				break;
			default:
				std::cout << "Unknown number of channels\n";
				break;
			}

			// transfer texture data to gpu
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_format, x, y, 0, m_format, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			// free image data (it is already on the GPU)
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load a texture from " << filenames[i] << std::endl;
			stbi_image_free(data);
		}
	}

	// enable texture filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// enable texture clamp
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_glHandle);
}
