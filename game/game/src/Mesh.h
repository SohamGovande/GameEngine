#pragma once
#include <vector>
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

struct Mesh
{
	float* vertices, *textures, *normals, *tangents;
	unsigned int* indices;
	unsigned int vCount, iCount;

	inline Mesh()
		: vertices(nullptr), textures(nullptr), normals(nullptr), tangents(nullptr), indices(nullptr),
		vCount(0), iCount(0)
	{
	}

	inline void free()
	{
		delete[] vertices;
		delete[] textures;
		delete[] normals;
		delete[] tangents;
		delete[] indices;
	}

	inline bool hasTangentAttribute() const
	{
		return tangents != nullptr;
	}
};

struct GlModel
{
	VertexArray vao;
	IndexBuffer ibo;
	std::vector<VertexBuffer> vbos;

	GlModel(VertexArray&& vao, IndexBuffer&& ibo, std::vector<VertexBuffer>&& vbos)
		: vao(std::move(vao)), ibo(std::move(ibo)), vbos(std::move(vbos))
	{}
	GlModel(const GlModel& other) = delete;
	GlModel(GlModel&& other)
		: vao(std::move(other.vao)), ibo(std::move(other.ibo)), vbos(std::move(other.vbos))
	{}
	
	GlModel& operator=(const GlModel& other) = delete;
	GlModel& operator=(GlModel&& other)
	{
		if (this != &other)
		{
			vao = std::move(other.vao);
			ibo = std::move(other.ibo);
			vbos = std::move(other).vbos;
		}
		return *this;
	}
};

