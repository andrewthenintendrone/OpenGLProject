#pragma once
#include <string>
#include "Color.h"
#include <glad\glad.h>

class Texture
{
public:

	Texture() {};
	Texture(const char* filename);
	Texture(unsigned int width, unsigned int height, GLenum format, unsigned char* pixels = nullptr);
	virtual ~Texture();

	bool load(const char* filename);

	void create(unsigned int width, unsigned int height, GLenum format, unsigned char* pixels = nullptr);

	void createDummy(Color color);

	const std::string& getFilename() const { return m_filename; }

	void bind(unsigned int slot) const;

	unsigned int getHandle() const { return m_glHandle; }

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	const unsigned char* getPixels() const { return m_loadedPixels; }

protected:

	std::string m_filename = "none";
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	unsigned int m_glHandle = 0;
	unsigned int m_format = 0;
	unsigned char* m_loadedPixels = nullptr;
};
