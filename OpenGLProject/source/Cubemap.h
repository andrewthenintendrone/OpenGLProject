#pragma once
#include <vector>
#include <string>

class Cubemap
{
	unsigned int id = 0;

	void load(const std::vector<std::string>& filenames);
};