#include "ResourceMgr/ModelResource.h"
#include "HeightGen.h"
#include "Terrain.h"
#include "TerrainGen.h"
#include "World.h"


TerrainGen::TerrainGen(int seed)
	: heightGen(seed),
	random(),
	distribution(0, TERRAIN_SIZE),
	zeroToOne(0, 1)
{
	random.seed(seed);
}

float TerrainGen::getTerrainHeight(float posX, float posZ)
{
	return heightGen.generateHeight(posX, posZ);
}

void TerrainGen::addObjects(World& world, int chunkX, int chunkZ, unsigned int count, float yOffset, float scale, ModelResource* model, EntityID id)
{
	for (unsigned int i = 0; i < count; i++)
	{
		const float x = distribution(random) + chunkX * TERRAIN_SIZE;
		const float z = distribution(random) + chunkZ * TERRAIN_SIZE;
		const float y = getTerrainHeight(x, z);

		Entity entity(*model);

		entity.addEID(id);

		entity.scale = scale;
		entity.position.x = x;
		entity.position.y = y + yOffset;
		entity.position.z = z;

		world.copyEntityIntoWorld(entity);
	}
}

void TerrainGen::generate(World& world, const ResourceMgr& resourceMgr, int chunkX, int chunkZ)
{
	Terrain& terrain = world.addTerrain(resourceMgr, *this, chunkX, chunkZ);

	for (float z = 0; z < TERRAIN_VERTEX_COUNT; z++)
	{
		for (float x = 0; x < TERRAIN_VERTEX_COUNT; x++)
		{
			float posX = x * TERRAIN_INTERVAL;
			float posZ = z * TERRAIN_INTERVAL;

			terrain.setTerrainHeight(posX, posZ, getTerrainHeight(chunkX * TERRAIN_SIZE + posX, chunkZ * TERRAIN_SIZE + posZ));
		}
	}

	

	terrain.generateMesh(resourceMgr);

	addObjects(world, chunkX, chunkZ, 5, 0, 4, resourceMgr.evergreenTree, TREE);
	addObjects(world, chunkX, chunkZ, 25, 0, 3, resourceMgr.fernModel, FERN);
	addObjects(world, chunkX, chunkZ, 2,  0, 2, resourceMgr.axeModel, LANTERN);
}
