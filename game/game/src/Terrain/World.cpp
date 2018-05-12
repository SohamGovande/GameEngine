#include "Renderer/Renderers/MasterRenderer.h"
#include "Entity/Components/MotionComponent.h"
#include "World.h"
#include "ModelLoader.h"

World::World(const ResourceMgr& resourceMgr)
	: entities(), terrains(), 
	person(nullptr)
{
	Entity& lantern = addEntity(*resourceMgr.lanternModel);
	lantern.scale = 4;

	person = &addEntity(*resourceMgr.playerModel);

	person->addEID(PLAYER);
	person->addComponent(new MotionComponent);
	person->scale = 1.5f;

	Entity& bt0 = addEntity(*resourceMgr.birchTree);
	bt0.scale = 5;
	bt0.moveTo(glm::vec3(0, 30, 0));

	Entity& bt1 = addEntity(*resourceMgr.bt1);
	bt1.scale = 5;
	bt1.moveTo(glm::vec3(30, 30, 0));
	
	Entity& bt2 = addEntity(*resourceMgr.bt2);
	bt2.scale = 5;
	bt2.moveTo(glm::vec3(60, 30, 0));

	Entity& dragon = addEntity(*resourceMgr.dragonModel);
	dragon.scale = 5;
	dragon.moveTo(glm::vec3(0, 30, 60));
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
			return terrain.getTerrainHeight(x, z);

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