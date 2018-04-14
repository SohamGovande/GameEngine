#include "Renderer/Renderers/MasterRenderer.h"
#include "Entity/Components/MotionComponent.h"
#include "World.h"
#include "ModelLoader.h"

World::World(const ResourceMgr& resourceMgr)
	: entities(), terrains(), 
	person(nullptr)
{
	Entity temp(*resourceMgr.playerModel);
	person = &addEntity(temp);

	person->addEID(PLAYER);
	person->addComponent(new MotionComponent);
//	person->position.x = 35;
//	person->position.z = 35;
	person->scale = 1.5f;
}

World::~World()
{
	for (Terrain* terrain : terrains)
		delete terrain;
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
	for (Terrain* terrain : terrains)
		renderer.processTerrain(terrain);
}

float World::getTerrainHeight(float x, float z) const
{
	int chunkX = (int)(x / TERRAIN_SIZE);
	int chunkZ = (int)(z / TERRAIN_SIZE);

	for (Terrain* terrain : terrains)
		if (terrain->getChunkX() == chunkX && terrain->getChunkZ() == chunkZ)
			return terrain->getTerrainHeight(x, z);

	return 0.0f;
}


Entity& World::addEntity(const Entity& object)
{
	entities.push_back(object);

	return entities.back();
}

