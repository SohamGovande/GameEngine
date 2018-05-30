#pragma once
#include <array>
#include "TerrainConstants.h"

class World;

struct UnsignedXZ
{
	unsigned int x, z;
};

struct FloatXZ
{
	float x, z;

	friend FloatXZ operator-(const FloatXZ& left, const FloatXZ& right);
	inline operator glm::vec2&()
	{
		return *reinterpret_cast<glm::vec2*>(&x);
	}
};

class Heightmap
{
private:
	float* data;
	unsigned int rowColumnCount;
	float interval;

public:
	Heightmap(unsigned int size, float interval);
	~Heightmap();

	float* index(UnsignedXZ coord) const;
	float* operator[] (FloatXZ coord) const;
};
