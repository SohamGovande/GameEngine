#include "Renderer/Renderers/MasterRenderer.h"
#include "Entity/Components/MotionComponent.h"
#include "World.h"
#include "ModelLoader.h"

World::World(const ResourceMgr& resourceMgr)
	: entities(), terrains(), 
	person(nullptr)
{
	person = &addEntity(*resourceMgr.playerModel);

	person->addEID(PLAYER);
	person->addComponent(new MotionComponent);
	person->scale = 1.5f;
}

World::~World()
{
	
}

void World::tick()
{
	for (Entity& object : entities)
		object.tick();
}

void World::sendEntities(MasterRenderer& renderer)
{
	for (Entity& object : entities)
		renderer.markEntityForRendering(object);
}

void World::sendTerrain(MasterRenderer& renderer)
{
	for (Terrain& terrain : terrains)
		renderer.processTerrain(terrain);
}

float World::getTerrainHeight(float x, float z) const
{
	int chunkX = (int)(x / TERRAIN_SIZE);
	int chunkZ = (int)(z / TERRAIN_SIZE);

	for (const Terrain& terrain : terrains)
		if (terrain.getChunkX() == chunkX && terrain.getChunkZ() == chunkZ)
			return terrain.getTerrainHeight(x - chunkX * TERRAIN_SIZE, z - chunkZ * TERRAIN_SIZE);

	return 0.0f;
}

template<typename... VaArgs>
Entity& World::addEntity(VaArgs&&... args)
{
	entities.emplace_back(args...);
	return entities.back();
}

Entity& World::copyEntityIntoWorld(const Entity& entity)
{
	entities.push_back(entity);
	return entities.back();
}