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

GlModel Loader::loadModel(float* verticesArray, float* texturesArray, float* normalsArray, unsigned int* indicesArray,
	unsigned int vertexCount, unsigned int indexCount)
{
	VertexArray vao;

	VertexBuffer verticesVbo (verticesArray, sizeof(float) * 3 * vertexCount);
	vao.addBuffer(verticesVbo, VertexBufferLayout::simple<float>(3));

	VertexBuffer texturesVbo(texturesArray, sizeof(float) * 2 * vertexCount);
	vao.addBuffer(texturesVbo, VertexBufferLayout::simple<float>(2));

	VertexBuffer normalsVbo(normalsArray, sizeof(float) * 3 * vertexCount);
	vao.addBuffer(normalsVbo, VertexBufferLayout::simple<float>(3));

	std::vector<VertexBuffer> vbos;
	vbos.emplace_back(verticesVbo);
	vbos.emplace_back(texturesVbo);
	vbos.emplace_back(normalsVbo);

	IndexBuffer ibo(indicesArray, indexCount);
	return { vao, ibo, vbos };
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
	mesh.vCount = reader.read<unsigned int>();

	mesh.vertices = new float[mesh.vCount * 3];
	mesh.textures = new float[mesh.vCount * 2];
	mesh.normals = new float[mesh.vCount * 3];

	mesh.vertices = reader.readBlock<float, unsigned int>(mesh.vCount * 3);
	mesh.textures = reader.readBlock<float, unsigned int>(mesh.vCount * 2);
	mesh.normals = reader.readBlock<float, unsigned int>(mesh.vCount * 3);

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
	}

	reader.close();
	std::cout << "Model " << filename << " had " << mesh.vCount << " vertices and " << mesh.iCount / 3 << " triangles." << std::endl;

	return mesh;
}