#pragma once
#include <random>
#include "HeightGen.h"
#include "Entity/Entity.h"
#include "Entity/EntityRegistry.h"
#include "Entity/EntityConstructor.h"

class World;
class ModelResource;

class TerrainGen
{
private:
	HeightGen heightGen;
	std::mt19937 random;
	std::uniform_real_distribution<float> distribution;
	std::uniform_real_distribution<float> zeroToOne;

	void addObjects(World& world, int chunkX, int chunkZ, unsigned int count, float yOffset, float scale, const EntityConstructor& constructor);

public:
	TerrainGen(int seed);

	float getTerrainHeight(float posX, float posZ);
	
	void generate(World& world, ResourceMgr& resourceMgr, const EntityRegistry& entityRegistry, int chunkX, int chunkZ);
};