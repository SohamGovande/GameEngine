#pragma once
#include <vector>
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

struct Mesh
{
	float* vertices, *textures, *normals;
	unsigned int* indices;
	unsigned int vCount, iCount;

	inline Mesh()
		: vertices(nullptr), textures(nullptr), normals(nullptr), indices(nullptr),
		vCount(0), iCount(0)
	{
	}

	inline void free()
	{
		if (vertices != nullptr)
			delete[] vertices;
		if (textures != nullptr)
			delete[] textures;
		if (normals != nullptr)
			delete[] normals;
		if (indices != nullptr)
			delete[] indices;
	}
};

struct GlModel
{
	VertexArray vao;
	IndexBuffer ibo;
	std::vector<VertexBuffer> vbos;
};

