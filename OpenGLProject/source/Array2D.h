#pragma once
#include <vector>
#include <iostream>

template <class T>
class Array2D
{
public:

	Array2D(unsigned int width = 1, unsigned int height = 1) : m_width(width), m_height(height)
	{
		m_buffer.resize(m_width * m_height);
	}

	const T& operator () (unsigned int x, unsigned int y) const
	{
		if (y * m_width + x < m_buffer.size())
		{
			return m_buffer[y * m_width + x];
		}
		else
		{
			std::cout << "Tried to access element " << x << "-" << y << " of a " << m_width << "-" << m_height << " array" << std::endl;
			return m_buffer.front();
		}
	}

	T& operator () (unsigned int x, unsigned int y)
	{
		if (y * m_width + x < m_buffer.size())
		{
			return m_buffer[y * m_width + x];
		}
		else
		{
			std::cout << "Tried to access element " << x << "-" << y << " of a " << m_width << "-" << m_height << " array" << std::endl;
			return m_buffer.front();
		}
	}

private:

	unsigned int m_width;
	unsigned int m_height;

	std::vector<T> m_buffer;
};