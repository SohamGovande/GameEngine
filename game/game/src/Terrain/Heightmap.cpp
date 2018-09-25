#include <glm/mat2x2.hpp>
#include "Heightmap.h"
#include "World.h"

Heightmap::Heightmap(unsigned int rowColumnCount, float interval)
	:  rowColumnCount(rowColumnCount), interval(interval)
{
	data = new float[rowColumnCount*rowColumnCount];
}

Heightmap::Heightmap(const Heightmap& other)
	: rowColumnCount(other.rowColumnCount), interval(other.interval)
{
	const unsigned int newSize = rowColumnCount*rowColumnCount;
	data = new float[newSize];
	memcpy(data, other.data, newSize);
}

Heightmap::Heightmap(Heightmap&& other)
	: data(other.data), rowColumnCount(other.rowColumnCount), interval(other.interval)
{
	other.data = nullptr;
}

Heightmap::~Heightmap()
{
	release();
}

Heightmap& Heightmap::operator=(const Heightmap& other)
{
	if (this != &other)
	{
		release();
		rowColumnCount = other.rowColumnCount;
		interval = other.interval;

		const unsigned int newSize = rowColumnCount*rowColumnCount;
		data = new float[newSize];
		memcpy(data, other.data, newSize);
	}
	return *this;
}

Heightmap& Heightmap::operator=(Heightmap&& other)
{
	if (this != &other)
	{
		release();

		data = other.data;
		rowColumnCount = other.rowColumnCount;
		interval = other.interval;

		other.data = nullptr;
	}
	return *this;
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

void Heightmap::release()
{
	delete[] data;
}
