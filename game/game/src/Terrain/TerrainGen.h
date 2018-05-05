#pragma once
#include <random>
#include "HeightGen.h"
#include "Entity/Entity.h"

class World;
class ModelResource;

class TerrainGen
{
private:
	HeightGen heightGen;
	std::mt19937 random;
	std::uniform_real_distribution<float> distribution;
	std::uniform_real_distribution<float> zeroToOne;

	void addObjects(World& world, int chunkX, int chunkZ, unsigned int count, float yOffset, float scale, ModelResource* model, EntityID id);

public:
	TerrainGen(int seed);

	float getTerrainHeight(float posX, float posZ);
	
	void generate(World& world, const ResourceMgr& resourceMgr, int chunkX, int chunkZ);
};