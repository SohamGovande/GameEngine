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
	float computeBarycentricHeight(int chunkX, int chunkZ, const glm::vec2& bl, const glm::vec2& tr, const glm::vec2& other, float x, float z) const;

public:
	World(ResourceMgr& resourceMgr);
	~World();

	void tick();

	void sendEntities(MasterRenderer& renderer);

	void sendTerrain(MasterRenderer& renderer);

	float getInterpolatedTerrainHeight(float x, float z) const;
	float getExactTerrainHeight(float x, float z) const;

	template<typename... ConstructorArgs>
	Entity& newEntity(ConstructorArgs&&... args);

	template<typename... VaArgs>
	inline Terrain& addTerrain(VaArgs&&... args) { terrains.emplace_back(args...); return terrains.back(); }

	inline Entity& getPerson() { return *person; }
	inline const Entity& getPerson() const { return *person; }

	inline std::list<Entity>& getEntities() { return entities; };
	inline const std::list<Entity>& getEntities() const { return entities; }

	inline std::vector<Entity*> getEntitiesByID(unsigned int id)
	{
		std::vector<Entity*> targets;
		for (Entity& entity : entities)
			if (entity.hasEntityID(id))
				targets.push_back(&entity);
		return targets;
	}

	inline unsigned int countEntitiesByID(unsigned int id)
	{
		return std::count_if(entities.begin(), entities.end(), [id](const Entity& entity) {
			return entity.hasEntityID(id);
		});
	}
};
