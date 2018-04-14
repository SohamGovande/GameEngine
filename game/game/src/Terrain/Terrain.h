#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>

#include "Renderer/MaterialModel.h"
#include "ResourceMgr/ResourceMgr.h"
#include "Mesh.h"
#include "TerrainGen.h"
#include "TerrainConstants.h"

struct HeightmapKey
{
	float x, z;
	inline HeightmapKey(float x, float z)
		: x(x), z(z) {}
	inline bool operator==(const HeightmapKey& other) const
	{
		return other.x == x && other.z == z;
	}
};

//hash function so that a HeightmapKey can be put inside unordered_map
namespace std
{
	template<>
	struct hash<HeightmapKey>
	{
		//hash function
		unsigned int operator()(const HeightmapKey& key) const
		{
			return hash<float>()(key.x) ^ hash<float>()(key.z);
		}
	};
}


//A chunk of terrain units
class Terrain
{
private:
	TerrainGen& generator;

	std::unordered_map<HeightmapKey, float> heightmap;
	int chunkX, chunkZ;

	Mesh mesh;
	MaterialModel* model;
public:
	Terrain(TerrainGen& generator, int chunkX, int chunkZ);
	~Terrain();

	void setTerrainHeight(float x, float z, float y);
	float getTerrainHeight(float x, float z);

	glm::vec3 getTerrainNormal(float x, float z);

	void generateMesh(const ResourceMgr& resourceMgr);
	inline const MaterialModel& getTerrainModel() const { return *model; }
	inline MaterialModel& getTerrainModel() { return *model; }

	inline int getChunkX() const { return chunkX; }
	inline int getChunkZ() const { return chunkZ; }
};
