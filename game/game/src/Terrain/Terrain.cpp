#include <glm/vec3.hpp>
#include <iostream>

#include "Terrain.h"
#include "MathUtils.h"
#include "ModelLoader.h"

Terrain::Terrain(TerrainGen& generator, int chunkX, int chunkZ)
	: generator(generator), chunkX(chunkX), chunkZ(chunkZ), model(nullptr)
{
}

Terrain::~Terrain()
{
	delete[] mesh.vertices;
	delete[] mesh.indices;
	delete[] mesh.normals;
	delete[] mesh.textures;
	delete model;
}

void Terrain::setTerrainHeight(float x, float z, float y) 
{
	heightmap[HeightmapKey(x, z)] = y;
}


float Terrain::getTerrainHeight(float x, float z)
{
	auto it = heightmap.find(HeightmapKey(x, z));
	if (it != heightmap.end())
		return it->second;
	
	return generator.getTerrainHeight(x, z);
}


glm::vec3 Terrain::getTerrainNormal(float x, float z)
{
	float hL = getTerrainHeight(x-TERRAIN_INTERVAL, z);
	float hR = getTerrainHeight(x+TERRAIN_INTERVAL, z);
	float hD = getTerrainHeight(x, z-TERRAIN_INTERVAL);
	float hU = getTerrainHeight(x, z+TERRAIN_INTERVAL);
	return glm::vec3(hL - hR, 2, hD - hU);
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
			vertices[vertexPointer * 3 + 1] = getTerrainHeight(posX, posZ);
			vertices[vertexPointer * 3 + 2] = posZ;
	
			glm::vec3 normal = getTerrainNormal(posX, posZ);

			normals[vertexPointer * 3 + 0] = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;
			
			textureCoords[vertexPointer * 2 + 0] = (float) x / ((float)TERRAIN_VERTEX_COUNT - 1) * 16;
			textureCoords[vertexPointer * 2 + 1] = (float) z / ((float)TERRAIN_VERTEX_COUNT - 1) * 16;
			vertexPointer++;
		}
	}
	
	int pointer = 0;
	for (int gz = 0; gz < TERRAIN_VERTEX_COUNT - 1; gz++) {
		for (int gx = 0; gx < TERRAIN_VERTEX_COUNT - 1; gx++) {

			int topLeft = (gz * (int) TERRAIN_VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * (int) TERRAIN_VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;

			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
	
	model = new MaterialModel(
		resourceMgr.grass, 
		Loader::loadModel(vertices, textureCoords, normals, indices, vCount, iCount),
		"terrain"
	);

	mesh.vertices = vertices;
	mesh.indices = indices;
	mesh.textures = textureCoords;
	mesh.normals = normals;
	mesh.vCount = vCount;
	mesh.iCount = iCount;
}
