#pragma once
#include "TerrainConstants.h"

struct UnsignedXZ
{
	unsigned int x, z;
};

struct FloatXZ
{
	float x, z;
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
