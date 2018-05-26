#include "Heightmap.h"

Heightmap::Heightmap(unsigned int rowColumnCount, float interval)
	:  rowColumnCount(rowColumnCount), interval(interval)
{
	data = new float[rowColumnCount * rowColumnCount];
}

Heightmap::~Heightmap()
{
	delete[] data;
}

float* Heightmap::index(UnsignedXZ coord) const
{
	if (coord.x > rowColumnCount - 1 || coord.z > rowColumnCount - 1)
		return nullptr;
	return data + (coord.x * rowColumnCount + coord.z);
}

float* Heightmap::operator[](FloatXZ coord) const
{
	UnsignedXZ xz{ (unsigned int)(coord.x / interval), (unsigned int)(coord.z / interval) };
	if (xz.x == coord.x / interval && xz.z == coord.z / interval)
		return index(xz);

	return nullptr;
}