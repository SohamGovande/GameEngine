#pragma once
#include <string>
#include "Mesh.h"

namespace Loader
{
	Mesh loadObjMeshData(const std::string& filename, bool calculateTangents);
}
