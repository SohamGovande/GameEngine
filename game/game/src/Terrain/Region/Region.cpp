#include "Region.h"
#include "Terrain/World.h"
#include "BinIO/BinaryReader.t.h"
#include "BinIO/BinaryWriter.t.h"

Region::Region(int regionX, int regionZ)
	: regionX(regionX), regionZ(regionZ)
{
	
}

Region::~Region()
{
	
}

void Region::save()
{
	BinaryWriter writer("world/regions/" + std::to_string(regionX) + "_" + std::to_string(regionZ) + ".region");
	
	writer.close();
}

void Region::load()
{
}
