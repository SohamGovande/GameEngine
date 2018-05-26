#include "HeightGen.h"
#include "MathUtils.h"
#include "TerrainConstants.h"

HeightGen::HeightGen(int seed)
	: seed(seed * 982 + 9876), 
	generator(0.0f, 1.0f),
	noise()
{
	for (int i = 0; i < TERRAIN_OCTAVE_COUNT; i++)
		noise[i] = PerlinNoise(seed + i);
}

HeightGen::~HeightGen()
{
}

float HeightGen::generateHeight(float x, float z)
{
	constexpr float octaveFactor = 0.3f;
	constexpr float terrainSmoothness = 320;
	float cOctaveFactor = 1;

	float total = 0;
	for (int i = 0; i < TERRAIN_OCTAVE_COUNT; i++)
	{
		total += (float) (noise[i].noise(
			x / terrainSmoothness / cOctaveFactor,
			z / terrainSmoothness / cOctaveFactor,
			0.8
		) * cOctaveFactor * TERRAIN_HEIGHT_AMPLITUDE);
		cOctaveFactor *= octaveFactor;
	}
	return total;
}

float HeightGen::getInterpolatedNoise(float x, float z)
{
	float intX = Math::roundDown(x, TERRAIN_INTERVAL);
	float intZ = Math::roundDown(z, TERRAIN_INTERVAL);
	float fracX = (x - intX) / TERRAIN_INTERVAL;
	float fracZ = (z - intZ) / TERRAIN_INTERVAL;

	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + TERRAIN_INTERVAL, intZ);
	float v3 = getSmoothNoise(intX, intZ + TERRAIN_INTERVAL);
	float v4 = getSmoothNoise(intX + TERRAIN_INTERVAL, intZ + TERRAIN_INTERVAL);

	float i1 = Math::cosineInterpolation(v1, v2, fracX);
	float i2 = Math::cosineInterpolation(v3, v4, fracX);
	return Math::cosineInterpolation(i1, i2, fracZ);
}


