#pragma once
#include <array>
#include <glm/vec2.hpp>
#include "TerrainConstants.h"

class World;

class Heightmap
{
private:
	float* data;
	unsigned int rowColumnCount;
	float interval;

public:
	Heightmap(unsigned int size, float interval);
	~Heightmap();

	float* index(unsigned int x, unsigned int z) const;
	float* operator[] (glm::vec2 coord) const;

	inline unsigned int getRowColumnCount() const { return rowColumnCount; }
	inline const float* getData() const { return data; }
};
