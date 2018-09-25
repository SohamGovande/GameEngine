#pragma once
#include <vector>
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

struct GlModel
{
	VertexArray vao;
	IndexBuffer ibo;
	std::vector<VertexBuffer> vbos;

	GlModel(VertexArray&& vao, IndexBuffer&& ibo, std::vector<VertexBuffer>&& vbos);
	GlModel(const GlModel& other) = delete;
	GlModel(GlModel&& other);

	GlModel& operator=(const GlModel& other) = delete;
	GlModel& operator=(GlModel&& other);
};
