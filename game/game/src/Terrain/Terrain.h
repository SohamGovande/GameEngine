#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>

#include "Renderer/MaterialModel.h"
#include "ResourceMgr/ResourceMgr.h"
#include "Mesh.h"
#include "TerrainGen.h"
#include "Heightmap.h"

struct TerrainTextureInfo {
	TextureResource& texture;
	TextureResource* specularMap;
	float reflectivity, shineDistanceDamper;

	inline TerrainTextureInfo(TextureResource& tex)
		: texture(tex), specularMap(nullptr), reflectivity(0), shineDistanceDamper(0)
	{}

	inline TerrainTextureInfo(TextureResource& tex,  TextureResource& specular)
		: texture(tex), specularMap(&specular), reflectivity(0), shineDistanceDamper(0)
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
	Terrain(ResourceMgr& resourceMgr, TerrainGen& generator, int chunkX, int chunkZ);
	Terrain(const Terrain& other) = delete;
	Terrain(Terrain&& other) = delete;
	~Terrain();

	Terrain& operator=(const Terrain& other) = delete;
	Terrain& operator=(Terrain&& other) = delete;

	void setTerrainHeight(float x, float z, float y);
	float getHeight(float x, float z) const;

	glm::vec3 getTerrainNormal(float x, float z);

	void generateMesh(const ResourceMgr& resourceMgr);
	inline const GlModel& getTerrainModel() const { return *model; }
	inline GlModel& getTerrainModel() { return *model; }

	inline const std::vector<TerrainTextureInfo>& getTextures() const { return textures; }
	inline std::vector<TerrainTextureInfo>& getTextures() { return textures; }
	inline const std::vector<Texture>& getBlendMaps() const { return blendMaps; }

	inline const Mesh& getMesh() const { return mesh; }
	inline const Heightmap& getHeightmap() const { return heightmap; }

	inline int getChunkX() const { return chunkX; }
	inline int getChunkZ() const { return chunkZ; }
};
