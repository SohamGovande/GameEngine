#pragma once
#include <string>
#include <memory>
#include <tuple>
#include <unordered_map>

#include "ResourceMgr/MeshResource.h"
#include "ResourceMgr/TextureResource.h"
#include "Renderer/MaterialModel.h"
#include "Mesh.h"

namespace Loader
{

	GlModel loadModel(float* verticesArray, float* texturesArray, float* normalsArray, unsigned int* indicesArray, 
		unsigned int vertexCount, unsigned int indexCount);

	inline GlModel loadModel(const Mesh& mesh)
	{
		return loadModel(
			mesh.vertices, mesh.textures, mesh.normals, mesh.indices,
			mesh.vCount, mesh.iCount
		);
	}

	Mesh loadObjMeshData(const std::string& filename);

	GlModel loadObjModel(const std::string& filename);
}
