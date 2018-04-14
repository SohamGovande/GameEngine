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

void TerrainGen::generate(World& world, const ResourceMgr& resourceMgr, int chunkX, int chunkZ)
{
	Terrain* terrain = new Terrain(*this, chunkX, chunkZ);
	for (float z = 0; z < TERRAIN_VERTEX_COUNT; z++)
	{
		for (float x = 0; x < TERRAIN_VERTEX_COUNT; x++)
		{
			float posX = chunkX * TERRAIN_SIZE + x * TERRAIN_INTERVAL;
			float posZ = chunkZ * TERRAIN_SIZE + z * TERRAIN_INTERVAL;

			terrain->setTerrainHeight(posX, posZ, getTerrainHeight(posX, posZ));
		}
	}
	
	terrain->generateMesh(resourceMgr);

	MaterialModel* treeModel = *resourceMgr.treeModel;
	treeModel->fullyRender = true;

	MaterialModel* fernModel = *resourceMgr.axeModel;
	
	for (int i = 0; i < 10; i++)
	{
		float x = distribution(random) + chunkX * TERRAIN_SIZE;
		float z = distribution(random) + chunkZ * TERRAIN_SIZE;
		float y = getTerrainHeight(x, z);

		Entity entity(treeModel);
		
		entity.addEID(TREE);
		entity.scale = 6;
		entity.position.x = x;
		entity.position.y = y - 1.5f;
		entity.position.z = z;
		entity.rotation.y = zeroToOne(random) * 360;

		world.addEntity(entity);
	}
	
	for (int i = 0; i < 25; i++)
	{
		float x = distribution(random) + chunkX * TERRAIN_SIZE;
		float z = distribution(random) + chunkZ * TERRAIN_SIZE;
		float y = getTerrainHeight(x, z);

		Entity entity(fernModel);

		entity.addEID(FERN);

		entity.scale = 1;
		entity.position.x = x;
		entity.position.y = y;
		entity.position.z = z;

		world.addEntity(entity);
	}

	world.addTerrain(terrain);
}
