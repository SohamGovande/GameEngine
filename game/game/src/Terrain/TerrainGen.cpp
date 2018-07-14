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

void TerrainGen::addObjects(World& world, int chunkX, int chunkZ, unsigned int count, float yOffset, float scale, const EntityConstructor& constructor)
{
	for (unsigned int i = 0; i < count; i++)
	{
		const float x = distribution(random) + chunkX * TERRAIN_SIZE;
		const float z = distribution(random) + chunkZ * TERRAIN_SIZE;
		const float y = getTerrainHeight(x, z);

		Entity& entity = world.newEntity();

		constructor.construct(entity);

		entity.scale = scale;
		entity.moveTo(glm::vec3(x, y + yOffset, z));
	}
}

void TerrainGen::generate(World& world, ResourceMgr& resourceMgr, const EntityRegistry& entityRegistry, int chunkX, int chunkZ)
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
	
	addObjects(world, chunkX, chunkZ, 2, -5, 6, entityRegistry.getConstructor("evergreen_tree"));
	addObjects(world, chunkX, chunkZ, 2, -5, 3, entityRegistry.getConstructor("green_tree"));
	addObjects(world, chunkX, chunkZ, 25, 0, 3, entityRegistry.getConstructor("fern"));
	addObjects(world, chunkX, chunkZ, 1, 1, 2, entityRegistry.getConstructor("axe"));
	for (Entity& entity : world.getEntities())
		if (entity.hasEntityID(3))
			entity.rotation.x = 90;

	addObjects(world, chunkX, chunkZ, 1, 10, 10, entityRegistry.getConstructor("cube"));
	addObjects(world, chunkX, chunkZ, 1, 10, 2, entityRegistry.getConstructor("barrel"));
	unsigned int lanterns = world.countEntitiesByID(7);
	if (lanterns < 3)
		addObjects(world, chunkX, chunkZ, 1, 0, 8, entityRegistry.getConstructor("lantern"));
}
