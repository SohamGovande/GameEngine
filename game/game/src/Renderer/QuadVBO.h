#pragma once
#include <array>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

class QuadVBO
{
private:
	std::array<float, 24> quadVertices;
	VertexBuffer vbo;
	VertexArray vao;
	VertexBufferLayout layout;

public:
	QuadVBO();
	~QuadVBO();

	inline const VertexBuffer& getVbo() const { return vbo; }
	inline const VertexArray& getVao() const { return vao; }
	inline const VertexBufferLayout& getVboLayout() const { return layout; }
};
