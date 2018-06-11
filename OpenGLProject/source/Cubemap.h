#pragma once
#include <vector>
#include <string>

class Cubemap
{
public:
	unsigned int id = 0;

	void load(std::vector<std::string> filenames);

	void bind(unsigned int slot) const;

private:

	std::vector<std::string> m_filenames;
};