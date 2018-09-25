#pragma once
#include "ResourceMgr/TextureResource.h"

struct TerrainTextureInfo
{
	TextureResource* texture, *specularMap;
	float reflectivity, shineDistanceDamper;

	TerrainTextureInfo(TextureResource& tex);
	TerrainTextureInfo(TextureResource& tex, TextureResource& specular);

	inline const TextureResource& getTexture() const { return *texture; }
	inline TextureResource& getTexture() { return *texture; }
	
	inline const TextureResource& getSpecularMap() const { return *specularMap; }
	inline TextureResource& getSpecularMap() { return *specularMap; }

	inline bool hasSpecularMap() const { return specularMap != nullptr; }
};
