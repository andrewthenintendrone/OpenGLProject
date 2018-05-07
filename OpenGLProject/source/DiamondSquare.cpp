#include "DiamondSquare.h"
#include <random>

DiamondSquare& DiamondSquare::getInstance()
{
	static DiamondSquare instance;
	return instance;
}

DiamondSquare::DiamondSquare()
{
	for (int y = 0; y < height; y += featureSize)
	{
		for (int x = 0; x < width; x += featureSize)
		{
			setSample(x, y, rand() % 100 / 100.0f);
		}
	}

	init();
}

void DiamondSquare::init()
{
	int sampleSize = featureSize;
	float scale = 1.0f;

	while (sampleSize > 1)
	{
		diamondSquare(sampleSize, scale);

		sampleSize /= 2;
		scale /= 2.0f;
	}
}

void DiamondSquare::diamondSquare(int stepSize, float scale)
{
	int halfStep = stepSize / 2;

	for (int y = halfStep; y < height + halfStep; y += stepSize)
	{
		for (int x = halfStep; x < width + halfStep; x += stepSize)
		{
			sampleSquare(x, y, stepSize, rand() % 100 / 100.0f * scale);
		}
	}

	for (int y = 0; y < height; y += stepSize)
	{
		for (int x = 0; x < width; x += stepSize)
		{
			sampleDiamond(x + halfStep, y, stepSize, rand() % 100 / 100.0f * scale);
			sampleDiamond(x, y + halfStep, stepSize, rand() % 100 / 100.0f * scale);
		}
	}
}

void DiamondSquare::sampleSquare(int x, int y, int size, float value)
{
	int hs = size / 2;

	float a = sample(x - hs, y - hs);
	float b = sample(x + hs, y - hs);
	float c = sample(x - hs, y + hs);
	float d = sample(x + hs, y + hs);

	setSample(x, y, ((a + b + c + d) / 4.0f) + value);
}

void DiamondSquare::sampleDiamond(int x, int y, int size, float value)
{
	int hs = size / 2;

	float a = sample(x - hs, y - hs);
	float b = sample(x + hs, y - hs);
	float c = sample(x - hs, y + hs);
	float d = sample(x + hs, y + hs);

	setSample(x, y, ((a + b + c + d) / 4.0f) + value);
}

float DiamondSquare::sample(int x, int y)
{
	return values[(x & (width - 1)) + (y & (height - 1)) * width];
}

void DiamondSquare::setSample(int x, int y, float value)
{
	values[(x & (width - 1)) + (y & (height - 1)) * width] = value;
}
