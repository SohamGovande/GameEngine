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

private:
	float computeBarycentricHeight(int chunkX, int chunkZ, FloatXZ bl, FloatXZ tr, FloatXZ other, float x, float z) const;

public:
	World(const ResourceMgr& resourceMgr);
	~World();

	void tick();

	void sendEntities(MasterRenderer& renderer);

	void sendTerrain(MasterRenderer& renderer);

	float getInterpolatedTerrainHeight(float x, float z) const;
	float getExactTerrainHeight(float x, float z) const;

	template<typename... VaArgs>
	Entity& addEntity(VaArgs&&... args);
	Entity& copyEntityIntoWorld(const Entity& object);

	template<typename... VaArgs>
	inline Terrain& addTerrain(VaArgs&&... args) { terrains.emplace_back(args...); return terrains.back(); }

	inline Entity& getPerson() { return *person; }
	inline const Entity& getPerson() const { return *person; }

	inline std::list<Entity>& getEntities() { return entities; };
	inline const std::list<Entity>& getEntities() const { return entities; }
};
