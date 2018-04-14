#pragma once
#include <random>
#include "HeightGen.h"

class World;

class TerrainGen
{
private:
	HeightGen heightGen;
	std::mt19937 random;
	std::uniform_real_distribution<float> distribution;
	std::uniform_real_distribution<float> zeroToOne;

public:
	TerrainGen(int seed);

	float getTerrainHeight(float posX, float posZ);
	
	void generate(World& world, const ResourceMgr& resourceMgr, int chunkX, int chunkZ);
};