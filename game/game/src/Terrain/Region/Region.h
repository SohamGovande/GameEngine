#pragma once
#include <vector>
#include <string>
#include "Terrain/Terrain.h"
#include "RegionConstants.h"

class World;

class Region
{
private:
	std::vector<Terrain*> terrains;
	int regionX, regionZ;

public:
	Region(int regionX, int regionZ);
	~Region();

	void save();
	void load();
};
