#include "GlModel.h"

GlModel::GlModel(VertexArray&& vao, IndexBuffer&& ibo, std::vector<VertexBuffer>&& vbos)
	: vao(std::move(vao)), ibo(std::move(ibo)), vbos(std::move(vbos))
{}

GlModel::GlModel(GlModel&& other)
	: vao(std::move(other.vao)), ibo(std::move(other.ibo)), vbos(std::move(other.vbos))
{}

GlModel& GlModel::operator=(GlModel&& other)
{
	if (this != &other)
	{
		vao = std::move(other.vao);
		ibo = std::move(other.ibo);
		vbos = std::move(other).vbos;
	}
	return *this;
}