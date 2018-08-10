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

float HeightGen::generateHeight(float x, float z)
{
	constexpr float octaveFactor = 0.4f;
	constexpr float terrainSmoothness = 320.0f;
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
