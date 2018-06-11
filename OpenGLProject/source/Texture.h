#pragma once
#include <string>
#include "Color.h"

class Texture
{
public:

	enum Format : unsigned int
	{
		RED = 1,
		RG,
		RGB,
		RGBA
	};

	Texture();
	Texture(const char* filename);
	Texture(unsigned int width, unsigned int height, Format format, unsigned char* pixels = nullptr);
	virtual ~Texture();

	bool load(const char* filename);

	void create(unsigned int width, unsigned int height, Format format, unsigned char* pixels = nullptr);

	void createDummy(Color color);

	const std::string& getFilename() const { return m_filename; }

	void bind(unsigned int slot) const;

	unsigned int getHandle() const { return m_glHandle; }

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	const unsigned char* getPixels() const { return m_loadedPixels; }

protected:

	std::string m_filename;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_glHandle;
	unsigned int m_format;
	unsigned char* m_loadedPixels;
};
