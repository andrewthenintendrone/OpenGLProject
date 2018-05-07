#pragma once

class DiamondSquare
{
public:

	static DiamondSquare& getInstance();

	float sample(int x, int y);
	void setSample(int x, int y, float value);

private:

	// private constructor
	DiamondSquare();

	float featureSize = 32.0f;

	void init();

	void diamondSquare(int stepSize, float scale);

	void sampleSquare(int x, int y, int size, float value);
	void sampleDiamond(int x, int y, int size, float value);

	int width = 256;
	int height = 256;

	float values[256 * 256];
};