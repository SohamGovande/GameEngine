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
	std::list<Terrain> terrains;
	Entity* person;

public:
	World(const ResourceMgr& resourceMgr);
	~World();

	void tick();

	void sendEntities(MasterRenderer& renderer);

	void sendTerrain(MasterRenderer& renderer);

	float getTerrainHeight(float x, float z) const;

	template<typename... VaArgs>
	Entity& addEntity(VaArgs&... args);
	Entity& copyEntityIntoWorld(const Entity& object);

	inline Terrain& addTerrain(const Terrain& terrain) { terrains.push_back(terrain); return terrains.back(); }

	inline Entity& getPerson() { return *person; }

	inline std::list<Entity>& getEntities() { return entities; };
};
