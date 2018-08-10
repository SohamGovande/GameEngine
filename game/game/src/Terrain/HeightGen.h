#pragma once
#include <random>
#include "TerrainConstants.h"
#include "Vendor/PerlinNoise.h"

#define TERRAIN_HEIGHT_AMPLITUDE 90.0f
#define TERRAIN_OCTAVE_COUNT 3

class HeightGen
{
private:
	int seed;
	std::mt19937 random;
	std::uniform_real_distribution<float> generator;
	PerlinNoise noise[TERRAIN_OCTAVE_COUNT];

public:

	HeightGen(int seed);
	float generateHeight(float x, float z);
};

