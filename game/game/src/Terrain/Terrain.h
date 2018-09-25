#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>

#include "Renderer/MaterialModel.h"
#include "Renderer/GlModel.h"
#include "ResourceMgr/ResourceMgr.h"
#include "Mesh.h"
#include "TerrainGen.h"
#include "Heightmap.h"
#include "TerrainTextureInfo.h"

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
