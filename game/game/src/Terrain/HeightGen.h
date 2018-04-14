#pragma once
#include <random>
#include "TerrainConstants.h"
#include "Vendor/PerlinNoise.h"

#define TERRAIN_HEIGHT_AMPLITUDE 150.0f
#define TERRAIN_OCTAVE_COUNT 2

class HeightGen
{
private:
	int seed;
	std::mt19937 random;
	std::uniform_real_distribution<float> generator;
	PerlinNoise noise[TERRAIN_OCTAVE_COUNT];

public:

	HeightGen(int seed);
	~HeightGen();
	float generateHeight(float x, float z);

	float getInterpolatedNoise(float x, float z);

	inline float getSmoothNoise(float x, float z)
	{
		float corners = (getNoise(x - TERRAIN_INTERVAL, z - TERRAIN_INTERVAL)
			+ getNoise(x + TERRAIN_INTERVAL, z - TERRAIN_INTERVAL)
			+ getNoise(x - TERRAIN_INTERVAL, z + TERRAIN_INTERVAL)
			+ getNoise(x + TERRAIN_INTERVAL, z + TERRAIN_INTERVAL)) / 4.0f / 4.0f; //4 corners, and corners will have a 25% weight so / 4.0 again
		float sides = (getNoise(x - TERRAIN_INTERVAL, z)
			+ getNoise(x + TERRAIN_INTERVAL, z)
			+ getNoise(x, z - TERRAIN_INTERVAL)
			+ getNoise(x, z + TERRAIN_INTERVAL)) / 4.0f / 2.0f;
		float center = getNoise(x, z) / 4.0f / 1.0f;

		return corners + sides + center;
	}

	inline float getNoise(float x, float z)
	{
		random.seed((unsigned int) (x * 2975 + z * 8715 + seed + 91763));
		return generator(random) * TERRAIN_HEIGHT_AMPLITUDE;
	}
};

