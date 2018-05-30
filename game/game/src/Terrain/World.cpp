#include "Renderer/Renderers/MasterRenderer.h"
#include "Entity/Components/MotionComponent.h"
#include "World.h"
#include "ModelLoader.h"
#include "MathUtils.h"

World::World(const ResourceMgr& resourceMgr)
	: entities(), terrains(), 
	person(nullptr)
{
	person = &addEntity(*resourceMgr.playerModel);

	person->moveTo(glm::vec3(0, 100, 0));
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

float World::getInterpolatedTerrainHeight(float x, float z) const
{
	int chunkX = (int) floorf(x / TERRAIN_SIZE);
	int chunkZ = (int) floorf(z / TERRAIN_SIZE);

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

			FloatXZ bl{ (unsigned int)(relativeX / TERRAIN_INTERVAL) * TERRAIN_INTERVAL, (unsigned int)(relativeZ / TERRAIN_INTERVAL) * TERRAIN_INTERVAL };
			FloatXZ br{ bl.x, bl.z + TERRAIN_INTERVAL };
			FloatXZ tl{ bl.x + TERRAIN_INTERVAL, bl.z };
			FloatXZ tr{ tl.x, br.z };

			FloatXZ offset{ relativeX - bl.x, relativeZ - bl.z };
			glm::vec2 coords(x, z);
			glm::vec2 relativeCoords = (glm::vec2(relativeX, relativeZ) - (glm::vec2&)bl) / TERRAIN_INTERVAL;

			float hBl = getExactTerrainHeight(chunkX * TERRAIN_SIZE + bl.x, chunkZ * TERRAIN_SIZE + bl.z);
			float hBr = getExactTerrainHeight(chunkX * TERRAIN_SIZE + br.x, chunkZ * TERRAIN_SIZE + br.z);
			float hTl = getExactTerrainHeight(chunkX * TERRAIN_SIZE + tl.x, chunkZ * TERRAIN_SIZE + tl.z);
			float hTr = getExactTerrainHeight(chunkX * TERRAIN_SIZE + tr.x, chunkZ * TERRAIN_SIZE + tr.z);

			if (offset.x > offset.z) //triangle is bottomleft, topleft, topright
			{
				return Math::getBarycentricHeight(
					{0, hBl, 0},
					{1, hTl, 0},
					{1, hTr, 1},
					relativeCoords
				);
			}
			else if (offset.x < offset.z) //triangle is bottomleft, bottomright,  topright
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
				const float beginHeight = terrain.getHeight(bl.x, bl.z);
				return (terrain.getHeight(tr.x, tr.z) - beginHeight) * interpolation + beginHeight;
			}
			return 0;
		}
	}
	return 0.0f;
}

float World::computeBarycentricHeight(int chunkX, int chunkZ, FloatXZ bl, FloatXZ tr, FloatXZ other, float x, float z) const
{
	glm::vec2 coord(x, z);
	glm::vec3 barycentric = Math::getBarycentricCoords({ x, z }, bl, tr, other);

	float hBl = getExactTerrainHeight(chunkX * TERRAIN_SIZE + bl.x, chunkZ * TERRAIN_SIZE + bl.z);
	float hTr = getExactTerrainHeight(chunkX * TERRAIN_SIZE + tr.x, chunkZ * TERRAIN_SIZE + tr.z);
	float hOther = getExactTerrainHeight(chunkX * TERRAIN_SIZE + other.x, chunkZ * TERRAIN_SIZE + other.z);
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