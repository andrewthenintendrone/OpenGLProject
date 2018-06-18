#include <glad\glad.h>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

Texture::Texture(const char* filename)
{
	load(filename);
}

Texture::Texture(unsigned int width, unsigned int height, GLenum format, unsigned char* pixels)
{
	create(width, height, format, pixels);
}

Texture::~Texture()
{
	if (m_glHandle != 0)
		glDeleteTextures(1, &m_glHandle);
	if (m_loadedPixels != nullptr)
		stbi_image_free(m_loadedPixels);
}

bool Texture::load(const char* filename)
{
	// discard old texture if there is one
	if (m_glHandle != 0)
	{
		glDeleteTextures(1, &m_glHandle);
		m_glHandle = 0;
		m_width = 0;
		m_height = 0;
		m_filename = "none";
	}

	int x = 0, y = 0, comp = 0;
	m_loadedPixels = stbi_load(filename, &x, &y, &comp, STBI_default);

	if (m_loadedPixels)
	{
		glGenTextures(1, &m_glHandle);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);

		switch (comp)
		{
		case STBI_grey:
			m_format = GL_ALPHA;
			break;
		case STBI_grey_alpha:
			m_format = GL_RG;
			break;
		case STBI_rgb:
			m_format = GL_RGB;
			break;
		case STBI_rgb_alpha:
			m_format = GL_RGBA;
			break;
		default:
			break;
		};

		glTexImage2D(GL_TEXTURE_2D, 0, m_format, x, y, 0, m_format, GL_UNSIGNED_BYTE, m_loadedPixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_width = (unsigned int)x;
		m_height = (unsigned int)y;
		m_filename = filename;
		return true;
	}

	return false;
}

void Texture::create(unsigned int width, unsigned int height, GLenum format, unsigned char* pixels)
{
	if (m_glHandle != 0)
	{
		glDeleteTextures(1, &m_glHandle);
		m_glHandle = 0;
		m_filename = "none";
	}

	m_width = width;
	m_height = height;
	m_format = format;

	glGenTextures(1, &m_glHandle);
	glBindTexture(GL_TEXTURE_2D, m_glHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::createDummy(Color color)
{
	if (m_glHandle != 0)
	{
		glDeleteTextures(1, &m_glHandle);
		m_glHandle = 0;
		m_filename = "none";
	}

	m_width = 1;
	m_height = 1;
	m_format = GL_RGBA;

	glGenTextures(1, &m_glHandle);
	glBindTexture(GL_TEXTURE_2D, m_glHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	unsigned char pixels[4] { color.r, color.g, color.b, color.a };

	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_glHandle);
}