#include "TerrainTextureInfo.h"

TerrainTextureInfo::TerrainTextureInfo(TextureResource& tex)
	: texture(&tex), specularMap(nullptr), reflectivity(0), shineDistanceDamper(0)
{
}

TerrainTextureInfo::TerrainTextureInfo(TextureResource& tex, TextureResource& specular)
	: texture(&tex), specularMap(&specular), reflectivity(0), shineDistanceDamper(0)
{
}
