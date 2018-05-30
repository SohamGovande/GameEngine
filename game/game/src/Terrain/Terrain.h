#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>

#include "Renderer/MaterialModel.h"
#include "ResourceMgr/ResourceMgr.h"
#include "Mesh.h"
#include "TerrainGen.h"
#include "Heightmap.h"
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

struct TerrainTextureInfo {
	TextureResource* const texture;
	TextureResource* const specularMap;
	float reflectivity, shineDistanceDamper;

	inline TerrainTextureInfo(TextureResource* tex)
		: texture(tex), specularMap(nullptr), reflectivity(0), shineDistanceDamper(0)
	{}

	inline TerrainTextureInfo(TextureResource* tex,  TextureResource* specular)
		: texture(tex), specularMap(specular), reflectivity(0), shineDistanceDamper(0)
	{}

	inline bool hasSpecularMap() const { return specularMap != nullptr; }
};

//A chunk of terrain units
class Terrain
{
private:
	TerrainGen& generator;

	Heightmap heightmap;
	int chunkX, chunkZ;

	Mesh mesh;
	GlModel* model;

	std::vector<TerrainTextureInfo> textures;
	std::vector<Texture> blendMaps;
public:
	Terrain(const ResourceMgr& resourceMgr, TerrainGen& generator, int chunkX, int chunkZ);
	Terrain(const Terrain& other) = delete;
	Terrain(Terrain&& other) = delete;
	~Terrain();

	void setTerrainHeight(float x, float z, float y);
	float getHeight(float x, float z) const;

	glm::vec3 getTerrainNormal(float x, float z);

	void generateMesh(const ResourceMgr& resourceMgr);
	inline const GlModel& getTerrainModel() const { return *model; }
	inline GlModel& getTerrainModel() { return *model; }

	inline const std::vector<TerrainTextureInfo>& getTextures() const { return textures; }
	inline std::vector<TerrainTextureInfo>& getTextures() { return textures; }
	inline const std::vector<Texture>& getBlendMaps() const { return blendMaps; }

	inline int getChunkX() const { return chunkX; }
	inline int getChunkZ() const { return chunkZ; }
};
