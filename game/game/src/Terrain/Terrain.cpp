#include <glm/vec3.hpp>
#include <iostream>

#include "Terrain.h"
#include "MathUtils.h"
#include "ModelLoader.h"
#include "Renderer/GlMacros.h"

Terrain::Terrain(const ResourceMgr& resourceMgr, TerrainGen& generator, int chunkX, int chunkZ)
	: generator(generator), chunkX(chunkX), chunkZ(chunkZ), model(nullptr),
	heightmap(TERRAIN_VERTEX_COUNT, TERRAIN_INTERVAL)
{
	Heightmap hmap(5, 3.f);

	textures.emplace_back(resourceMgr.grass);
	textures.emplace_back(resourceMgr.dirt);

	blendMaps.emplace_back(128,  128, false);
	const Texture& tex = blendMaps.back();
	tex.bind();
	constexpr int texSize = 128 * 128 * 4;
	unsigned char* data = new unsigned char[texSize];
	memset(data, 0, sizeof(unsigned char) * texSize);
	
	PerlinNoise noise;

	for (unsigned int i = 0; i < texSize / 4; i++)
	{
		constexpr float scale = 0.05f;
		float xCoord = scale * (i % 128);
		float yCoord = scale * (i / 128);

		float val = noise.noise(xCoord, yCoord, 0.4);
		data[i * 4] = val * 255;
		data[i * 4 + 1] = 255 * (1 - val);
	}

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
}

Terrain::~Terrain()
{
	mesh.free();
	if (model != nullptr)
		delete model;
}

void Terrain::setTerrainHeight(float x, float z, float y)
{
	float* point = heightmap[{x, z}];
	if (point != nullptr)
		*point = y;
}


float Terrain::getTerrainHeight(float x, float z) const
{
	float* height = heightmap[{x, z}];
	if (height == nullptr)
		return generator.getTerrainHeight(x + chunkX * TERRAIN_SIZE, z + chunkZ * TERRAIN_SIZE);
	return *height;
}

glm::vec3 Terrain::getTerrainNormal(float x, float z)
{
	float hL = getTerrainHeight(x - TERRAIN_INTERVAL, z);
	float hR = getTerrainHeight(x + TERRAIN_INTERVAL, z);
	float hD = getTerrainHeight(x, z - TERRAIN_INTERVAL);
	float hU = getTerrainHeight(x, z + TERRAIN_INTERVAL);
	glm::vec3 normal(hL - hR, 2, hD - hU);
	
	if (x == chunkX * TERRAIN_SIZE || z == chunkZ * TERRAIN_SIZE)
		std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
	return normal;
}

void Terrain::generateMesh(const ResourceMgr& resourceMgr)
{
	constexpr unsigned int vCount = (int) (TERRAIN_VERTEX_COUNT * TERRAIN_VERTEX_COUNT);
	float* vertices = new float[vCount * 3];
	float* normals = new float[vCount * 3];
	float* textureCoords = new float[vCount * 2];
	
	constexpr unsigned int iCount = 6 * ((int) TERRAIN_VERTEX_COUNT - 1)*((int) TERRAIN_VERTEX_COUNT - 1);
	unsigned int* indices = new unsigned int[iCount];

	int vertexPointer = 0;
	for (int z = 0; z < TERRAIN_VERTEX_COUNT; z++) {
		for (int x = 0; x < TERRAIN_VERTEX_COUNT; x++) {
			float relativePosX = (float)x * TERRAIN_INTERVAL;
			float relativePosZ = (float)z * TERRAIN_INTERVAL;
			float posX = chunkX * TERRAIN_SIZE + relativePosX;
			float posZ = chunkZ * TERRAIN_SIZE + relativePosZ;

			vertices[vertexPointer * 3 + 0] = posX;
			vertices[vertexPointer * 3 + 1] = getTerrainHeight(relativePosX, relativePosZ);
			vertices[vertexPointer * 3 + 2] = posZ;
	
			glm::vec3 normal = glm::normalize(getTerrainNormal(relativePosX, relativePosZ));

			normals[vertexPointer * 3 + 0] = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;
			
			textureCoords[vertexPointer * 2 + 0] = (float) x / ((float)TERRAIN_VERTEX_COUNT - 1);
			textureCoords[vertexPointer * 2 + 1] = (float) z / ((float)TERRAIN_VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}
	
	unsigned int pointer = 0;
	for (unsigned int gz = 0; gz < TERRAIN_VERTEX_COUNT - 1; gz++) {
		for (unsigned int gx = 0; gx < TERRAIN_VERTEX_COUNT - 1; gx++) {

			unsigned int topLeft = (gz * (int) TERRAIN_VERTEX_COUNT) + gx;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = ((gz + 1) * (int) TERRAIN_VERTEX_COUNT) + gx;
			unsigned int bottomRight = bottomLeft + 1;

			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
	
	mesh.vertices = vertices;
	mesh.indices = indices;
	mesh.textures = textureCoords;
	mesh.normals = normals;
	mesh.vCount = vCount;
	mesh.iCount = iCount;

	model = new GlModel(Loader::loadModel(mesh));
}
