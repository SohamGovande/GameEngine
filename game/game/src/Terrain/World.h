#pragma once
#include <vector>

#include "Entity/Entity.h"
#include "Terrain/Terrain.h"
#include "ResourceMgr/ResourceMgr.h"
#include "Renderer/Renderers/MasterRenderer.h"
#include "MathUtils.h"

class World
{
private:
	std::list<Entity> entities;
	std::vector<Terrain*> terrains;
	Entity* person;

public:
	World(const ResourceMgr& resourceMgr);
	~World();

	void tick();

	void sendEntities(MasterRenderer& renderer);

	void sendTerrain(MasterRenderer& renderer);

	float getTerrainHeight(float x, float z) const;

	Entity& addEntity(const Entity& object);

	inline void addTerrain(Terrain* terrain) { terrains.push_back(terrain); }

	inline Entity& getPerson() { return *person; }

	inline std::list<Entity>& getEntities() { return entities; };
};
