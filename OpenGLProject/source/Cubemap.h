#pragma once
#include <vector>
#include <string>
#include "Texture.h"

class Cubemap
{
public:

	void load(std::vector<std::string> filenames);

	void bind(unsigned int slot) const;

	unsigned int getHandle() const { return m_glHandle; }

protected:

	std::vector<std::string> m_filenames;
	unsigned int m_glHandle = 0;
	unsigned int m_format = 0;
	unsigned char* m_loadedPixels = nullptr;
};