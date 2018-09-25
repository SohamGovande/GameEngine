#pragma once
#include <string>
#include <memory>
#include <tuple>
#include <unordered_map>

#include "ResourceMgr/TextureResource.h"
#include "Renderer/MaterialModel.h"
#include "Mesh.h"

namespace Loader
{
	GlModel loadModelToGL(const Mesh& mesh, bool includeTangents);
	Mesh loadBinaryMeshData(const std::string& filename);
	Mesh loadObjMeshData(const std::string& filename, bool calculateTangents);
}
