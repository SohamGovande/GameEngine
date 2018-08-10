#include "Renderer/Renderers/MasterRenderer.h"
#include "Entity/Components/MotionComponent.h"
#include "World.h"
#include "ModelLoader.h"
#include "MathUtils.h"

World::World(const EntityRegistry& entityRegistry)
	: entities(), terrains(), 
	person(nullptr)
{
	person = &newEntity();
	entityRegistry.construct("player", *person);
	person->moveTo(glm::vec3(0, 100, 0));
}

void World::tick()
{
 	for (Entity& object : entities)
		object.tick();
}

void World::sendEntities(MasterRenderer& renderer)
{
	for (Entity& object : entities)
		renderer.addEntity(object);
}

void World::sendTerrain(MasterRenderer& renderer)
{
	for (Terrain& terrain : terrains)
		renderer.processTerrain(terrain);
}

float World::getInterpolatedTerrainHeight(float x, float z) const
{
	int chunkX = (int)floorf(x / TERRAIN_SIZE);
	int chunkZ = (int)floorf(z / TERRAIN_SIZE);

	for (const Terrain& terrain : terrains)
	{
		if (terrain.getChunkX() == chunkX && terrain.getChunkZ() == chunkZ)
		{
			/*
			down = X-
			up = X+
			left = Z-
			right = Z+
			*/
			const float relativeX = x - chunkX * TERRAIN_SIZE;
			const float relativeZ = z - chunkZ * TERRAIN_SIZE;

			const glm::vec2 bl{ (unsigned int)(relativeX / TERRAIN_INTERVAL) * TERRAIN_INTERVAL, (unsigned int)(relativeZ / TERRAIN_INTERVAL) * TERRAIN_INTERVAL };
			const glm::vec2 br{ bl.x, bl.y + TERRAIN_INTERVAL };
			const glm::vec2 tl{ bl.x + TERRAIN_INTERVAL, bl.y };
			const glm::vec2 tr{ tl.x, br.y };

			const glm::vec2 offset{ relativeX - bl.x, relativeZ - bl.y };
			glm::vec2 coords(x, z);
			glm::vec2 relativeCoords = (glm::vec2(relativeX, relativeZ) - (glm::vec2&)bl) / TERRAIN_INTERVAL;

			float hBl = getExactTerrainHeight(chunkX * TERRAIN_SIZE + bl.x, chunkZ * TERRAIN_SIZE + bl.y);
			float hBr = getExactTerrainHeight(chunkX * TERRAIN_SIZE + br.x, chunkZ * TERRAIN_SIZE + br.y);
			float hTl = getExactTerrainHeight(chunkX * TERRAIN_SIZE + tl.x, chunkZ * TERRAIN_SIZE + tl.y);
			float hTr = getExactTerrainHeight(chunkX * TERRAIN_SIZE + tr.x, chunkZ * TERRAIN_SIZE + tr.y);

			if (offset.x > offset.y) //triangle is bottomleft, topleft, topright
			{
				return Math::getBarycentricHeight(
					{0, hBl, 0},
					{1, hTl, 0},
					{1, hTr, 1},
					relativeCoords
				);
			}
			else if (offset.x < offset.y) //triangle is bottomleft, bottomright,  topright
			{
				return Math::getBarycentricHeight(
					{ 0, hBl, 0 },
					{ 0, hBr, 1 },
					{ 1, hTr, 1 },
					relativeCoords
				);
			}
			else /*if (offset.x == offset.z)*/ //it lies on the line y = x
			{
				const float interpolation = offset.x / TERRAIN_INTERVAL;
				const float beginHeight = terrain.getHeight(bl.x, bl.y);
				return (terrain.getHeight(tr.x, tr.y) - beginHeight) * interpolation + beginHeight;
			}
			return 0;
		}
	}
	return 0.0f;
}

float World::computeBarycentricHeight(int chunkX, int chunkZ, const glm::vec2& bl, const glm::vec2& tr, const glm::vec2& other, float x, float z) const
{
	glm::vec2 coord(x, z);
	glm::vec3 barycentric = Math::getBarycentricCoords({ x, z }, bl, tr, other);

	float hBl = getExactTerrainHeight(chunkX * TERRAIN_SIZE + bl.x, chunkZ * TERRAIN_SIZE + bl.y);
	float hTr = getExactTerrainHeight(chunkX * TERRAIN_SIZE + tr.x, chunkZ * TERRAIN_SIZE + tr.y);
	float hOther = getExactTerrainHeight(chunkX * TERRAIN_SIZE + other.x, chunkZ * TERRAIN_SIZE + other.y);
	glm::vec3 weights = (glm::vec3(1.0f) - barycentric) * 0.5f;

	return weights.x * hBl + weights.y * hTr + weights.z * hOther;
}

float World::getExactTerrainHeight(float x, float z) const
{
	int chunkX = (int)floorf(x / TERRAIN_SIZE);
	int chunkZ = (int)floorf(z / TERRAIN_SIZE);

	for (const Terrain& terrain : terrains)
		if (terrain.getChunkX() == chunkX && terrain.getChunkZ() == chunkZ)
			return terrain.getHeight(x - chunkX * TERRAIN_SIZE, z - chunkZ * TERRAIN_SIZE);

	return 0.0f;
}

template<typename... ConstructorArgs>
Entity& World::newEntity(ConstructorArgs&&... args)
{
	entities.emplace_back(args...);
	return entities.back();
}
