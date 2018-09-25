#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/detail/func_geometric.inl>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>

#include "Renderer/VertexBufferLayout.h"
#include "BinIO/BinaryReader.t.h"
#include "ModelLoader.h"

#define Assert(x) if (!(x)) __debugbreak();

GlModel Loader::loadModelToGL(const Mesh& mesh, bool includeTangents)
{
	VertexArray vao;

	std::vector<VertexBuffer> vbos;
	vbos.emplace_back(mesh.vertices, sizeof(float) * 3 * mesh.vCount);
	vao.addBuffer(vbos.back(), VertexBufferLayout::simple<float>(3));

	vbos.emplace_back(mesh.textures, sizeof(float) * 2 * mesh.vCount);
	vao.addBuffer(vbos.back(), VertexBufferLayout::simple<float>(2));

	vbos.emplace_back(mesh.normals, sizeof(float) * 3 * mesh.vCount);
	vao.addBuffer(vbos.back(), VertexBufferLayout::simple<float>(3));

	if (mesh.hasTangentAttribute() && includeTangents)
	{
		vbos.emplace_back(mesh.tangents, sizeof(float) * 3 * mesh.vCount);
		vao.addBuffer(vbos.back(), VertexBufferLayout::simple<float>(3));
	}

	return GlModel(std::move(vao), IndexBuffer(mesh.indices, mesh.iCount), std::move(vbos));
}

template<typename T>
static void readIndices(BinaryReader& reader, Mesh& mesh)
{
	mesh.indices = new unsigned int[mesh.iCount];
	unsigned int nextIndex = 0;

	for (unsigned int i = 0; i < mesh.iCount; i++)
		mesh.indices[i] = reader.read<T>();

}

Mesh Loader::loadBinaryMeshData(const std::string& filename)
{
	Mesh mesh;

	BinaryReader reader("res/models/" + filename + ".dat");
	reader.ensureHeader("model", Version(1, 0, 0));
	
	mesh.vCount = reader.read<unsigned int>();

	mesh.vertices = reader.readBlock<float, unsigned int>(mesh.vCount * 3);
	mesh.textures = reader.readBlock<float, unsigned int>(mesh.vCount * 2);
	mesh.normals = reader.readBlock<float, unsigned int>(mesh.vCount * 3);
	
	if (reader.read<bool>()) //whether tangents are stored in this model
		mesh.tangents = reader.readBlock<float, unsigned int>(mesh.vCount * 3);
	else
		mesh.tangents = nullptr;

	mesh.iCount = reader.read<unsigned int>();
	unsigned char type = reader.read<unsigned char>();

	switch (type)
	{
	case 0:
		readIndices<unsigned char>(reader, mesh);
		break;
	case 1:
		readIndices<unsigned short>(reader, mesh);
		break;
	case 2:
		readIndices<unsigned int>(reader, mesh);
		break;
	default:
		std::cout << "Incompatible indices type: " << (unsigned int) type << std::endl;
	}

	reader.close();
	
	return mesh;
}