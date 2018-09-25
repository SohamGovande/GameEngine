#pragma once
#include <array>
#include <glm/vec2.hpp>

class Heightmap
{
private:
	float* data;
	unsigned int rowColumnCount;
	float interval;

public:
	Heightmap(unsigned int size, float interval);
	Heightmap(const Heightmap& other);
	Heightmap(Heightmap&& other);
	~Heightmap();

	Heightmap& operator=(const Heightmap& other);
	Heightmap& operator=(Heightmap&& other);

	float* index(unsigned int x, unsigned int z) const;
	float* operator[](const glm::vec2& coord) const;

	inline unsigned int getRowColumnCount() const { return rowColumnCount; }
	inline const float* getData() const { return data; }

private:
	void release();
};
