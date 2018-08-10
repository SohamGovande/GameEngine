#include <glm/mat2x2.hpp>
#include "Heightmap.h"
#include "World.h"

Heightmap::Heightmap(unsigned int rowColumnCount, float interval)
	:  rowColumnCount(rowColumnCount), interval(interval)
{
	data = new float[rowColumnCount * rowColumnCount];
}

Heightmap::~Heightmap()
{
	delete[] data;
}

float* Heightmap::index(unsigned int x, unsigned int z) const
{
	if (x > rowColumnCount - 1 || z > rowColumnCount - 1)
		return nullptr;
	return data + (x * rowColumnCount + z);
}

float* Heightmap::operator[](const glm::vec2& coord) const
{
	unsigned int x = (unsigned int)(coord.x / interval);
	unsigned int z = (unsigned int)(coord.y / interval);
	if (x == (float)(coord.x / interval) && (float)(z == coord.y / interval))
		return index(x, z);

	return nullptr;
}
